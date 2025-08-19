/*
* Copyright(c) 2011-2012 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef CM_REPORT_H
#define CM_REPORT_H

#include <string>
#include <systemc.h>

#ifdef CM_REPORT_OUT

#define CM_REPORT_NOT_WORK  (char)0
#define CM_REPORT_WORK      (char)1

#define CM_REPORT_DEBUG     (unsigned char)1

#define REPORT_READ         (unsigned char)0
#define REPORT_WRITE        (unsigned char)1
#define REPORT_UNLOCKTARGET (unsigned char)2
#define REPORT_UNLOCKBUS    (unsigned char)3
#define REPORT_RESPONSE     (unsigned char)4
#define REPORT_SETSLAVEAREA (unsigned char)5
#define REPORT_RESIZE       (unsigned char)6

#define CM_INFO_REPORT( msg_type, msg )\
          CmReport::InfoReport(this->name(),(char *)msg,msg_type);

#define CM_WARNING_REPORT( msg_type, msg )\
          CmReport::WarningReport(this->name(),(char *)msg,msg_type);

#define CM_ERROR_REPORT( msg_type, msg )\
          CmReport::ErrorReport(this->name(),(char *)msg,msg_type);

#define CM_FATAL_REPORT( msg_type, msg )\
          CmReport::FatalReport(this->name(),(char *)msg,msg_type);

class CmReport{
 public:
  static const std::string get_base(void);
  static const std::string get_message(char);
  static void InfoReport(const char *, char *, const char *);
  static void WarningReport(const char *, char *, const char *);
  static void ErrorReport(const char *, char *, const char *);
  static void FatalReport(const char *, char *, const char *);
//  static void InitSet(const char *, const char, const char);
  static void InitSet( char work=0,
                       char debug=0,
                       char *FileName="CMREPORT.log", 
		       char NoLimit=1 );
 private:
  static char WorkingState;
  static const std::string company_name;
  static const std::string product_name;
  static const std::string msg_typ_base;
  static const std::string ReportMessage[];
};

#endif    // CM_REPORT_OUT

#endif    // CM_REPORT_H

