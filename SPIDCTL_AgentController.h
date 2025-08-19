// ---------------------------------------------------------------------
// $Id: SPIDCTL_AgentController.h,v 1.2 2021/11/18 07:22:52 minhquocha Exp $
//
// Copyright(c) 2020-2021 Renesas Electronics Corporation
// Copyright(c) 2020-2021 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of SPIDCTL_AgentController.h
// Ref: {SPIDCTL_UD_Copyright_001}
#ifndef __SPIDCTL_AGENT_CONTROLLER_H__
#define __SPIDCTL_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "rvc_common_model.h"

#define MAX_SPID            64    // Max number of SPID

// Description: Declaration of SPIDCTL_AgentController class
// Ref: {SPIDCTL_UD_ATT1_004}
class SPIDCTL_AgentController: public rvc_common_model
{
public:
    // Constructor/Destructor
    SPIDCTL_AgentController(std::string name): rvc_common_model(name) { ; }
    virtual ~SPIDCTL_AgentController(void) { ; }

    // Variable
    std::map<unsigned int, unsigned int> mSPIDInitValMap;

    // Interface (APIs)
    virtual bool GetResetStatus(void) = 0;
    virtual bool CheckClockPeriod(void) = 0;
    virtual void NotifyUpdateSPIDValue(unsigned int Index) = 0;
    virtual bool IsKeyCodeEnable(void) = 0;
    virtual void SetKeyCodeEnable(bool isEnable) = 0;
    virtual bool IsMaskLocked(unsigned int RegIndex) = 0;
    virtual void SetMaskLocked(unsigned int RegIndex, bool isLocked) = 0;
};
#endif //__SPIDCTL_AGENT_CONTROLLER_H__
