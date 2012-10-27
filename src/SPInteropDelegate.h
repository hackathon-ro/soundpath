//
//  SPInteropDelegate.h
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#import <Foundation/Foundation.h>

@protocol SPInteropDelegate <NSObject>
// Business Methods
- (void)nodeInfo:(NSString*)nid;
- (void)nodeRelated:(NSString*)nid;
- (void)nodeClose:(NSString*)nid;
- (void)nodeLoad:(NSString*)nid;
- (void)nodeInformation:(NSString*)nid;
@end
