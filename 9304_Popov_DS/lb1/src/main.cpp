#include <iostream>
#include "opt.h"
#include "ThreadLab.h"
#include "ProcessLab.h"


int main(int argc, char* argv[])
{
    const int appMode = opt::getOptForApp(argc, argv);
    
    switch (appMode)
    {
    case opt::THREAD_LAB:
        startLabInThreadMode();
        break;

    case opt::PROCESS_LAB:
        startLabInProcessMode();
        break;

    default:
        std::cout << opt::helpStr << std::endl;
        break;
    }

    return 0;
}