//
//  InfoViewController.m
//  SoundPath
//
//  Created by Bogdan Florescu on 10/28/12.
//
//

#import "InfoViewController.h"
#import "SPHTTPClient.h"
#import "SPUtils.h"
#import "Band.h"

@interface InfoViewController ()

@end

@implementation InfoViewController

@synthesize table;
@synthesize dataSource;
@synthesize name;
@synthesize artistPhoto;
@synthesize fans;
@synthesize band_id;
@synthesize band;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
       
    }
    return self;
}

#pragma mark -
#pragma mark UITableViewDataSource

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 50;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    cell.textLabel.text = [NSString stringWithFormat:@"%@", [self.dataSource objectAtIndex:indexPath.row]];
    
    return cell;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	return [NSString stringWithFormat:@"Section %i", section];
}

#pragma mark -
#pragma mark UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    NSLog(@"selected cell at section #%d and row #%d", indexPath.section, indexPath.row);
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void) viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString * token = [defaults objectForKey:@"FBAccessTokenKey"];
    
    NSDictionary * params = [NSDictionary dictionaryWithObjectsAndKeys:[defaults objectForKey:kUid],@"uid",token,@"token", nil];
    
    [SPHTTPClient getBandInfo:params withId:self.band_id andBlock:^(NSDictionary* response) {
        if (response) {
            NSLog(@"%@",response);
            
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
            
            
            if([response valueForKey:@"top_tracks"] &&
               ![(NSNull*)[response valueForKey:@"top_tracks"] isEqual:[NSNull null]])
                self.dataSource = [response valueForKey:@"top_tracks"];

            
            if([response valueForKey:@"name"] &&
               ![(NSNull*)[response valueForKey:@"name"] isEqual:[NSNull null]])
                band.name = [response valueForKey:@"name"];
            
            [self.table reloadData];
            
        }
    }];

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
