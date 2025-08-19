// ---------------------------------------------------------------------
// $Id: rvc_common_model.h,v 1.6 2021/04/06 08:01:38 ngathunguyen Exp $
//
// Copyright(c) 2019 - 2021 Renesas Electronics Corporation
// Copyright(c) 2019 - 2021 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef _RVC_COMMON_MODEL_H_
#define _RVC_COMMON_MODEL_H_
#include <vector>
#include "systemc.h"
#include <map>
#include <stdarg.h>
#include <sstream>

class rvc_common_model {
public:
    rvc_common_model(void);
    rvc_common_model(std::string name);
    ~rvc_common_model(void) {;}
    void common_re_printf(std::string group, const char *message,...);
    std::string HandleCommand (const std::vector<std::string>& args);
protected:
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    double CalculateCLKPosEdge (const double clock_period, const bool add_period, const double current_time, const double start_time);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
private:
    std::map<std::string, bool> mMessageLevel;
    std::string mInstName;
    std::string mFileName;
    std::string mCmdId;
    int mLineNum;
    bool strmatch(const char *ptn, const char *str);
    void Get_Fileline(std::string filename, int line_number);
    std::string Own_Handle_Command(std::vector<std::string>& args);
    std::string User_Def_Command(std::vector<std::string>& args);
    std::vector<std::string> str2vec(std::string str, const char sep);
};

#endif /* _RVC_COMMON_MODEL_H_ */
