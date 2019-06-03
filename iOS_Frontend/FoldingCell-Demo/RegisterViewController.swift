//
//  RegisterViewController.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/6/2.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material

class RegisterViewController: UIViewController {

    @IBOutlet weak var baseview: UIView!
    
    @IBOutlet var detailfields: [TextField]!
    
    @IBOutlet weak var comfirmpassword: TextField!
    
    @IBOutlet weak var confirmbutton: UIButton!
    override func viewDidLoad() {
        super.viewDidLoad()

        confirmbutton.backgroundColor = themeLightColor
        confirmbutton.layer.cornerRadius = 4
        confirmbutton.addShadow()
        baseview.layer.cornerRadius = 20
        baseview.addShadow()
        confirmbutton.addTarget(self, action: #selector(RegisterViewController.confirmbuttonclicked(_:)), for: .touchUpInside)
        
        
        // Do any additional setup after loading the view.
    }
    
    @objc func confirmbuttonclicked(_ sender: AnyObject){
        if detailfields[1].text! == "" || detailfields[0].text! == "" || detailfields[3].text! == "" || detailfields[2].text! == "" {
            AlertControl.default.showMessage(title: "错误", message: "输入不能为空", viewCon: self)
            return
        }
        if detailfields[1].text != comfirmpassword.text {
            AlertControl.default.showMessage(title: "错误", message: "密码不一致", viewCon: self)
            return
        }
        userInfo.append(detailfields[0].text!)
        userInfo.append(detailfields[2].text!)
        userInfo.append(detailfields[3].text!)
        userInfo.append("1")
        userPassword = detailfields[1].text!
        var res = NetworkManager.default.postS(cmd: ["register", detailfields[0].text!, detailfields[1].text!,detailfields[2].text!,detailfields[3].text!].joined(separator: " "))
    
        userID = res
        
//        AlertControl.default.showMessage(title: "提示", message: "您的ID是\(res)", viewCon: self)
        
        var vv = UILabel(frame: CGRect(x: 80, y: 250, width: 200, height: 150))
        vv.alpha = 1.0
        vv.backgroundColor = .white
        vv.layer.cornerRadius = 10
        vv.addShadow()
        vv.textAlignment = .center
        vv.text = "注册成功！\n \n 您的ID是\(res)"
        vv.numberOfLines = 3
        self.view.addSubview(vv)
        
        DispatchQueue.main.asyncAfter(deadline: DispatchTime.now() + 2.5) {
            vv.removeFromSuperview()
            self.present(TBViewController(), animated: true, completion: nil)
        }
        
        
        
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
