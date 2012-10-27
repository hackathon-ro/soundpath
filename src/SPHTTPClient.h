//
//  SPHTTPClient.h
//  SoundPath
//
//  Created by Bogdan Florescu on 10/28/12.
//
//

#import "AFHTTPClient.h"

@interface SPHTTPClient : AFHTTPClient

+ (id)sharedHTTPClient;

//+ (BOOL) checkConnection;

+ (void) getBands:(NSDictionary*) params andBlock:(void (^)(NSDictionary *response))block;

@end
