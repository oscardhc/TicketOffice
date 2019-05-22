//
//  MainTableViewController.swift
//
// Copyright (c) 21/12/15. Ramotion Inc. (http://ramotion.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

import FoldingCell
import UIKit

let themeLightColor = UIColor(red: 222/255, green: 219/255, blue: 234/255, alpha: 1)
let themeHeavyColor = UIColor(red: 90/255, green: 76/255, blue: 148/255, alpha: 1)

class MainTableViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, UIScrollViewDelegate {

    enum Const {
        static let closeCellHeight: CGFloat = 179
        static let openCellHeight: CGFloat = 488
        static let rowsCount = 10
    }
    
    var cellHeights: [CGFloat] = []
    
    @IBOutlet weak var tableView: UITableView!
    
    let maxHeaderHeight: CGFloat = 88;
    let minHeaderHeight: CGFloat = 44;
    
    lazy var myNavItem: UINavigationItem = {
        var _myNavItem = UINavigationItem(title: "")
        return _myNavItem
    }()
    lazy var myNavBar: UINavigationBar = {
        var _myNavBar = UINavigationBar(frame: CGRect(x: 0, y: -50, width: 375, height: 275))
        
        _myNavBar.addShadow()
        _myNavBar.items = [myNavItem]
        
        
        
        //        myNavBar.backgroundColor = UIColor(red: 197/255, green: 192/255, blue: 217/255, alpha: 1)
        _myNavBar.backgroundColor = themeLightColor
        return _myNavBar
    }()
    
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
//        self.headerHeightConstraint.constant = self.maxHeaderHeight
        
        tableView.delegate = self
        tableView.dataSource = self
        
//        myNavBar.alpha = 0.3
        self.view.addSubview(myNavBar)
        
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setup()
//        let str = NetworkManager.default.post(cmd: "query_profile 2020")
    }

    private func setup() {
        cellHeights = Array(repeating: Const.closeCellHeight, count: Const.rowsCount)
        tableView.estimatedRowHeight = Const.closeCellHeight
        tableView.rowHeight = UITableView.automaticDimension
//        tableView.backgroundColor = UIColor(patternImage: #imageLiteral(resourceName: "background"))
        if #available(iOS 10.0, *) {
            tableView.refreshControl = UIRefreshControl()
            tableView.refreshControl?.addTarget(self, action: #selector(refreshHandler), for: .valueChanged)
        }
    }
    
    @objc func refreshHandler() {
        let deadlineTime = DispatchTime.now() + .seconds(1)
        DispatchQueue.main.asyncAfter(deadline: deadlineTime, execute: { [weak self] in
            if #available(iOS 10.0, *) {
                self?.tableView.refreshControl?.endRefreshing()
            } 
            self?.tableView.reloadData()
        })
    }
}

// MARK: - TableView

extension MainTableViewController {

    func tableView(_: UITableView, numberOfRowsInSection _: Int) -> Int {
        return 10
    }

    func tableView(_: UITableView, willDisplay cell: UITableViewCell, forRowAt indexPath: IndexPath) {
        guard case let cell as DemoCell = cell else {
            return
        }

        cell.backgroundColor = .clear

        if cellHeights[indexPath.row] == Const.closeCellHeight {
            cell.unfold(false, animated: false, completion: nil)
        } else {
            cell.unfold(true, animated: false, completion: nil)
        }

        cell.number = indexPath.row
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "FoldingCell", for: indexPath) as! FoldingCell
        let durations: [TimeInterval] = [0.26, 0.2, 0.2]
        cell.durationsForExpandedState = durations
        cell.durationsForCollapsedState = durations
        cell.addShadow()
//        var rec = MyTapGesture(target: self, action: #selector(MainTableViewController.holdOnCell(_:)))
//        rec.idx = indexPath.row
//        cell.addGestureRecognizer(rec)
        return cell
    }
    
    @objc func holdOnCell(_ sender: MyTapGesture) {
//        print(sender.idx)
    }

    func tableView(_: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return cellHeights[indexPath.row]
    }

    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {

        let cell = tableView.cellForRow(at: indexPath) as! FoldingCell

        if cell.isAnimating() {
            return
        }

        var duration = 0.0
        let cellIsCollapsed = cellHeights[indexPath.row] == Const.closeCellHeight
        if cellIsCollapsed {
            cellHeights[indexPath.row] = Const.openCellHeight
            cell.unfold(true, animated: true, completion: nil)
            duration = 0.5
        } else {
            cellHeights[indexPath.row] = Const.closeCellHeight
            cell.unfold(false, animated: true, completion: nil)
            duration = 0.8
        }

        UIView.animate(withDuration: duration, delay: 0, options: .curveEaseOut, animations: { () -> Void in
            tableView.beginUpdates()
            tableView.endUpdates()
            
            // fix https://github.com/Ramotion/folding-cell/issues/169
            if cell.frame.maxY > tableView.frame.maxY {
                tableView.scrollToRow(at: indexPath, at: UITableView.ScrollPosition.bottom, animated: true)
            }
        }, completion: nil)
    }
    
//    func scrollViewDidScroll(_ scrollView: UIScrollView) {
//        let offsetY = scrollView.contentOffset.y
//        if offsetY >= 0 && offsetY <= 150{
//            myNavBar.backgroundColor = getColorFromHex(rgbValue: 0xff602f).withAlphaComponent(offsetY / 150)
//        }else if offsetY > 150 {
//            myNavBar.backgroundColor = getColorFromHex(rgbValue: 0xff602f)
//        }else {
//            myNavBar.backgroundColor = getColorFromHex(rgbValue: 0xff602f).withAlphaComponent(0)
//        }
//
//        if offsetY < 0 {
//
//            UIView.animate(withDuration: 0.1, animations: {
//                self.myNavBar.alpha = 0
//            })
//
//        }else{
//            UIView.animate(withDuration: 0.1, animations: {
//                self.myNavBar.alpha = 1
//            })
//        }
//    }
    
}

func getColorFromHex(rgbValue: Int) -> (UIColor) {
    
    return UIColor(red: ((CGFloat)((rgbValue & 0xFF0000) >> 16)) / 255.0,
                   green: ((CGFloat)((rgbValue & 0xFF00) >> 8)) / 255.0,
                   blue: ((CGFloat)(rgbValue & 0xFF)) / 255.0,
                   alpha: 1.0)
    
}

class MyTapGesture: UILongPressGestureRecognizer {
    var idx: Int = 0
}
