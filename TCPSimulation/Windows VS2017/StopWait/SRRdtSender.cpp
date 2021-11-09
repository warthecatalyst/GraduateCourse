#include "stdafx.h"
#include "SRRdtSender.h"
#include "Global.h"
#include "Tool.h"

SRRdtSender::SRRdtSender() :N(4), seqSize(8) {
	base = 0;
	nextSeqnum = 0;
	packets.resize(seqSize);
	pktSta.resize(seqSize, false);
}

SRRdtSender::SRRdtSender(int _N, int _seqSize) :N(_N), seqSize(_seqSize) {
	base = 0;
	nextSeqnum = 0;
	packets.resize(seqSize);
	pktSta.resize(seqSize, false);
}

SRRdtSender::~SRRdtSender() {

}

void SRRdtSender::PrintSlideWindow()
{
	int i;
	for (i = 0; i < seqSize; i++)
	{
		if (i == base)
			std::cout << "[";
		if (isInWindow(i) && i >= nextSeqnum)			//可用，未发送
			std::cout << "可用";
		else if (isInWindow(i) && pktSta[i])		//发送，已确认
			std::cout << "已确认";
		else if (isInWindow(i))						//已发送，未确认
			std::cout << "已发送";
		if (i == (base + N) % seqSize)
			std::cout << "]";
		if (!isInWindow(i))
			std::cout << "不可用";						//不可用，窗口外
		std::cout << " ";
	}
	std::cout << std::endl;
}

//判断序号是否在窗口内
bool SRRdtSender::isInWindow(int seqnum)
{
	//return seqnum >= base && seqnum <= (base + wndsize) % seqsize;
	if (base < (base + N) % seqSize)
	{
		return seqnum >= base && seqnum < base + N;
	}
	else
	{
		return seqnum >= base || seqnum < (base + N) % seqSize;
	}
}

bool SRRdtSender::getWaitingState()
{
	return (base + N) % seqSize == (nextSeqnum) % seqSize;
}

bool SRRdtSender::send(const Message& message)
{
	if (getWaitingState())
	{//缓冲区满，等待ack
		std::cout << "Sender:窗口已满，发送数据请稍等\n\n";
		return false;
	}
	packets[nextSeqnum].acknum = -1;
	packets[nextSeqnum].seqnum = nextSeqnum;
	memcpy(packets[nextSeqnum].payload, message.data, sizeof(message.data));
	packets[nextSeqnum].checksum = pUtils->calculateCheckSum(packets[nextSeqnum]);
	pUtils->printPacket("Sender:发送方发送报文", packets[nextSeqnum]);
	//显示滑动窗口
	std::cout << "\nSender:发送前窗口：";
	PrintSlideWindow();
	//发送报文
	pns->sendToNetworkLayer(RECEIVER, packets[nextSeqnum]);
	//启动定时器，sr协议中每个分组对应一个定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, nextSeqnum);
	//发送完毕，更新状态
	nextSeqnum = (nextSeqnum + 1) % seqSize;//别忘了取模
	std::cout << "Sender:发送后窗口：";
	PrintSlideWindow();
	std::cout << std::endl;
	return true;
}

void SRRdtSender::receive(const Packet& ackPkt)
{
	int checksum = pUtils->calculateCheckSum(ackPkt);
	if (checksum != ackPkt.checksum)
	{
		pUtils->printPacket("Sender:接收的ack损坏，校验和不相等", ackPkt);
		return;
	}
	else
	{
		pns->stopTimer(SENDER, ackPkt.acknum);
		if (isInWindow(ackPkt.acknum))
		{//更新窗口
			pktSta[ackPkt.acknum] = true;
			while (pktSta[base] == true)
			{//移动base
				pktSta[base] = false;
				base = (base + 1) % seqSize;
			}
			std::cout << "\nSender:收到ack，滑动窗口移动：";
			PrintSlideWindow();
			std::cout << std::endl;
		}
	}
}

void SRRdtSender::timeoutHandler(int seqnum)
{
	std::cout << "\nSender:数据包超时：" << seqnum << "\n\n";
	//不用管其他的序号，接收方有缓冲区，即使失序也不要紧
	pns->sendToNetworkLayer(RECEIVER, packets[seqnum]);
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqnum);
	std::cout << "\nSender:重发数据包完毕:" << seqnum << "\n\n";
}
