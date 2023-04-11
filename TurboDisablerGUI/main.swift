//
//  main.swift
//  TurboDisablerGUI
//
//  Created by Viktor Remennik on 09.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

import Foundation
import Cocoa

let app = NSApplication.shared
let delegate = AppDelegate()
app.delegate = delegate

_ = NSApplicationMain(CommandLine.argc, CommandLine.unsafeArgv)

