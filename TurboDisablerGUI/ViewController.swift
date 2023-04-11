//
//  ViewController.swift
//  TurboDisablerGUI
//
//  Created by Viktor Remennik on 09.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

import Cocoa

class ViewController: NSViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        print("viewDidLoad")

        // Do any additional setup after loading the view.
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
            print("View did set")
        }
    }


}

