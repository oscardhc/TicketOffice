//
//  BuyViewController.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/5/26.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material
var selectedType = -1
class BuyViewController: UIViewController {
    
    
    
    @IBOutlet weak var timeInforLabel: UILabel!
    @IBOutlet weak var trainID2Label: UILabel!
    
    @IBOutlet weak var BuyerName2Label: UILabel!
    @IBOutlet weak var BuyerIDLabel: UILabel!
    @IBOutlet weak var BuyerTelLabel: UILabel!
    @IBOutlet weak var BuyerEmailLabel: UILabel!
    
    @IBOutlet weak var ticketPrice2Label: UILabel!
    @IBOutlet weak var BuyerID2Label: UILabel!
    

    @IBOutlet weak var startstationEng2Label: UILabel!
    @IBOutlet weak var startstation2Label: UILabel!
    @IBOutlet weak var Tostation2EngLabel: UILabel!
    @IBOutlet weak var Tostation2Label: UILabel!
    @IBOutlet weak var confirmButtom: Button!
    
    @IBOutlet weak var ticketTypeLabel: UILabel!
    @IBOutlet weak var BuyerNameLabel: UILabel!
    @IBOutlet weak var startstationLabel: UILabel!
    
    @IBOutlet weak var TostationLabel: UILabel!
    
    @IBOutlet weak var trainIDLabel: UILabel!
    
    @IBOutlet weak var timeLabel: UILabel!
    
    
    @IBOutlet weak var view1: UIView!
    @IBOutlet var ticketTypeLabels: [UILabel]!
    
    @IBOutlet var ticketLeftLabels: [UILabel]!
    
    @IBOutlet var ticketpriceLabels: [UILabel]!
    @IBOutlet weak var view2: UIView!
    
    @IBOutlet var selectTypeButton: [UIButton]!
    
    @IBOutlet weak var colorBlock: UIView!
    
    @IBOutlet weak var confirmView: UIView!
    @IBOutlet weak var ticketImage: UIImageView!
    
    @IBOutlet weak var baseView: UIView!
    
