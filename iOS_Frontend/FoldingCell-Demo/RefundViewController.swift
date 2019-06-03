//
//  RefundViewController.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/5/27.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material

class RefundViewController: UIViewController {

    @IBOutlet weak var refundtickettype: TextField!
    @IBOutlet weak var refunddate: TextField!
    @IBOutlet weak var refundnum: TextField!
    @IBOutlet weak var refundID: TextField!
    @IBOutlet weak var confirmbutton: Button!
    @IBOutlet weak var baseview: UIView!
    override func viewDidLoad() {
        super.viewDidLoad()
        
        
        refundID.placeholder = "列车ID"
        refundnum.placeholder = "退票张数"
        refunddate.placeholder = "退票日期"
        refundtickettype.placeholder = "退票类型"
            
        baseview.layer.cornerRadius = 10
        baseview.addShadow()
        confirmbutton.backgroundColor = themeLightColor
        confirmbutton.addTarget(self, action: #selector(RefundViewController.confirmbtnclicked(_:)), for: .touchUpInside)
        
    }
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        if refundData != [] {
            refundID.text = refundData[0]
            refundnum.text = refundData[8]
            refunddate.text = refundData[2]
            refundtickettype.text = refundData[7]
        }
    }
    @objc func cancleBtnClicked(_ sender: AnyObject) {
        self.dismiss(animated: true, completion: nil)
    }
    @objc func confirmbtnclicked(_ sender: Any){
        let sth = refundData[1]
        let sss = refundData[4]
        let ar: [String] = ["refund_ticket", userID,refundnum!.text!,refundID!.text!, sth, sss, refunddate!.text!, refundtickettype!.text!]
        NetworkManager.default.postS(cmd: ar.joined(separator: " "))
        AlertControl.default.showMessage(title: "提示", message: "退票成功", viewCon: self)
        
        self.present(TBViewController(), animated: true, completion: nil)
        
        
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
