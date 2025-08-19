/*************************************************************************
 *
 * g_reg.cpp
 *
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include <algorithm>
#include "g_reg.h"
#include "CmErrMsg.h"

using namespace std;
using namespace CmErrMsg;



/// <summary>set valid access size to control</summary>
/// <param name="size">access size</param>
/// <returns>none</returns>
void GRegBase::SetAccessSize( G_REG_SIZE_T size )
{
  m_acc_size_lst.push_back( size );
}

/// <summary>clear access size list</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void GRegBase::ClearAccessSize( void )
{
  m_acc_size_lst.clear();
}

/// <summary>check address</summary>
/// <param name="addr">address</param>
/// <returns>true(hit)/false(no hit)</returns>
bool GRegBase::IsSearchHit ( unsigned int addr ) const
{
  return ( addr >= m_reg_addr && addr < m_reg_addr + (unsigned int)m_reg_size );
}

/// <summary>check access size </summary>
/// <param name="size">access size</param>
/// <returns>true(valid)/false(invalid)</returns>
bool GRegBase::IsRegAccessSizeValid( G_REG_SIZE_T size )
{
  bool ret = false;
  vector<G_REG_SIZE_T>::iterator itr;

  if ( m_acc_size_lst.empty() ) { // no access control
    ret = true;
  }
  else { // access control is enabled
    itr = find( m_acc_size_lst.begin(), m_acc_size_lst.end(), size );
    if ( m_acc_size_lst.end() != itr && m_reg_size >= size ) { // valid access size
      ret = true;
    }
    else { // invalid access size
      // do nothing
    }
  }

  return ret;
}


/// <summary>debug write to register</summary>
/// <param name="addr">address</param>
/// <param name="data">pointer to data array</param>
/// <param name="size">access size</param>
/// <returns>none</returns>
template <class TR>
void GRegDataBase<TR>::WriteBody (
  unsigned int   addr,
  unsigned char* data,
  G_REG_SIZE_T   size )
{
  if ( G_REG_SIZE_4BYTE == size ) {
    m_greg->Write32( addr, data, m_reg_mask );
  }
  else if ( G_REG_SIZE_2BYTE == size ) {
    m_greg->Write16( addr, data, m_reg_mask );
  }
  else if ( G_REG_SIZE_1BYTE == size ) {
    m_greg->Write8 ( addr, data, m_reg_mask );
  }
  else {
    // do nothing
  }
}


/// <summary>nomal write to register</summary>
/// <param name="addr">address</param>
/// <param name="data">pointer to data array</param>
/// <param name="size">access size</param>
/// <returns>none</returns>
template <class TR>
void
GRegDataBase<TR>::Write(
  unsigned int   addr,
  unsigned char* data,
  G_REG_SIZE_T   size )
{
  if ( !IsRegAccessSizeValid(size) ) {
    sprintf( mErrMsg, "Invalid access size(%d).", (unsigned int)size );
    printWarningMessage( GetName(), "Write", mErrMsg );
  }
  else {
    WriteBody( addr, data, size );
  }
}


/// <summary>debug read to register</summary>
/// <param name="addr">address</param>
/// <param name="data">pointer to data array</param>
/// <param name="size">access size</param>
/// <returns>none</returns>
template <class TR>
void
GRegDataBase<TR>::ReadBody (
  unsigned int   addr,
  unsigned char* data,
  G_REG_SIZE_T   size ) const
{
  if ( G_REG_SIZE_4BYTE == size ) {
    m_greg->Read32( addr, data );
  }
  else if ( G_REG_SIZE_2BYTE == size ) {
    m_greg->Read16( addr, data );
  }
  else if ( G_REG_SIZE_1BYTE == size ) {
    m_greg->Read8 ( addr, data );
  }
  else {
    // do nothing
  }
}


/// <summary>nomal read to register</summary>
/// <param name="addr">address</param>
/// <param name="data">pointer to data array</param>
/// <param name="size">access size</param>
/// <returns>none</returns>
template <class TR>
void
GRegDataBase<TR>::Read (
  unsigned int   addr,
  unsigned char* data,
  G_REG_SIZE_T   size )
{
  if ( !IsRegAccessSizeValid(size) ) {
    sprintf( mErrMsg, "Invalid access size(%d).", (unsigned int)size );
    printWarningMessage( GetName(), "Read", mErrMsg );
    std::fill_n( data, (unsigned int)size, 0 );
  } else {
    ReadBody( addr, data, size );
  }
}


/// <summary>nomal read to register</summary>
/// <param name="addr">address</param>
/// <param name="data">pointer to data array</param>
/// <param name="size">access size</param>
/// <returns>none</returns>
void
GRegData32::Write32 (
  unsigned int   addr,
  unsigned char* data,
  const GRegMask mask )
{
  m_data32 = (*(reinterpret_cast<unsigned int*> (data))) & mask.m_mask32;
}


void
GRegData32::Read32 (
  unsigned int   addr,
  unsigned char* data ) const
{
  *(reinterpret_cast<unsigned int*> (data)) = m_data32;
}


void
GRegData32::Write16 (
  unsigned int   addr,
  unsigned char* data,
  const GRegMask mask )
{
  unsigned int index = (addr >> 1) & 0x1;
  m_data8[ index << 1 ]     = data[0] & mask.m_mask8[ index << 1 ];
  m_data8[(index << 1) + 1] = data[1] & mask.m_mask8[(index << 1) + 1];
}


void
GRegData32::Read16 (
  unsigned int   addr,
  unsigned char* data ) const
{
  unsigned int index = (addr >> 1) & 0x1;
  data[0] = m_data8[ index << 1 ];
  data[1] = m_data8[(index << 1) + 1];
}


void
GRegData32::Write8 (
  unsigned int   addr,
  unsigned char* data,
  const GRegMask mask )
{
  unsigned int index = addr & 0x3;
  m_data8[index] = (*data) & mask.m_mask8[index];
}


void
GRegData32::Read8 (
  unsigned int   addr,
  unsigned char* data ) const
{
  unsigned int index = addr & 0x3;
  *data = m_data8[index];
}


void
GRegData16::Write16 (
  unsigned int   addr,
  unsigned char* data,
  const GRegMask mask )
{
  m_data8[0] = data[0] & mask.m_mask8[0];
  m_data8[1] = data[1] & mask.m_mask8[1];
}


void
GRegData16::Read16 (
  unsigned int   addr,
  unsigned char* data ) const
{
  data[0] = m_data8[0];
  data[1] = m_data8[1];
}


void
GRegData16::Write8 (
  unsigned int   addr,
  unsigned char* data,
  const GRegMask mask )
{
  unsigned int index = addr & 0x1;
  m_data8[index] = (*data) & mask.m_mask8[index];
}


void
GRegData16::Read8 (
  unsigned int   addr,
  unsigned char* data ) const
{
  unsigned int index = addr & 0x1;
  *data = m_data8[index];
}


void
GRegData8::Write8 (
  unsigned int   addr,
  unsigned char* data,
  const GRegMask mask )
{
  m_data8 = (*data) & mask.m_mask8[0];
}


void
GRegData8::Read8 (
  unsigned int   addr,
  unsigned char* data ) const
{
  *data = m_data8;
}




template void GRegDataBase<GRegData8 >::WriteBody( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
template void GRegDataBase<GRegData16>::WriteBody( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
template void GRegDataBase<GRegData32>::WriteBody( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
template void GRegDataBase<GRegData8 >::Write    ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
template void GRegDataBase<GRegData16>::Write    ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
template void GRegDataBase<GRegData32>::Write    ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
template void GRegDataBase<GRegData8 >::ReadBody ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size ) const;
template void GRegDataBase<GRegData16>::ReadBody ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size ) const;
template void GRegDataBase<GRegData32>::ReadBody ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size ) const;
template void GRegDataBase<GRegData8 >::Read     ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
template void GRegDataBase<GRegData16>::Read     ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
template void GRegDataBase<GRegData32>::Read     ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
