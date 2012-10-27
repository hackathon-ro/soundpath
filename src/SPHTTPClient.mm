//
//  SPHTTPClient.m
//  SoundPath
//
//  Created by Bogdan Florescu on 10/28/12.
//
//

#import "SPHTTPClient.h"
#import "MBProgressHUD.h"
#import "SPUtils.h"
#import "AFNetworking.h"

@implementation SPHTTPClient

#define ERROR_DOMAIN @"com.sample.url.error"

#pragma mark - Life and Birth

+ (id)sharedHTTPClient {
    static SPHTTPClient *_sharedClient = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _sharedClient = [[SPHTTPClient alloc] initWithBaseURL:[NSURL URLWithString:kSERVER_BASE]];
    });
    
    return _sharedClient;
}

- (id)initWithBaseURL:(NSURL *)url {
    self = [super initWithBaseURL:url];
    if (!self) {
        return nil;
    }
    
    [self setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        NSLog(@"changed %d", status);
        //your code here
    }];
    
    [self registerHTTPOperationClass:[AFJSONRequestOperation class]];
    [self setDefaultHeader:@"Accept" value:@"application/json"];
    
    return self;
}

//#pragma mark - check internet connection
//+ (BOOL) checkConnection {
//    
//    UIWindow * window = (UIWindow *)[UIApplication sharedApplication].keyWindow;
//    UINavigationController * navController = (UINavigationController*) window.rootViewController;
//    
//    
//    [MBProgressHUD hideAllHUDsForView:navController.visibleViewController.view animated:YES];
//    
//    
//    Reachability *r = [Reachability reachabilityWithHostName:@"www.google.com"];
//    
//    NetworkStatus internetStatus = [r currentReachabilityStatus];
//    
//    if ((internetStatus != ReachableViaWiFi) && (internetStatus != ReachableViaWWAN))
//        return NO;
//    
//    return YES;
//}

#pragma mark - Custom requests


+ (void) getBands:(NSDictionary*) params andBlock:(void (^)(NSDictionary *tweets))block {
    
    [[SPHTTPClient sharedHTTPClient]
     postPath:kServiceBands
     parameters:params
     success:^(AFHTTPRequestOperation *operation, id JSON) {
         //        NSDictionary *mutableTweets = [NSDictionary dictionary];
         //
         ////        NSLog(@"%@ JSON %@\n\n\n",[operation responseString], JSON);
         //
         //         mutableTweets = [NSDictionary dictionaryWithDictionary:JSON objectForKey:@"userdata"] objectForKey:@"User"]];
         
         if (block) {
             block([NSDictionary dictionaryWithDictionary:JSON]);
             
         }
         
     } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
#if __IPHONE_OS_VERSION_MIN_REQUIRED
         [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Error", nil) message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:NSLocalizedString(@"OK", nil), nil] show];
#else
         [[NSAlert alertWithMessageText:NSLocalizedString(@"Error", nil) defaultButton:NSLocalizedString(@"OK", nil) alternateButton:nil otherButton:nil informativeTextWithFormat:[error localizedDescription]] runModal];
#endif
         if (block) {
             block(nil);
         }
     }];
}


@end
