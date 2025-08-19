/*************************************************************************
 *
 * SELF2DEST_BASE.h
 *
 * Copyright(c) 2016 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#ifndef __SELF2DEST_BASE__
#define __SELF2DEST_BASE__

#include "OSCI2.h"
#include "global.h"
#include <tlm_utils/peq_with_get.h>

#define SELF2DEST_BASE_TRANS_MAX_DATA_LEN 32


using namespace std;

class SELF2DEST_BASE 
: 
public sc_module,
public virtual TlmFwTransportIf,
public virtual TlmBwTransportIf 
{
public:

//constructor destructor
	SC_HAS_PROCESS(SELF2DEST_BASE);

	SELF2DEST_BASE(sc_module_name name = "SELF2DEST_BASE"); 
	~SELF2DEST_BASE(void);

//pure virtual function
	TlmSyncEnum nb_transport_fw( TlmTransactionType& trans,
				       TlmPhase& phase, sc_core::sc_time& t);
	TlmSyncEnum nb_transport_bw( TlmTransactionType& trans,
			       TlmPhase& phase, sc_core::sc_time& t);
	unsigned int transport_dbg( TlmTransactionType& r );
	void invalidate_direct_mem_ptr( sc_dt::uint64 startRange,
				  sc_dt::uint64 endRange );
	bool get_direct_mem_ptr( TlmTransactionType& trans,
			   tlm::tlm_dmi&       dmiData );
	void b_transport( TlmTransactionType& trans, sc_core::sc_time& t );

//func
	void setConfig(unsigned char penum, unsigned char *pe_bmid,  unsigned int self_off_add, unsigned int self_size, ADDRESS_TYPE base_address, unsigned int pe_off_add);

//variable
	TlmInitiatorSocket<BUS_WIDTH_VCI32> is;  // initiator socket
  	TlmTargetSocket<BUS_WIDTH_VCI32>    ts;  // target socket	

	sc_in<bool> sys_resetn; // reset port

protected :
	map<unsigned char, unsigned char> m_mode_bmid_pe; //Argument is Bus Master ID. Value is PE index
	map<TlmTransactionType*, ADDRESS_TYPE> mConvertAddInfo; //Argument is the pointer of transaction. Value is address of the previous to convert.

	unsigned int mSelfOffAdd; // offset address start position of self area
	unsigned int mSelfSize;	// size of self area
	ADDRESS_TYPE mBaseAdd; // base address
	unsigned int mPeOffAdd; // offset address of PE
	bool mReset; // While reset signal is low, mReset is true.
	tlm_utils::peq_with_get< TlmTransactionType > mRespEventPEQ; // associate transaction with event. use this when transaction of nb_transport_fw has invalid Bus Master ID. 

//func	
	virtual bool convertSelfAdd (TlmTransactionType& trans);
	virtual void restoreSelfAdd(TlmTransactionType& trans);
	virtual void processToAccessInvalidBmid (TlmTransactionType& trans);

	//SC_METHOD
	void assertResetSig(void);
	void negateResetSig(void);
	void respProcess (void);


#ifdef SELF2DEST_BASE_UT_TEST
	void outputLog (const char *this_name
			, const char *func_name
			, TlmTransactionType& trans
			, bool debug);
#endif //SELF2DEST_BASE_UT_TEST
};
#endif
