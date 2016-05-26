// Copyright (c) 2016 Lightricks. All rights reserved.
// Created by Boris Talesnik.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol DocumentProps <NSObject>

@property NSString *UID;

@property NSString *title;

@end

@protocol DocumentRevision <NSObject>

@property id<DocumentProps> props;

@property NSUInteger revisionNumber;

@property (strong, nonatomic) UIImage *thumbnail;

@end

NS_ASSUME_NONNULL_END
