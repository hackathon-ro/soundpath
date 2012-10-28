//
//  BandApi.m
//  SoundPath
//
//  Created by Bogdan Florescu on 10/28/12.
//
//

#import "BandApi.h"
#import "Band.h"
#import "SPUtils.h"
#import "SPHTTPClient.h"

// constants
static NSString * kSoundPathDB = @"SoundPath.sqlite";

@interface BandApi (PrivateCoreDataStack)

// Properties
@property (nonatomic, retain, readonly) NSManagedObjectModel *managedObjectModel;
@property (nonatomic, retain, readonly) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain, readonly) NSPersistentStoreCoordinator *persistentStoreCoordinator;

// Methods
+ (NSString *)applicationCachesDirectory;
@end



@implementation BandApi

#pragma mark -
#pragma mark Properties
@synthesize delegate;

#pragma mark -
#pragma mark Object

/**
 * Initialize.
 */
-(id)init {
    DLog();
    
    // super
    if ((self = [super init])) {
        
        // fields
        queue = [[NSOperationQueue alloc] init];
        [queue setMaxConcurrentOperationCount:1];
        
        // context
        NSManagedObjectContext *context = [self managedObjectContext];
        if (!context) {
            NSLog(@"Band CoreData Corrupted Cache");
        }
    }
    
    // return
    return self;
}


#pragma mark -
#pragma mark Business


/**
 * Reset.
 */
- (void)reset {
    DLog();
    
    // queue
    [queue cancelAllOperations];
    
    // unlock
    [managedObjectContext unlock];
    
}

/**
 * Popular.
 */
- (void)getBand:(NSString*) band_id {
    DLog();
    
    // queue
    [queue addOperationWithBlock:^{
        
        // context
        [managedObjectContext lock];
        
        // cached
        Band *band = [self getBandFromCache:band_id];
        
        // query
        if (band == nil) {
            band = [self getBandFromServer:band_id];
        }
        [managedObjectContext unlock];
        
        NSLog(@"get band %@",band.page_id);
        
        // get related bands
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSString * token = [defaults objectForKey:@"FBAccessTokenKey"];
        
        NSDictionary * params = [NSDictionary dictionaryWithObjectsAndKeys:[defaults objectForKey:kUid],@"uid",token,@"token", nil];
        
        [SPHTTPClient getRelatedBands:params withId:band.page_id andBlock:^(NSArray *rresponse) {
            if (rresponse) {
                DLog();
                NSLog(@"%@",rresponse);
                
                for(NSDictionary * dd in rresponse){
                    [self getRelatedBand:[dd valueForKey:@"page_id"] forBand:band.page_id];
                }
                
            }
        }];

        
        // delegate
        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
            
            // delegate
            if (delegate != nil && [delegate respondsToSelector:@selector(loadBand:)]) {
                [delegate loadBand:band_id];
            }
            
        }];
        
    }];
    
}

- (void)getBandByOid:(int) oid {
    DLog();
    
    // queue
    [queue addOperationWithBlock:^{
        
        // context
        [managedObjectContext lock];
        
        // cached
        Band *band = [self getBandFromCacheByOid:oid];
        
        // query
        if (!band) {
            [managedObjectContext unlock];
            return;
        }
        [managedObjectContext unlock];
        
        NSLog(@"get band %@",band.page_id);
        
        // get related bands
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSString * token = [defaults objectForKey:@"FBAccessTokenKey"];
        
        NSDictionary * params = [NSDictionary dictionaryWithObjectsAndKeys:[defaults objectForKey:kUid],@"uid",token,@"token", nil];
        
        [SPHTTPClient getRelatedBands:params withId:band.page_id andBlock:^(NSArray *rresponse) {
            if (rresponse) {
                DLog();
                NSLog(@"%@",rresponse);
                
                for(NSDictionary * dd in rresponse){
                    [self getRelatedBand:[dd valueForKey:@"page_id"] forBand:band.page_id];
                }
                
            }
        }];
        
        
//        // delegate
//        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
//            
//            // delegate
//            if (delegate != nil && [delegate respondsToSelector:@selector(loadBand:)]) {
//                [delegate loadBand:band_id];
//            }
//            
//        }];
        
    }];
    
}

