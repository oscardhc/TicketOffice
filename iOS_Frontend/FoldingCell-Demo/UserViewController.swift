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
var jumpflag = false

class UserViewController: UIViewController {
    
    
    @IBOutlet weak var LogoutButton: Button!
    
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
    
    var modifyCard, orderCard, refundCard, aboutUsCard : CardHighlight!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        
        self.view.addSubview(rightbtn)
        
        modifyButton.setBackgroundImage(UIImage(named: "edit"), for: .normal)
        modifyButton.imageView?.contentMode = .scaleAspectFit
        modifyButton.removeFromSuperview()
        orderButton.setBackgroundImage(UIImage(named: "order"), for: .normal)
        orderButton.imageView?.contentMode = .scaleAspectFit
        orderButton.removeFromSuperview()
        refundButton.setBackgroundImage(UIImage(named: "refund"), for: .normal)
        refundButton.imageView?.contentMode = .scaleAspectFit
        refundButton.removeFromSuperview()
        aboutusButton.setBackgroundImage(UIImage(named: "us"), for: .normal)
        aboutusButton.imageView?.contentMode = .scaleAspectFit
        aboutusButton.removeFromSuperview()
        nameLabel.textAlignment = .center
        
        //LogoutButton.backgroundImage(for: .normal) = UIImage(named: "logout")
        LogoutButton.layer.cornerRadius = 4
        LogoutButton.addShadow()
        LogoutButton.addTarget(self, action: #selector(UserViewController.logoutBtnClicked(btn:)), for: .touchUpInside)
        
        modifyCard = addCard(title: "修改", frame: CGRect(x: 33, y: 320, width: 90 , height: 108), targetVC: UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "ConfirmViewController"), image: UIImage(named: "edit")!)
        orderCard = addCard(title: "订单", frame: CGRect(x: 143, y: 320, width: 90 , height: 108), targetVC: UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "OrderViewController"), image: UIImage(named: "order")!)
        refundCard = addCard(title: "退票", frame: CGRect(x: 253, y: 320, width: 90 , height: 108), targetVC: UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "RefundViewController"), image: UIImage(named: "refund")!)
        aboutUsCard = addCard(title: "我们", frame: CGRect(x: 33, y: 450, width: 90 , height: 108), targetVC: UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "AboutUsViewController"), image: UIImage(named: "us")!)
        
    }
    
    func addCard(title: String, frame: CGRect, targetVC: UIViewController, image: UIImage) -> CardHighlight {
        let card = CardHighlight(frame: frame)
        card.backgroundColor = .white
        card.icon = image
        card.title = title
        card.textColor = UIColor.black
        card.hasParallax = true
        card.shouldPresent(targetVC, from: self, fullscreen: true)
        view.addSubview(card)
        return card
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
//        super.viewWillAppear(animated)
        
        if userID == "" {
            if shouldGoToQuery {
                shouldGoToQuery = false
                self.tabBarController?.selectedIndex = 0
                return;
            }
            let newView = UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "LoginViewController")
            self.present(newView, animated: true, completion: nil)
            return;
        }
        
        refreshUserData(fun: {
            self.nameLabel.text = userInfo[0]
        })
        print("view will appear")
        
        if jumpflag == true {
            print("!!!!!!")
            jumpflag = false
            DispatchQueue.main.asyncAfter(deadline: .now() + 0) {
                self.refundCard.open()
            }
        }
        
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
//        print("RIGHTBTNCLICKED!")
        modifyCard.open()
    }
    
    @objc func logoutBtnClicked(btn: Button) {
        userID = ""
        userInfo = []
        let newView = UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "LoginViewController")
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

func refreshUserData(fun: @escaping () -> Void) {
    NetworkManager.default.postA(cmd: ["query_profile", userID].joined(separator: " ") , done: { (ret) in
        let tmp = ret.split(separator: " ")
        print(tmp)
        userInfo = []
        for ii in tmp {
            userInfo.append(String(ii))
        }
        fun()
    })
}

func getUserProfile(id: String) -> [String] {
    let res = NetworkManager.default.postS(cmd: ["query_profile", id].joined(separator: " "))
    let tmp = res.split(separator: " ")
    var ar :[String] = []
    for ii in tmp {
        ar.append(String(ii))
    }
    return ar;
}
