/*
* Copyright(c) 2012 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef DUMMY_MASTER_H
#define DUMMY_MASTER_H

#include "OSCI2.h"    // class for User Modeling environment based on TLM2.0

template< unsigned int BUSWIDTH >
class DummyMaster : public sc_module, public virtual TlmBwTransportIf
{
  public:
    // bus socket : TlmInitiatorSocket
    TlmInitiatorSocket<BUSWIDTH> is;

    // constructor
    DummyMaster( sc_module_name module_name="DummyMaster" ):is("is")
    { is(*this); }

    // destructor
    ~DummyMaster()
    {}

    void invalidate_direct_mem_ptr( sc_dt::uint64 startRange,
				    sc_dt::uint64 endRange )
    {}

    TlmSyncEnum nb_transport_bw(TlmTransactionType& trans, 
				TlmPhase&           phase, 
				sc_core::sc_time&   t)
    { return tlm::TLM_UPDATED; }

};

#endif /* DUMMY_MASTER_H */
