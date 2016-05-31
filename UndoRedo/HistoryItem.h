// Copyright (c) 2016 Lightricks. All rights reserved.
// Created by Boris Talesnik.

#import "Document.h"

NS_ASSUME_NONNULL_BEGIN

@protocol DocumentEditingSequence;

@protocol Cancelable;

@protocol HistoryOperation <NSObject>

- (void)applyOnSequence:(id<DocumentEditingSequence>)sequence
         withCancelable:(id<Cancelable>)cancelable;

@property (readonly, nonatomic) UIImage *thumbnail;

@property (readonly, nonatomic) NSString *operationType;

@end

@protocol HistoryItem <NSObject>

@property (readonly, nonatomic) id<DocumentRevision> revision;

@property (readonly, nonatomic) id<HistoryOperation> operation;

@property (readonly, nonatomic) BOOL forceSnapshot;

@property (readonly, nonatomic, nullable) id<DocumentRevision> prerequisite;

@end

@protocol HistoryItemsProvider;

@protocol ItemsRunner

/// Needs provider to get \c HistoryItems for prerequisite revisions.
- (instancetype)initWithItemProvider:(id<HistoryItemsProvider>)provider;

/// Runs item's operation after applying the operation of prerequisite (if exists) revision in
/// recursive manner. Same operation as prerequisite can be skipped for optimization.
- (void)runItem:(id<HistoryItem>)item onSequence:(id<DocumentEditingSequence>)sequence
 withCancelable:(id<Cancelable>)cancelable;

@end

NS_ASSUME_NONNULL_END
