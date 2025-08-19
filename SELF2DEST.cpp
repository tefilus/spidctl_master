/*************************************************************************
 *
 * SELF2DEST.cpp
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
#include "SELF2DEST.h"

using namespace CmErrMsg;

const char SELF2DEST::mDataForRead0[SELF2DEST_TRANS_MAX_DATA_LEN] = {0};

/// <summary>constructor</summary>
#ifdef SELF2DEST_TST
SELF2DEST::SELF2DEST(sc_module_name name, FILE *fp): 
	sc_module(name),
	is("is"),
	ts("ts"),
	sys_resetn("sys_resetn"),
	mSelfOffAdd(0),
	mSelfSize(0),
	mPeOffAdd(0),
	mReset(false),
	mRespEventPEQ( "mRespEventPEQ" ),
	tstFp(fp)
#else
SELF2DEST::SELF2DEST(sc_module_name name): 
	sc_module(name),
	is("is"),
	ts("ts"),
	sys_resetn("sys_resetn"),
	mSelfOffAdd(0),
	mSelfSize(0),
	mPeOffAdd(0),
	mRespEventPEQ( "mRespEventPEQ" ),
	mReset(false)
#endif //SELF2DEST_TST
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
SELF2DEST::~SELF2DEST(void) {}

/// <summary>set necessary value which is got by reading configulation file in NSMVG4BARRV01</summary>
/// <param name="penum">the numbar of effective PE</param>
/// <param name="pe_bmid">*pe_bmid is array.Argument is PE index. Value is Bus Master ID</param>
/// <param name="self_off_add">offset address start position of self area</param>
/// <param name="self_size">size of self area</param>
void SELF2DEST::setConfig(unsigned char penum, unsigned char *pe_bmid,  unsigned int self_off_add, unsigned int self_size, unsigned int base_add, unsigned int pe_off_add)
{
#ifdef SELF2DEST_DEBUG
	printf("%s:",sc_time_stamp().to_string().c_str()); fflush(stdout);
        printf("%s::setConfig\n", name()); fflush(stdout);
#endif //SELF2DEST_DEBUG

	for (unsigned char i = 0; i < penum; i++)
	{	
#ifdef SELF2DEST_DEBUG
		printf("  peid=0x%x\tbmid=0x%x\n", i, pe_bmid[i]); fflush(stdout);
#endif //SELF2DEST_DEBUG

		m_mode_bmid_pe[pe_bmid[i]] = i;
		//m_mode_bmid_pe[pe_bmid[i]].valid = bmid_valid[pe_bmid[i]];
	}
	mSelfOffAdd = self_off_add;
	mSelfSize = self_size;
	mBaseAdd = base_add;
	mPeOffAdd = pe_off_add;

#ifdef SELF2DEST_DEBUG
	printf("  mSelfOffAdd=0x%x\tmSelfSize=0x%x\tmBaseAdd=0x%x\tmPeOffAdd=0x%x\n", mSelfOffAdd, mSelfSize, mBaseAdd, mPeOffAdd);
	fflush(stdout);
#endif //SELF2DEST_DEBUG
}

/// <summary>SELF2DEST process this when reset signal was asserted</summary>
void SELF2DEST::assertResetSig (void)
{	
#ifdef SELF2DEST_DEBUG
	printf("\n%s:%s::%s\n", sc_time_stamp().to_string().c_str(),name(),"assertResetSig");
	map<TlmTransactionType*, unsigned int>::iterator it = mConvertAddInfo.begin();
	while (it !=  mConvertAddInfo.end())
	{
		printf("transaction add=0x%x\tconvert add=0x%x\n", (*it).first , (*it).second);fflush(stdout);
		++it;
	}
#endif //SELF2DEST_DEBUG

	//process 
	mReset = true;
	mConvertAddInfo.erase(mConvertAddInfo.begin(), mConvertAddInfo.end());
	
#ifdef SELF2DEST_DEBUG
	printf("CLEAR\n");
	map<TlmTransactionType*, unsigned int>::iterator it1 = mConvertAddInfo.begin();
	while (it !=  mConvertAddInfo.end())
	{
		printf("transaction add=0x%x\tconvert add=0x%x\n", (*it).first , (*it).second);fflush(stdout);
		++it1;
	}
	fflush(stdout);
#endif //SELF2DEST_DEBUG
}

/// <summary>SELF2DEST process this when reset signal was negated</summary>
void SELF2DEST::negateResetSig (void)
{	
	mReset = false;
}

///  <summary>call nb_transport_bw when transaction of nb_transport_fw has invalid Bus Master ID .</summary>
void SELF2DEST::respProcess (void)
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
TlmSyncEnum SELF2DEST::nb_transport_fw( TlmTransactionType& trans,
                                       	TlmPhase& phase, sc_core::sc_time& t)
{
#if defined(SELF2DEST_TST)
	if(tstFp)
	{
    		fprintf(tstFp,"%s:",sc_time_stamp().to_string().c_str());
#if defined(PLTFRM_TST)
    		fprintf(tstFp,"%s\n        ",this->name());
#endif //PLTFRM_TST
    		fprintf(tstFp,"SELF_ADRESS_DECODER:nb_transport_fw() invoked\n");
    		if( trans.get_command() == tlm::TLM_READ_COMMAND )
		{
    		 	 fprintf(tstFp,"        TLM_READ_COMMAND\n");
    		}	
		else
		{
    		  	fprintf(tstFp,"        TLM_WRITE_COMMAND\n");
		}
    		fprintf(tstFp,"        address     =0x%x\n",
    					trans.get_address());
    		int dataLength = trans.get_data_length();
    		fprintf(tstFp,"        dataLength  =%d\n", dataLength);
    		unsigned char *data = trans.get_data_ptr();
    		for(int i=0; i<dataLength; i++)
    		{  	fprintf(tstFp,"        data[%d]     =0x%x\n", i,data[i]);
		}
    		fprintf(tstFp,"        streamWidth=%d\n",
    		        trans.get_streaming_width());
    		if( trans.get_response_status() == tlm::TLM_INCOMPLETE_RESPONSE )
    		{	
			fprintf(tstFp,"        TLM_INCOMPLETE_RESPONSE\n");
    		}
		else
		{
    			fprintf(tstFp,"        responseStatus error\n");
		}
  	}
#endif // SELF2DEST_TST

	//while reset signale is low.
	if (mReset)
	{	
		read0writeNeglect(trans);
		phase = tlm::BEGIN_RESP;
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
		return (tlm::TLM_UPDATED);
	}
	// self to destination
	else if(! convertSelfAdd(trans)) 
    	{
		//read 0 or ignore to write 
		read0writeNeglect(trans); 
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
TlmSyncEnum SELF2DEST::nb_transport_bw( TlmTransactionType& trans,
                               		TlmPhase& phase, sc_core::sc_time& t)
{
#if defined(SELF2DEST_TST)
	if(tstFp)
	{
      		fprintf(tstFp,"%s:",sc_time_stamp().to_string().c_str());
#if defined(PLTFRM_TST)
      		fprintf(tstFp,"%s\n        ",this->name());
#endif //PLTFRM_TST
      		fprintf(tstFp,"SELF_ADDRESS_DECODER:nb_transport_bw() invoked\n");
    	}
#endif //SELF2DEST_TST
	
	//destionation to self

	restoreSelfAdd(trans);
#ifdef SELF2DEST_TEST
	outputAdd(trans, "nb_transport_bw");
#endif //SELF2DEST_TEST
	
	return( ts->nb_transport_bw(trans, phase, t) );
}

/// <summary>transport API for debug access</summary>
/// <param name="r">transaction</param>
/// <returns>data number</returns>
unsigned int SELF2DEST::transport_dbg( TlmTransactionType& r )
{
#if defined(SELF2DEST_TST)
	fflush(stdout);
	if(tstFp)
	{
		fprintf(tstFp,"%s:",sc_time_stamp().to_string().c_str());
#if defined(PLTFRM_TST)
		fprintf(tstFp,"%s\n        ",this->name());
#endif //PLTFRM_TST
		fprintf(tstFp,"SELF_ADDRESS_DECODER:transport_dbg() invoked\n");
		if( r.get_command() == tlm::TLM_READ_COMMAND )
		{
		  	fprintf(tstFp,"        TLM_READ_COMMAND\n");
		}
		else
		{
		  	fprintf(tstFp,"        TLM_WRITE_COMMAND\n");
		}
		fprintf(tstFp,"        address     =0x%x\n",
		        r.get_address());
		int dataLength = r.get_data_length();
		fprintf(tstFp,"        dataLength  =%d\n",
		        dataLength);
		unsigned char *data = r.get_data_ptr();
		for(int i=0; i<dataLength; i++)
		{  
			fprintf(tstFp,"        data[%d]     =0x%x\n",
		        				  i,data[i]);
		}
		fprintf(tstFp,"        streamWidth=%d\n",
		        r.get_streaming_width());
		if( r.get_response_status() == tlm::TLM_INCOMPLETE_RESPONSE )
		{
			fprintf(tstFp,"        TLM_INCOMPLETE_RESPONSE\n");
		}
		else
		{
		  	fprintf(tstFp,"        responseStatus error\n");
		}
		TlmG3mExtension *pG3mExtension = NULL;
		r.get_extension( pG3mExtension );
		if( pG3mExtension )
		{
		 	 fprintf(tstFp,"        IfPointer   =0x%x\n",
		        	  pG3mExtension->getIfPointer());
		}
	}
	fflush(stdout);
#endif // SELF2DEST_TST

	if (! convertSelfAdd(r))
	{
		r.set_response_status(tlm::TLM_OK_RESPONSE);
		read0writeNeglect(r);
	}
	else
	{
		is->transport_dbg( r ) ;
		restoreSelfAdd(r);
	}
#ifdef SELF2DEST_TEST
	outputAdd(r, "nb_transport_dbg");
#endif //SELF2DEST_TEST
	return(r.get_data_length());
}

/// <summary>dummy</summary>
void SELF2DEST::invalidate_direct_mem_ptr( sc_dt::uint64 startRange,
                                  	sc_dt::uint64 endRange )
{
#if 0 // DMI is not supported
  	ts->invalidate_direct_mem_ptr(startRange, endRange);
#endif// DMI is not supported
}

/// <summary>dummy</summary>
bool SELF2DEST::get_direct_mem_ptr( TlmTransactionType& trans,
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
void SELF2DEST::b_transport(TlmTransactionType& trans, 
					sc_core::sc_time& t )
{	
#if defined(SELF2DEST_TST)
	fflush(stdout);
	if(tstFp)
	{
		fprintf(tstFp,"%s:SELF_ADDRESS_DECODER:b_transport() invoked\n",
		        sc_time_stamp().to_string().c_str());
		if( trans.get_command() == tlm::TLM_READ_COMMAND )
		{
		  	fprintf(tstFp,"        TLM_READ_COMMAND\n");
		}
		else
		{
		 	fprintf(tstFp,"        TLM_WRITE_COMMAND\n");
		}
		fprintf(tstFp,"        address     =0x%x\n",
		        trans.get_address());
		int dataLength = trans.get_data_length();
		fprintf(tstFp,"        dataLength  =%d\n",
		        dataLength);
		unsigned char *data = trans.get_data_ptr();
		for(int i=0; i<dataLength; i++)
		{
		  	fprintf(tstFp,"        data[%d]     =0x%x\n",
		        	  i,data[i]);
		}
		fprintf(tstFp,"        streamWidth=%d\n",
		        trans.get_streaming_width());
		if( trans.get_response_status() == tlm::TLM_INCOMPLETE_RESPONSE )
		{
		 	fprintf(tstFp,"        TLM_INCOMPLETE_RESPONSE\n");
		}
		else
		{
			fprintf(tstFp,"        responseStatus error\n");
		}
		TlmG3mExtension *pG3mExtension = NULL;
		trans.get_extension( pG3mExtension );
		if( pG3mExtension )
		{
		 	fprintf(tstFp,"        IfPointer   =0x%x\n",
		          	pG3mExtension->getIfPointer());
		}
	}
	fflush(stdout);
#endif // SELF2DEST_TST

	//if reset signal is low or converttiog self address is unseccessful. 
	if (mReset || (! convertSelfAdd(trans)))
	{	
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
		read0writeNeglect(trans);
	}
	else
	{	
		is->b_transport(trans, t);
		restoreSelfAdd(trans);
	}
#ifdef SELF2DEST_TEST
	outputAdd(trans, "b_transport");
#endif //SELF2DEST_TEST
	
}

/// <summary>align address</summary>
/// <param name="trans">transaction</param>
/// <returns>successful or unsuccessful</returns>
bool SELF2DEST::alignAdd(TlmTransactionType& trans)
{
	unsigned int pre_add = trans.get_address();
	unsigned char data_length = trans.get_data_length();
	unsigned char *data = trans.get_data_ptr();
	if (4 < data_length)
	{
		printWarningMessage(this->name(), "alignAdd", " Barrier Sync register cannot be received transaction of which data length is 5 byte and over.");
		return(false);
	}
        else
	{ 
		if(data_length > 1)
        	{
        	        unsigned int diff = ((pre_add + data_length - 1) & 0xFFFFFFFC) - pre_add;
        	        if( diff < data_length )
        	        {
        	                trans.set_address(pre_add + diff);
				mAlignAddLength[&trans] = data_length;
        	                trans.set_data_length(1);
			
				trans.set_data_ptr(&data[diff]);
			
				mConvertAddInfo[&trans] = pre_add;
        	        }
        	}
		return (true);
	}
}

/// <summary>realign address</summary>
/// <param name="trans">transaction</param>
void SELF2DEST::realignAdd(TlmTransactionType& trans)
{

	if (mConvertAddInfo.find(&trans) != mConvertAddInfo.end() && mAlignAddLength.find(&trans) != mAlignAddLength.end())
        {
		unsigned int align_add = trans.get_address();
		unsigned int pre_add = mConvertAddInfo[&trans];
		unsigned int pre_data_length = mAlignAddLength[&trans];		
		unsigned char diff = align_add - pre_add;
	
		unsigned char* data = trans.get_data_ptr();
                trans.set_data_ptr(&data[-diff]);

		if (trans.get_command() == tlm::TLM_READ_COMMAND)
		{
			data = trans.get_data_ptr();
			for (unsigned int i = 0; i < pre_data_length; i++)
			{
				if (i != diff)
				{
					data[i] = 0;
				}
			}
		}


		trans.set_data_length(pre_data_length);
		mAlignAddLength.erase(&trans);
                trans.set_address(mConvertAddInfo[&trans]);
                mConvertAddInfo.erase(&trans);
        }
}

/// <summary>convert self address to destination address</summary>
/// <param name="trans">transaction</param>
/// <returns>successful or unseccessful</returns>
bool SELF2DEST::convertSelfAdd(TlmTransactionType& trans)
{
	unsigned int pre_add = trans.get_address(); 

#ifdef SELF2DEST_DEBUG
	printf("  base_add=0x%x\t\n",  mBaseAdd); fflush(stdout);
#endif //SELF2DEST_DEBUG

	if (! alignAdd(trans))
	{
		return (false);
	}	


	unsigned int off_add = trans.get_address() - mBaseAdd; 
#ifdef SELF2DEST_DEBUG
	printf("  offadd=0x%x\tmSelfOffAdd=0x%x\tmSelfSize=0x%x\n",  off_add, mSelfOffAdd, mSelfSize); fflush(stdout);
#endif //SELF2DEST_DEBUG
	//Is the address in self area?
	
	if (mSelfOffAdd <= off_add && off_add < mSelfOffAdd + mPeOffAdd)
	{	
		TlmG3mExtension *G3mExt;
		trans.get_extension(G3mExt);
		unsigned char bmid = G3mExt->getPeId(); 

		//SCHEAP don't support debug access via self area 
		if (bmid == DEBUG_MASTER_BMID)
		{
			printWarningMessage(this->name(), "convertSelfAdd", "Cannot access self register at debug access mode.");
			return(false);
		}
		//Does the bus master id exist? 
		else if (m_mode_bmid_pe.find(bmid)  == m_mode_bmid_pe.end()) //&& m_mode_bmid_pe[bmid].valid == true)
		{	
			char warning_msg_buf[1024];
			sprintf(warning_msg_buf, "%d is invalid Bus Master ID.", bmid);
			printWarningMessage(this->name(),"convertSelfAdd", warning_msg_buf);
			return(false);
		}
		else
		{
			//convert address in transaction.
			trans.set_address(mBaseAdd + off_add + mSelfSize + mPeOffAdd * m_mode_bmid_pe[bmid]);
#ifdef SELF2DEST_DEBUG
			printf("  get_convertAdd=0x%x\n", trans.get_address()); fflush(stdout);
#endif //SELF2DEST_DEBUG
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
void SELF2DEST::restoreSelfAdd(TlmTransactionType& trans)
{	
	realignAdd(trans);

	if (mConvertAddInfo.find(&trans) != mConvertAddInfo.end())
	{	
		trans.set_address(mConvertAddInfo[&trans]);
		mConvertAddInfo.erase(&trans);	
	}	
}

/// <summary>read 0 or ignore to write</summary>
/// <param name="trans">transaction</param>
/// <returns>none</returns>
void SELF2DEST::read0writeNeglect (TlmTransactionType& trans)
{	
	if (trans.get_command() == tlm::TLM_READ_COMMAND)
        {
                memcpy( trans.get_data_ptr(), mDataForRead0, trans.get_data_length() );
        }
}

#ifdef SELF2DEST_TEST
void SELF2DEST::outputAdd (TlmTransactionType& trans, char str[128])
{
	printf("%s:",sc_time_stamp().to_string().c_str()); fflush(stdout);
	printf("%s::%s\n", name(), str); fflush(stdout);
	if( trans.get_command() == tlm::TLM_READ_COMMAND )
        {
                printf("  TLM_READ_COMMAND\n");fflush(stdout);
        }
        else
        {
                printf("  TLM_WRITE_COMMAND\n");fflush(stdout);
        }
	int data_length = trans.get_data_length();
        unsigned char *data = trans.get_data_ptr();

	TlmG3mExtension *G3mExt;
        trans.get_extension(G3mExt);
        unsigned char bmid = G3mExt->getPeId();

        printf("  transaction=0x%x\n", (int)&trans, bmid); fflush(stdout);
        printf("  address=0x%x\n  BMID=0x%x\n", (int)trans.get_address(), bmid); fflush(stdout);
	for (unsigned int i = 0; i < data_length; i++)
	{
		printf("  data[%d]=0x%x\n",i, data[i]); fflush(stdout);
	}
}
#endif //SELF2DEST_TEST
