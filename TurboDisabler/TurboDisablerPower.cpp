//
//  TurboDisablerPower.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 31.03.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisabler.hpp"
#include <i386/proc_reg.h>


enum
{   kPowerStateOff,
    kPowerStateSleep,
    kPowerStateOn,
//
    kNumPowerStates
};

static IOPMPowerState powerStates[kNumPowerStates] =
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
//    LOG("TurboDisabler: powerStateWillChangeTo %lu\n", stateNumber);
//    return (kIOPMAckImplied);
//}

IOReturn TurboDisabler::setPowerState(unsigned long powerStateOrdinal, IOService* whatDevice)
{   LOG("TurboDisabler: setPowerState %lu\n", powerStateOrdinal);
    if (currentTurboState) enableTurbo();
    else disableTurbo();

    return (kIOPMAckImplied);
}

//IOReturn TurboDisabler::powerStateDidChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice)
//{
//    LOG("TurboDisabler: powerStateDidChangeTo %lu\n", stateNumber);
//    return (kIOPMAckImplied);
//
//}
