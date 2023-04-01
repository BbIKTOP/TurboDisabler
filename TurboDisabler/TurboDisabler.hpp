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
//#include <IOKit/IOMessage.h>
//#include <IOKit/IONotifier.h>
//#include <IOKit/IOPlatformExpert.h>
#include <IOKit/IONVRAM.h>
//#include <IOKit/IOReturn.h>
//#include <IOKit/IOTypes.h>
//#include <IOKit/IODeviceTreeSupport.h>

//#include <libkern/c++/OSSymbol.h>
//#include <libkern/libkern.h>

#include <i386/proc_reg.h>

#include <os/log.h>

#define LOG(f, ...) IOLog(f, ##__VA_ARGS__)

#define TURBO_MODE_DISABLED_MSR_BIT 0x4000000000 /* 1<<32 */
#define ENABLED_PROP_NAME "TurboMode" /* Used both for Power and NVRAM */

class TurboDisabler : public IOService
{   OSDeclareDefaultStructors(TurboDisabler);

public:
//    virtual bool init(OSDictionary* dict) override;
//    virtual IOService* probe(IOService* provider, SInt32* score) override;
//    virtual void free(void) override;

    virtual bool start(IOService* provider) override;
    virtual void stop(IOService * provider) override;
    virtual IOReturn setProperties(OSObject* properties) override;

    void updateTurboProperty(void);
    void setTurbo(bool mode);

protected:
    void initPowerManagement(IOService* provider);
    void deinitPowerManagement(void);

    void readTurboFromNvram(void);
    void writeTurboToNvram(void);

    void enableTurbo(void);
    void disableTurbo(void);
    bool getTurbo(void);
    bool getInitialTurbo(void);
    void setInitialTurbo(bool state);
    void setTurbo(void);

//    virtual IOReturn        powerStateWillChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice) override;
    virtual IOReturn        setPowerState(unsigned long powerStateOrdinal, IOService* whatDevice) override;
//    virtual IOReturn        powerStateDidChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice) override;

private:


    int initialTurboState;
    bool currentTurboState = true;

    friend class TurboDisablerClient;
};

#endif /* TurboDisabler_hpp */

