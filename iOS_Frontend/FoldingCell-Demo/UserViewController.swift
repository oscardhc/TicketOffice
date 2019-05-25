//
//  UserViewController.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/4/30.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material

var userInfo = [String]();

class UserViewController: UIViewController {
    
    lazy var rightbtn: UIButton = {
        var _rightbtn = UIButton(frame: CGRect(x: 20, y: 150, width: 120, height: 120))
        _rightbtn.setImage(UIImage(named: "hd_7"), for: .normal)
        _rightbtn.addTarget(self, action: #selector(rightBtnClicked(btn:)), for: .touchUpInside)
        _rightbtn.contentMode = .scaleAspectFill
        _rightbtn.layer.masksToBounds = true
        _rightbtn.layer.cornerRadius = _rightbtn.frame.width / 2
        return _rightbtn
    }()
    
    lazy var editbtn: Button = {
        var b = Button()
        var w = self.view.frame.width / 4
        var x = (0.2) * self.view.frame.width / 4
        var y = 300.0
        b.frame = CGRect(x: x, y: CGFloat(y), width: w, height: w)
        b.setImage(UIImage(named: "tkk.gif"), for: .normal)
        b.setTitle("修改信息", for: .normal)
        b.addShadow()
        return b
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        self.view.addSubview(lable)
        self.view.addSubview(rightbtn)
        self.view.addSubview(editbtn)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        if userInfo.count == 0 {
            lable.text = "请登录"
        } else {
            lable.text = userInfo[0];
        }
        
//        self.navigationController?.navigationBar.isHidden = false
//        self.navigationController?.navigationBar.barTintColor = UIColor.white
//        self.navigationController?.view.addSubview(rightbtn)
//        print(self.navigationItem)
//        print(self.navigationController?.view.window?.frame)
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)

    }
    
    @objc func rightBtnClicked(btn: UIBarButtonItem) {
        print("RIGHTBTNCLICKED!")
        let newView = UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "LoginViewController")
//        newView.title = "登录"
//        self.rightbtn.removeFromSuperview()
//        self.navigationController?.pushViewController(newView, animated: true)
        self.present(newView, animated: true, completion: nil)
    }
    
    lazy var lable: UILabel = {
        var _l = UILabel(frame: CGRect(x: 200, y: 200, width: 250, height: 100))
        if userInfo.count == 0 {
            _l.text = "请登录"
        } else {
            _l.text = userInfo[0];
        }
        return _l
    }()
    
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
