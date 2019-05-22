//
//  UserViewController.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/4/30.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit

class UserViewController: UIViewController {
    
    lazy var rightbtn: UIButton = {
        var _rightbtn = UIButton(frame: CGRect(x: self.view.frame.width - 100, y: 50, width: 80, height: 80))
        _rightbtn.setImage(UIImage(named: "hd_7"), for: .normal)
        _rightbtn.addTarget(self, action: #selector(rightBtnClicked(btn:)), for: .touchUpInside)
        _rightbtn.contentMode = .scaleAspectFill
        _rightbtn.layer.masksToBounds = true
        _rightbtn.layer.cornerRadius = _rightbtn.frame.width / 2
        return _rightbtn
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        self.view.addSubview(lable)
        self.navigationController?.view.addSubview(rightbtn)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        
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
        var _l = UILabel(frame: CGRect(x: 100, y: 200, width: 250, height: 100))
        _l.text = "USER MANAGEMENT PAGE"
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
