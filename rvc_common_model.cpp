// ---------------------------------------------------------------------
// $Id: rvc_common_model.cpp,v 1.4 2021/01/21 01:34:21 ngathunguyen Exp $
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
#include "rvc_common_model.h"


// Default Constructor
rvc_common_model::rvc_common_model()
{//{{{
    mInstName = "";
    mLineNum = 0;
    mFileName = "";
    mCmdId = "self";
   
    mMessageLevel.insert(std::pair<std::string, bool>("fatal",   true ));
    mMessageLevel.insert(std::pair<std::string, bool>("error",   true ));
    mMessageLevel.insert(std::pair<std::string, bool>("warning", false));
    mMessageLevel.insert(std::pair<std::string, bool>("info",    false));
}//}}}

// Constructor has 1 argument
rvc_common_model::rvc_common_model(std::string  name)
{//{{{
    mInstName = name;
    mLineNum = 0;
    mFileName = "";
    mCmdId = "self";
   
    mMessageLevel.insert(std::pair<std::string, bool>("fatal",   true ));
    mMessageLevel.insert(std::pair<std::string, bool>("error",   true ));
    mMessageLevel.insert(std::pair<std::string, bool>("warning", false));
    mMessageLevel.insert(std::pair<std::string, bool>("info",    false));
}//}}}

///*********************************
/// Function     : CalculateCLKPostrue
/// Description  : Calculate synchronous time with clock
///*********************************
double rvc_common_model::CalculateCLKPosEdge (const double clock_period, const bool add_period, const double current_time, const double start_time)
{//{{{
    
    if (clock_period == 0) return 0;
    double period_num = 0;
    period_num = (current_time - start_time) / clock_period;
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (add_period == false) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        mod_period_num++;
    }
    return (((double)mod_period_num * clock_period) - (current_time - start_time));
}//}}}

///*********************************
/// Function     : GetTimeResolution
/// Description  : Get time resolution
///*********************************
void rvc_common_model::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
  if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
      resolution_value = 1000000000000000LL;
      resolution_unit = SC_FS;
  } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
      resolution_value = 1000000000000LL;
      resolution_unit = SC_PS;
  } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
      resolution_value = 1000000000;
      resolution_unit = SC_NS;
  } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
      resolution_value = 1000000;
      resolution_unit = SC_US;
  } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
      resolution_value = 1000;
      resolution_unit = SC_MS;
  } else {
      resolution_value = 1;
      resolution_unit = SC_SEC;
  }
}//}}}

///*********************************
/// Function     : SeparateString
/// Description  : Separate some words from a string to store a vector
///*********************************
void rvc_common_model::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

