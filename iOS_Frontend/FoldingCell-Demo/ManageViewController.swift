//
//  ManageViewController.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/5/30.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Cards

class ManageViewController: UIViewController {

    
   
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        if userID == "" {
            trainCard.isUserInteractionEnabled = false
            userCard.isUserInteractionEnabled = false
            userCard.shouldPresent(nil, from: nil)
            AlertControl.default.showMessage(title: "错误", message: "您不具有此权限", viewCon: self)
            return;
        }
        if userInfo == [""] {
            trainCard.isUserInteractionEnabled = false
            userCard.isUserInteractionEnabled = false
            userCard.shouldPresent(nil, from: nil)
            AlertControl.default.showMessage(title: "错误", message: "您不具有此权限", viewCon: self)
            return;
        }
        if  userInfo[3] !=  "2" {
            trainCard.isUserInteractionEnabled = false
            userCard.isUserInteractionEnabled = false
            userCard.shouldPresent(nil, from: nil)
            AlertControl.default.showMessage(title: "错误", message: "您不具有此权限", viewCon: self)
            return;
        }
    }
    
    var trainCard, userCard: CardHighlight!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        trainCard = addCard(title: "列车管理", frame: CGRect(x: 38, y: 170, width: 300, height: 240), targetVC: UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "ManageTrainViewController"), image: UIImage(named: "train")!)
        userCard =  addCard(title: "用户管理", frame: CGRect(x: 38, y: 440, width: 300 , height: 240), targetVC: UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "ManageUserViewController"), image: UIImage(named: "user")!)
        
    }
    func addCard(title: String, frame: CGRect, targetVC: UIViewController, image: UIImage) -> CardHighlight {
        let card = CardHighlight(frame: frame)
        card.backgroundColor = .white
        card.backgroundImage = image
        card.title = title
        card.textColor = UIColor.white
        card.hasParallax = true
        card.shouldPresent(targetVC, from: self, fullscreen: true)
        view.addSubview(card)
        card.cardRadius = 20
        card.shadowBlur = 15
        card.shadowOpacity = 0.5
        return card;
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
