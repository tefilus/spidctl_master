/*
* Copyright(c) 2011 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __BUS_TIME_BASE_H__
#define __BUS_TIME_BASE_H__

#include "OSCI2.h"     // class for User Modeling environment based on TLM2.0
#include "CmErrMsg.h"  // for error message

extern sc_time_unit glb_resolution_unit;

class BusTimeBase
{

 public:
  /// <summary>constructer</summary>
  BusTimeBase( void ):mResetPort((sc_in<bool> *)0){}

  /// <summary>destructer</summary>
  ~BusTimeBase( void ){}
  
  /// <summary>elapse time</summary>
  /// <param name="t">time</param>
  /// <param name="u">unit</param>
  /// <returns>if reset occurs</returns>
  bool elapseTime( double t, BusTimeUnit_t u )
  {
    sc_time time = sc_time( t, u );
    if( mResetPort ){
      wait( time, mResetPort->negedge_event() );
      if( !mResetPort->read() ){
	return true;
      }
    }
    else{
      wait( time );
    }
    return false;
  }

  /// <summary>elapse time</summary>
  /// <param name="t">sc_time</param>
  /// <returns>if reset occurs</returns>
  bool elapseTime( BusTime_t t )
  {
    if( mResetPort ){
      wait( t, mResetPort->negedge_event() );
      if( !mResetPort->read() ){
	return true;
      }
    }
    else{
      wait( t );
    }
    return false;
  }

  /// <summary>elapse time(use glb_resolution as unit)</summary>
  /// <param name="t">time</param>
  /// <returns>if reset occurs</returns>
  bool elapseTime( double t )
  {
    sc_time time = sc_time( t, glb_resolution_unit );
    if( mResetPort ){
      wait( time, mResetPort->negedge_event() );
      if( !mResetPort->read() ){
	return true;
      }
    }
    else{
      wait( time );
    }
    return false;
  }

  /// <summary>read reset port value</summary>
  /// <returns>reset port value</returns>
  bool readResetPort( void )
  {
    if( mResetPort ){
      return( mResetPort->read() );
    }
    return true;
  }

  /// <summary>set reset port</summary>
  /// <param name="resetPort">reset port</param>
  void setResetForBusTimeBase( sc_in<bool> *resetPort )
  {
    mResetPort = resetPort;
  }

  /// <summary>change double to sc_time</summary>
  /// <returns>the value</returns>
  BusTime_t toBusTime( double t )
  {
    return( sc_time(t, glb_resolution_unit) );
  }

 private:
  sc_in<bool> *mResetPort; //reset port

};

#endif //__BUS_TIME_BASE_H__
