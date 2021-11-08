#pragma once
#include "RdtSender.h"
#include "DataStructure.h"
#include <vector>

class GBNRdtSender:public RdtSender
{
private:
	int nextSeqnum;		//��һ��Ҫ���͵�Packet�ı��
	int base;			//�������ڵ�base
	const int N;		//�������ڵĴ�С������Ϊ4
	const int SeqNumMax; //ָʾPacket��ŵ�λ��������Ϊ8
	std::vector<Packet> packets;	//���淢�͵ı��������ش�
	
	void PrintSlideWindow();	//��ӡ������������debug


public:

	bool getWaitingState();
	bool send(const Message& message);						//����Ӧ�ò�������Message����NetworkServiceSimulator����,������ͷ��ɹ��ؽ�Message���͵�����㣬����true;�����Ϊ���ͷ����ڵȴ���ȷȷ��״̬���ܾ�����Message���򷵻�false
	void receive(const Packet& ackPkt);						//����ȷ��Ack������NetworkServiceSimulator����	
	void timeoutHandler(int seqNum);					//Timeout handler������NetworkServiceSimulator����

public:
	GBNRdtSender();
	GBNRdtSender(int _N, int _SeqNumMax);
	virtual ~GBNRdtSender();
};

