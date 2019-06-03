//
//  RecordView.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/5/30.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit

class RecordView: UIView {

    /*
    // Only override draw() if you perform custom drawing.
    // An empty implementation adversely affects performance during animation.
    override func draw(_ rect: CGRect) {
        // Drawing code
    }
    */
    
    init(x: Int, y: Int, id: String, start: String, to: String, starttime: String, endtime: String, price: String) {
        super.init(frame: CGRect(x: x, y: y, width: 320, height: 150))
        var _id = UILabel(frame: CGRect(x: 142, y: 10, width: 90, height: 20))
        _id.text = id
        _id.fontSize = 10
        _id.textAlignment = .center
        var purpleview = UIView(frame: CGRect(x:0, y:0, width: 40, height: Int(self.frame.height)))
        var purpleview2 = UIView(frame: CGRect(x:17, y:0, width: 53, height: Int(self.frame.height)))
        var trainimage = UIImageView(frame: CGRect(x: 10, y: 50, width: 50, height: 50))
        trainimage.image = UIImage(named: "train-1")
        var image = UIImageView(frame: CGRect(x: 142, y: 30, width: 85, height: 10))
        image.image = UIImage(named: "dotline")
        
        
        var _start = UILabel(frame: CGRect(x: 80, y: 20, width: 70, height: 30))
        _start.text = start
        _start.fontSize = 17
        _start.textAlignment = .center
        var _to = UILabel(frame: CGRect(x: 230, y: 20, width: 70, height: 30))
        _to.text = to
        _to.fontSize = 17
        _to.textAlignment = .center
        
        var tra = transferTime(t1: starttime, t2: endtime)
        var _time = UILabel(frame: CGRect(x: 153, y: 35, width: 100, height: 30))
        _time.text = "\(tra / 60)小时\(tra % 60)分钟"
        _time.fontSize = 13
        
        var _starttime = UILabel(frame: CGRect(x: 100, y: 80, width: 80, height: 50))
        var _endtime = UILabel(frame: CGRect(x: 240, y: 80, width: 80, height: 50))
        var _starttime1 = UILabel(frame: CGRect(x: 100, y: 55, width: 80, height: 50))
        var _endtime1 = UILabel(frame: CGRect(x: 230, y: 55, width: 80, height: 50))
        _starttime1.text = "出发时间"
        _endtime1.text = "到达时间"
        _starttime.text = starttime
        _endtime.text = endtime
        _starttime.fontSize = 14
        _endtime.fontSize = 14
        _starttime1.fontSize = 14
        _endtime1.fontSize = 14
        
        var _price = UILabel(frame: CGRect(x: 90, y: 120, width: 200, height: 25))
        _price.fontSize = 12
        _price.text = price
        _price.textAlignment = .center
        
    
        
        
        
        purpleview.backgroundColor = themeHeavyColor
        purpleview2.backgroundColor = themeHeavyColor
        self.backgroundColor = .white
        self.addSubview(_id)
        self.addSubview(_start)
        self.addSubview(_to)
        self.addSubview(purpleview)
        self.addSubview(purpleview2)
        self.addSubview(trainimage)
        self.addSubview(_time)
        self.addSubview(image)
        self.addSubview(_starttime)
        self.addSubview(_starttime1)
        self.addSubview(_endtime)
        self.addSubview(_endtime1)
        self.addSubview(_price)
        
        self.clipsToBounds = false
        self.layer.cornerRadius = 20
        purpleview.layer.cornerRadius = 20
        self.addShadow()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

func longToShort(a: String) -> String {
    return String(format: "%.1f", Double(a.dropFirst())!)
}
func longToShort2(a: String) -> String {
    return String(format: "%.1f", Double(a) as! CVarArg)
}
