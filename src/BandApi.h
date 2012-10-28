//
//  BandApi.h
//  SoundPath
//
//  Created by Bogdan Florescu on 10/28/12.
//
//

#import <Foundation/Foundation.h>
@class Band;

/*
 * Delegate.
 */
@protocol APIDelegate <NSObject>
- (void)loadBand:(NSString*) band_id;
@end

@interface BandApi : NSObject<NSFetchedResultsControllerDelegate>
{
    // delegate
	__weak id<APIDelegate> delegate;
    
    // queue
    NSOperationQueue *queue;

    // core data
    NSManagedObjectModel *managedObjectModel;
    NSManagedObjectContext *managedObjectContext;
    NSPersistentStoreCoordinator *persistentStoreCoordinator;
}

@property (nonatomic, weak) id <APIDelegate> delegate;

- (NSMutableArray*)fetchRecords;
- (void)getBand:(NSString*) band_id;
- (void)getRelatedBand:(NSString*) related_band_id forBand:(NSString*) band_id;
- (Band*) getBandFromCacheByOid:(int) oid;

@end