///*********************************
///// Function     : ConvertClockFreq
///// Description  : Check frequency value and frequency unit
/////*********************************
void rvc_common_model::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in)
{//{{{
    if(unit_in == "GHz"){
        freq_out = freq_in * (sc_dt::uint64)(1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
    }
}//}}}

///*********************************
/// Function     : HandleCommand
/// Description  : Handle user command
///*********************************
std::string rvc_common_model::HandleCommand (const std::vector<std::string>& args)
{//{{{
    std::vector<std::string> _args = args;
    std::string ret = "";
   
    if (args[0] == "command") {
        _args.erase(_args.begin());
        ret = this->Own_Handle_Command(_args);
        if (ret != "") {
          ret += "\n";
        }
    }
    // send a command to all targets
    else {
        bool cmd_found = false;
        std::string baseid_message;
       
        baseid_message = this->Own_Handle_Command(_args);
        if (baseid_message.find("is invalid", 0) == std::string::npos) { // defined command
            cmd_found = true;
            if (baseid_message != "") {
              ret += baseid_message + "\n";
            }
        }
   
        if (!cmd_found) {
            ret = "Error (" + mInstName + ") command name \"" + _args[0] + "\" is invalid.\n";
        }
    }
    return ret;
}//}}}

///*********************************
/// Function     : Own_Handle_Command
/// Description  : Proceed user command
///*********************************
std::string rvc_common_model::Own_Handle_Command(std::vector<std::string>& args)
{//{{{
    std::string ret = "";
    std::string err_msg = "Error ("   + mInstName + ") ";   // error message
  
    std::string specified_cmd = " (";
    std::vector<std::string>::iterator cmd_it;
    for (cmd_it=args.begin() ; cmd_it!=args.end() ; cmd_it++) {
        specified_cmd += " " + *cmd_it;
    }
    specified_cmd += " ) : Type " + mInstName + " help";
  
    if ((int)args.size() <= 0) {
        ret = err_msg + "command name \"\" is invalid.";
        return ret;
    }
  
    if (args[0] == "help") {
        ret += "    --- command ---\n";
        ret += "    help                                      Show direction\n";
        ret += "    MessageLevel   <fatal|error|warning|info> Select debug message level (Default: fatal,error)\n";
    }
    else if (args[0] == "MessageLevel") {
        // read mode
        if ((int)args.size() == 1) {
            std::map<std::string, bool>::iterator it;
            for (it = mMessageLevel.begin(); it != mMessageLevel.end(); it++) {
                if (it->second) {
                    ret += " " + it->first;
                }
            }
        }
      // write mode
        else if ((int)args.size() == 2) {
            // update to new setting
            mMessageLevel.clear();
            
            std::vector<std::string> arg_vec = str2vec(args[1], '|');
            std::vector<std::string>::iterator it;
            for (it = arg_vec.begin(); it != arg_vec.end(); it++) {
                mMessageLevel[*it] = true;
            }
        }
      else {
          ret = err_msg + "wrong number of arguments" + specified_cmd;
      }
    }
    else{
        ret = User_Def_Command(args);
    }

    return ret;
}//}}}

///*********************************
/// Function     : str2vec
/// Description  : Split a string into a vector
///*********************************
std::vector<std::string> rvc_common_model::str2vec(std::string str, const char sep)
{//{{{
  std::vector<std::string> buf;
  int index = 0;

  for (int i=0 ; i<(int)str.size() ; i++) {
    if (str[i] == sep) {
      if (i > index) {
        buf.push_back(str.substr((unsigned int)index, (unsigned int)(i-index)));
      }
      index = i+1;
    }
  }
  if (index < (int)str.size()) {
    buf.push_back(str.substr((unsigned int)index));
  }
  return buf;
}

///*********************************
/// Function     : User_Def_Command
/// Description  : Handle user define command
///*********************************
std::string rvc_common_model::User_Def_Command(std::vector<std::string>& args)
{//{{{
    std::string ret = "";
    std::string err_msg = "Error ("   + mInstName + ") ";   // error message
   
    std::string specified_cmd = " (";
    std::vector<std::string>::iterator cmd_it;
    for (cmd_it=args.begin() ; cmd_it!=args.end() ; cmd_it++) {
        specified_cmd += " " + *cmd_it;
    }
    specified_cmd += " ) : Type " + mInstName + " help";
   
    ret = err_msg + "command name \"command " + (std::string)args[0] + "\" is invalid.";
    return ret;
}//}}}

///*********************************
/// Function     : common_re_printf
/// Description  : Print the message
///*********************************
void rvc_common_model::common_re_printf(std::string group, const char *message, ...)
{//{{{
    
    // message group check
#ifndef REGIF_SC_REPORT
#if 1 // MessageLevel
    if (!mMessageLevel[group]) return;
#endif // MessageLevel
#endif//REGIF_SC_REPORT

    if (message == NULL) return;
    Get_Fileline(__FILE__, __LINE__);
    // print header
    #ifdef SYSTEMC_H
    std::stringstream t_stream;
    std::string cur_time;
   
    double tu_value = 0;
    std::string tu_name = "";
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        tu_value = 1000000000000000LL;
        tu_name = " fs";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        tu_value = 1000000000000LL;
        tu_name = " ps";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        tu_value = 1000000000;
        tu_name = " ns";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        tu_value = 1000000;
        tu_name = " us";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        tu_value = 1000;
        tu_name = " ms";
    }
    else {
        tu_value = 1;
        tu_name = " s";
    }
    t_stream << sc_time_stamp().value() * (uint64) (sc_get_time_resolution().to_seconds() * tu_value) << tu_name;
   
    cur_time = t_stream.str();
    #else
    std::string cur_time = "";
    #endif
   
#ifndef REGIF_SC_REPORT
    #ifdef REGIF_MSG_COLOR
    // color setting
    //
    // severity color       color Linux       Windows
    // fatal    RED         RED   \033[0;31m  FOREGROUND_RED
    // error    RED         BLUE  \033[0;34m  FOREGROUND_BLUE
    // warning  RED
    // info     BLUE
    #ifdef _WIN32
    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (group == "fatal" || group == "error" || group == "warning") {
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED);
    }
    else if (group == "info" ){
        SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE);
    }
    #else
    if (group == "fatal" || group == "error" || group == "warning") {
        printf("\033[0;31m");
    }
    else if (group == "info" ){
        printf("\033[0;34m");
    }
    #endif
    #endif
    printf("%8s [%20s] (%10s) ", group.c_str(), cur_time.c_str(), mInstName.c_str());
