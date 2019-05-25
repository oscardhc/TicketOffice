//
//  DateFormat.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/5/24.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit

class DateFormat: NSObject {
    static func dateConvertString(date:Date, dateFormat:String="yyyy-MM-dd") -> String {
        let timeZone = TimeZone.init(identifier: "UTC")
        let formatter = DateFormatter()
        formatter.timeZone = timeZone
        formatter.locale = Locale.init(identifier: "zh_CN")
        formatter.dateFormat = dateFormat
        let date = formatter.string(from: date)
        return date.components(separatedBy: " ").first!
    }
    static func stringConvertDate(string:String, dateFormat:String="yyyy-MM-dd") -> Date {
        let dateFormatter = DateFormatter.init()
        dateFormatter.dateFormat = "yyyy-MM-dd"
        let date = dateFormatter.date(from: string)
        return date!
    }
}
