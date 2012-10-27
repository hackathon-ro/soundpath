//
//  CinderAppDelegateIPhone.h
//  cinder
//
//  Created by Leonard Chioveanu on 10/26/12.
//
//

#import <UIKit/UIKit.h>
#include "cinder/app/AppCocoaTouch.h"

@interface CinderAppDelegateIPhone : NSObject <UIApplicationDelegate, UIAccelerometerDelegate> {
	cinder::app::AppCocoaTouch	*app;
    //    UIWindow				*window;
    //    CinderViewCocoaTouch	*cinderView;
}
@end
