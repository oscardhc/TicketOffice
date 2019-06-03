//
//  AlertControl.swift
//  FoldingCell-Demo
//
//  Created by Haichen Dong on 2019/5/10.
//  Copyright © 2019 Alex K. All rights reserved.
//

import UIKit

class AlertControl {
    
    static let `default` = AlertControl()
    
    let alertController = UIAlertController(title: "",
                                            message: "", preferredStyle: .alert)
    
    let cancelAction = UIAlertAction(title: "好的", style: .cancel, handler: nil)
    private init() {
        alertController.addAction(cancelAction)
    }
    
    func showMessage(title: String, message: String, viewCon: UIViewController) -> Void {
        alertController.title = title
        alertController.message = message
        viewCon.present(alertController, animated: true, completion: nil)
    }
}
