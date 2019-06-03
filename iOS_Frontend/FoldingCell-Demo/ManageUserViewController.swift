//
//  ManageUserViewController.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/5/30.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material

class ManageUserViewController: UIViewController {
    @IBOutlet weak var confirmbutton: UIButton!
    
    
    @IBOutlet var detailTextfields: [TextField]!
    
    
    
    @IBOutlet weak var baseview: UIView!

    @IBOutlet var searchbutton: UIButton!
    @IBOutlet weak var searchfield: UITextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        baseview.layer.cornerRadius = 10
        baseview.addShadow()
        searchfield.placeholder = "用户ID"
        searchbutton.addTarget(self, action: #selector(ManageUserViewController.searchbtnclicked), for: .touchUpInside)
        confirmbutton.backgroundColor = themeLightColor
        confirmbutton.addTarget(self, action: #selector(ManageUserViewController.confirmbuttonclicked(_:)), for: .touchUpInside)

        detailTextfields[1].minimumFontSize = 1.0
        detailTextfields[1].adjustsFontSizeToFitWidth = true
        
        // Do any additional setup after loading the view.
    }
    @objc func confirmbuttonclicked(_ sender: AnyObject){
        NetworkManager.default.postS(cmd: ["modify_profile", userID, detailTextfields[0].text!, userPassword, detailTextfields[1].text!,detailTextfields[2].text!].joined(separator: " "))
         AlertControl.default.showMessage(title: "提示", message: "修改成功！", viewCon: self)
        //self.dismiss(animated: true, completion: nil)
    }
    
    
    @objc func searchbtnclicked(btn: Any){
        let res = getUserProfile(id: (searchfield.text ?? "") == "" ? "2019" : searchfield.text!)
        
        for (idx, tx) in detailTextfields.enumerated() {
            tx.text = res[idx]
        }
        //searchfield.isEnabled = false
        
        
        
        
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
