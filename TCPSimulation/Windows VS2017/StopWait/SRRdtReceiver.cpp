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
			if (status[i]) std::cout << "���յ� ";
			else std::cout << "�ڴ� ";
		}
		else {
			std::cout << "������ ";
		}
	}
	std::cout << std::endl;
}

void SRRdtReceiver::receive(const Packet& packet) {
	int checksum = pUtils->calculateCheckSum(packet);
	if (checksum != packet.checksum) {
		pUtils->printPacket("Receiver:���շ�û����ȷ�յ����ͷ��ı���,����У�����", packet);
	}
	else {
		if (!inWin(packet.seqnum)) {
			pUtils->printPacket("[Debug]�������������ݷ���", packet);
			//std::cout << "\n\n�������������ݷ���\n\n";
			//lastAckPkt.acknum = base;
			lastAckPkt.acknum = packet.seqnum;
			lastAckPkt.seqnum = -1;//����
			memset(lastAckPkt.payload, '.', Configuration::PAYLOAD_SIZE);
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);
		}
		else {
			packets[packet.seqnum] = packet;//Packet�������˸�ֵ�����
			status[packet.seqnum] = true;
			lastAckPkt.acknum = packet.seqnum;
			lastAckPkt.seqnum = 0;//����
			memset(lastAckPkt.payload, '.', sizeof(lastAckPkt.payload));//����
			pUtils->printPacket("[Debug]���շ�����ack", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);
			while (status[base])
			{
				Message msg;
				memcpy(msg.data, packets[base].payload, sizeof(packets[base].payload));
				pns->delivertoAppLayer(RECEIVER, msg);
				pUtils->printPacket("Receiver:�ݽ��ϲ�:", packets[base]);
				status[base] = false;
				base = (base + 1) % seqSize;
			}
			std::cout << "\n[RECEIVER]�յ����ݰ��������ƶ���";
			PrintSlideWindow();
			std::cout << std::endl;
		}
	}
}