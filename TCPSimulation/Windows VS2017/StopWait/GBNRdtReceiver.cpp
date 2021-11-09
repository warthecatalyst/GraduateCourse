#include "stdafx.h"
#include "GBNRdtReceiver.h"
#include "Global.h"

GBNRdtReceiver::GBNRdtReceiver() :seqSize(8) {
	expectedSeqNum = 0;
	lastAckPkt.acknum = -1;
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;
	memset(lastAckPkt.payload, '.', Configuration::PAYLOAD_SIZE);
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

GBNRdtReceiver::GBNRdtReceiver(int _seqSize) : seqSize(_seqSize) {
	expectedSeqNum = 0;
	lastAckPkt.acknum = -1;
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;
	memset(lastAckPkt.payload, '.', Configuration::PAYLOAD_SIZE);
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

GBNRdtReceiver::~GBNRdtReceiver() {
	
}

void GBNRdtReceiver::receive(const Packet& packet) {
	if (packet.checksum != pUtils->calculateCheckSum(packet))
	{//数据包损坏，不作出应答
		pUtils->printPacket("[Debug]接收方没有正确收到发送方的报文,数据校验错误", packet);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);//发送上次的
		return;
	}
	if (packet.seqnum != expectedSeqNum)
	{//不是想要的数据包，不作出应答
		pUtils->printPacket("[Debug]不是期望的数据分组", packet);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);//发送上次的
		return;
	}
	else
	{
		Message msg;
		memcpy(msg.data, packet.payload, sizeof(packet.payload));
		pns->delivertoAppLayer(RECEIVER, msg);
		pUtils->printPacket("[Debug]接收方正确并发送到上层APP：", packet);
		//std::cout << "递交给应用数据分组：" << packet.seqnum << "\n\n";
		lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		pUtils->printPacket("[Debug]接收方发送确认报文", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		expectedSeqNum = (expectedSeqNum + 1) % seqSize;//别忘了取模
	}
	std::cout << "\n[RECEIVER]确认号ack：" << lastAckPkt.acknum << "\n\n";
}