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
    
    @IBOutlet weak var BuyerIDLabel: UILabel!
    @IBOutlet weak var BuyerTelLabel: UILabel!
    @IBOutlet weak var BuyerEmailLabel: UILabel!
    
    @IBOutlet weak var confirmButtom: Button!
    
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
    

    @IBOutlet weak var baseView: UIView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
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
