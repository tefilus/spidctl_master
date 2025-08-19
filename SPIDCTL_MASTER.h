#include "BusMasterBase.h"
#include "SPIDCTL_MASTER_Func.h"
#include "rvc_common_model.h"

class SPIDCTL_MASTER:
	public sc_module, 
	public BusMasterBase<32, 2>
{
public:
    SC_HAS_PROCESS(SPIDCTL_MASTER);

    // Constructor/Destructor
    SPIDCTL_MASTER(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency, const char* config_file);
    ~SPIDCTL_MASTER();

    void process(void);

	// Clock/Reset
	sc_in<sc_dt::uint64> PCLK; // clock port
	sc_in<bool> Reset;         // reset port

    // Socket
    TlmTargetSocket<BUS_WIDTH_APB>* ts; // target socket to access register

    // Class SPIDCTL_Func pointer
    SPIDCTL_MASTER_Func* mpSPIDCTLFunc; // pointer of SPIDCTL_Func


};
