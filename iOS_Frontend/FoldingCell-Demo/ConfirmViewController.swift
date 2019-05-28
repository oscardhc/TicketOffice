//
//  ConfirmViewController.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/5/26.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material

class ConfirmViewController: UIViewController {
    
    @IBOutlet weak var confirmbutton: UIButton!
    
    
    @IBOutlet var detailTextfields: [TextField]!
    
    
    @IBOutlet weak var idTextfield: TextField!
    @IBOutlet weak var baseview: UIView!
    

    override func viewDidLoad() {
        
        super.viewDidLoad()
        var navBar = UINavigationBar(frame: CGRect(x: 0, y: 30, width: view.frame.width, height: 300))
        var navItem = UINavigationItem()
        var navBtn = UIBarButtonItem(title: "返回", style: .done, target: self, action: #selector(cancleBtnClicked(_:)))
        navItem.rightBarButtonItem = navBtn
        navBar.items = [navItem]
        self.view.addSubview(navBar)

        // Do any additional setup after loading the view.
    }
    @objc func cancleBtnClicked(_ sender: AnyObject) {
        self.dismiss(animated: true, completion: nil)
    }
    
    @objc func confirmbuttonclicked(_ sender: AnyObject){
        NetworkManager.default.postS(cmd: ["modify_profile", userID, detailTextfields[0].text!, userPassword, detailTextfields[1].text!,detailTextfields[2].text!].joined(separator: " "))
        refreshUserData()
        self.dismiss(animated: true, completion: nil)
    }
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        baseview.layer.cornerRadius = 10
        baseview.addShadow()
        idTextfield.isEnabled = false
        detailTextfields[3].isEnabled = false
        
        for (idx, tx) in detailTextfields.enumerated() {
            tx.text = userInfo[idx]
        }
        
        
        confirmbutton.backgroundColor = themeLightColor
        confirmbutton.addTarget(self, action: #selector(ConfirmViewController.confirmbuttonclicked(_:)), for: .touchUpInside)
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
