#include "stdafx.h"
#include "GBNRdtSender.h"
#include "Global.h"


GBNRdtSender::GBNRdtSender() :N(4), seqSize(8) {
	base = 0;
	nextSeqnum = 0;
	packets.resize(seqSize);
}

GBNRdtSender::GBNRdtSender(int _N, int _seqSize) : N(_N), seqSize(_seqSize) {
	base = 0;
	nextSeqnum = 0;
	packets.resize(seqSize);
}

GBNRdtSender::~GBNRdtSender()
{
}

bool GBNRdtSender::getWaitingState() {
	return ((nextSeqnum - base + seqSize)%seqSize >= N);
}

bool GBNRdtSender::send(const Message& message) {
	if (this->getWaitingState()) {
		std::cout << "Sender:窗口已满，发送数据请稍等\n\n";
		return false;
	}

	//makePacket()
	packets[nextSeqnum].acknum = -1;
	packets[nextSeqnum].seqnum = nextSeqnum;
	memcpy(packets[nextSeqnum].payload, message.data, sizeof(message.data));
	packets[nextSeqnum].checksum = pUtils->calculateCheckSum(packets[nextSeqnum]);
	pUtils->printPacket("[Debug]发送方发送报文", packets[nextSeqnum]);

	//print slidewindow
	std::cout << "\nSender:发送报文之前的滑动窗口:";
	PrintSlideWindow();

	//udtsend()
	if (base == nextSeqnum)	//start timer
	{
		pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
	}
	pns->sendToNetworkLayer(RECEIVER, packets[nextSeqnum]);
	nextSeqnum = (nextSeqnum + 1) % seqSize;
	std::cout << "\nSender:发送报文之后的滑动窗口:";
	PrintSlideWindow();
	std::cout << std::endl;
	return true;
}

void GBNRdtSender::receive(const Packet& ackPkt) {
	bool inWin = (ackPkt.acknum - base + seqSize) % seqSize < N;

	int checkSum = pUtils->calculateCheckSum(ackPkt);

	if (inWin && checkSum == ackPkt.checksum) {
		base = (ackPkt.acknum + 1) % seqSize;
		if (base == nextSeqnum)
		{
			pns->stopTimer(SENDER, 0);
		}
		else
		{//重启计时器
			pns->stopTimer(SENDER, 0);
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
		}
		std::cout << "\n[SENDER]收到ack，滑动窗口移动：";
		PrintSlideWindow();
		std::cout << std::endl;
	}
}

//处理超时
void GBNRdtSender::timeoutHandler(int seqNum) {
	std::cout << "\nSender:发送超时,重新发送数据包" << std::endl;

	pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
	for (int i = base; i != nextSeqnum; i = (i + 1) % seqSize) {
		pns->sendToNetworkLayer(RECEIVER, packets[i]);
		pUtils->printPacket("Sender:发送方重新发送分组:", packets[i]);
	}

	std::cout << "\nSender:重发完毕" << std::endl;
}

void GBNRdtSender::PrintSlideWindow() {
	int i;
	for (int i = 0; i < seqSize; i++) {
		if (i == base)
			std::cout << "[";
		if (i == (base + N) % seqSize)
			std::cout << "]";
		if (i == nextSeqnum)
			std::cout << "*";
		std::cout << i;
	}
	std::cout << std::endl;
}