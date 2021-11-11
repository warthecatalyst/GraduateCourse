#include "stdafx.h"
#include "SRRdtReceiver.h"
#include "Global.h"

SRRdtReceiver::SRRdtReceiver() :N(4), seqSize(8) {
	base = 0;
	packets.resize(seqSize);
	status.resize(seqSize,false);
	lastAckPkt.acknum = -1;
	memset(lastAckPkt.payload, '.', Configuration::PAYLOAD_SIZE);
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

SRRdtReceiver::SRRdtReceiver(int _N, int _seqSize) :N(_N), seqSize(_seqSize) {
	base = 0;
	packets.resize(seqSize);
	status.resize(seqSize);
	lastAckPkt.acknum = -1;
	lastAckPkt.seqnum = -1;
	memset(lastAckPkt.payload, '.', Configuration::PAYLOAD_SIZE);
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

SRRdtReceiver::~SRRdtReceiver() {

}

bool SRRdtReceiver::inWin(int i) {
	return (i - base + seqSize) % seqSize < N;
}

void SRRdtReceiver::PrintSlideWindow() {
	for (int i = 0; i < seqSize; i++) {
		if (i == base) std::cout << "[";
		if (i == (base + N) % seqSize) std::cout << "]";
		if (inWin(i)) {
			if (status[i]) std::cout << "已收到 ";
			else std::cout << "期待 ";
		}
		else {
			std::cout << "不可用 ";
		}
	}
	std::cout << std::endl;
}

void SRRdtReceiver::receive(const Packet& packet) {
	int checksum = pUtils->calculateCheckSum(packet);
	if (checksum != packet.checksum) {
		pUtils->printPacket("Receiver:接收方没有正确收到发送方的报文,数据校验错误", packet);
	}
	else {
		if (!inWin(packet.seqnum)) {
			pUtils->printPacket("[Debug]不是期望的数据分组", packet);
			//std::cout << "\n\n不是期望的数据分组\n\n";
			//lastAckPkt.acknum = base;
			lastAckPkt.acknum = packet.seqnum;
			lastAckPkt.seqnum = -1;//无用
			memset(lastAckPkt.payload, '.', Configuration::PAYLOAD_SIZE);
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);
		}
		else {
			packets[packet.seqnum] = packet;//Packet类重载了赋值运算符
			status[packet.seqnum] = true;
			lastAckPkt.acknum = packet.seqnum;
			lastAckPkt.seqnum = 0;//无用
			memset(lastAckPkt.payload, '.', sizeof(lastAckPkt.payload));//无用
			pUtils->printPacket("[Debug]接收方发送ack", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);
			while (status[base])
			{
				Message msg;
				memcpy(msg.data, packets[base].payload, sizeof(packets[base].payload));
				pns->delivertoAppLayer(RECEIVER, msg);
				pUtils->printPacket("Receiver:递交上层:", packets[base]);
				status[base] = false;
				base = (base + 1) % seqSize;
			}
			std::cout << "\n[RECEIVER]收到数据包，窗口移动：";
			PrintSlideWindow();
			std::cout << std::endl;
		}
	}
}