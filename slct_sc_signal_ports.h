#ifndef DEF_SLCT_SC_SIGNAL_PORTS_H
#define DEF_SLCT_SC_SIGNAL_PORTS_H

#include "systemc.h"
#include "CmErrMsg.h"

using namespace CmErrMsg;

template <class T>
class slct_sc_in
: public sc_in<T>
{
 public:
  
  // typedefs
  
  typedef T                                             data_type;
  
  typedef sc_signal_in_if<data_type>                    if_type;
  typedef sc_port<if_type,1,SC_ONE_OR_MORE_BOUND>       base_type;
  typedef sc_in<data_type>                              this_type;
  
  typedef if_type                                       in_if_type;
  typedef base_type                                     in_port_type;
  typedef sc_signal_inout_if<data_type>                 inout_if_type;
  typedef sc_port<inout_if_type,1,SC_ONE_OR_MORE_BOUND> inout_port_type;
  
 public:
  // constructors
  
  slct_sc_in()
    : sc_in<T>()
    {}
  
  explicit slct_sc_in( const char* name_ )
    : sc_in<T>(name_)
    {}
  
  explicit slct_sc_in( const in_if_type& interface_ )
    : sc_in<T>(interface_)
    {}
  
  slct_sc_in( const char* name_, const in_if_type& interface_ )
    : sc_in<T>( name_, interface_ )
    {}
  
  explicit slct_sc_in( in_port_type& parent_ )
    : sc_in<T>( parent_ )
    {}
  
  slct_sc_in( const char* name_, in_port_type& parent_ )
    : sc_in<T>( name_, parent_ )
    {}
  
  explicit slct_sc_in( inout_port_type& parent_ )
    : sc_in<T>( parent_ )
    {}
  
  slct_sc_in( const char* name_, inout_port_type& parent_ )
    : sc_in<T>( name_, parent_ )
    {}
  
  slct_sc_in( this_type& parent_ )
    : sc_in<T>( parent_ )
    {}
  
  slct_sc_in( const char* name_, this_type& parent_ )
    : sc_in<T>( name_, parent_ )
    {}
  
  
  // destructor
  
  virtual ~slct_sc_in()
    {}
  
  // bind to in interface
  
  void bind( const in_if_type& interface_, const bool reconCheck=true )
    {
      if (this->get_interface()==0)
	sc_in<T>::bind( interface_ );
      else if (reconCheck){
	char error_message_buf[1024];
	sprintf( error_message_buf,
		 "tried to be bound again." );
	printWarningMessage( this->name(), 0, error_message_buf );
      }
   }
  
  void operator () ( const in_if_type& interface_, const bool reconCheck=true )
    {
      bind( interface_, reconCheck );
    }
  
  // bind to parent in port
  
  void bind( in_port_type& parent_, const bool reconCheck=true )
    {
      if (this->get_interface()==0)
	sc_in<T>::bind( parent_ ); 
      else if (reconCheck){
	char error_message_buf[1024];
	sprintf( error_message_buf,
		 "tried to be bound again." );
	printWarningMessage( this->name(), 0, error_message_buf );
      }
    }
  
  void operator () ( in_port_type& parent_, const bool reconCheck=true )
    {
      bind( parent_, reconCheck );
    }
  
  
  // bind to parent inout port
  
  void bind( inout_port_type& parent_, const bool reconCheck=true )
    {
      if (this->get_interface()==0)
	sc_in<T>::bind( parent_ ); 
      else if (reconCheck){
	char error_message_buf[1024];
	sprintf( error_message_buf,
		 "tried to be bound again." );
	printWarningMessage( this->name(), 0, error_message_buf );
      }
    }
  
  void operator () ( inout_port_type& parent_, const bool reconCheck=true )
    {
      bind( parent_, reconCheck );
    }
  
};


template <class T>
class slct_sc_out
: public sc_out<T>
{
 public:
  // typedefs
  typedef sc_signal_inout_if<T>               IF;
  
  typedef T                                   data_type;
  
  typedef sc_out<data_type>                   this_type;
  typedef sc_inout<data_type>                 base_type;
  
  typedef typename base_type::in_if_type      in_if_type;
  typedef typename base_type::in_port_type    in_port_type;
  typedef typename base_type::inout_if_type   inout_if_type;
  typedef typename base_type::inout_port_type inout_port_type;
  
 public:
  
  // constructors
  
  slct_sc_out()
    : sc_out<T>()
    {}
  
  explicit slct_sc_out( const char* name_ )
    : sc_out<T>( name_ )
    {}
  
  explicit slct_sc_out( inout_if_type& interface_ )
    : sc_out<T>( interface_ )
    {}
  
  slct_sc_out( const char* name_, inout_if_type& interface_ )
    : sc_out<T>( name_, interface_ )
    {}
  
  explicit slct_sc_out( inout_port_type& parent_ )
    : sc_out<T>( parent_ )
    {}
  
  slct_sc_out( const char* name_, inout_port_type& parent_ )
    : sc_out<T>( name_, parent_ )
    {}
  
  slct_sc_out( this_type& parent_ )
    : sc_out<T>( parent_ )
    {}
  
  slct_sc_out( const char* name_, this_type& parent_ )
    : sc_out<T>( name_, parent_ )
    {}
  
  
  // destructor (does nothing)
  
  virtual ~slct_sc_out()
    {}
  
  
  
  
  // bind an interface of type IF to this port
  
  void bind( IF& interface_, const bool reconCheck=true )
    {
      if (this->get_interface()==0)
	sc_out<T>::bind( interface_ ); 
      else if (reconCheck){
	char error_message_buf[1024];
	sprintf( error_message_buf,
		 "tried to be bound again." );
	printWarningMessage( this->name(), 0, error_message_buf );
      }
    }
  
  void operator () ( IF& interface_, const bool reconCheck=true )
    {
      bind( interface_, reconCheck );
    }
  
  
  // bind a parent port with type IF to this port
  
  void bind( this_type& parent_, const bool reconCheck=true )
    {
      if (this->get_interface()==0)
	sc_out<T>::bind( parent_ ); 
      else if (reconCheck){
	char error_message_buf[1024];
	sprintf( error_message_buf,
		 "tried to be bound again." );
	printWarningMessage( this->name(), 0, error_message_buf );
      }
    }
  
  void operator () ( this_type& parent_, const bool reconCheck=true )
    {
      bind( parent_, reconCheck );
    }

};

#endif // DEF_SLCT_SC_SIGNAL_PORTS_H
