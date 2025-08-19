/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef G_REG_H
#define G_REG_H

#include <iostream>
#include <vector>
#include <cstring>

enum G_REG_SIZE_T {
  G_REG_SIZE_1BYTE = 0x1,
  G_REG_SIZE_2BYTE = 0x2,
  G_REG_SIZE_4BYTE = 0x4
};


class GRegMask {
public:
  union {
    unsigned int  m_mask32;
    unsigned char m_mask8[4];
  };
  void     SetRegMask( unsigned int mask ) { m_mask32 = mask; }
  explicit GRegMask ( unsigned int mask ) : m_mask32 ( mask ) {}
  ~GRegMask( void ){}
};


class GRegBase {
public:
  virtual void Reset ( void ) = 0;
  virtual void WriteBody( unsigned int addr, unsigned char* data, G_REG_SIZE_T size ) = 0;
  virtual void Write    ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size ) = 0;
  virtual void ReadBody ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size ) const = 0;
  virtual void Read     ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size ) = 0;
  void         SetName( const char* name ) { strcpy( m_name, name ); }
  const char*  GetName( void ) const { return m_name; }
  void         SetAddress( unsigned int addr ) { m_reg_addr = addr; };
  unsigned int GetAddress( void ) const { return m_reg_addr; }
  void         SetAccessSize( G_REG_SIZE_T size );
  void         ClearAccessSize( void );
  bool         IsRegAccessSizeValid ( G_REG_SIZE_T size );
  bool IsSearchHit ( unsigned int addr ) const;

  GRegBase( void ):
    m_reg_size(G_REG_SIZE_4BYTE), m_reg_addr(0) { strcpy( m_name, "GRegBase" ); }
  GRegBase( const char* name ):
    m_reg_size(G_REG_SIZE_4BYTE), m_reg_addr(0) { strcpy( m_name, name ); }
  GRegBase( const char* name, G_REG_SIZE_T reg_size, unsigned int reg_addr ):
    m_reg_size(reg_size), m_reg_addr(reg_addr)  { strcpy( m_name, name ); }
  virtual ~GRegBase ( void ) {}

protected:
  unsigned int m_reg_addr;  // base address of register
  char         mErrMsg[64]; // for error message

private:
  char                      m_name[512];    // name of register
  G_REG_SIZE_T              m_reg_size;     // size of register
  std::vector<G_REG_SIZE_T> m_acc_size_lst; // list of valid access size
};


template <class TR>
class GRegDataBase : public GRegBase {
public:
  void SetRegMask( unsigned int reg_mask ) { m_reg_mask.SetRegMask( reg_mask ); };
  void WriteBody ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
  void Write     ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
  void ReadBody  ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size ) const;
  void Read      ( unsigned int addr, unsigned char* data, G_REG_SIZE_T size );
  void SetRegPtr ( TR* greg ) { m_greg = greg; }

  GRegDataBase( void ): 
    GRegBase(), m_greg(0), m_reg_mask (0xFFFFFFFF) {}
  GRegDataBase( const char* name ): 
    GRegBase(name), m_greg(0), m_reg_mask (0xFFFFFFFF) {}
  GRegDataBase ( const char* name, G_REG_SIZE_T reg_size, unsigned int reg_addr ):
    GRegBase(name, reg_size, reg_addr), m_greg(0), m_reg_mask(0xFFFFFFFF) {}

protected:
    TR*      m_greg;      // pointer to data of register
    GRegMask m_reg_mask;  // mask of register
};


class GRegData32 {
public:
  union {
    unsigned int  m_data32;
    unsigned char m_data8[4];
  };

  void Write32( unsigned int addr, unsigned char* data, const GRegMask mask );
  void Read32 ( unsigned int addr, unsigned char* data ) const;
  void Write16( unsigned int addr, unsigned char* data, const GRegMask mask );
  void Read16 ( unsigned int addr, unsigned char* data ) const;
  void Write8 ( unsigned int addr, unsigned char* data, const GRegMask mask );
  void Read8  ( unsigned int addr, unsigned char* data ) const;
};


class GRegData16 {
public:
  unsigned char m_data8[2]; // array of 1byte size data of register

  void Write32( unsigned int addr, unsigned char* data, const GRegMask mask ) {}
  void Read32 ( unsigned int addr, unsigned char* data ) const {}
  void Write16( unsigned int addr, unsigned char* data, const GRegMask mask );
  void Read16 ( unsigned int addr, unsigned char* data ) const;
  void Write8 ( unsigned int addr, unsigned char* data, const GRegMask mask );
  void Read8  ( unsigned int addr, unsigned char* data ) const;
};


class GRegData8 {
public:
  unsigned char m_data8; // 1byte size data of register

  void Write32( unsigned int addr, unsigned char* data, const GRegMask mask ) {}
  void Read32 ( unsigned int addr, unsigned char* data ) const {}
  void Write16( unsigned int addr, unsigned char* data, const GRegMask mask ) {}
  void Read16 ( unsigned int addr, unsigned char* data ) const {}
  void Write8 ( unsigned int addr, unsigned char* data, const GRegMask mask );
  void Read8  ( unsigned int addr, unsigned char* data ) const;
};

#endif // G_REG_H
