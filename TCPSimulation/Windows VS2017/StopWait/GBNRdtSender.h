#pragma once
#include "RdtSender.h"
#include "DataStructure.h"
#include <vector>

class GBNRdtSender:public RdtSender
{
private:
	int nextSeqnum;		//下一个要发送的Packet的编号
	int base;			//滑动窗口的base
	const int N;		//滑动窗口的大小，设置为4
	const int SeqNumMax; //指示Packet编号的位数，设置为8
	std::vector<Packet> packets;	//保存发送的报文用于重传
	
	void PrintSlideWindow();	//打印滑动窗口用于debug


public:

	bool getWaitingState();
	bool send(const Message& message);						//发送应用层下来的Message，由NetworkServiceSimulator调用,如果发送方成功地将Message发送到网络层，返回true;如果因为发送方处于等待正确确认状态而拒绝发送Message，则返回false
	void receive(const Packet& ackPkt);						//接受确认Ack，将被NetworkServiceSimulator调用	
	void timeoutHandler(int seqNum);					//Timeout handler，将被NetworkServiceSimulator调用

public:
	GBNRdtSender();
	GBNRdtSender(int _N, int _SeqNumMax);
	virtual ~GBNRdtSender();
};

