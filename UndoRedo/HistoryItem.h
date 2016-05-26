// Copyright (c) 2016 Lightricks. All rights reserved.
// Created by Boris Talesnik.

#import "Document.h"

NS_ASSUME_NONNULL_BEGIN

@protocol Cancelable;

@protocol HistoryOperation <NSObject>

- (void)applyWithCancelable:(id<Cancelable>)cancelable;

@property (readonly, nonatomic) UIImage *thumbnail;

@property (readonly, nonatomic) NSString *operationType;

@end

@protocol HistoryItem <NSObject>

@property (readonly, nonatomic) id<DocumentRevision> revision;

@property (readonly, nonatomic) id<HistoryOperation> operation;

@property (readonly, nonatomic, nullable) id<DocumentRevision> prerequisite;

@end

@interface ImageResource : NSObject

@end

/// Example of operation.
@interface HistoryOperationSnapshot : NSObject <HistoryOperation>

/// initialized with system state (target) and data from history (contentSnapshot).
- (instancetype)initWithTargetResoure:(ImageResource *)target;

@property (readonly, nonatomic) ImageResource *contentSnapshot;

@end

@protocol HistoryItemsProvider;

@protocol ItemsRunner

/// Needs provider to get \c HistoryItems for prerequisite revisions.
- (instancetype)initWithItemProvider:(id<HistoryItemsProvider>)provider;

/// Runs item's operation after applying the operation of prerequisite (if exists) revision in
/// recursive manner. Same operation as prerequisite can be skipped for optimization.
- (void)runItem:(id<HistoryItem>)item withCancelable:(id<Cancelable>)cancelable;

@end

NS_ASSUME_NONNULL_END
