#pragma once
#include "RdtReceiver.h"
#include <vector>
class SRRdtReceiver
	:public RdtReceiver
{
	//SR协议中接收方也需要缓冲区和滑动窗口
private:
	const int N;
	const int seqSize;
	Packet lastAckPkt;
	std::vector<Packet> packets;
	std::vector<bool> status;
	int base;

	void PrintSlideWindow();
	bool inWin(int i);
public:
	SRRdtReceiver();
	SRRdtReceiver(int _N, int _seqSize);
	void receive(const Packet& packet);
	~SRRdtReceiver();
};

