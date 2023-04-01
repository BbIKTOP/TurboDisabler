//
//  TurboDisablerClient.c
//  TurboDisablerClient
//
//  Created by Viktor Remennik on 25.03.2023.
//

#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include <IOKit/IOUserClient.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IONotifier.h>
#include <IOKit/IOPlatformExpert.h>
#include <IOKit/IOReturn.h>
#include <IOKit/IOTypes.h>

#include <os/log.h>

#include "TurboDisablerClient.hpp"


#define super IOUserClient

OSDefineMetaClassAndStructors(TurboDisablerClient, super);

bool TurboDisablerClient::initWithTask(task_t owningTask,
                                       void* securityToken,
                                       UInt32 type,
                                       OSDictionary* properties)
{
    if (!owningTask) {
        return (false);
    }

    if (super::initWithTask(owningTask, securityToken, type, properties)) {
        return (false);
    }
    task = owningTask;
    return (true);
}

bool TurboDisablerClient::start(IOService* provider)
{
    if (! super::start(provider)) {
        return (false);
    }
    owner = OSDynamicCast(TurboDisabler, provider);
    if (!owner) {
        return (false);
    }
    return (true);
}

IOReturn TurboDisablerClient::clientClose(void)
{
    return (kIOReturnSuccess);
}

void    TurboDisablerClient::stop(IOService* provider)
{}
void    TurboDisablerClient::free(void)
{}
