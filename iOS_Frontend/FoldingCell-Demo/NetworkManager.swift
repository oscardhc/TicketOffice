//
//  NetworkManager.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/5/10.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
import Network

class NetworkManager {
    
    static let `default` = NetworkManager()
    
    private init() {
        
    }
    
    func post(url: String = "https://ttrs.dhc.moe/exec", cmd: String) -> String {
        var req = URLRequest(url: URL(string: url)!)
        req.httpMethod = "POST"
        req.httpBody = ("cmd=" + cmd).data(using: .utf8)!

        //        let task = URLSession.shared.dataTask(with: req) { data, res, err in
//            print(String(data: data!, encoding: .utf8))
//        }
//        task.resume()
//        NSURLConnection.sendAsynchronousRequest(req, queue: OperationQueue()) { (res, dat, err) in
//
//        }
        
        var data = try! NSURLConnection.sendSynchronousRequest(req, returning: nil)
        let json = try! JSONSerialization.jsonObject(with: data, options: []) as! Dictionary<String, Any>
        
        print("POST *" + cmd + "*    RECEIVED *" + (json["result"] as! String) + "*")
        
        return json["result"] as! String
    }
}
