/*************************************************************************
 *
 * G4_CommonPrototype.h
 *
 * Copyright(c) 2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#ifndef __G4_CommonPrototype__
#define __G4_CommonPrototype__

#include "OSCI2.h"
#include "global.h"

extern sc_time_unit glb_resolution_unit;

class G4_CommonPrototype : public sc_module
{
public:
//constructor destructor
	SC_HAS_PROCESS(G4_CommonPrototype);

	G4_CommonPrototype(sc_module_name name, const char* clk_name); 
	~G4_CommonPrototype(void);

//function
public: 
	void calCycle(void);
	unsigned char read_config_file_PeNum(const char* filename, const char* class_name, unsigned char init_val_penum);
protected:

//variable
public:
	sc_in<sc_dt::uint64> clk; //clock port
protected:
	sc_dt::uint64 mCycle; //1 cycle

};
#endif //__G4_CommonPrototype__
