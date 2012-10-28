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
- (void)nodeInfo:(int)nid;
- (void)nodeTapped:(int)nid;
@end
