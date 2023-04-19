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
#include <IOKit/IOTimerEventSource.h>

#include <i386/proc_reg.h>

#include <os/log.h>

#include "settings.hpp"

#ifndef DEBUG
#define LOG(f, ...) IOLog("TurboDisabler: " f, ##__VA_ARGS__)
#else
#define LOG(f, ...) ;
#endif

class TurboDisabler : public IOService
{   OSDeclareDefaultStructors(TurboDisabler);

public:

    virtual bool start(IOService* provider) override;
    virtual void stop(IOService * provider) override;
    //    virtual bool init(OSDictionary* dict) override;
    //    virtual IOService* probe(IOService* provider, SInt32* score) override;
    //    virtual void free(void) override;
    virtual IOReturn setProperties(OSObject* properties) override;

    //    virtual IOReturn        powerStateWillChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice) override;
    virtual IOReturn setPowerState(unsigned long powerStateOrdinal, IOService* whatDevice) override;
    //    virtual IOReturn        powerStateDidChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice) override;
    virtual bool requestTerminate(IOService * provider, IOOptionBits options) override;

    enum TurboState
    {   turboStateOn = 1,
        turboStateOff = 0,
        turboStateUnknown = -1
    };


protected:
    void initPowerManagement(IOService* provider);
    void deinitPowerManagement(void);

    bool readTurboFromNvram(TurboState *);
    bool writeTurboToNvram(TurboState state);

    void enableCPUTurbo(void);
    void disableCPUTurbo(void);
    TurboState getCPUTurbo(void);
    void setCPUTurbo(TurboState state);

    void updateTurboProperty(TurboState state);

    static void timerCallbackDispatcher(OSObject *owner, IOTimerEventSource * sender);

    void timerCallback(IOTimerEventSource * sender);

    TurboState getSavedTurboState(void);
    void setSavedTurboState(TurboState);

//    void readAndCheckSavedState(void); // main logic for reading saved state
//    void checkAndSaveState(TurboState); // Main logic for set/save state
//    bool isStateAvailable(void);

    bool setTimer(void);
    void deleteTimer(void);

private:
    volatile TurboState currentState = turboStateUnknown;
    static IOTimerEventSource * timer;
    volatile TurboState timerLastState = turboStateUnknown;

    IOLock* lock = nullptr;

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
};

#endif /* TurboDisabler_hpp */

