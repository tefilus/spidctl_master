#pragma once
#include "BusMasterIf.h"
#include "BusMasterFuncIf.h"

class SPIDCTL_MASTER_Func : public BusMasterFuncIf {
public:
	SPIDCTL_MASTER_Func(BusMasterIf<32>*);
	~SPIDCTL_MASTER_Func(void);
	void transportProcess(void);
	void notifyTransSend(TlmTransactionType& trans, BusTime_t& t);
	void notifyTransComp(TlmTransactionType& trans, BusTime_t& t);

private:
	BusMasterIf<32>* mBusIf;
	unsigned char mData[1024];
	TlmBasicPayload mPayload;
	TlmG3mExtension mG3mExt;
};

