//
//  TurboDisablerNVRAM.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 01.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisabler.hpp"


void TurboDisabler::readTurboFromNvram(void)
{   IORegistryEntry *nvram = IORegistryEntry::fromPath("/options", gIODTPlane);
    if (!nvram || !OSDynamicCast(IODTNVRAM, nvram))
    {   LOG("TurboDisabler: readTurboFromNvram: failed to read NVRAM\n");
        OSSafeReleaseNULL(nvram);
        return;
    }

    OSData *storedMode = OSDynamicCast(OSData, nvram->getProperty(ENABLED_PROP_NAME));
    if (storedMode == nullptr || storedMode->getLength() <= 0)
    {   LOG("TurboDisabler: readTurboFromNvram: key " ENABLED_PROP_NAME " not found, defaulting to \"disabled\"\n");
        currentTurboState = false;
        OSSafeReleaseNULL(nvram);
        return;
    }
    const  uint8_t * storedModeData = static_cast<const uint8_t *>(storedMode->getBytesNoCopy());
    LOG("TurboDisabler: readTurboFromNvram: stored value for \""  ENABLED_PROP_NAME "\" is (%d bytes): %2.2X...\n", (int)storedMode->getLength(), (unsigned int)storedModeData[0]);

    currentTurboState = storedModeData[0];
    LOG("TurboDisabler: readTurboFromNvram: stored value for \""  ENABLED_PROP_NAME "\" is %s\n", currentTurboState ? "ON" : "OFF");

    OSSafeReleaseNULL(nvram);
}

void TurboDisabler::writeTurboToNvram(void)
{   IORegistryEntry *nvram = IORegistryEntry::fromPath("/options", gIODTPlane);
    if (!nvram || !OSDynamicCast(IODTNVRAM, nvram))
    {   LOG("TurboDisabler: writeTurboToNvram: failed to read NVRAM\n");
        OSSafeReleaseNULL(nvram);
        return;
    }

    OSData *storedMode = OSData::withBytes(currentTurboState ? "\x01" : "\x00", 1);
    if (!nvram->setProperty(ENABLED_PROP_NAME, storedMode))
    {   LOG("TurboDisabler: writeTurboToNvram: failed to write NVRAM\n");
    }

    OSSafeReleaseNULL(storedMode);
    OSSafeReleaseNULL(nvram);
}
