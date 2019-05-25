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
import Material

let themeLightColor = UIColor(red: 222/255, green: 219/255, blue: 234/255, alpha: 1)
let themeHeavyColor = UIColor(red: 90/255, green: 76/255, blue: 148/255, alpha: 1)

class MainTableViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, UIScrollViewDelegate, UIPickerViewDelegate, UIPickerViewDataSource {
    
    var trainData = [[String]]()
    var ticketData = [[[String]]]()
    
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return 30
    }
    
    func pickerView(_ pickerView: UIPickerView, viewForRow row: Int, forComponent component: Int, reusing view: UIView?) -> UIView {
        var l = view as? UILabel
        if l == nil {
            l = UILabel()
            l?.font = .systemFont(ofSize: 14)
            l?.textAlignment = .center
        }
        l?.text = "2019-06-" + String(format: "%02d" , row + 1)
//        l?.te
        return l!
    }
    

    enum Const {
        static let closeCellHeight: CGFloat = 179
        static let openCellHeight: CGFloat = 488
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
        var _myNavBar = UINavigationBar(frame: CGRect(x: 0, y: -50, width: 375, height: 350))
        
        _myNavBar.addShadow()
        _myNavBar.items = [myNavItem]
        //        myNavBar.backgroundColor = UIColor(red: 197/255, green: 192/255, blue: 217/255, alpha: 1)
        _myNavBar.backgroundColor = .white
        return _myNavBar
    }()
    lazy var titleLable: UILabel = {
        var i = UILabel()
        i.frame = CGRect(x: 18, y: 138, width: 100, height: 50)
        i.text = "查询"
//        i.fontSize = 30
        i.font = UIFont.boldSystemFont(ofSize: 32)
        return i
    }()
    lazy var fromInput: TextField = {
        var i = TextField()
        i.frame = CGRect(x: 20, y: 210, width: 120, height: 30)
        i.placeholder = "出发站"
        i.placeholderActiveColor = themeHeavyColor
        i.dividerActiveColor = themeHeavyColor
        i.text = "上海虹桥"
        return i
    }()
    lazy var toInput: TextField = {
        var i = TextField()
        i.frame = CGRect(x: 150, y: 210, width: 120, height: 30)
        i.placeholder = "到达站"
        i.placeholderActiveColor = themeHeavyColor
        i.dividerActiveColor = themeHeavyColor
        i.text = "杭州东"
        return i
    }()
    lazy var dateLable: UILabel = {
        var i = UILabel()
        i.frame = CGRect(x: 20, y: 250, width: 70, height: 50)
        i.text = "出发日期"
        return i
    }()
    lazy var dateSelect: UIPickerView = {
        var i = UIPickerView()
        i.frame = CGRect(x: 100, y: 250, width: 170, height: 50)
        i.delegate = self
        i.dataSource = self
        return i
    }()
    lazy var submitBtn: Button = {
        var i = Button()
        i.frame = CGRect(x: 280, y: 210, width: 50, height: 50)
        i.setTitle("查询", for: .normal)
        i.backgroundColor = themeLightColor
        i.addTarget(self, action: #selector(MainTableViewController.submit(_:)), for: .touchUpInside)
        return i
    }()
    
    @objc func submit(_ sender: Button) {
        if fromInput.isEmpty || toInput.isEmpty {
            
        } else {
            let cmd = ["query_ticket", fromInput.text!, toInput.text!, "2019-06-" + String(format: "%02d" ,dateSelect.selectedRow(inComponent: 0) + 1), "CDGKOTZ"].joined(separator: " ")
            print(cmd)
            NetworkManager.default.postA(cmd: cmd) { (ret) in
                self.parseTrainData(ret: ret)
                self.setup()
                self.tableView.reloadData()
            }
        }
    }
    
    func parseTrainData(ret: String) {
        trainData = [[String]]()
        ticketData = [[[String]]]()
        let ar = ret.split { (chr) -> Bool in
            return chr == " " || chr == "\n"
        }
        let cnt = Int(String(ar[0]))!
        var cur = 1
        for _ in 0...cnt - 1 {
            var tmp: [String] = []
            var tickets: [[String]] = []
            tmp.append(String(ar[cur]))
            tmp.append(String(ar[cur + 1]))
            tmp.append(String(ar[cur + 2]))
            tmp.append(String(ar[cur + 3]))
            tmp.append(String(ar[cur + 4]))
            tmp.append(String(ar[cur + 5]))
            tmp.append(String(ar[cur + 6]))
            cur = cur + 7
            var tst = String(ar[cur])
            while tst.count > 0 && tst[tst.startIndex].unicodeScalars.first!.value >= UInt32(0x4E00) {
                tickets.append([String(ar[cur]), String(ar[cur + 1]), String(ar[cur + 2])])
                if (cur + 3 < ar.count) {
                    cur = cur + 3
                    tst = String(ar[cur])
                } else {
                    break
                }
            }
            trainData.append(tmp)
            ticketData.append(tickets)
        }
        print(trainData)
        print(ticketData)
    }
    
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
//        self.headerHeightConstraint.constant = self.maxHeaderHeight
        
        tableView.delegate = self
        tableView.dataSource = self
        
//        myNavBar.alpha = 0.3
        self.view.addSubview(myNavBar)
        myNavBar.addSubview(titleLable)
        myNavBar.addSubview(fromInput)
        myNavBar.addSubview(toInput)
        myNavBar.addSubview(dateLable)
        myNavBar.addSubview(dateSelect)
        myNavBar.addSubview(submitBtn)
        
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setup()
        if #available(iOS 10.0, *) {
            tableView.refreshControl = UIRefreshControl()
            tableView.refreshControl?.addTarget(self, action: #selector(refreshHandler), for: .valueChanged)
        }
        
//        let str = NetworkManager.default.post(cmd: "query_profile 2020")
    }

    private func setup() {
        cellHeights = Array(repeating: Const.closeCellHeight, count: trainData.count)
        tableView.estimatedRowHeight = Const.closeCellHeight
        tableView.rowHeight = UITableView.automaticDimension
//        tableView.backgroundColor = UIColor(patternImage: #imageLiteral(resourceName: "background"))
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
        return trainData.count
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
        cell.fromLable.text = trainData[indexPath.row][1]
        cell.toLable.text = trainData[indexPath.row][4]
        cell.trainIDLable.text = trainData[indexPath.row][0]
        cell.startTimeLable.text = trainData[indexPath.row][3]
        cell.arriveTimeLable.text = trainData[indexPath.row][6]
        
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
