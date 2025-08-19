// SPIDCTL_MASTER.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SPIDCTL_MASTER.h"

SPIDCTL_MASTER::SPIDCTL_MASTER(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency, const char* config_file):
	sc_module(name),
    PCLK("PCLK"),
    Reset("Reset"),
    mpSPIDCTLFunc((SPIDCTL_MASTER_Func*)0)
{
    setMasterResetPort32(&Reset);
    setInitiatorSocket32((char *)"is");

    mpSPIDCTLFunc = new SPIDCTL_MASTER_Func(mBusMasterIf32[0]);

    mBusMasterIf32[0]->setFuncModulePtr(mpSPIDCTLFunc);
    mBusMasterIf32[0]->setBusProtocol(BUS_APB);
    mBusMasterIf32[0]->setTransNmbLmt(1);

    SC_THREAD(process);
}

SPIDCTL_MASTER::~SPIDCTL_MASTER()
{
}

void SPIDCTL_MASTER::process()
{
    while (true)
    {
        mpSPIDCTLFunc->transportProcess();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
