//
//  TBViewController.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/4/26.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit

class TBViewController: UITabBarController {

    override func viewDidLoad() {
        super.viewDidLoad()

        let storyboard = UIStoryboard(name: "Main", bundle: nil)
        let view1 = storyboard.instantiateViewController(withIdentifier: "MainTableViewController")
        view1.title = "查询"
//        let nav1 = UINavigationController(rootViewController: view1)
//        var bar = UINavigationBar(frame: CGRect(x: 0, y: 20, width: 375, height: 300))
//        view1.view.addSubview(bar)
        
        let view2 = storyboard.instantiateViewController(withIdentifier: "UserViewController")
        view2.title = "我的"
        let nav2 = UINavigationController(rootViewController: view2)
        
        let view3 = storyboard.instantiateViewController(withIdentifier: "ManageViewController")
        view3.title = "管理"
        let nav3 = UINavigationController(rootViewController: view3)
        
        print(view1.navigationController?.navigationItem)
//        view1.navigationController?.navigationItem
        
        if #available(iOS 11.0, *) {
            nav2.navigationBar.prefersLargeTitles = true
            nav2.navigationItem.largeTitleDisplayMode = .always
            nav3.navigationBar.prefersLargeTitles = true
            nav3.navigationItem.largeTitleDisplayMode = .always
        } else {
            // Fallback on earlier versions
        }
        
        self.viewControllers = [view1, nav3, nav2]
        self.selectedIndex = 0
        
        // Do any additional setup after loading the view.
    }
    
    override func tabBar(_ tabBar: UITabBar, didSelect item: UITabBarItem) {
//        self.selectIndex
//        print(self.selectedIndex)
//        if item.title == "管理" &&
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
