/*************************************************************************
 *
 *  CmReport.cpp
 *
 * Copyright(c) 2011-2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

/* $Id:$ */

#ifdef CM_REPORT_OUT

#include "CmReport.h"
#include <string>  // for string type
#ifdef WINDOWS_DEF
#ifndef VISTA_VPP_ENABLED
#include < sstream >
#endif // VISTA_VPP_ENABLED koichi 12/01/2006
#endif // WINDOWS_DEF

char CmReport::WorkingState = CM_REPORT_NOT_WORK;
const std::string CmReport::company_name("NECEL/");  // Don't EDIT!
const std::string CmReport::product_name("V850E2RHEAP/");
const std::string CmReport::msg_typ_base("/" + company_name + product_name);
const std::string CmReport::ReportMessage[]
                    = {": [GE000] read invoked. ",
                       ": [GE001] write invoked. ",
                       ": [GE002] unlockTarget invoked. ",
                       ": [GE003] unlockBus invoked. ",
                       ": [GE004] response invoked. ",
                       ": [GE005] setSlaveArea invoked. ",
                       ": [GE006] resize invoked. ",
                      };

const std::string
CmReport::get_base(void)
{
  return msg_typ_base;
}

const std::string
CmReport::get_message(char index)
{
  return ReportMessage[index];
}

void CmReport::InfoReport( const char *name, char *msg, const char *msg_type )
{
  if( WorkingState == CM_REPORT_WORK ){
    std::ostringstream msg_stream;
    msg_stream << name << msg;
    SC_REPORT_INFO(msg_type,msg_stream.str().c_str());
  }
}

void CmReport::WarningReport( const char *name, char *msg, const char *msg_type )
{
  if( WorkingState == CM_REPORT_WORK ){
    std::ostringstream msg_stream;
    msg_stream << name << msg;
    SC_REPORT_WARNING(msg_type,msg_stream.str().c_str());
  }
}

void CmReport::ErrorReport( const char *name, char *msg, const char *msg_type )
{
  if( WorkingState == CM_REPORT_WORK ){
    std::ostringstream msg_stream;
    msg_stream << name << msg;
    SC_REPORT_ERROR(msg_type,msg_stream.str().c_str());
  }
}

void CmReport::FatalReport( const char *name, char *msg, const char *msg_type )
{
  if( WorkingState == CM_REPORT_WORK ){
    std::ostringstream msg_stream;
    msg_stream << name << msg;
    SC_REPORT_FATAL(msg_type,msg_stream.str().c_str());
  }
}

//void CmReport::InitSet( const char work,
//                        const char *FileName, 
//                        const char debug,
//                        const char Limit )
void CmReport::InitSet( char work,
                        char debug,
                        char *FileName,
                        char NoLimit)
{
  WorkingState = work;
  {
    // default actions
    if( debug == CM_REPORT_DEBUG ){
      sc_report_handler::set_actions(SC_INFO,    SC_DISPLAY);
      sc_report_handler::set_actions(SC_WARNING, SC_DISPLAY);
      sc_report_handler::set_actions(SC_ERROR,   SC_DISPLAY);
      sc_report_handler::set_actions(SC_FATAL,   SC_DISPLAY | SC_ABORT);
    }
    else{
      sc_report_handler::set_actions(SC_INFO,    SC_LOG);
      sc_report_handler::set_actions(SC_WARNING, SC_LOG);
      sc_report_handler::set_actions(SC_ERROR,   SC_LOG);
      sc_report_handler::set_actions(SC_FATAL,   SC_LOG | SC_ABORT);
    }
    
    // specific actions
    //std::string producer1_subcategory = CmReport::get_base() +
    //                                    std::string("Producer1/INVOKE/");
    //sc_report_handler::set_actions(producer1_subcategory.c_str(),
    //                               SC_INFO,
    //                               SC_LOG | SC_DISPLAY);
    //std::string consumer1_subcategory = CmReport::get_base() +
    //                                    std::string("Consumer1/DEBUG/");
    //sc_report_handler::set_actions(consumer1_subcategory.c_str(),
    //                               SC_FATAL,
    //                               SC_DISPLAY);
  }
  {
    // default limit
    //sc_report_handler::stop_after(SC_INFO,    0);
    //sc_report_handler::stop_after(SC_WARNING, 0);
    sc_report_handler::stop_after(SC_ERROR,   NoLimit);
    //sc_report_handler::stop_after(SC_FATAL,   3);
    
    // specific limit
    //std::string consumer1_subcategory = CmReport::get_base() +
    //                                    std::string("Consumer1/DEBUG/");
    //sc_report_handler::stop_after(consumer1_subcategory.c_str(),
    //                              SC_FATAL,
    //                              0);
  }
  // set log file name
  //sc_report_handler::set_log_file_name(config->get_log_file_name().c_str());
  sc_report_handler::set_log_file_name(FileName);
//  sc_report_handler::set_log_file_name(stdout);
}
#endif  // CM_REPORT_OUT
