//
//  BuyViewController.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/5/26.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material

class BuyViewController: UIViewController {
    
    var selectedType = -1
    
    @IBOutlet weak var timeInforLabel: UILabel!
    @IBOutlet weak var trainID2Label: UILabel!
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
    
    
    @IBOutlet var ticketTypeLabels: [UILabel]!
    
    @IBOutlet var ticketLeftLabels: [UILabel]!
    
    @IBOutlet var ticketpriceLabels: [UILabel]!
    
    @IBOutlet var selectTypeButton: [UIButton]!
    
    @IBOutlet weak var colorBlock: UIView!
    
    @IBOutlet weak var confirmView: UIView!
    @IBOutlet weak var ticketImage: UIImageView!
    
    @IBOutlet weak var baseView: UIView!
    
    @IBOutlet weak var copyright: UILabel!
    @IBAction func confirmBtnClicked(_ sender: Any) {
        if selectedType != -1 {
            
            
            self.confirmView.isHidden = false
            ticketImage.addShadow()
            confirmView.addShadow()
            self.baseView.isHidden = true
            
        }
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
        
        startstation2Label.text = trainData[currentlySelectedRow][1]
        Tostation2Label.text = trainData[currentlySelectedRow][4]
        trainID2Label.text = trainData[currentlySelectedRow][0]
        timeInforLabel.text = trainData[currentlySelectedRow][2] + "日" + trainData[currentlySelectedRow][3] + "开"
        BuyerID2Label.text = userInfo[1]
        BuyerNameLabel.text = userInfo[0]
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
            ticketpriceLabels[i].text = ticketData[currentlySelectedRow][i][2]
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
    }
    
    @objc func handleSelectType(_ sender: Button) {
//        for i in 0..<5 {
//            selectTypeButton[i].backgroundColor = UIColor.white.withAlphaComponent(0.0)
//        }
//        selectTypeButton[sender.tag].backgroundColor = themeLightColor
        if selectedType == -1 {
            self.selectedType = sender.tag
            self.colorBlock.frame = self.selectTypeButton[self.selectedType].frame
            UIView.animate(withDuration: 0.5) {
               self.colorBlock.backgroundColor = themeLightColor
            }
        } else {
            UIView.animate(withDuration: 0.3, delay: 0.0, options: .curveEaseOut, animations: {
                self.selectedType = sender.tag
                self.colorBlock.frame = self.selectTypeButton[self.selectedType].frame
            }, completion: nil)
        }
        UIView.animate(withDuration: 0.3) {
            self.selectTypeButton[self.selectedType].backgroundColor = UIColor.white.withAlphaComponent(0.0)
            
            
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
