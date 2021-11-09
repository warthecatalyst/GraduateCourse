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
		std::cout << "Sender:���������������������Ե�\n\n";
		return false;
	}

	//makePacket()
	packets[nextSeqnum].acknum = -1;
	packets[nextSeqnum].seqnum = nextSeqnum;
	memcpy(packets[nextSeqnum].payload, message.data, sizeof(message.data));
	packets[nextSeqnum].checksum = pUtils->calculateCheckSum(packets[nextSeqnum]);
	pUtils->printPacket("[Debug]���ͷ����ͱ���", packets[nextSeqnum]);

	//print slidewindow
	std::cout << "\nSender:���ͱ���֮ǰ�Ļ�������:";
	PrintSlideWindow();

	//udtsend()
	if (base == nextSeqnum)	//start timer
	{
		pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
	}
	pns->sendToNetworkLayer(RECEIVER, packets[nextSeqnum]);
	nextSeqnum = (nextSeqnum + 1) % seqSize;
	std::cout << "\nSender:���ͱ���֮��Ļ�������:";
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
		{//������ʱ��
			pns->stopTimer(SENDER, 0);
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
		}
		std::cout << "\n[SENDER]�յ�ack�����������ƶ���";
		PrintSlideWindow();
		std::cout << std::endl;
	}
}

//����ʱ
void GBNRdtSender::timeoutHandler(int seqNum) {
	std::cout << "\nSender:���ͳ�ʱ,���·������ݰ�" << std::endl;

	pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
	for (int i = base; i != nextSeqnum; i = (i + 1) % seqSize) {
		pns->sendToNetworkLayer(RECEIVER, packets[i]);
		pUtils->printPacket("Sender:���ͷ����·��ͷ���:", packets[i]);
	}

	std::cout << "\nSender:�ط����" << std::endl;
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