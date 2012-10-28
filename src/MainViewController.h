//
//  SoundPath MainViewController
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#import <UIKit/UIKit.h>
#import "SoundPathApp.h"
#import "SPInteropDelegate.h"

@interface MainViewController : UIViewController <UIPopoverControllerDelegate,
                                                    UIAlertViewDelegate,
                                                    SPInteropDelegate>

@property (strong, nonatomic) UIPopoverController * popOver;

@property (nonatomic, strong) NSMutableDictionary * data;
@end
