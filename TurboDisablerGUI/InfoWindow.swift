//
//  AboutWindow.swift
//  TurboDisablerGUI
//
//  Created by Viktor Remennik on 11.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

import Foundation
import Cocoa

class InfoWindowController: NSWindowController {
    
    @IBOutlet var infoWindow: NSWindow!
    @IBOutlet weak var infoIcon: NSImageView!
    
    override var windowNibName: String! {
        return "InfoWindow"
    }
    
    override func windowDidLoad() {
//        infoIcon.image?.isTemplate = true
    }
    
    func showWin(_ sender: Any?) {
        _ = window
        infoWindow?.center()
        infoWindow?.makeKeyAndOrderFront(self)
    }
}
