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
    
    func postS(url: String = "https://ttrs.dhc.moe/exec", cmd: String) -> String {
        var req = URLRequest(url: URL(string: url)!)
        req.httpMethod = "POST"
        req.httpBody = ("cmd=" + cmd).data(using: .utf8)!
        
        var data = try! NSURLConnection.sendSynchronousRequest(req, returning: nil)
        let json = try! JSONSerialization.jsonObject(with: data, options: []) as! Dictionary<String, Any>
        
        print("POST *" + cmd + "*    RECEIVED *" + (json["result"] as! String) + "*")
        
        return json["result"] as! String
    }
    
    func postA(url: String = "https://ttrs.dhc.moe/exec", cmd: String, done: @escaping (String) -> Void) -> Void {
        var req = URLRequest(url: URL(string: url)!)
        req.httpMethod = "POST"
        req.httpBody = ("cmd=" + cmd).data(using: .utf8)!
        NSURLConnection.sendAsynchronousRequest(req, queue: .main, completionHandler: { (res, data, err) in
            let json = try! JSONSerialization.jsonObject(with: data!, options: []) as! Dictionary<String, Any>
            
            print("POST *" + cmd + "*    RECEIVED *" + (json["result"] as! String) + "*")
            
            done(json["result"] as! String)
        })
    }
}
