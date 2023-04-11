//
//  TurboDisablerClient.swift
//  TurboDisablerGUI
//
//  Created by Viktor Remennik on 09.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

import Foundation
import IOKit

class TurboDisablerClient {
    let serviceName="TurboDisabler"
    let propName="TurboMode"
    var service: io_service_t!
    var key: CFString!
    
    //    init?()     {
    init() {
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
        if !isServiceLoaded() {
            return false
        }
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
        if !isServiceLoaded() {
            return
        }
        IORegistryEntrySetCFProperty(service, key, newMode ? kCFBooleanTrue : kCFBooleanFalse)
    }
    
    func getService() -> io_service_t
    {
        var s: io_service_t
        
        key=CFStringCreateWithCString(kCFAllocatorDefault, propName, CFStringGetSystemEncoding())
        if #available(macOS 12.0, *) {
            s = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching(serviceName))
        }
        else
        {
            // Fallback on earlier versions
            s = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching(serviceName))
        }
        return(s)
    }
    
    func isServiceLoaded() -> Bool
    {
        service = getService()
        return service != 0
    }
}

