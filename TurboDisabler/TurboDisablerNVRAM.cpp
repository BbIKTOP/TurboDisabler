//
//  TurboDisablerNVRAM.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 01.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisabler.hpp"


bool TurboDisabler::readTurboFromNvram(TurboDisabler::TurboState * turboState)
{   bool state;

    *turboState = turboStateUnknown;

    IORegistryEntry *nvram = fromPath("/options", gIODTPlane);
    if (!nvram)
    {   LOG("readTurboFromNvram: failed to read from NVRAM\n");
        OSSafeReleaseNULL(nvram);
        return (false);
    }

    OSData *storedMode = OSDynamicCast(OSData, nvram->getProperty(ENABLED_PROP_NAME));
    if (storedMode == nullptr || storedMode->getLength() <= 0)
    {   LOG("readTurboFromNvram: key " ENABLED_PROP_NAME " not found, defaulting to \"enabled\"\n");
        OSSafeReleaseNULL(nvram);
        *turboState = turboStateOn;
        return (true);
    }
    const uint8_t * storedModeData = static_cast<const uint8_t *>(storedMode->getBytesNoCopy());
//    LOG("readTurboFromNvram: stored value for \""  ENABLED_PROP_NAME "\" is (%d byte(s)): %2.2X...\n", (int)storedMode->getLength(), (unsigned int)storedModeData[0]);

    state = storedModeData[0] != 0;
//    LOG("readTurboFromNvram: stored value for \""  ENABLED_PROP_NAME "\" is %s\n", state ? "ON" : "OFF");

    OSSafeReleaseNULL(nvram);
    *turboState = state ? turboStateOn : turboStateOff;
    return (true);
}

bool TurboDisabler::writeTurboToNvram(TurboDisabler::TurboState state)
{   if (state == turboStateUnknown)
    {   LOG("writeTurboToNvram: new state is unknown\n");
        return (false);
    }

    IORegistryEntry *nvram = fromPath("/options", gIODTPlane);
    if (!nvram)
    {   LOG("writeTurboToNvram: failed to read from NVRAM\n");
        OSSafeReleaseNULL(nvram);
        return (false);
    }

    uint8_t buffer = state == turboStateOn ? 1 : 0;
    OSData *storedMode = OSData::withBytes(&buffer, 1);
    if (!nvram->setProperty(ENABLED_PROP_NAME, storedMode))
    {   LOG("writeTurboToNvram: failed to write to NVRAM\n");
    }
//    LOG("writeTurboToNvram: wrote %d\n", (int)buffer);

    OSSafeReleaseNULL(storedMode);
    OSSafeReleaseNULL(nvram);
    return (true);
}
