//
//  SPUtils.h
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#import <Foundation/Foundation.h>

#define iPad                            (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)
#define iPhone                          (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPhone)

/**
 * Utils.
 */
@interface SPUtils : NSObject
+ (BOOL)isRetina;
@end
