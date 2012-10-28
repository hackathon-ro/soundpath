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

+ (void) getBands:(NSDictionary*) params andBlock:(void (^)(NSArray *response))block;
+ (void) getRelatedBands:(NSDictionary*) params withId:(NSString*)page_id andBlock:(void (^)(NSArray *response))block;
+ (void) getBandInfo:(NSDictionary *)params withId:(NSString*)page_id andBlock:(void (^)(NSDictionary *))block;

@end
