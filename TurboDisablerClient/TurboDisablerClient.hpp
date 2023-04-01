//
//  TurboDisablerClient.h
//  TurboDisablerClient
//
//  Created by Viktor Remennik on 25.03.2023.
//

#ifndef TurboDisablerClient_hpp
#define TurboDisablerClient_hpp

#include "TurboDisabler.hpp"


class TurboDisablerClient : public IOUserClient
{
    OSDeclareDefaultStructors(TurboDisablerClient);

public:
    virtual bool initWithTask(task_t owningTask, void* securityToken, UInt32 type, OSDictionary* properties) override;
    virtual bool start(IOService* provider) override;
    virtual IOReturn clientClose(void) override;
    virtual void stop(IOService* provider) override;
    virtual void free(void) override;

    TurboDisabler *owner;
    task_t task;
};


#endif /* TurboDisablerClient_hpp */
