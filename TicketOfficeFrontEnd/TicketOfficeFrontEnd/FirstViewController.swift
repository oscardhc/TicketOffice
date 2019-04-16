//
//  FirstViewController.swift
//  TicketOfficeFrontEnd
//
//  Created by Haichen Dong on 2019/4/10.
//  Copyright © 2019 Haichen Dong. All rights reserved.
//

import UIKit

class FirstViewController: UIViewController {

    var window : CGRect!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        window = self.view.frame
        print(window)
        
        let url = URL(string: "http://127.0.0.1:5000/name/asd")
        let session = URLSession(configuration: .default)
        let task = session.dataTask(with: url!) {
            (data, response, error) in do {
                let r = try JSONSerialization.jsonObject(with: data!, options: []) as! Dictionary<String, Any>
                print(r)
            } catch {
                print("error")
            }
        }
        task.resume()
        self.view.addSubview(bkground)
    }

    lazy var bkground : UIImageView =  {
        let img = UIImageView(image: UIImage(named: "1"))
        img.frame = self.window
        img.alpha = 0.5
        img.contentMode = .scaleAspectFill
        return img
    }()
    
}

