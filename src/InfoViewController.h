//
//  InfoViewController.h
//  SoundPath
//
//  Created by Bogdan Florescu on 10/28/12.
//
//

#import <UIKit/UIKit.h>

@class Band;

@interface InfoViewController : UIViewController<UITableViewDataSource, UITableViewDelegate>

@property (nonatomic, strong) IBOutlet UITableView * table;
@property (nonatomic, strong) NSArray * dataSource;
@property (nonatomic, strong) UIImageView * artistPhoto;
@property (nonatomic, strong) UILabel * name;
@property (nonatomic, strong) UILabel * fans;
@property (nonatomic, strong) NSString * band_id;
@property (nonatomic, strong) Band * band;
@end
