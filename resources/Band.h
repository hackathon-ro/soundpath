//
//  Band.h
//  SoundPath
//
//  Created by Bogdan Florescu on 10/28/12.
//
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class Band;

@interface Band : NSManagedObject

@property (nonatomic, retain) NSString * page_id;
@property (nonatomic, retain) NSString * likes;
@property (nonatomic, retain) NSString * lastfm_url;
@property (nonatomic, retain) NSString * listeners;
@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NSString * pic_url;
@property (nonatomic, retain) NSString * plays;
@property (nonatomic, retain) NSString * tags;
@property (nonatomic, retain) NSString * youtube_url;
@property (nonatomic, retain) NSSet *liked_bands;
@end

@interface Band (CoreDataGeneratedAccessors)

- (void)addLiked_bandsObject:(Band *)value;
- (void)removeLiked_bandsObject:(Band *)value;
- (void)addLiked_bands:(NSSet *)values;
- (void)removeLiked_bands:(NSSet *)values;

@end
