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
    
    override var windowNibName: NSNib.Name! { // StatusWindow.xib is the file nam for the xib
        return NSNib.Name("InfoWindow")
    }
}
