#pragma once
#include "RdtSender.h"
#include "DataStructure.h"
#include <vector>

class SRRdtSender :
    public RdtSender
{
private:
	const int seqSize;
	const int N;
	std::vector<Packet> packets;
	std::vector<bool> pktSta;
	int base, nextSeqnum;

	void PrintSlideWindow();
	bool isInWindow(int seqnum);

public:
	SRRdtSender(int _N, int _seqSize);
	SRRdtSender();
	bool send(const Message& message);
	bool getWaitingState();
	void timeoutHandler(int seqnum);
	void receive(const Packet& ackPkt);
	virtual ~SRRdtSender();
};