    @IBOutlet weak var copyright: UILabel!
    @IBAction func confirmBtnClicked(_ sender: Any) {
        if selectedType != -1 {
            print(ticketData[currentlySelectedRow][selectedType])
            ticketPrice2Label.text =  "¥" + longToShort2(a: ticketData[currentlySelectedRow][selectedType][2])
            ticketTypeLabel.text = ticketData[currentlySelectedRow][selectedType][0]
            
            self.confirmView.isHidden = false
            ticketImage.addShadow()
            confirmView.addShadow()
            self.baseView.isHidden = true
//            confirmView.backgroundColor = .red
//            confirmView.alpha = 1.0
        }
        NetworkManager.default.postS(cmd: ["buy_ticket", userID, "1",trainIDLabel.text!,startstationLabel.text!,Tostation2Label.text!,queryDate, ticketData[currentlySelectedRow][selectedType][0]].joined(separator: " "))
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        var navBar = UINavigationBar(frame: CGRect(x: 0, y: 30, width: view.frame.width, height: 300))
        var navItem = UINavigationItem()
        var navBtn = UIBarButtonItem(title: "返回", style: .done, target: self, action: #selector(cancleBtnClicked(_:)))
        navItem.rightBarButtonItem = navBtn
        navBar.items = [navItem]
        self.view.addSubview(navBar)
        // Do any additional setup after loading the view.
        view1.backgroundColor = themeHeavyColor
        view1.layer.cornerRadius = 10
        view1.addShadow()
        view2.backgroundColor = themeHeavyColor
        view2.layer.cornerRadius = 10
        view2.addShadow()
        
        confirmButtom.backgroundColor = themeLightColor
        confirmButtom.layer.cornerRadius = 4
        
        
        confirmButtom.addShadow()
        
//        startstationLabel.textAlignment = .center
        startstationLabel.adjustsFontSizeToFitWidth = true
//        TostationLabel.textAlignment = .center
        TostationLabel.adjustsFontSizeToFitWidth = true
        startstationLabel.text = trainData[currentlySelectedRow][1]
        TostationLabel.text = trainData[currentlySelectedRow][4]
        print(ticketData[currentlySelectedRow][0][1])
        
        ticketTypeLabel.text = ticketData[currentlySelectedRow][0][0]
        let tra = transferTime(t1: trainData[currentlySelectedRow][3], t2: trainData[currentlySelectedRow][6])
        timeLabel.text = "\(tra / 60)小时\(tra % 60)分钟"
        trainIDLabel.text = trainData[currentlySelectedRow][0]
        
        BuyerNameLabel.text = userInfo[0]
        BuyerIDLabel.text = userID
        BuyerEmailLabel.text = userInfo[1]
        BuyerTelLabel.text = userInfo[2]
        
        startstation2Label.text = trainData[currentlySelectedRow][1]
        startstationEng2Label.text = trainData[currentlySelectedRow][1].transformToPinyin()
        Tostation2Label.text = trainData[currentlySelectedRow][4]
        Tostation2EngLabel.text = trainData[currentlySelectedRow][4].transformToPinyin()
        trainID2Label.text = trainData[currentlySelectedRow][0]
        timeInforLabel.text = trainData[currentlySelectedRow][2] + "日" + trainData[currentlySelectedRow][3] + "开"
        BuyerID2Label.text = userInfo[1]
        BuyerName2Label.text = userInfo[0]
        confirmView.isHidden = true
        colorBlock.backgroundColor = UIColor.white.withAlphaComponent(0.0)
        for i in 0..<5 {
            selectTypeButton[i].setTitle("", for: .normal)
            selectTypeButton[i].tag = i
            selectTypeButton[i].backgroundColor = UIColor.white.withAlphaComponent(0.0)
        }
        for i in 0..<ticketData[currentlySelectedRow].count {
            ticketTypeLabels[i].text = ticketData[currentlySelectedRow][i][0]
            ticketLeftLabels[i].text = ticketData[currentlySelectedRow][i][1]
            ticketpriceLabels[i].text = "¥" + longToShort(a: ticketData[currentlySelectedRow][i][2]) 
            selectTypeButton[i].addTarget(self, action: #selector(BuyViewController.handleSelectType(_:)), for: .touchUpInside)
        }
        for i in ticketData[currentlySelectedRow].count..<5 {
            ticketTypeLabels[i].text = ""
            ticketLeftLabels[i].text = ""
            ticketpriceLabels[i].text = ""
        }
        
    }
    @objc func cancleBtnClicked(_ sender: AnyObject) {
        self.dismiss(animated: true, completion: nil)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        baseView.layer.cornerRadius = 10
        baseView.addShadow()
        selectedType = -1
    }
    
    @objc func handleSelectType(_ sender: Button) {
        print("! \(sender.tag)")
//        for i in 0..<5 {
//            selectTypeButton[i].backgroundColor = UIColor.white.withAlphaComponent(0.0)
//        }
//        selectTypeButton[sender.tag].backgroundColor = themeLightColor
        if selectedType == -1 {
            selectedType = sender.tag
            self.colorBlock.frame = self.selectTypeButton[selectedType].frame
            UIView.animate(withDuration: 0.5) {
               self.colorBlock.backgroundColor = themeLightColor
            }
        } else {
            UIView.animate(withDuration: 0.3, delay: 0.0, options: .curveEaseOut, animations: {
                selectedType = sender.tag
                self.colorBlock.frame = self.selectTypeButton[selectedType].frame
            }, completion: nil)
        }
        UIView.animate(withDuration: 0.3) {
            self.selectTypeButton[selectedType].backgroundColor = UIColor.white.withAlphaComponent(0.0)
            
            
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

extension String {
    func transformToPinyin() -> String {
        let stringRef = NSMutableString(string: self) as CFMutableString
        // 转换为带音标的拼音
        CFStringTransform(stringRef,nil, kCFStringTransformToLatin, false);
        // 去掉音标
        CFStringTransform(stringRef, nil, kCFStringTransformStripCombiningMarks, false);
        let pinyin = stringRef as String;
        return pinyin
    }
}
