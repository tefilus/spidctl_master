/*
* Copyright(c) 2011 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

int           token_to_int(char*, const char*, const char*);
unsigned int  token_to_uint(char*, const char*, const char*);
unsigned long token_to_ulong(char*, const char*, const char*);
uint64        token_to_ulonglong(char*, const char*, const char*);
double        token_to_double(char*, const char*, const char*);
unsigned int token_to_uint_default( char *, unsigned int, const char *, char * );
uint64        token_to_ulonglong_default(char*, uint64, const char*, const char*);
