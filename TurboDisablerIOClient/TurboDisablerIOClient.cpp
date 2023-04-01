//
//  TurboDisablerIOClient.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 01.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisablerIOClient.hpp"

int main(int argc, char*argv[])
{   bool mode = false;
    char *serviceName = (char*)SERVICE_NAME;

    if (argc > 1 && argv[argc - 1][0] == '+')
        mode = false;

    printf("Use \"%s +\" to enable cpu turbo mode, any other to disable turbo mode.\n", argv[0]);
    printf("Will now try to set turbo mode to the \"%s\" state.\n", mode ? "On" : "Off");

    io_service_t ioService = IOServiceGetMatchingService(kIOMainPortDefault,
                             IOServiceMatching(serviceName));

    if (!ioService)
    {   printf("Service \"%s\" not found, probably TurboDisabler.kext is not loaded.\n", serviceName);
        return (1);
    }

    CFStringRef ioKey = CFStringCreateWithCString(kCFAllocatorDefault,
                        ENABLED_PROP_NAME,
                        CFStringGetSystemEncoding());

    kern_return_t res = IORegistryEntrySetCFProperty(ioService,
                        ioKey,
                        mode ? kCFBooleanTrue : kCFBooleanFalse);
    if (res != KERN_SUCCESS)
        printf("IORegistryEntrySetCFProperty error %ox8x\n", (int)res);

    CFRelease(ioKey);

    IOObjectRelease(ioService);

    printf("turbo mode has been set to %s\n", mode ? "ON" : "OFF");

    return (0);
}
