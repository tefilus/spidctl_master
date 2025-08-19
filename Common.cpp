/*************************************************************************
 *
 *  Common.cpp
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
#include <stdlib.h>  // for strtol, strtoul, strtod
#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno

#include "CmErrMsg.h" // for WinMsg
#include "Common.h"

using namespace CmErrMsg; // for WinMsg

int token_to_int(char *token, const char *moduleName, const char *error_message){
  long int result;
  char    *unrecognized;

  result = strtol(token, &unrecognized, 0);

  // token check
  if (*unrecognized != '\0'){
    char error_message_buf[1024];
    sprintf(error_message_buf,
      "Unexpected token was specified [%s] on %s. Treat %d instead.",
      token, error_message, result);
    printWarningMessage(moduleName, "token_to_int", error_message_buf);
  }

  // range check
  if (result < INT_MIN){
    char error_message_buf[1024];
    sprintf(error_message_buf,
      "Specified value exceed INT_MIN on %s. Treat INT_MIN instead.",
      error_message);
    printWarningMessage(moduleName, "token_to_int", error_message_buf);
    result = INT_MIN;
  }
  if (result > INT_MAX){
    char error_message_buf[1024];
    sprintf(error_message_buf,
      "Specified value exceed INT_MAX on %s. Treat INT_MAX instead.",
      error_message);
    printWarningMessage(moduleName, "token_to_int", error_message_buf);
    result = INT_MAX;
  }

  return (int)result;
}


unsigned int token_to_uint(char *token, const char *moduleName, const char *error_message){
  unsigned long int result;
  char             *unrecognized;

  result = strtoul(token, &unrecognized, 0);

  // token check
  if (*unrecognized != '\0'){
    char error_message_buf[1024];
    sprintf(error_message_buf,
      "Unexpected token was specified [%s] on %s. Treat %d instead.",
      token, error_message, result);
    printWarningMessage(moduleName, "token_to_uint", error_message_buf);
  }

  // range check
  if (result > UINT_MAX){
    char error_message_buf[1024];
    sprintf(error_message_buf,
      "Specified value exceed UINT_MAX on %s. Treat UINT_MAX instead.",
      error_message);
    printWarningMessage(moduleName, "token_to_uint", error_message_buf);
    result = UINT_MAX;
  }

  return (unsigned int)result;
}


unsigned int token_to_uint_default( char *token, unsigned int default_value, const char *module, char *error )
{
  if (strchr(token, '-') == NULL) {
    unsigned long value;
    char *endptr = NULL;
    errno = 0;

    value = strtoul(token, &endptr, 0);
    if (*endptr == '\0' && (value != ULONG_MAX || errno != ERANGE)
        && value <= (unsigned long)UINT_MAX) {
      return (unsigned int)value;
    }
  }

  char buf[1024];
  sprintf(buf,
    "Unexpected token was specified [%s] on %s. Treat default value [%u] instead.",
    token, error, default_value);
  printWarningMessage(module, "token_to_uint_default", buf);

  return default_value;
}


unsigned long token_to_ulong(char *token, const char *moduleName, const char *error_message){
  unsigned long result;
  char         *unrecognized;

  result = strtoul(token, &unrecognized, 0);

  // token check
  if (*unrecognized != '\0'){
    char error_message_buf[1024];
    sprintf(error_message_buf,
      "Unexpected token was specified [%s] on %s. Treat %u instead.",
      token, error_message, result);
    printWarningMessage(moduleName, "token_to_ulong", error_message_buf);
  }

  return result;
}


unsigned long long token_to_ulonglong(char *token, const char *moduleName, const char *error_message){
  unsigned long long  result = 0;
  char   *unrecognized = NULL;

#ifdef LINUX
  result = strtoull(token, &unrecognized, 0);
#endif // LINUX
#ifdef WINDOWS_DEF
  result = _strtoui64(token, &unrecognized, 0);
#endif // WINDOWS_DEF


  // token check
  if (*unrecognized != '\0'){
    char error_message_buf[1024];
    sprintf(error_message_buf,
      "Unexpected token was specified [%s] on %s. Treat %llu instead.",
      token, error_message, result);
    printWarningMessage(moduleName, "token_to_ulonglong", error_message_buf);
  }

  return result;
}


unsigned long long token_to_ulonglong_default( char *token,
                          unsigned long long default_value,
                          const char *moduleName,
                          const char *error_message )
{
  unsigned long long result = 0;
  char    *endptr = NULL;

  if (strchr(token, '-') == NULL) {
    errno = 0;

#ifdef LINUX
    result = strtoull(token, &endptr, 0);
#endif // LINUX
#ifdef WINDOWS_DEF
    result = _strtoui64(token, &endptr, 0);
#endif // WINDOWS_DEF

#if !defined(ULLONG_MAX)
    if (*endptr == NULL) {
#else
    if (*endptr == NULL && !(result == ULLONG_MAX && errno == ERANGE)) {
#endif
      return result;
    }
  }

  char buf[1024];
	sprintf(buf,
		  "Unexpected token was specified [%s] on %s. Treat %llu instead.",
		  token,
      error_message,
      default_value);
  printWarningMessage(moduleName, "token_to_ulonglong", buf);

  return default_value;
}


double token_to_double(char *token, const char *moduleName, const char *error_message){
  double  result;
  char   *unrecognized;

  result = strtod(token, &unrecognized);

  // token check
  if (*unrecognized != '\0'){
    char error_message_buf[1024];
    sprintf(error_message_buf,
      "Unexpected token was specified [%s] on %s. Treat %lf instead.",
      token, error_message, result);
    printWarningMessage(moduleName, "token_to_double", error_message_buf);
  }

  return result;
}
