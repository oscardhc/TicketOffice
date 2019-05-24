//
//  LoginViewController.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/5/4.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit
//import SkyFloatingLabelTextField
import Material
//import SwiftyGif

class LoginViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        loginFrame.layer.cornerRadius = 10
        
        inp = TextField(frame: usernameFrame.frame)
        inp.placeholder = "USERID"
        inp.placeholderActiveColor = themeHeavyColor
        inp.dividerActiveColor = themeHeavyColor
        loginFrame.addSubview(inp)
        
        pas = TextField(frame: passwordFrame.frame)
        pas.placeholder = "PASSWORD"
        pas.placeholderActiveColor = themeHeavyColor
        pas.dividerActiveColor = themeHeavyColor
        loginFrame.addSubview(pas)
        
        loginFrame.addGestureRecognizer(UITapGestureRecognizer(target: self, action: #selector(LoginViewController.handleTap(_:))))
        imageView.isUserInteractionEnabled = true
        imageView.addGestureRecognizer(UITapGestureRecognizer(target: self, action: #selector(LoginViewController.handleTap(_:))))
        
        btn = Button(frame: btnField.frame)
        btn.setTitle("确认", for: .normal)
        btn.addTarget(self, action: #selector(LoginViewController.loginClicked(_:)), for: .touchUpInside)
        btn.backgroundColor = themeLightColor
        loginFrame.addSubview(btn)
    }
    
    @IBOutlet weak var imageView: UIImageView!
    var inp: TextField!
    var pas: TextField!
    var btn: Button!
    var imgView = UIImageView()
    lazy var alert: UIAlertController = {
        var _a = UIAlertController(title: "少女手冲中", message: "\n\n\n\n\n\n\n", preferredStyle: .alert)
        self.imgView.frame = CGRect(x: 75, y: 50, width: 120, height: 120)
//        self.imgView.image = resolveGifImage()[0]
        self.imgView.animationImages = resolveGifImage()
        self.imgView.animationDuration = 0.1
        self.imgView.animationRepeatCount = 0
        self.imgView.startAnimating()
        print(self.imgView.frame)
        _a.view.addSubview(self.imgView)
        return _a
    }()
    
    @IBOutlet weak var btnField: UIView!
    @IBOutlet weak var usernameFrame: UIView!
    @IBOutlet weak var loginFrame: UIView!
    @IBOutlet weak var passwordFrame: UIView!
    
    @objc func loginClicked(_ sender: Button) {
        print("clicked!")
//        self.dismiss(animated: true, completion: nil)
        self.present(alert, animated: true, completion: {
            DispatchQueue.main.asyncAfter(deadline: .now() + 1, execute: {
                self.alert.dismiss(animated: true, completion: {
                    self.dismiss(animated: true, completion: nil)
                })
            })
        })
    }
    
    @objc func handleTap(_ sender: UITapGestureRecognizer) {
        print("tapped! \(inp.isFirstResponder) \(pas.isFirstResponder)")
        if inp.isFirstResponder {
            inp.resignFirstResponder()
        }
        if pas.isFirstResponder {
            pas.resignFirstResponder()
        }
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
       
//        self.navigationController?.navigationBar.isHidden = true
//        loginFrame.backgroundColor = UIColor.red
        loginFrame.addShadow()
        
//        self.view.insertSubview(bgImage, at: self.view.subviews.count)
//        print(self.view.subviews)
    }
    
    override func viewWillDisappear(_ animated: Bool) {
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

extension UIView {
    func addShadow() {
//        print(self.layer.cornerRadius)
        self.layer.masksToBounds = true
        self.clipsToBounds = false
        self.layer.shadowColor = UIColor.gray.cgColor
        self.layer.shadowOffset = CGSize(width: 4, height: 4)
        self.layer.shadowRadius = 10
        self.layer.shadowOpacity = 0.4
    }
}

func resolveGifImage()->[UIImage]{
    var images:[UIImage] = []
    let gifPath = Bundle.main.path(forResource: "tkk", ofType: "gif")
    if gifPath != nil{
        if let gifData = try? Data(contentsOf: URL.init(fileURLWithPath: gifPath!)){
            let gifDataSource = CGImageSourceCreateWithData(gifData as CFData, nil)
            let gifcount = CGImageSourceGetCount(gifDataSource!)
            for i in 0...gifcount - 1{
                let imageRef = CGImageSourceCreateImageAtIndex(gifDataSource!, i, nil)
                let image = UIImage(cgImage: imageRef!)
                images.append(image)
            }
        }
    }
    print(images.count)
    return images
}
