/*************************************************************************
 *
 * SELF2DEST_BASE.cpp
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

#include <map>

#include "CmErrMsg.h"
#include "SELF2DEST_BASE.h"

using namespace CmErrMsg;


/// <summary>constructor</summary>
SELF2DEST_BASE::SELF2DEST_BASE(sc_module_name name): 
	sc_module(name),
	is("is"),
	ts("ts"),
	sys_resetn("sys_resetn"),
	mSelfOffAdd(0),
	mSelfSize(0),
	mPeOffAdd(0),
	mRespEventPEQ( "mRespEventPEQ" ),
	mReset(false)
{

	is( *this );
	ts( *this );

	SC_METHOD(assertResetSig);
		sensitive << sys_resetn.neg();
		dont_initialize();
	SC_METHOD(negateResetSig);
		sensitive << sys_resetn.pos();
		dont_initialize();
	SC_METHOD(respProcess);
		sensitive << mRespEventPEQ.get_event();
		dont_initialize();
}

/// <summary>destructor</summary>
SELF2DEST_BASE::~SELF2DEST_BASE(void) {}

/// <summary>set necessary value which is got by reading configulation file in NSMVG4BARRV01</summary>
/// <param name="penum">the numbar of effective PE</param>
/// <param name="pe_bmid">*pe_bmid is array.Argument is PE index. Value is Bus Master ID</param>
/// <param name="self_off_add">offset address start position of self area</param>
/// <param name="self_size">size of self area</param>
void SELF2DEST_BASE::setConfig(unsigned char penum, unsigned char *pe_bmid,  unsigned int self_off_add, unsigned int self_size, ADDRESS_TYPE base_add, unsigned int pe_off_add)
{
	for (unsigned char i = 0; i < penum; i++)
	{	
		m_mode_bmid_pe[pe_bmid[i]] = i;
	}
	mSelfOffAdd = self_off_add;
	mSelfSize = self_size;
	mBaseAdd = base_add;
	mPeOffAdd = pe_off_add;
}

/// <summary>SELF2DEST_BASE process this when reset signal was asserted</summary>
void SELF2DEST_BASE::assertResetSig (void)
{
#ifdef SELF2DEST_BASE_UT_TEST
        printf("\n%s:%s::%s\n", sc_time_stamp().to_string().c_str()
		,name(),"assertResetSig");
        map<TlmTransactionType*, ADDRESS_TYPE>::iterator 
			it = mConvertAddInfo.begin();
        while (it !=  mConvertAddInfo.end())
        {
                printf("transaction add=0x%llx\tconvert add=0x%llx\n"
			, (*it).first , (*it).second);
		fflush(stdout);
                ++it;
        }
#endif //SELF2DEST_BASE_UT_TEST
	
	//process 
	mReset = true;
	mConvertAddInfo.erase(mConvertAddInfo.begin(), mConvertAddInfo.end());

#ifdef SELF2DEST_BASE_UT_TEST
        printf("CLEAR\n");
        map<TlmTransactionType*, ADDRESS_TYPE>::iterator 
		it1 = mConvertAddInfo.begin();
        while (it1 !=  mConvertAddInfo.end())
        {
                printf("transaction add=0x%x\tconvert add=0x%x\n"
			, (*it1).first , (*it1).second);
		fflush(stdout);
                ++it1;
        }
        fflush(stdout);
#endif //SELF2DEST_BASE_UT_TEST
}

/// <summary>SELF2DEST_BASE process this when reset signal was negated</summary>
void SELF2DEST_BASE::negateResetSig (void)
{	
	mReset = false;
}

///  <summary>call nb_transport_bw when transaction of nb_transport_fw has invalid Bus Master ID .</summary>
void SELF2DEST_BASE::respProcess (void)
{
	TlmTransactionType *trans;
  	TlmPhase            phase;
  	sc_time             t;

	while ((trans = mRespEventPEQ.get_next_transaction()) != 0)
	{
    		phase = tlm::BEGIN_RESP;
    		t     = SC_ZERO_TIME;
    		trans->set_response_status(tlm::TLM_OK_RESPONSE);
		ts->nb_transport_bw(*trans, phase, t );
	}
}

/// <summary>transport API for forward pass of AT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="phase">TLM phase</param>
  /// <param name="t">time</param>
  /// <returns>synchronization information</returns>
TlmSyncEnum SELF2DEST_BASE::nb_transport_fw( TlmTransactionType& trans,
                                       	TlmPhase& phase, sc_core::sc_time& t)
{
#ifdef SELF2DEST_BASE_UT_TEST
	outputLog(this->name(), "nb_transport_fw", trans, false);
#endif //SELF2DEST_BASE_UT_TEST

	//while reset signale is low.
	if (mReset)
	{	
		processToAccessInvalidBmid(trans);
		phase = tlm::BEGIN_RESP;
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
		return (tlm::TLM_UPDATED);
	}
	// self to destination
	else if(! convertSelfAdd(trans)) 
    	{
		//read 0 or ignore to write 
		processToAccessInvalidBmid(trans); 
		phase = tlm::END_REQ;
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
		mRespEventPEQ.notify( trans );
		return(tlm::TLM_ACCEPTED );
  	}
	else	
	{
  		return(is->nb_transport_fw(trans,phase,t));
	}
}

/// <summary>transport API for backward pass of AT</summary>
/// <param name="trans">transaction</param>
/// <param name="phase">TLM phase</param>
/// <param name="t">time</param>
/// <returns>synchronization information</returns>
TlmSyncEnum SELF2DEST_BASE::nb_transport_bw( TlmTransactionType& trans,
                               		TlmPhase& phase, sc_core::sc_time& t)
{
#ifdef SELF2DEST_BASE_UT_TEST
	outputLog(this->name(), "nb_transport_bw", trans, false);
#endif //SELF2DEST_BASE_UT_TEST
	
	//destionation to self

	restoreSelfAdd(trans);
	
	return( ts->nb_transport_bw(trans, phase, t) );
}

/// <summary>transport API for debug access</summary>
/// <param name="r">transaction</param>
/// <returns>data number</returns>
unsigned int SELF2DEST_BASE::transport_dbg( TlmTransactionType& r )
{
#ifdef SELF2DEST_BASE_UT_TEST
	outputLog(this->name(), "transport_dbg", r, true);
#endif //SELF2DEST_BASE_UT_TEST
	
	if(mReset && r.get_command() == tlm::TLM_WRITE_COMMAND)
	{
		r.set_response_status(tlm::TLM_OK_RESPONSE);
	}
	else if (! convertSelfAdd(r))
	{
		r.set_response_status(tlm::TLM_OK_RESPONSE);
		processToAccessInvalidBmid(r);
	}
	else
	{
		is->transport_dbg( r ) ;
		restoreSelfAdd(r);
	}
	return(r.get_data_length());
}

/// <summary>dummy</summary>
void SELF2DEST_BASE::invalidate_direct_mem_ptr( sc_dt::uint64 startRange,
                                  	sc_dt::uint64 endRange )
{
#if 0 // DMI is not supported
  	ts->invalidate_direct_mem_ptr(startRange, endRange);
#endif// DMI is not supported
}

/// <summary>dummy</summary>
bool SELF2DEST_BASE::get_direct_mem_ptr( TlmTransactionType& trans,
                           		tlm::tlm_dmi&       dmiData )
{
#if 0 // DMI is not supportd
  	if( !decode(trans,false) ){
    	return( false );
  	}
  	return( mpInterface->get_direct_mem_ptr(trans,dmiData) );
#else // DMI is not supported
  	return( false );
#endif// DMI is not supported
}

/// <summary>transport API for LT</summary>
/// <param name="trans">transaction</param>
/// <param name="t">time</param>
void SELF2DEST_BASE::b_transport(TlmTransactionType& trans, 
					sc_core::sc_time& t )
{	
#ifdef SELF2DEST_BASE_UT_TEST
	outputLog(this->name(), "b_transport", trans, false);
#endif //SELF2DEST_BASE_UT_TEST
	//if reset signal is low or converttiog self address is unseccessful. 
	if (mReset || (! convertSelfAdd(trans)))
	{	
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
		processToAccessInvalidBmid(trans);
	}
	else
	{	
		is->b_transport(trans, t);
		restoreSelfAdd(trans);
	}
}



/// <summary>convert self address to destination address</summary>
/// <param name="trans">transaction</param>
/// <returns>successful or unseccessful</returns>
bool SELF2DEST_BASE::convertSelfAdd(TlmTransactionType& trans)
{
	ADDRESS_TYPE pre_add = trans.get_address(); 
	unsigned int off_add = trans.get_address() - mBaseAdd; 
	//Is the address in self area?
	if (mSelfOffAdd <= off_add && off_add < mSelfOffAdd + mPeOffAdd)
	{	
		TlmG3mExtension *G3mExt;
		trans.get_extension(G3mExt);
		unsigned char bmid = G3mExt->getPeId(); 

		//SCHEAP don't support debug access via self area 
		if (bmid == DEBUG_MASTER_BMID)
		{
			return(false);
		}
		//Does the bus master id exist? 
		else if (m_mode_bmid_pe.find(bmid)  == m_mode_bmid_pe.end())
		{
			char warning_msg_buf[1024];
			sprintf(warning_msg_buf, 
				"%d is invalid Bus Master ID.", bmid);
			printWarningMessage(this->name(),
					"convertSelfAdd", warning_msg_buf);
			return(false);
		}
		else
		{
			//convert address in transaction.
			trans.set_address(mBaseAdd + off_add + mSelfSize 
					+ mPeOffAdd * m_mode_bmid_pe[bmid]);
			//Save the infomation of address convertted
			mConvertAddInfo[&trans] = pre_add;
			return(true);	
		}
	}
	else
	{
		return(true);
	}
}

/// <summary>restore address converted</summary>
/// <param name="trans">transaction</param>
/// <returns>none</returns>
void SELF2DEST_BASE::restoreSelfAdd(TlmTransactionType& trans)
{	
	if (mConvertAddInfo.find(&trans) != mConvertAddInfo.end())
	{	
		trans.set_address(mConvertAddInfo[&trans]);
		mConvertAddInfo.erase(&trans);	
	}	
}

/// <summary>read 0 or ignore to write</summary>
/// <param name="trans">transaction</param>
/// <returns>none</returns>
void SELF2DEST_BASE::processToAccessInvalidBmid (TlmTransactionType& trans)
{	
	if (trans.get_command() == tlm::TLM_READ_COMMAND)
        {
		unsigned char *data = trans.get_data_ptr();
		for (unsigned int i = 0; i < trans.get_data_length(); i++)
		{
			data[i] = 0;
		}
        }
}

#ifdef SELF2DEST_BASE_UT_TEST
/// <summary>Debugging function</summary>
void SELF2DEST_BASE::outputLog (const char *this_name
                                        , const char *func_name
					, TlmTransactionType& trans
					, bool debug)
{
	unsigned char bmid;
	if (! debug)
	{
        	TlmG3mExtension *G3mExt;
        	trans.get_extension(G3mExt);
        	bmid = G3mExt->getPeId();
	}

	printf("%s:%s::%s\n"
        	,sc_time_stamp().to_string().c_str()
        	,this_name
        	,func_name);
	if (! debug)
	{
		printf ("\tbmid=0x%x\n", bmid);
	}
	printf ("\taddress=0x%llx\n", trans.get_address());
	printf("\ttransaction_add=0x%llx\n", &trans);
	unsigned char *data = trans.get_data_ptr();
	for (unsigned char i = 0; i <  trans.get_data_length(); i++)
        {
                printf("\tdata[%d]=0x%llx\n", i, data[i]);
        }
        fflush(stdout);
}
#endif //SELF2DEST_BASE_UT_TEST
