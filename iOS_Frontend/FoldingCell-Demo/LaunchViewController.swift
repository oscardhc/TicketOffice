//
//  LaunchViewController.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/5/21.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit

class LaunchViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        print("launched!")
        RunLoop.current.run(until: NSDate(timeIntervalSinceNow:1) as Date)
        print("slept!")
        // Do any additional setup after loading the view.
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
