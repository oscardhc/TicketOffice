//
//  DemoCell.swift
//  FoldingCell
//
//  Created by Alex K. on 25/12/15.
//  Copyright © 2015 Alex K. All rights reserved.
//

import FoldingCell
import UIKit

class DemoCell: FoldingCell {
    
    
    @IBOutlet var ticketLeft: [UILabel]!
    @IBOutlet weak var ticketType5Label: UILabel!
    @IBOutlet weak var ticketType4Label: UILabel!
    @IBOutlet weak var ticketType3Label: UILabel!
    @IBOutlet weak var ticketType2Label: UILabel!
    @IBOutlet weak var ticketType1Label: UILabel!
    @IBOutlet weak var timeLabel: UILabel!
    
    @IBOutlet weak var stationLabel: UILabel!
    @IBOutlet weak var fromLable: UILabel!
    @IBOutlet weak var toLable: UILabel!
    @IBOutlet weak var trainIDLable: UILabel!
    @IBOutlet weak var startTimeLable: UILabel!
    @IBOutlet weak var arriveTimeLable: UILabel!
    @IBOutlet weak var ticketLable: UILabel!
    @IBOutlet weak var scroolView: UIScrollView!
    @IBOutlet weak var btn: UIButton!
    
    var number: Int = 0 {
        didSet {
//            closeNumberLabel.text = String(number)
//            openNumberLabel.text = String(number)
        }
    }
    
    override func awakeFromNib() {
        foregroundView.layer.cornerRadius = 10
        foregroundView.layer.masksToBounds = true
        super.awakeFromNib()
    }

    override func animationDuration(_ itemIndex: NSInteger, type _: FoldingCell.AnimationType) -> TimeInterval {
        let durations = [0.26, 0.2, 0.2]
        return durations[itemIndex]
    }
}

// MARK: - Actions ⚡️

extension DemoCell {

//    @IBAction func buttonHandler(_: AnyObject) {
//        print("tap")
//        var s = UIStoryboard(name: "Main", bundle: nil).instantiateViewController(withIdentifier: "BuyViewController")
//    }
}
