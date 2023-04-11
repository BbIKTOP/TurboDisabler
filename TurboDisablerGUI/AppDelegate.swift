//
//  AppDelegate.swift
//  TurboDisablerGUI
//
//  Created by Viktor Remennik on 09.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

import Cocoa


class AppDelegate: NSObject, NSApplicationDelegate {
    
    var statusItem: NSStatusItem!
    var  statusButton: NSStatusBarButton!
    
    let iconOn =  NSImage(systemSymbolName: "bolt.fill", accessibilityDescription: "TurboDisabler Control")
    let iconOff =  NSImage(systemSymbolName: "bolt.slash.fill", accessibilityDescription: "TurboDisabler Control")
    let iconUnknown =  NSImage(systemSymbolName: "bolt.trianglebadge.exclamationmark.fill", accessibilityDescription: "TurboDisabler Control")
    
    let titleTextOn = "Turbo is on now"
    let titleTextOff = "Turbo is off now"
    let titleTextUnknown = "TurboDisabler.kext is not loaded"

    let switchTextOn = "Switch turbo off"
    let switchTextOff = "Switch turbo on"
    let switchTextUnknown = ""

    var menu: NSMenu!
    var menuItemTitle: NSMenuItem!
    var menuItemSwitch: NSMenuItem!
    var menuItemQuit: NSMenuItem!

    var timer: Timer!
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        iconOn?.isTemplate = true
        iconOff?.isTemplate = true
        iconUnknown?.isTemplate = true
        
        statusItem = NSStatusBar.system.statusItem(withLength: NSStatusItem.variableLength)
        statusButton = statusItem.button
        
//        statusButton.image = NSImage(systemSymbolName: "bolt.slash.fill", accessibilityDescription: "TurboDisabler Control")
        
        menuItemTitle = NSMenuItem(title: "TurboDisabler", action: nil, keyEquivalent: "")
        menuItemTitle.image = iconUnknown
        menuItemSwitch = NSMenuItem(title: "Switch", action: #selector(actionSwitchMode), keyEquivalent: "t")
        menuItemQuit = NSMenuItem(title: "Quit", action: #selector(actionQuit), keyEquivalent: "q")
        
        
        menu = NSMenu()
        menu.addItem(menuItemTitle)
        menu.addItem(menuItemSwitch)
        menu.addItem(NSMenuItem.separator())
        menu.addItem(menuItemQuit)
        
        statusItem.menu = menu
        
        titleView = TitleView(frame: NSRect(x: 0, y: 0, width: 250, height: 70))
        menuItemTitle.view = titleView
        
        titleView.setTitle(title: "Test")
        titleView.setAbout(action: actionAbout)
                
        timer = Timer.scheduledTimer(timeInterval: 1, target: self, selector: #selector(setupTextAndIcons), userInfo: nil, repeats: true)
        timer?.fire()
    }
    
    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
    
    func applicationSupportsSecureRestorableState(_ app: NSApplication) -> Bool {
        return true
    }
    
/* Interface setup ************************************************************ */
   
    @objc func setupTextAndIcons() {
//        print("Turbo is " + getTurboMode().description)
        switch getTurboMode() {
        case .turboUnknown:
            statusButton.image = iconUnknown
            titleView.setImage(image: iconUnknown!)
            titleView.setTitle(title: titleTextUnknown)
            menuItemSwitch.title = switchTextUnknown
            
            menuItemSwitch.isHidden = true
            
        case .turboOn:
            statusButton.image = iconOn
            titleView.setImage(image: iconOn!)
            titleView.setTitle(title: titleTextOn)
            menuItemSwitch.title = switchTextOn
            
            menuItemSwitch.isHidden = false

        case .turboOff:
            statusButton.image = iconOff
            titleView.setImage(image: iconOff!)
            titleView.setTitle(title: titleTextOff)
            menuItemSwitch.title = switchTextOff
            
            menuItemSwitch.isHidden = false
        }
    }
/* Interaction with driver ***************************************************** */
    enum Modes: CustomStringConvertible {
        case turboOn
        case turboOff
        case turboUnknown
        
        var description: String {
            get {
                switch self {
                case .turboOn:
                    return "On"
                case .turboOff:
                    return "Off"
                case .turboUnknown:
                    return "Unknown"
                }
            }
        }
    }

    var client = TurboDisablerClient()
    var currentMode: Modes = .turboUnknown
    func getTurboMode() -> Modes {
        if !client.isServiceLoaded() {
            currentMode = .turboUnknown
        }
        else
        {
            currentMode = client.getMode() ? .turboOn : .turboOff
        }
        return currentMode
    }
    func setTurboMode(mode: Bool) {
        if client.isServiceLoaded() {
            client.setMode(mode)
        }
        _ = getTurboMode()
    }
    
/* Actions ******************************************************/
    
    @objc func actionSwitchMode() {
        switch getTurboMode() {
        case .turboUnknown: return
        case .turboOn: setTurboMode(mode: false)
        case .turboOff: setTurboMode(mode: true)
        }
        setupTextAndIcons()
//        print("Turbo set to " + getTurboMode().description)
    }
    
    @objc func actionQuit() {
        NSApplication.shared.terminate(self)
    }
    func actionAbout()
    {
        let aboutController = InfoWindowController()
        aboutController.show()
    }
/* Title menu item ******************************************************/
    var titleView: TitleView!
}