#endif//REGIF_SC_REPORT

    // print body
    va_list argptr;
    va_start(argptr, message);
    if (argptr == NULL) return;
#ifdef REGIF_SC_REPORT
    char str[1024];
    sprintf(str, "[%20s] ", cur_time.c_str());
    vsprintf(str+23, message, argptr);
    if (group == "fatal") {
        SC_REPORT_FATAL(mInstName.c_str(), str);
    }
    else if (group == "error") {
        SC_REPORT_ERROR(mInstName.c_str(), str);
    }
    else if (group == "warning") {
        SC_REPORT_WARNING(mInstName.c_str(), str);
    }
    else if (group == "info") {
        SC_REPORT_INFO(mInstName.c_str(), str);
    }
#else //REGIF_SC_REPORT
    vprintf(message, argptr);
#endif//REGIF_SC_REPORT

#ifndef REGIF_SC_REPORT
    #ifdef DumpFileNameLineNum
    // print footer
    if (group == "fatal" || group == "error") {
        printf(" [%s:%d]\n", mFileName.c_str(), mLineNum);
    }

    #endif//DumpFileNameLineNum
#endif//REGIF_SC_REPORT
    va_end(argptr);

#ifndef REGIF_SC_REPORT
    #ifdef REGIF_MSG_COLOR
    #ifdef _WIN32
    SetConsoleTextAttribute(hStdout, csbi.wAttributes);
    #else
    printf("\033[m");
    #endif
    #endif
#endif//REGIF_SC_REPORT

    fflush(stdout);
    fflush(stderr);
   
    if (group == "fatal") {
        exit(1);
    }
}//}}}

///*********************************
/// Function     : Get_Fileline
/// Description  : Get the file name and line number
///*********************************
void rvc_common_model::Get_Fileline(std::string filename, int line_number) 
{//{{{
#ifdef _WIN32
  filename = filename.substr(filename.rfind("\\")+1);
#else
  filename = filename.substr(filename.rfind("/")+1);
#endif
  mFileName = filename;
  mLineNum  = line_number;
}//}}}

///*********************************
/// Function     : strmatch
/// Description  : Redefine strmatch function
///*********************************
bool rvc_common_model::strmatch(const char *ptn, const char *str) 
{
  bool ret;

  if (ptn == NULL || str == NULL) return false;

  switch( *ptn ) {
  case '\0':
    ret = '\0' == *str;
    break;
  case '*' :
    ret = strmatch( &ptn[1], str ) || (('\0' != *str) && strmatch( ptn, &str[1] ));
    break;
  case '?' :
    ret = ('\0' != *str) && strmatch( &ptn[1], &str[1] );
    break;
  default  :
    ret = ((unsigned char)*ptn == (unsigned char)*str) && strmatch( &ptn[1], &str[1] );
    break;
  }
  return ret;
}

// vim600: set foldmethod=marker :
