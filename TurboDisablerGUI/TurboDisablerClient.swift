//
//  TurboDisablerClient.swift
//  MenuLesson
//
//  Created by Viktor Remennik on 05.04.2023.
//

import Foundation
import IOKit

class TurboDisablerClient {
    let serviceName="TurboDisabler"
    let propName="TurboMode"
    var service:io_service_t=0
    var key:CFString
    
    init?()     {
        key=CFStringCreateWithCString(kCFAllocatorDefault, propName, CFStringGetSystemEncoding())
        service = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching(serviceName))
        if !isServiceLoaded()        {
            print("TurboDisabler service not found");
//            return nil;
        }
    }
    
    deinit
    {
        print("deinit")
        if isServiceLoaded()        {
            IOObjectRelease(service)
        }
    }
    
    
    func getMode() -> Bool
    {
        let currentModeRef=IORegistryEntrySearchCFProperty(service, kIOPowerPlane, key, kCFAllocatorDefault, 0)
        var currentMode: CFBoolean?
        
        if CFGetTypeID( currentModeRef) == CFBooleanGetTypeID()
        {
            currentMode=(currentModeRef as! CFBoolean)
        }
        
        print("Current mode is \(currentMode!)")
        return(currentMode == kCFBooleanTrue)
    }
    
    func setMode(_ newMode:Bool)
    {
        IORegistryEntrySetCFProperty(service, key, newMode ? kCFBooleanTrue : kCFBooleanFalse)
    }
    
    func isServiceLoaded() -> Bool
    {
        return service != 0
    }
}
