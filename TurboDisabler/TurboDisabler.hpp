//
//  TurboDisabler.h
//  TurboDisabler
//
//  Created by Viktor Remennik on 25.03.2023.
//

#ifndef TurboDisabler_hpp
#define TurboDisabler_hpp

#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include <IOKit/IOUserClient.h>
#include <IOKit/IONVRAM.h>

#include <i386/proc_reg.h>

#include <os/log.h>

#include "settings.hpp"

#ifndef DEBUG
#define LOG(f, ...) IOLog(f, ##__VA_ARGS__)
#else
#define LOG(f, ...) ;
#endif

class TurboDisabler : public IOService
{   OSDeclareDefaultStructors(TurboDisabler);

public:
    void setTurbo(bool mode);

    virtual bool start(IOService* provider) override;
    virtual void stop(IOService * provider) override;
    //    virtual bool init(OSDictionary* dict) override;
//    virtual IOService* probe(IOService* provider, SInt32* score) override;
//    virtual void free(void) override;
    virtual IOReturn setProperties(OSObject* properties) override;

    //    virtual IOReturn        powerStateWillChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice) override;
    virtual IOReturn        setPowerState(unsigned long powerStateOrdinal, IOService* whatDevice) override;
    //    virtual IOReturn        powerStateDidChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice) override;
    virtual bool requestTerminate(IOService * provider, IOOptionBits options) override;

protected:
    void initPowerManagement(IOService* provider);
    void deinitPowerManagement(void);

    void readTurboFromNvram(void);
    void writeTurboToNvram(void);

    void enableTurbo(void);
    void disableTurbo(void);
    bool getTurbo(void);
    void setTurbo(void);

    void updateTurboProperty(void);

private:
    bool currentTurboState = true;

public:
    enum
    {   kPowerStateOff,
        kPowerStateSleep,
        kPowerStateOn,
        //
        kNumPowerStates
    };
private:
    static IOPMPowerState powerStates[kNumPowerStates];


    friend class TurboDisablerClient;
};

#endif /* TurboDisabler_hpp */

