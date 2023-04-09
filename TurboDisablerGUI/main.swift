//
//  main.swift
//  TurboDisablerControl
//
//  Created by Viktor Remennik on 08.04.2023.
//

import Foundation
import Cocoa

let app = NSApplication.shared
let delegate = AppDelegate()
app.delegate = delegate

_ = NSApplicationMain(CommandLine.argc, CommandLine.unsafeArgv)

