//
//  RefundViewController.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/5/27.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit

class RefundViewController: UIViewController {

    @IBOutlet weak var confirmbutton: UIButton!
    @IBOutlet weak var baseview: UIView!
    override func viewDidLoad() {
        super.viewDidLoad()
        var navBar = UINavigationBar(frame: CGRect(x: 0, y: 30, width: view.frame.width, height: 300))
        var navItem = UINavigationItem()
        var navBtn = UIBarButtonItem(title: "返回", style: .done, target: self, action: #selector(cancleBtnClicked(_:)))
        navItem.rightBarButtonItem = navBtn
        navBar.items = [navItem]
        self.view.addSubview(navBar)

        baseview.layer.cornerRadius = 10
        baseview.addShadow()
        confirmbutton.backgroundColor = themeLightColor
        

        
        // Do any additional setup after loading the view.
    }
    @objc func cancleBtnClicked(_ sender: AnyObject) {
        self.dismiss(animated: true, completion: nil)
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
