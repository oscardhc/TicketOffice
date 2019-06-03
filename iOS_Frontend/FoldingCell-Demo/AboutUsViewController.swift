//
//  AboutUsViewController.swift
//  FoldingCell-Demo
//
//  Created by 李志坚 on 2019/5/27.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit

class AboutUsViewController: UIViewController {

    @IBOutlet weak var hahaha: UIScrollView!
    @IBOutlet weak var baseview: UIView!
    override func viewDidLoad() {
        super.viewDidLoad()
//        var navBar = UINavigationBar(frame: CGRect(x: 0, y: 30, width: view.frame.width, height: 300))
//        var navItem = UINavigationItem()
//        var navBtn = UIBarButtonItem(title: "返回", style: .done, target: self, action: #selector(cancleBtnClicked(_:)))
//        navItem.rightBarButtonItem = navBtn
//        navBar.items = [navItem]
//        self.view.addSubview(navBar)
        let whjj = UILabel(frame: CGRect(x: 45, y: 0, width: 150, height: 30))
        let flyy = UILabel(frame: CGRect(x: 260, y: 0, width: 150, height: 30))
        let dhcc = UILabel(frame: CGRect(x: 470, y: 0, width: 150, height: 30))
        
        whjj.text = "陈大同的附体"
        flyy.text = "陈大同的妻子"
        dhcc.text = "一个不爱陈大同的男人"
        flyy.fontSize = 12
        whjj.fontSize = 12
        dhcc.fontSize = 12
        whjj.textColor = themeLightColor
        flyy.textColor = themeLightColor
        dhcc.textColor = themeLightColor
        
        whjj.textAlignment = .center
        flyy.textAlignment = .center
        dhcc.textAlignment = .center
        
        
        
        let whj = UIImageView(frame: CGRect(x: 45, y: 30, width: 150, height: 150))
        whj.image = UIImage(named: "whj")
        let fly = UIImageView(frame: CGRect(x: 260, y: 30, width: 150, height: 150))
        
        let dhc = UIImageView(frame: CGRect(x: 470, y: 30, width: 150, height: 150))
        fly.image = UIImage(named: "fly")
        dhc.image = UIImage(named: "dhc")
        
        
        hahaha.addSubview(whj)
        hahaha.addSubview(dhc)
        hahaha.addSubview(fly)
        hahaha.addSubview(flyy)
        hahaha.addSubview(whjj)
        hahaha.addSubview(dhcc)
        
        
        hahaha.isPagingEnabled = false
        // 可以滚动的区域
        hahaha.contentSize = CGSize(width: 650, height: 0)
        // 显⽰示⽔水平滚动条
        hahaha.showsHorizontalScrollIndicator = true
        // 显⽰示垂直滚动条
        hahaha.showsVerticalScrollIndicator = true
        // 滚动条样式
        hahaha.indicatorStyle = .default
        // 设置回弹效果
        hahaha.bounces = true
        // 设置scrollView可以滚动
        hahaha.isScrollEnabled = true
        // 当scrollsToTop=true时，点击设备状态栏会自动滚动到顶部
        hahaha.scrollsToTop = true
        // 缩放的最小比例
        hahaha.minimumZoomScale = 0.5
        // 放大的最大比例
        hahaha.maximumZoomScale = 2.0
        // 缩放回弹
        hahaha.bouncesZoom = true
        baseview.layer.cornerRadius = 10;
        baseview.addShadow()

        // Do any additional setup after loading the view.
    }
    @objc func cancleBtnClicked(_ sender: AnyObject) {
        self.dismiss(animated: true, completion: nil)
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
