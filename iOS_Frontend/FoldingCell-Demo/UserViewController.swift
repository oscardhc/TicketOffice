//
//  UserViewController.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/4/30.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material
import Cards

var userInfo = ["yyu","yyu@sjtu.edu.cn","233333333","2"];
var userID = "2019"
var userPassword = "2333333"

class UserViewController: UIViewController {
    
    
    @IBOutlet weak var LogoutButton: UIButton!
    
    @IBOutlet weak var aboutusButton: UIButton!
    
    @IBOutlet weak var refundButton: UIButton!
    @IBOutlet weak var nameLabel: UILabel!
    
    @IBOutlet weak var modifyButton: UIButton!
    
    @IBOutlet weak var orderButton: UIButton!
    
    lazy var rightbtn: UIButton = {
        var _rightbtn = UIButton(frame: CGRect(x: 20, y: 150, width: 120, height: 120))
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
        self.view.addSubview(rightbtn)
        
        modifyButton.setBackgroundImage(UIImage(named: "edit"), for: .normal)
        modifyButton.imageView?.contentMode = .scaleAspectFit
        orderButton.setBackgroundImage(UIImage(named: "order"), for: .normal)
        orderButton.imageView?.contentMode = .scaleAspectFit
        refundButton.setBackgroundImage(UIImage(named: "refund"), for: .normal)
        refundButton.imageView?.contentMode = .scaleAspectFit
        aboutusButton.setBackgroundImage(UIImage(named: "us"), for: .normal)
        aboutusButton.imageView?.contentMode = .scaleAspectFit
        
        //LogoutButton.backgroundImage(for: .normal) = UIImage(named: "logout")
        LogoutButton.layer.cornerRadius = 10
        
        // Aspect Ratio of 5:6 is preferred
        let card = CardHighlight(frame: CGRect(x: 100, y: 300, width: 100 , height: 120))
        
//        card.backgroundColor = UIColor(red: 0, green: 94/255, blue: 112/255, alpha: 1)
        card.backgroundColor = .white
//        card.icon = UIImage(named: "flappy")
        card.title = "退票"
//        card.itemTitle = "Flappy Bird"
//        card.itemSubtitle = "Flap That !"
        card.textColor = UIColor.black
        
        card.hasParallax = true
        
        print(card.layer.cornerRadius)
//        card.layer.cornerRadius = 0
        
        let cardContentVC = UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "RefundViewController")
        card.shouldPresent(cardContentVC, from: self, fullscreen: true)
        card.addShadow()
        view.addSubview(card)
        
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
//        super.viewWillAppear(animated)
        
        modifyButton.addShadow()
        modifyButton.addTarget(self, action: #selector(UserViewController.goToUserDetail(_:)), for: .touchUpInside)
        orderButton.addShadow()
        refundButton.addShadow()
        refundButton.addTarget(self, action: #selector(UserViewController.goToRefund(_:)), for: .touchUpInside)
        aboutusButton.addShadow()
        aboutusButton.addTarget(self, action: #selector(UserViewController.goToAboutUs(_:)), for: .touchUpInside)
        LogoutButton.addShadow()
        refreshUserData()
        
        
//        self.navigationController?.navigationBar.isHidden = false
//        self.navigationController?.navigationBar.barTintColor = UIColor.white
//        self.navigationController?.view.addSubview(rightbtn)
//        print(self.navigationItem)
//        print(self.navigationController?.view.window?.frame)
    }
    
    @objc func goToUserDetail(_ sender: Any?) {
        self.present(UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "ConfirmViewController"), animated: true, completion: nil)
    }
    @objc func goToRefund(_ sender: Any?){
        self.present(UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "RefundViewController"), animated: true, completion: nil)
    }
    @objc func goToAboutUs(_ sender: Any?){
        self.present(UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "AboutUsViewController"), animated: true, completion: nil)
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
    
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}

func refreshUserData() {
    NetworkManager.default.postA(cmd: ["query_profile", userID].joined(separator: " ") , done: { (ret) in
        let tmp = ret.split(separator: " ")
        userInfo = []
        for ii in tmp {
            userInfo.append(String(ii))
        }
    })
}
