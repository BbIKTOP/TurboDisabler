//
//  TurboDisablerProperties.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 31.03.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisabler.hpp"



IOReturn TurboDisabler::setProperties(OSObject* properties)
{   LOG("TurboDisabler: setProperties\n");

    if (properties == nullptr)
    {   LOG("TurboDisabler: setProperties: no properties\n");
        return (kIOReturnUnsupported);
    }

    OSDictionary* propertiesDict;
    propertiesDict = OSDynamicCast(OSDictionary, properties);
    if (propertiesDict == nullptr)
    {   LOG("TurboDisabler: setProperties: cannot get properties\n");
        return (kIOReturnUnsupported);
    }
    OSObject * rawValue = propertiesDict->getObject(ENABLED_PROP_NAME);
    OSBoolean * boolValue = OSDynamicCast(OSBoolean, rawValue);
    if (boolValue == nullptr)
    {   LOG("TurboDisabler: setProperties: cannot get \"" ENABLED_PROP_NAME "\" property\n");
        currentTurboState = (bool)getTurbo();
    }
    else
    {   LOG("TurboDisabler: setProperties: new value %s\n", boolValue->getValue() ? "Enabled" : "Disabled");
        currentTurboState = boolValue->getValue();
    }
    LOG("TurboDisabler: setProperties: \"" ENABLED_PROP_NAME "\" set to %s\n", currentTurboState ? "ON" : "OFF");

    setTurbo();

    return (kIOReturnSuccess);
}

void TurboDisabler::updateTurboProperty(void)
{   OSBoolean * osTurbo = OSBoolean::withBoolean(currentTurboState);
    setProperty(ENABLED_PROP_NAME, osTurbo);
    OSSafeReleaseNULL(osTurbo);
}

