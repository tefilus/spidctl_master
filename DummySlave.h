/*
* Copyright(c) 2013 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef DUMMY_SLAVE_H
#define DUMMY_SLAVE_H

#include "OSCI2.h"    // class for User Modeling environment based on TLM2.0

template< unsigned int BUSWIDTH >
class DummySlave : public sc_module, public virtual TlmFwTransportIf
{
  public:
    // bus socket : TlmInitiatorSocket
    TlmTargetSocket<BUSWIDTH> ts;

    // constructor
    DummySlave( sc_module_name module_name="DummySlave" ):ts("ts")
    { ts(*this); }

    // destructor
    ~DummySlave()
    {}

    TlmSyncEnum nb_transport_fw(TlmTransactionType& trans, 
				TlmPhase&           phase, 
				sc_core::sc_time&   t)
    { return tlm::TLM_UPDATED; }

    void b_transport(TlmTransactionType& trans, 
		     sc_core::sc_time&   t)
    {}

    bool get_direct_mem_ptr(TlmTransactionType& trans,
			    tlm::tlm_dmi&  dmi_data)
    { return false; }

    unsigned int transport_dbg(TlmTransactionType& trans)
    { return 0; }

};

#endif /* DUMMY_SLAVE_H */
