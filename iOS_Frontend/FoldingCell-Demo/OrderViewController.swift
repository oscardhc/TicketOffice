//
//  OrderViewController.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/5/27.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Material

var refundData:[String] = []

class OrderViewController: UIViewController, UIPickerViewDataSource, UIPickerViewDelegate {
    
    let ccc = ["C","D","G","K","O","T","Z"]
    var recordNum = 0;
    var records:[[String]] = [];
    
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return pickerView == datePicker ? 30 : 7
    }
    
    func pickerView(_ pickerView: UIPickerView, viewForRow row: Int, forComponent component: Int, reusing view: UIView?) -> UIView {
        var l = view as? UILabel
        if l == nil {
            l = UILabel()
            l?.font = .systemFont(ofSize: 14)
            l?.textAlignment = .center
        }
        if pickerView == datePicker {
            l?.text = "2019-06-" + String(format: "%02d" , row + 1)
        } else {
            l?.text = ccc[row]
        }
        return l!
    }

    @IBOutlet weak var datePicker: UIPickerView!
    @IBOutlet weak var confirmBtn: Button!
    @IBOutlet weak var cataPicker: UIPickerView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.view.frame = CGRect(x: self.view.frame.minX, y: self.view.frame.minY, width: self.view.frame.width, height: 2000)
        // Do any additional setup after loading the view.
        datePicker.dataSource = self
        datePicker.delegate = self
        cataPicker.dataSource = self
        cataPicker.delegate = self
        cataPicker.removeFromSuperview()
        confirmBtn.addTarget(self, action: #selector(OrderViewController.confirmBtnClicked(_:)), for: .touchUpInside)
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
//        print(self.view.subviews)
        datePicker.frame = CGRect(x: 51, y: 64, width: 157, height: 66)
//        cataPicker.frame = CGRect(x: 220, y: 64, width: 49, height: 66)
    }
    
    @objc func confirmBtnClicked(_ sender: Button) {
        for v in self.view.subviews {
            if v != datePicker && v != confirmBtn {
                v.removeFromSuperview()
            }
        }
        let res = NetworkManager.default.postS(cmd: ["query_order",userID,"2019-06-" + String(format: "%02d" , datePicker.selectedRow(inComponent: 0) + 1), "CDGKOTZ"].joined(separator: " "))
        let ar = res.split { (c) -> Bool in
            return c == "\n"
        }
        recordNum = Int(ar[0])!
        for i in 0..<recordNum {
            records.append([])
            let tmp = ar[i + 1].split { (c) -> Bool in
                return c == " "
            }
            for j in 0...6 {
                records[i].append(String(tmp[j]))
            }
            var cur = 7
            while cur < tmp.count {
                if Int(tmp[cur + 1])! > 0 {
                    records[i].append(String(tmp[cur]))
                    records[i].append(String(tmp[cur + 1]))
                    records[i].append(String(tmp[cur + 2]))
                }
                cur = cur + 3
            }
        }
        print(recordNum)
        print(records)
        for i in 0..<recordNum {
            let card = RecordView(x: 25, y: 170 + 230 * i, id: records[i][0], start: records[i][1], to: records[i][4], starttime: records[i][3], endtime: records[i][6], price: "\(records[i][7]) \(records[i][8])张 ¥\(records[i][9])")
            self.view.addSubview(card)
            var _refundbtn = Button(frame: CGRect(x: 230, y: 170 + 230 * i + 168, width: 90, height: 35))
            //_saletrain.title = "发售列车"
            _refundbtn.setTitle("退票", for: .normal)
            _refundbtn.fontSize = 15
            _refundbtn.titleColor = .blue
            _refundbtn.backgroundColor = themeLightColor
            _refundbtn.layer.cornerRadius = 5
            _refundbtn.addShadow()
            self.view.addSubview(_refundbtn)
            _refundbtn.tag = i
            _refundbtn.addTarget(self, action: #selector(OrderViewController.refundbtnclicked(btn:)), for: .touchUpInside)
        }
        
        
        
    }
    
    @objc func refundbtnclicked(btn: Button) {
        jumpflag = true
        refundData = records[btn.tag]
        self.dismiss(animated: true) {
//            (vc as! UserViewController).modifyCard.open()
        }
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
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
