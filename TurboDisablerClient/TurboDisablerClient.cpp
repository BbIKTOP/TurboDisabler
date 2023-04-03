//
//  TurboDisablerClient.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 01.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisablerClient.hpp"

int main(int argc, char*argv[])
{   int mode = -1;
    char *serviceName = (char*)SERVICE_NAME;

    if (argc > 1 && strcmp(argv[1], "-h") == 0)
    {   printf("Usage: %s <+|->\n"
               "\"+\" to set turbo mode to on\n"
               "\"-\" to set turbo mode to off\n", argv[0]);
        return (1);
    }

    if (argc > 1)
    {   switch (argv[1][0])
        {   case '+':
                mode = true;
                break;
            case '-':
                mode = false;
                break;
            default:
                printf("Invalid option: \"%s\", exiting.\n", argv[1]);
                return (2);
        }
        printf("Will now try to set turbo mode to the \"%s\" state.\n", mode ? "On" : "Off");
    }

    io_service_t ioService = IOServiceGetMatchingService(kIOMainPortDefault,
                             IOServiceMatching(serviceName));

    if (!ioService)
    {   printf("Service \"%s\" not found, probably TurboDisabler.kext is not loaded.\n", serviceName);
        return (1);
    }

    CFStringRef ioKey = CFStringCreateWithCString(kCFAllocatorDefault,
                        ENABLED_PROP_NAME,
                        CFStringGetSystemEncoding());

    CFBooleanRef currentMode = (CFBooleanRef) IORegistryEntrySearchCFProperty(ioService, kIOPowerPlane, ioKey, kCFAllocatorDefault, 0);

    printf("Current turbo mode is %s\n", currentMode == kCFBooleanTrue ? "On" : "Off");
    CFRelease(currentMode);

    if (mode < 0)
    {   return (0);
    }

    kern_return_t  res = IORegistryEntrySetCFProperty(ioService,
                         ioKey,
                         mode ? kCFBooleanTrue : kCFBooleanFalse);
    if (res != KERN_SUCCESS)
        printf("IORegistryEntrySetCFProperty error %ox8x\n", (int)res);

    CFRelease(ioKey);

    IOObjectRelease(ioService);

    printf("Turbo mode has been set to %s\n", mode ? "On" : "Off");

    return (0);
}
