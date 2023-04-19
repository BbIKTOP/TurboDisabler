//
//  TurboDisablerPower.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 31.03.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisabler.hpp"
#include <i386/proc_reg.h>


IOPMPowerState TurboDisabler::powerStates[TurboDisabler::kNumPowerStates] =
{   {   // Off
        kIOPMPowerStateVersion1,          // version
        0,   // capabilityFlags
        0,                      // outputPowerCharacter
        0,                      // inputPowerRequirement
        0, 0, 0, 0, 0, 0, 0, 0
    },
    {   // Sleep
        kIOPMPowerStateVersion1,          // version
        kIOPMSleepCapability,   // capabilityFlags
        kIOPMSleep,                      // outputPowerCharacter
        kIOPMSleep,                      // inputPowerRequirement
        0, 0, 0, 0, 0, 0, 0, 0
    },
    {   // On
        kIOPMPowerStateVersion1,          // version
        kIOPMPowerOn | kIOPMDeviceUsable,   // capabilityFlags
        kIOPMPowerOn,                      // outputPowerCharacter
        kIOPMPowerOn,                      // inputPowerRequirement
        0, 0, 0, 0, 0, 0, 0, 0
    },
};

void TurboDisabler::initPowerManagement(IOService* provider)
{   PMinit();
    provider->joinPMtree(this);
    makeUsable();
    registerPowerDriver(this, powerStates, kNumPowerStates);
}
void TurboDisabler::deinitPowerManagement(void)
{   PMstop();
}

//IOReturn TurboDisabler::powerStateWillChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice)
//{
//    LOG("powerStateWillChangeTo %lu\n", stateNumber);
//    return (kIOPMAckImplied);
//}


// Power state changed
IOReturn TurboDisabler::setPowerState(unsigned long powerStateOrdinal, IOService* whatDevice)
{   LOG("setPowerState %lu\n", powerStateOrdinal);

    if (getSavedTurboState() == turboStateUnknown)
    {   LOG("setPowerState: Current turbo state is not read yet\n");
        return (kIOPMAckImplied);
    }

    LOG("setPowerState: Current turbo state is %s\n", getSavedTurboState() == turboStateOn ? "On" : "Off");

    if (getSavedTurboState() == turboStateUnknown)
    {   TurboState nvramState;
        if (readTurboFromNvram(&nvramState) && nvramState != turboStateUnknown)
        {   setSavedTurboState(nvramState);
        }
    }

    if (getSavedTurboState() != turboStateUnknown)
    {   setCPUTurbo(getSavedTurboState());
        writeTurboToNvram(getSavedTurboState());
    }

    return (kIOPMAckImplied);
}

//IOReturn TurboDisabler::powerStateDidChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice)
//{
//    LOG("powerStateDidChangeTo %lu\n", stateNumber);
//    return (kIOPMAckImplied);
//
//}


TurboDisabler::TurboState TurboDisabler::getSavedTurboState(void)
{   if (!IOLockTryLock(lock))
    {   LOG("getCurrentTurboState: Can't acquire lock");
        return (turboStateUnknown);
    }
    TurboDisabler::TurboState state = currentState;
    IOLockUnlock(lock);
    return (state);
}

void TurboDisabler::setSavedTurboState(TurboDisabler::TurboState state)
{   if (!IOLockTryLock(lock))
    {   LOG("setCurrentTurboState: Can't acquire lock");
        return;
    }
    currentState = state;
    IOLockUnlock(lock);
}

/*
void TurboDisabler::readAndCheckSavedState(void)
{   TurboState state;

    if (readTurboFromNvram(&state) && state != turboStateUnknown) // Primary source
    {   LOG("readAndCheckSavedState: NVRAM state is %d, current state is %d", state, getSavedTurboState());
        if (getSavedTurboState() != state)
        {   LOG("readAndCheckSavedState: saved state does not match current state, setting new");
            setCPUTurbo(state);
            setSavedTurboState(state);
            updateTurboProperty(state);
        }
        else
        {   LOG("readAndCheckSavedState: cannot get state");
        }
    }
}

void TurboDisabler::checkAndSaveState(TurboState state)
{   if (getSavedTurboState() != turboStateUnknown) // Known state has been already read, we could proceed
    {
        TurboState nvramState;
        if(!readTurboFromNvram(&nvramState) || state == turboStateUnknown)
        {
            LOG("checkAndSaveState: failed to read state from NVRAM");
            return;
        }
        LOG("checkAndSaveState: NVRAM state is %d, current state is %d", nvramState, getSavedTurboState());
        if (getSavedTurboState() != state || nvramState!=getSavedTurboState())
        {   LOG("checkAndSaveState: saved state does not match current state, setting new");
            setCPUTurbo(state);
            setSavedTurboState(state);
            updateTurboProperty(state);
            if(!writeTurboToNvram(state))
            {
                setSavedTurboState(turboStateUnknown);
            }
        }
    }
    else
    {   LOG("checkAndSaveState: cannot get state");
    }
}

bool TurboDisabler::isStateAvailable(void)
{   if (getSavedTurboState() != turboStateUnknown)
    {   return (true);
    }
    readAndCheckSavedState();
    if (getSavedTurboState() != turboStateUnknown)
    {   return (true);
    }
    return (false);
}
 */

