//
//  TitleView.swift
//  TurboDisablerGUI
//
//  Created by Viktor Remennik on 09.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

import Foundation
import Cocoa

class TitleView: NSView {

    var actionAbout: (() ->Void)!
    
    @IBOutlet weak var titleText: NSTextField!
    @IBOutlet weak var titleImage: NSImageView!
    @IBAction func actionAbout(_ sender: Any) {
        if actionAbout != nil { actionAbout() }
    }
    
    required init?(coder decoder: NSCoder) {
        super.init(coder: decoder)
        loadView()
    }
    
    override init(frame frameRect: NSRect) {
        super.init(frame: frameRect)
        loadView()
    }
    

    func loadView() {
        var topLevelObjects: NSArray!
        let isNibLoaded = Bundle.main.loadNibNamed("TitleView", owner: self, topLevelObjects: &topLevelObjects)
        if !isNibLoaded {
            print("Failed to load NIB from file")
        }
        let tv = (topLevelObjects.first(where: { $0 is NSView }) as? NSView)!
        self.addSubview(tv)
        
//        tv.layer?.backgroundColor = NSColor(named: "MenuTitleColor")?.cgColor
        
        actionAbout = defaultAbout
    }

    
    func setTitle(title: String) {
        if titleText == nil {
            print("titleText is nil")
        }
        titleText.stringValue = title
    }
    
    func setImage(image: NSImage) {
        titleImage.image = image
    }
    
    func setAbout(action: @escaping () -> Void) {
        actionAbout = action
    }
    
    func defaultAbout() {
        print("Default about")
    }
}
