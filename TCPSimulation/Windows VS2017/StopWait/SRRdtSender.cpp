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
		if (isInWindow(i) && i >= nextSeqnum)			//���ã�δ����
			std::cout << "����";
		else if (isInWindow(i) && pktSta[i])		//���ͣ���ȷ��
			std::cout << "��ȷ��";
		else if (isInWindow(i))						//�ѷ��ͣ�δȷ��
			std::cout << "�ѷ���";
		if (i == (base + N) % seqSize)
			std::cout << "]";
		if (!isInWindow(i))
			std::cout << "������";						//�����ã�������
		std::cout << " ";
	}
	std::cout << std::endl;
}

//�ж�����Ƿ��ڴ�����
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
	{//�����������ȴ�ack
		std::cout << "Sender:���������������������Ե�\n\n";
		return false;
	}
	packets[nextSeqnum].acknum = -1;
	packets[nextSeqnum].seqnum = nextSeqnum;
	memcpy(packets[nextSeqnum].payload, message.data, sizeof(message.data));
	packets[nextSeqnum].checksum = pUtils->calculateCheckSum(packets[nextSeqnum]);
	pUtils->printPacket("Sender:���ͷ����ͱ���", packets[nextSeqnum]);
	//��ʾ��������
	std::cout << "\nSender:����ǰ���ڣ�";
	PrintSlideWindow();
	//���ͱ���
	pns->sendToNetworkLayer(RECEIVER, packets[nextSeqnum]);
	//������ʱ����srЭ����ÿ�������Ӧһ����ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT, nextSeqnum);
	//������ϣ�����״̬
	nextSeqnum = (nextSeqnum + 1) % seqSize;//������ȡģ
	std::cout << "Sender:���ͺ󴰿ڣ�";
	PrintSlideWindow();
	std::cout << std::endl;
	return true;
}

void SRRdtSender::receive(const Packet& ackPkt)
{
	int checksum = pUtils->calculateCheckSum(ackPkt);
	if (checksum != ackPkt.checksum)
	{
		pUtils->printPacket("Sender:���յ�ack�𻵣�У��Ͳ����", ackPkt);
		return;
	}
	else
	{
		pns->stopTimer(SENDER, ackPkt.acknum);
		if (isInWindow(ackPkt.acknum))
		{//���´���
			pktSta[ackPkt.acknum] = true;
			while (pktSta[base] == true)
			{//�ƶ�base
				pktSta[base] = false;
				base = (base + 1) % seqSize;
			}
			std::cout << "\nSender:�յ�ack�����������ƶ���";
			PrintSlideWindow();
			std::cout << std::endl;
		}
	}
}

void SRRdtSender::timeoutHandler(int seqnum)
{
	std::cout << "\nSender:���ݰ���ʱ��" << seqnum << "\n\n";
	//���ù���������ţ����շ��л���������ʹʧ��Ҳ��Ҫ��
	pns->sendToNetworkLayer(RECEIVER, packets[seqnum]);
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqnum);
	std::cout << "\nSender:�ط����ݰ����:" << seqnum << "\n\n";
}
