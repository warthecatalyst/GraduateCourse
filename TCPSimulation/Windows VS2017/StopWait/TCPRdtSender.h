#pragma once
#include "RdtSender.h"
#include <vector>
class TCPRdtSender :
	public RdtSender
{
private:
	//bool waitingState;
	int base;							
	int nextSeqnum;						
	const int N;					
	const int seqSize;					
	std::vector<Packet> packets;
	int dupAckNum;							

private:
	bool InWin(int seqnum);
	void PrintSlideWindow();

public:
	TCPRdtSender();
	TCPRdtSender(int _N, int _seqSize);
	~TCPRdtSender();

public:
	bool getWaitingState();
	bool send(const Message& message);
	void timeoutHandler(int seqNum);
	void receive(const Packet& ackPkt);
};

