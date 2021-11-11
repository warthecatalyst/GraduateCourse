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
		std::cout << "SENDER:���������������������Ե�\n\n";
		return false;
	}
	else
	{
		packets[nextSeqnum].acknum = -1;//����ط���̫���������䷢�����շ���ackӦ��û�ð�
		packets[nextSeqnum].seqnum = nextSeqnum;//���к�
		memcpy(packets[nextSeqnum].payload, message.data, sizeof(message.data));
		packets[nextSeqnum].checksum = pUtils->calculateCheckSum(packets[nextSeqnum]);
		pUtils->printPacket("[Debug]���ͷ����ͱ���", packets[nextSeqnum]);
		//��ʾ��������
		std::cout << "\n[SENDER]����ǰ���ڣ�";
		PrintSlideWindow();
		if (base == nextSeqnum)
		{//gbnЭ����ֻ��һ����ʱ�����������ĸ����鳬ʱ�������ش�����δȷ�ϵķ���
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
		}
		pns->sendToNetworkLayer(RECEIVER, packets[nextSeqnum]);
		//������ϣ�����״̬
		nextSeqnum = (nextSeqnum + 1) % seqSize;//��Ҫ����ȡģ����
		//��ʾ��������
		std::cout << "[SENDER]���ͺ󴰿ڣ�";
		PrintSlideWindow();
		std::cout << std::endl;
		return true;
	}
}

void TCPRdtSender::timeoutHandler(int seqNum)
{
	std::cout << "\nSender:���ͳ�ʱ,���·������ݰ�" << std::endl;

	pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
	for (int i = base; i != nextSeqnum; i = (i + 1) % seqSize) {
		pns->sendToNetworkLayer(RECEIVER, packets[i]);
		pUtils->printPacket("Sender:���ͷ����·��ͷ���:", packets[i]);
	}

	std::cout << "\nSender:�ط����" << std::endl;
}

void TCPRdtSender::receive(const Packet& ackPkt)
{
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	if (checkSum != ackPkt.checksum)
	{
		pUtils->printPacket("[Debug]���յ�ack�𻵣�У��Ͳ����", ackPkt);
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
			std::cout << "\n[SENDER]�յ�ack:" << ackPkt.acknum << "�����������ƶ���";
			PrintSlideWindow();
			std::cout << std::endl;
		}
		else
		{//�Ѿ�ȷ�ϵ������ack
			dupAckNum = (dupAckNum + 1) % 3;
			if (dupAckNum == 2)
			{//�����ش�
				pns->sendToNetworkLayer(RECEIVER, packets[base]);
				std::cout << "\nSENDER:�յ�������������ack�������ش�\n\n";
			}
		}
	}
}