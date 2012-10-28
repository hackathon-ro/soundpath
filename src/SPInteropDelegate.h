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
- (void)nodeInfo:(unsigned int)nid;
- (void)nodeTapped:(unsigned int)nid;
@end
