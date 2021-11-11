#include "stdafx.h"
#include "TCPRdtSender.h"
#include "Global.h"

TCPRdtSender::TCPRdtSender() :N(4), seqSize(8) {
	base = 0;
	nextSeqnum = 0;
	dupAckNum = 0;
	packets.resize(seqSize);
}

TCPRdtSender::TCPRdtSender(int _N,int _seqSize) :N(_N), seqSize(_seqSize) {
	base = 0;
	nextSeqnum = 0;
	dupAckNum = 0;
	packets.resize(seqSize);
}

TCPRdtSender::~TCPRdtSender() {

}

bool TCPRdtSender::InWin(int seqnum) {
	return (seqnum - base + seqSize) % seqSize < N;
}

bool TCPRdtSender::getWaitingState() {
	return (nextSeqnum - base + seqSize) % seqSize < N;
}

bool TCPRdtSender::send(const Message& message)
{
	if (getWaitingState())
	{
		std::cout << "SENDER:窗口已满，发送数据请稍等\n\n";
		return false;
	}
	else
	{
		packets[nextSeqnum].acknum = -1;//这个地方不太懂，单向传输发给接收方的ack应该没用吧
		packets[nextSeqnum].seqnum = nextSeqnum;//序列号
		memcpy(packets[nextSeqnum].payload, message.data, sizeof(message.data));
		packets[nextSeqnum].checksum = pUtils->calculateCheckSum(packets[nextSeqnum]);
		pUtils->printPacket("[Debug]发送方发送报文", packets[nextSeqnum]);
		//显示滑动窗口
		std::cout << "\n[SENDER]发送前窗口：";
		PrintSlideWindow();
		if (base == nextSeqnum)
		{//gbn协议中只有一个定时器，即不管哪个分组超时，都会重传所有未确认的分组
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
		}
		pns->sendToNetworkLayer(RECEIVER, packets[nextSeqnum]);
		//发送完毕，更新状态
		nextSeqnum = (nextSeqnum + 1) % seqSize;//不要忘了取模运算
		//显示滑动窗口
		std::cout << "[SENDER]发送后窗口：";
		PrintSlideWindow();
		std::cout << std::endl;
		return true;
	}
}

void TCPRdtSender::timeoutHandler(int seqNum)
{
	std::cout << "\nSender:发送超时,重新发送数据包" << std::endl;

	pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
	for (int i = base; i != nextSeqnum; i = (i + 1) % seqSize) {
		pns->sendToNetworkLayer(RECEIVER, packets[i]);
		pUtils->printPacket("Sender:发送方重新发送分组:", packets[i]);
	}

	std::cout << "\nSender:重发完毕" << std::endl;
}

void TCPRdtSender::receive(const Packet& ackPkt)
{
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	if (checkSum != ackPkt.checksum)
	{
		pUtils->printPacket("[Debug]接收的ack损坏，校验和不相等", ackPkt);
	}
	else
	{
		if (InWin(ackPkt.acknum))
		{
			base = (ackPkt.acknum + 1) % seqSize;
			pns->stopTimer(SENDER, 0);
			if (base != nextSeqnum)
			{
				pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
			}
			dupAckNum = 0;
			std::cout << "\n[SENDER]收到ack:" << ackPkt.acknum << "，滑动窗口移动：";
			PrintSlideWindow();
			std::cout << std::endl;
		}
		else
		{//已经确认的冗余的ack
			dupAckNum = (dupAckNum + 1) % 3;
			if (dupAckNum == 2)
			{//快速重传
				pns->sendToNetworkLayer(RECEIVER, packets[base]);
				std::cout << "\nSENDER:收到连续三个冗余ack，快速重传\n\n";
			}
		}
	}
}