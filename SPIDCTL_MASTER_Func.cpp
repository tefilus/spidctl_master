#include "SPIDCTL_MASTER_Func.h"

SPIDCTL_MASTER_Func::SPIDCTL_MASTER_Func(BusMasterIf<32>* busIf)
	: mBusIf(busIf)
{
	int i;
	for (i = 0; i < (int)1024; i++) mData[i] = 0;
	mPayload.set_extension(&mG3mExt);
}

SPIDCTL_MASTER_Func::~SPIDCTL_MASTER_Func(void)
{
}

void SPIDCTL_MASTER_Func::transportProcess(void)
{
	mPayload.set_address(0xFF0A8000 + 0x00);	// set payload
	mBusIf->write(mPayload);					//write from socket
	mBusIf->read(mPayload);						//read from socket
}

void SPIDCTL_MASTER_Func::notifyTransComp(TlmTransactionType& trans, BusTime_t& t)
{
}

void SPIDCTL_MASTER_Func::notifyTransSend(TlmTransactionType& trans, BusTime_t& t)
{
}
