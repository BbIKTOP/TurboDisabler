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
{   LOG("Start.\n");

    if (!super::start(provider))
    {   LOG("start: IOService start error\n");
        return (false);
    }

    lock = IOLockAlloc();
    if (lock == nullptr)
    {   LOG("start: cannot allocate lock\n");
        return (false);
    }

    initPowerManagement(provider);
    registerService();

    TurboState nvramState;
    if (readTurboFromNvram(&nvramState) && nvramState != turboStateUnknown) // Primary source
    {   LOG("start: NVRAM state is %d, current state is %d", nvramState, getSavedTurboState());
        if (getSavedTurboState() != nvramState)
        {   LOG("start: saved state does not match current state, setting new");
            setCPUTurbo(nvramState);
            setSavedTurboState(nvramState);
            updateTurboProperty(nvramState);
        }
    }
    else
    {   LOG("start: cannot read state from NVRAM");
    }

    setTimer();

    return (true);
}
void TurboDisabler::stop(IOService* provider)
{   LOG("stop. Power state will be saved as %d\n", getSavedTurboState());

    if (getSavedTurboState() != turboStateUnknown)
    {   writeTurboToNvram(getSavedTurboState());
    }

    deleteTimer();
    deinitPowerManagement();

    if (lock != nullptr) IOLockFree(lock);
    super::stop(provider);
}

bool TurboDisabler::requestTerminate(IOService * provider, IOOptionBits options)
{   LOG("requestTerminate from \"%s\"\n", provider->getName());
    return (true);
}


//bool TurboDisabler::init(OSDictionary* dict)
//{
//    if (!super::init(dict)) {
//        LOG("init failed\n");
//        return (false);
//    }
//    LOG("init\n");
//
//    return (true);
//}
//
//void TurboDisabler::free(void)
//{
//    LOG("free\n");
//
//    super::free();
//}
//
//IOService* TurboDisabler::probe(IOService* provider, SInt32* score)
//{
//    LOG("probe\n");
//
//    return (super::probe(provider, score));
//}





