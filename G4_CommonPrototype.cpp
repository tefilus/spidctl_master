/*************************************************************************
 *
 * G4_CommonPrototype.cpp
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

#include <map>

#include "CmErrMsg.h"
#include "G4_CommonPrototype.h"

using namespace CmErrMsg;

extern int token_to_int(char *token, const char *moduleName, const char *error_message);
extern unsigned int token_to_uint(char *token, const char *moduleName, const char *error_message);
extern unsigned long token_to_ulong(char *token, const char *moduleName, const char *error_message);

/// <summary>constructor</summary>
/// <param name="name">sc_module_name</param>
/// <param name="clk_name">clock name</param>
G4_CommonPrototype::G4_CommonPrototype(sc_module_name name, const char* clk_name): 
	sc_module(name)
	,clk(clk_name)
	,mCycle(0xFFFFFFFFFFFFFFFFULL)
{
        SC_METHOD(calCycle);
                sensitive << clk;
                dont_initialize();	
}

/// <summary>destructor</summary>
G4_CommonPrototype::~G4_CommonPrototype(void) {}


/// <summary>calculate 1 cycle</summary>
void G4_CommonPrototype::calCycle (void)
{
        if( clk.read() > 0 )
        {
                switch(glb_resolution_unit)
                {
                        case SC_SEC:
                        mCycle = (uint64)(1.0)/clk.read();
                        break;
                        case SC_MS:
                        mCycle = (uint64)(1.0e+3)/clk.read();
                        break;
                        case SC_US:
                        mCycle = (uint64)(1.0e+6)/clk.read();
                        break;
                        case SC_NS:
                        mCycle = (uint64)(1.0e+9)/clk.read();
                        break;
                        case SC_PS:
                        mCycle = (uint64)(1.0e+12)/clk.read();
                        break;
                        case SC_FS:
                        mCycle = (uint64)(1.0e+15)/clk.read();
                        break;
                }
        }
	else
	{
		mCycle = 0xFFFFFFFFFFFFFFFFULL;
	}
}

///<summary>read the configulation file to get effective PE number</summary>
///<param name="filename">file name of configulation file</param>
///<param name="class_name">Name of the class that inherits from this class</param>
///<param name="init_val_penum">initialize value of PENUM</param>
unsigned char G4_CommonPrototype::read_config_file_PeNum(const char* filename, const char* class_name, unsigned char init_val_penum)
{
	char    word[512];
  char    seps[] = " ,=();\t\n\r";
  char    *token;
  bool      error_detected = false;
  unsigned char penum = init_val_penum;

  ifstream  config_file(filename);
  // read the config file
  while(1) {
    config_file.getline(word, 512, '\n');
    token = strtok(word, seps);


    // ----------------------------------------
    // For EOF
    if (config_file.eof()) {
      break;
    }

    // ----------------------------------------
    // For comment
    if ((token == NULL) || (strncmp(token, "//", 2) == 0)) {
      continue;
    }

    // ----------------------------------------
    // detect end marker
    // NOTICE:You have to check whether token is NULL first.
    if (strcmp(token, "[END]") == 0) {
      break;
    }

    // ----------------------------------------
    // get token
    // ----------------------------------------
    // For miscellaneous
    if (strcmp(token, "[PE_NUM]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        penum = token_to_uint( token,
				class_name,
				"[PE_NUM]" );
	if(( penum > PE_MAX_NUM ) || ((penum !=1) && ((penum % 2) !=0)) || ( penum == 0 )){
	  char error_message_buf[1024];
	  // for WinMsg
	  sprintf(error_message_buf,
		  "[PE_NUM] must be 1, 2, 4, 6, or 8, but specified [%d]."
		  , penum);
	  printErrorMessage(this->name(), "read_config_file_PeNum", error_message_buf);
	  exit(1);
	}
      }
      continue;
    }
  }  // end of configuration file read loop(infinite)
  return penum;	
}

