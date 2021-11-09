#pragma once
#include "RdtReceiver.h"
class GBNRdtReceiver :
    public RdtReceiver
{
private:
	int expectedSeqNum;
	const int seqSize;
	Packet lastAckPkt;


public:
	GBNRdtReceiver(int sSize);
	GBNRdtReceiver();
	~GBNRdtReceiver();

	void receive(const Packet& packet);
};

