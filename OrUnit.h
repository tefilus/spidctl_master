/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef OR_UNIT_H
#define OR_UNIT_H

template <class T, int N>
class OrUnit: public sc_module
{
public:
  sc_in<T>*  in_port[N];
  sc_out<T>* out_port;

  /* constructor */
  SC_HAS_PROCESS( OrUnit );
  OrUnit( sc_module_name name ): sc_module( name ) {
    char port_name[32];
    for ( int i = 0; i < N; i++ ) {
      sprintf( port_name, "in_port%d", i+1 );
      in_port[i] = new sc_in<T>( port_name );
    }
    sprintf( port_name, "out_port" );
    out_port = new sc_out<T>( port_name );

    SC_METHOD( OrProc );
    for ( int i = 0; i < N; i++ ) {
      sensitive << *in_port[i];
    }
    dont_initialize();
  }

  /* destructor */
  virtual ~OrUnit( void ) {
    for ( int i = 0; i < N; i++ ) {
      delete in_port[i];
      in_port[i] = 0;
    }
  }

private:
  /* private variables */

  /* processes */
  void OrProc( void ) {
    T out_val = in_port[0]->read();
    for ( int i = 1; i < N; i++ ) {
      out_val = out_val | in_port[i]->read();
    }

    if ( out_port->read() != out_val ) {
      out_port->write( out_val );
    }
    else {
      // do nothing
    }
  }
};

#endif // OR_UNIT_H
