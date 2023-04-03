//
//  TurboDisabler.c
//  TurboDisabler
//
//  Created by Viktor Remennik on 25.03.2023.
//


#include "TurboDisabler.hpp"


#define super IOService

OSDefineMetaClassAndStructors(TurboDisabler, super);


bool TurboDisabler::start(IOService* provider)
{   LOG("TurboDisabler: start.\n");

    if (!super::start(provider))
    {   LOG("TurboDisabler: start: IOService start error\n");
        return (false);
    }

    initPowerManagement(provider);
    registerService();

    readTurboFromNvram();
    setTurbo();
    LOG("TurboDisabler: start. Current turbo state is %s\n", currentTurboState ? "On" : "Off");

    return (true);
}
void TurboDisabler::stop(IOService* provider)
{   LOG("TurboDisabler: stop. Power state will be saved as %s\n", currentTurboState ? "On" : "Off");

    writeTurboToNvram();

    deinitPowerManagement();
    super::stop(provider);
}

bool TurboDisabler::requestTerminate(IOService * provider, IOOptionBits options)
{   LOG("TurboDisabler: requestTerminate from \"%s\"\n", provider->getName());
    return (true);
}


//bool TurboDisabler::init(OSDictionary* dict)
//{
//    if (!super::init(dict)) {
//        LOG("TurboDisabler: init failed\n");
//        return (false);
//    }
//    LOG("TurboDisabler: init\n");
//
//    return (true);
//}
//
//void TurboDisabler::free(void)
//{
//    LOG("TurboDisabler: free\n");
//
//    super::free();
//}
//
//IOService* TurboDisabler::probe(IOService* provider, SInt32* score)
//{
//    LOG("TurboDisabler: probe\n");
//
//    return (super::probe(provider, score));
//}