- (Band*) getBandFromCacheByOid:(int) oid{
    
    DLog();
    
    // context
    NSManagedObjectContext *moc = [self managedObjectContext];
    NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Band" inManagedObjectContext:moc];
    NSFetchRequest *request = [[NSFetchRequest alloc] init];
    [request setEntity:entityDescription];
    
    // predicate
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(oid == %i)", oid];
    [request setPredicate:predicate];
    
    // limit
    [request setFetchLimit:1];
    
    // fetch
    NSError *error = nil;
    NSArray *array = [moc executeFetchRequest:request error:&error];
    
    // result
    Band *b = nil;
    if (array != nil && [array count] > 0) {
        b = (Band*) [array objectAtIndex:0];
    }
    
    NSLog(@"get band from cache:%@",b.page_id);
    
    // return
    return b;
}

- (void)getRelatedBand:(NSString*) related_band_id forBand:(NSString*) band_id{
    DLog();
    
    // queue
    [queue addOperationWithBlock:^{
        
        // context
        [managedObjectContext lock];
        
        // cached
        Band *band = [self getBandFromCache:related_band_id];
        
        // query
        if (band == nil) {
            band = [self getBandFromServer:related_band_id];
        }
        [managedObjectContext unlock];
        
        NSLog(@"get related band %@",band.page_id);
        
        // add this band to related bands for band_id
        Band *b = [self getBandFromCache:band_id];
        if(b) {
            NSMutableSet * mset = [NSMutableSet set];
            [mset addObject:band];
            b.liked_bands = [NSSet setWithSet:mset];
        }
        
        // delegate
        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
            
            // delegate
            if (delegate != nil && [delegate respondsToSelector:@selector(loadBand:)]) {
                [delegate loadBand:band_id];
            }
            
        }];
        
    }];
    
}

- (Band*) getBandFromCache:(NSString*) band_id{
    
    DLog();

    // context
    NSManagedObjectContext *moc = [self managedObjectContext];
    NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Band" inManagedObjectContext:moc];
    NSFetchRequest *request = [[NSFetchRequest alloc] init];
    [request setEntity:entityDescription];

    // predicate
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(page_id == %@)", band_id];
    [request setPredicate:predicate];

    // limit
    [request setFetchLimit:1];

    // fetch
    NSError *error = nil;
    NSArray *array = [moc executeFetchRequest:request error:&error];

    // result
    Band *b = nil;
    if (array != nil && [array count] > 0) {
        b = (Band*) [array objectAtIndex:0];
    }
    
    NSLog(@"get band from cache:%@",b.page_id);
    
    // return
    return b;
}


- (Band*) getBandFromServer:(NSString*) band_id{

    DLog();
    
    // popular data
    Band *band = [self getBandFromCache:band_id];
    if (band == nil) {
        
        // create
        band = (Band*)[NSEntityDescription insertNewObjectForEntityForName:@"Band" inManagedObjectContext:managedObjectContext];
        band.page_id = band_id;
        band.pic_url = @"";
        band.likes = @"0";
        band.plays = @"0";
        band.listeners = @"0";
        band.tags = @"{}";
        band.name = @"";
        band.lastfm_url = @"";
        band.liked_bands = [NSSet set];

    }
    

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString * token = [defaults objectForKey:@"FBAccessTokenKey"];
    
    NSDictionary * params = [NSDictionary dictionaryWithObjectsAndKeys:[defaults objectForKey:kUid],@"uid",token,@"token", nil];
    [SPHTTPClient getBandInfo:params withId:band_id andBlock:^(NSDictionary *response) {
        if (response) {
            DLog();
            NSLog(@"%@",response);
            
            if([response valueForKey:@"lastfm_url"] &&
               ![(NSNull*)[response valueForKey:@"lastfm_url"] isEqual:[NSNull null]])
                    band.lastfm_url = [response valueForKey:@"lastfm_url"];
            if([response valueForKey:@"pic_url"] &&
               ![(NSNull*)[response valueForKey:@"pic_url"] isEqual:[NSNull null]])
                band.pic_url = [response valueForKey:@"pic_url"];
            
            if([response valueForKey:@"likes"] &&
               ![(NSNull*)[response valueForKey:@"likes"] isEqual:[NSNull null]])
                band.likes = [response valueForKey:@"likes"];
            
            if([response valueForKey:@"plays"] &&
               ![(NSNull*)[response valueForKey:@"plays"] isEqual:[NSNull null]])
                band.plays = [response valueForKey:@"plays"];
            
            if([response valueForKey:@"listeners"] &&
               ![(NSNull*)[response valueForKey:@"listeners"] isEqual:[NSNull null]])
                band.listeners = [response valueForKey:@"listeners"];
            
//            if([response valueForKey:@"tags"] &&
//               ![(NSNull*)[response valueForKey:@"tags"] isEqual:[NSNull null]])
//                band.tags = [response valueForKey:@"tags"];
            
            if([response valueForKey:@"name"] &&
               ![(NSNull*)[response valueForKey:@"name"] isEqual:[NSNull null]])
                band.name = [response valueForKey:@"name"];
            
            NSError *error = nil;
            
            // save
            if (! [managedObjectContext save:&error]) {
                
                // handle the error
                NSLog(@"SoundPath CoreData Error\n%@\n%@", error, [error userInfo]);
                [managedObjectContext rollback];
                
                // null
                return;
                
            }
            
            
        }
        else{
            // connection error
            
            // db rollback
            [managedObjectContext rollback];
            
            return;
        }
    }];


    NSLog(@"get band from server:%@",band.page_id);
    
    return band;

}



