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

// HTTP constants
static NSString * kSERVER_BASE = @"http://10.10.1.80:3000/";

// webservices paths constants
static NSString * kServiceBands = @"bands";

// NSUserDefaults constants
static NSString *kUid = @"uid";
static NSString *kImage = @"image";
static NSString *kName = @"name";

/**
 * Utils.
 */
@interface SPUtils : NSObject
+ (BOOL)isRetina;
@end
