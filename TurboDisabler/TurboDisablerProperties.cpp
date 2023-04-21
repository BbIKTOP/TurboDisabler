//
//  TurboDisablerProperties.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 31.03.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisabler.hpp"



IOReturn TurboDisabler::setProperties(OSObject* properties) // Process roperties from client app
{   LOG("setProperties");

    if (getSavedTurboState() == turboStateUnknown)
    {   TurboState nvramState;
        if (readTurboFromNvram(&nvramState) && nvramState != turboStateUnknown)
        {   setSavedTurboState(nvramState);
        }
    }
    if (getSavedTurboState() == turboStateUnknown)
    {   LOG("setProperties: current state is unknown");
        return (kIOReturnUnsupported);
    }

//    volatile unsigned long lh, ll;
//    lh = (unsigned long)this / 0x10000;
//    ll = (unsigned long)this & 0xFFFF;
//    LOG("setting properties for %4.4lX%4.4lX", lh, ll);


    if (properties == nullptr)
    {   LOG("setProperties: no properties");
        return (kIOReturnUnsupported);
    }

    OSDictionary* propertiesDict;
    propertiesDict = OSDynamicCast(OSDictionary, properties);
    if (propertiesDict == nullptr)
    {   LOG("setProperties: cannot get properties");
        return (kIOReturnUnsupported);
    }
    OSObject * rawValue = propertiesDict->getObject(ENABLED_PROP_NAME);
    OSBoolean * boolValue = OSDynamicCast(OSBoolean, rawValue);
    if (boolValue == nullptr)
    {   LOG("setProperties: cannot get \"" ENABLED_PROP_NAME "\" property");
    }
    else
    {   LOG("setProperties: new value %s\n", boolValue->getValue() ? "Enabled" : "Disabled");
        TurboState newState = boolValue->getValue() ? turboStateOn : turboStateOff;
        setSavedTurboState(newState);
        setCPUTurbo(newState);
        updateTurboProperty(newState);
        writeTurboToNvram(newState);
    }
    LOG("setProperties: \"" ENABLED_PROP_NAME "\" set to %d\n", getSavedTurboState());
    return (kIOReturnSuccess);
}

void TurboDisabler::updateTurboProperty(TurboDisabler::TurboState state)
{   if (state == turboStateUnknown)
    {   removeProperty(ENABLED_PROP_NAME);
        return;
    }
    OSBoolean * osTurbo = OSBoolean::withBoolean(state == turboStateOn);
    if (!setProperty(ENABLED_PROP_NAME, osTurbo))
    {   LOG("updateTurboProperty: cannot set \"" ENABLED_PROP_NAME "\" property");
    }
    OSSafeReleaseNULL(osTurbo);
}