#pragma mark -
#pragma mark Core Data stack

/**
 * Returns the path to the application's Cache directory.
 */
+ (NSString *)applicationCachesDirectory {
    DLog();
	return [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject];
}

/**
 * Returns the managed object context for the application.
 * If the context doesn't already exist, it is created and bound to the persistent store coordinator for the application.
 */
- (NSManagedObjectContext *) managedObjectContext {
	DLog();
	
	// context
    if (managedObjectContext != nil) {
        return managedObjectContext;
    }
	
	// store
    NSPersistentStoreCoordinator *coordinator = [self persistentStoreCoordinator];
    if (coordinator != nil) {
        managedObjectContext = [[NSManagedObjectContext alloc] init];
        [managedObjectContext setPersistentStoreCoordinator: coordinator];
    }
    return managedObjectContext;
}


/**
 * Returns the managed object model for the application.
 * If the model doesn't already exist, it is created by merging all of the models found in the application bundle.
 */
- (NSManagedObjectModel *)managedObjectModel {
	DLog();
	
	// model
    if (managedObjectModel != nil) {
        return managedObjectModel;
    }
    managedObjectModel = [NSManagedObjectModel mergedModelFromBundles:nil];
    return managedObjectModel;
}


/**
 * Returns the persistent store coordinator for the application.
 * If the coordinator doesn't already exist, it is created and the application's store added to it.
 */
- (NSPersistentStoreCoordinator *)persistentStoreCoordinator {
	DLog();
	
	// existing coordinator
    if (persistentStoreCoordinator != nil) {
        return persistentStoreCoordinator;
    }
	
	// store path
	NSString *storePath = [[BandApi applicationCachesDirectory] stringByAppendingPathComponent:kSoundPathDB];
	
	// store url
    NSURL *storeUrl = [NSURL fileURLWithPath:storePath];
	
	// options
	NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
							 [NSNumber numberWithBool:YES], NSMigratePersistentStoresAutomaticallyOption,
							 [NSNumber numberWithBool:YES], NSInferMappingModelAutomaticallyOption, nil];
	
	// init
	NSError *error = nil;
    persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self managedObjectModel]];
    if (![persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeUrl options:options error:&error]) {
		
		// something bad happened
		NSLog(@"Band CoreData Error\n%@\n%@", error, [error userInfo]);
		
//		// show info
//        if (delegate && [delegate respondsToSelector:@selector(apiFatal:message:)]) {
//            [delegate apiFatal:NSLocalizedString(@"Data Error", "Data Error")
//                       message:NSLocalizedString(@"Corrupted Cache. Please try to clear the cache or reinstall the application... Sorry about this.",@"Corrupted Cache. Please try to clear the cache or reinstall the application... Sorry about this.")];
//        }
        
    }
	
	// return
    return persistentStoreCoordinator;
}

- (NSMutableArray *)fetchRecords {
	
	// Define our table/entity to use
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"Band" inManagedObjectContext:managedObjectContext];
	
	// Setup the fetch request
	NSFetchRequest *request = [[NSFetchRequest alloc] init];
	[request setEntity:entity];
	
//	// Define how we will sort the records
//	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"saveDate" ascending:NO];
//	NSArray *sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
//	
//	[request setSortDescriptors:sortDescriptors];
	
	// Fetch the records and handle an error
	NSError *error;
	NSMutableArray *mutableFetchResults = [[managedObjectContext executeFetchRequest:request error:&error] mutableCopy];
	
     NSLog(@"save obj count:%i",[mutableFetchResults count]);
    
	if (!mutableFetchResults) {
		// Handle the error.
		// This is a serious error and should advise the user to restart the application
	}
	
	// Save our fetched data to an array
    NSLog(@"=>RESULTS:\n");
    
    for(Band * b in mutableFetchResults)
        NSLog(@"%@ %i",b.name,b.objectID.hash);
    
    return mutableFetchResults;
	
}

@end
