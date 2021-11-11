// StopWait.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "StopWaitRdtSender.h"
#include "StopWaitRdtReceiver.h"
#include "GBNRdtReceiver.h"
#include "GBNRdtSender.h"
#include "SRRdtSender.h"
#include "SRRdtReceiver.h"
#include "TCPRdtSender.h"

int main(int argc, char* argv[])
{
	RdtSender *ps = new StopWaitRdtSender();
	RdtReceiver * pr = new StopWaitRdtReceiver();
	RdtSender* gbns = new GBNRdtSender();
	RdtReceiver* gbnr = new GBNRdtReceiver();
	RdtSender* srs = new SRRdtSender();
	RdtReceiver* srr = new SRRdtReceiver();
	RdtSender* tcps = new TCPRdtSender();
	RdtReceiver* tcpr = new GBNRdtReceiver();
	pns->setRunMode(0);  //VERBOSģʽ
	// pns->setRunMode(1);  //����ģʽ
	pns->init();
	pns->setRtdSender(gbns);
	pns->setRtdReceiver(gbnr);
	pns->setInputFile(".\\Data\\input.txt");
	pns->setOutputFile(".\\Data\\output.txt");

	pns->start();

	delete ps;
	delete pr;
	delete gbns;
	delete gbnr;
	delete srs;
	delete srr;
	delete tcps;
	delete tcpr;
	delete pUtils;									//ָ��Ψһ�Ĺ�����ʵ����ֻ��main��������ǰdelete
	delete pns;										//ָ��Ψһ��ģ�����绷����ʵ����ֻ��main��������ǰdelete
	
	return 0;
}

