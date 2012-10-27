//
//  SPUtils.m
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#import "SPUtils.h"

@implementation SPUtils

+ (BOOL)isRetina {
    
    static CGFloat scale = 0.0;
    if (scale == 0.0) {
        
        if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] &&
            ([UIScreen mainScreen].scale == 2.0)) {
            scale = 2.0;
            return YES;
        } else {
            scale = 1.0;
            return NO;
        }   
        
    }
    return scale > 1.0;
}
@end
