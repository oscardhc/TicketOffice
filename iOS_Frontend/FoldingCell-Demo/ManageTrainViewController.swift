//
//  ManageTrainViewController.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/5/30.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material

class ManageTrainViewController: UIViewController {
    
    @IBOutlet weak var view1: UIImageView!
    

    override func viewWillAppear(_ animated: Bool) {
        
      
        super.viewWillAppear(animated)
    }
    
    @IBOutlet weak var searchTextField: TextField!
    @IBOutlet weak var searchBtn: UIButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        
        
        
        searchTextField.placeholder = "列车ID"
        
        
        self.view.frame = CGRect(x: self.view.frame.minX, y: self.view.frame.minY, width: self.view.frame.width, height: 2000)
        
        searchBtn.addTarget(self, action: #selector(ManageTrainViewController.searchBtnClicked(btn:)), for: .touchUpInside)
        // Do any additional setup after loading the view.
    }
    
    
    @objc func searchBtnClicked(btn: Any) {
        for v in self.view.subviews {
            if v != searchBtn && v != searchTextField {
                v.removeFromSuperview()
            }
        }
        let res = NetworkManager.default.postS(cmd: ["query_train", searchTextField!.text! != "" ? searchTextField!.text! : "11000C100102"].joined(separator: " "))
        if res == "0" {
            AlertControl.default.showMessage(title: "错误", message: "此列车不存在", viewCon: self)
            
        }
        parseTrainValue(ret: res)
        let stationNum = Int(trainValue[3])!
        var pricenum = Int(trainValue[4])!
//        var baseview = UIView(frame: CGRect(x: 20, y: 100, width: 320, height: 50))
        
//        baseview.layer.cornerRadius = 20
//        baseview.addShadow()
//        self.view.addSubview(baseview)
        var pricelabel = UILabel(frame: CGRect(x: 20, y: 100, width: 320, height: 50))
        pricelabel.textAlignment = .center
    
        pricelabel.backgroundColor = .white
        pricelabel.alpha = 1
        pricelabel.layer.cornerRadius = 5
        
        
        pricelabel.addShadow()
        //pricelabel.text = ""
        var strr = ""
        for i in 0..<pricenum{
            strr += ("  " + trainValue[5 + i] + "  ")
        }
        pricelabel.text = strr
        self.view.addSubview(pricelabel)
        for i in 1..<stationNum {
            var str = ""
            for j in 0..<pricenum{
                str += ("   ¥" + longToShort(a: stationValue[i][4 + j]))
            }
            var newv = RecordView(x: 20, y: 170 * i, id: "", start: stationValue[i - 1][0], to: stationValue[i][0], starttime: stationValue[i - 1][2], endtime: stationValue[i][1], price: str)
            self.view.addSubview(newv)
        }
        
        var _saletrain = Button(frame: CGRect(x: 60, y: 170 * stationNum + 10, width: 100, height: 50))
        //_saletrain.title = "发售列车"
        _saletrain.setTitle("发售车次", for: .normal)
        
        _saletrain.titleColor = themeHeavyColor
        _saletrain.backgroundColor = themeLightColor
        _saletrain.layer.cornerRadius = 4
        _saletrain.addShadow()
        self.view.addSubview(_saletrain)
        _saletrain.addTarget(self, action: #selector(ManageTrainViewController.saletrainbtnclicked(btn:)), for: .touchUpInside)
        
        var _deletetrain = Button(frame: CGRect(x: 200, y: 170 * stationNum + 10, width: 100, height: 50))
        //_saletrain.title = "发售列车"
        _deletetrain.setTitle("删除车次", for: .normal)
        _deletetrain.titleColor = themeHeavyColor
        _deletetrain.backgroundColor = themeLightColor
        _deletetrain.layer.cornerRadius = 4
        _deletetrain.addShadow()
        self.view.addSubview(_deletetrain)
        _deletetrain.addTarget(self, action: #selector(ManageTrainViewController.deletetrainbtnclicked(btn:)), for: .touchUpInside)
        
        
        
        
    }
    
    @objc func deletetrainbtnclicked(btn: Button){
        let res = NetworkManager.default.postS(cmd: ["delete_train", searchTextField!.text! != "" ? searchTextField!.text! : "11000C100102"].joined(separator: " "))
        if res == "0" {
            AlertControl.default.showMessage(title: "错误", message: "此列车已发售，不能删除", viewCon: self)
        }
        else {
            AlertControl.default.showMessage(title: "提示", message: "删除成功", viewCon: self)
        }
    }
    
    @objc func saletrainbtnclicked(btn: Button){
        let res = NetworkManager.default.postS(cmd: ["sale_train", searchTextField!.text! != "" ? searchTextField!.text! : "11000C100102"].joined(separator: " "))
        if res == "0" {
            AlertControl.default.showMessage(title: "错误", message: "此列车已发售", viewCon: self)
        }
        btn.setTitle("已发售", for: .normal)
        btn.isEnabled = false
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
