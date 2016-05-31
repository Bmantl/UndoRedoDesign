// Copyright (c) 2016 Lightricks. All rights reserved.
// Created by Boris Talesnik.

#import "HistoryItem.h"
#import "Protocols.h"

NS_ASSUME_NONNULL_BEGIN

@protocol Snapshot

- (NSData *)snapshot;

- (void)restoreFromSnapshotData:(NSData *)data;

@end

@protocol DocumentEditingSequence <NSObject>

@property (strong, readonly, nonatomic) id<DocumentProps> props;

@property (strong, readonly, nonatomic) id<DocumentRevision> currentRevision;

@property (strong, readonly, nonatomic) id<Snapshot> applicationState;

@end

typedef void(^HistoryActionComplete)(id<Cancelable> token, id<DocumentEditingSequence> sequence);

@protocol HistoryManagerAsync

/// since undo/redo operations are async it is HistoryOperation responsibility to synchronize the
/// actuall editing task or run it on correct thread.
/// (Which means that its interface should async too).
- (id<Cancelable>)undoOnSequence:(id<DocumentEditingSequence>)sequence
                      completion:(HistoryActionComplete)completion;

- (id<Cancelable>)redoOnSequence:(id<DocumentEditingSequence>)sequence
                    completion:(HistoryActionComplete)completion;

- (id<Cancelable>)addOperation:(id<HistoryOperation>)operation
                    toSequence:(id<DocumentEditingSequence>)sequence
              withPrerequisite:(nullable id<DocumentRevision>)revision
                  forceSnaphot:(BOOL)forceSnaphot
                    completion:(HistoryActionComplete)completion;

- (id<Cancelable>)takeSnapshot:(id<DocumentEditingSequence>)sequence
                  forceFlatten:(BOOL)flatten completion:(HistoryActionComplete)completion;

- (id<Cancelable>)applyRevision:(id<DocumentRevision>)revision
                     onSequence:(id<DocumentEditingSequence>)sequence
                 withCompletion:(HistoryActionComplete)completion;

- (id<Cancelable>)commitSequence:(id<DocumentEditingSequence>)sequence completion:(id)completion;

@property (readonly, nonatomic) id<HistoryOperationsRegistry> operationsRegistry;

@property (readonly, nonatomic) id<DocumentRevisionsProvider> revisionsProvider;


@end

@protocol HistoryManager

@property (readonly, nonatomic) BOOL canUndo;
@property (readonly, nonatomic) BOOL canRedo;

- (void)undo;

- (void)redo;

- (id<DocumentRevision>)addOperation:(id<HistoryOperation>)operation
                    withPrerequisite:(nullable id<DocumentRevision>)revision;

- (void)commit;

/// sets current revision and applies its operation.
@property (strong, nonatomic) id<DocumentRevision> currentRevision;

@property (readonly, nonatomic) id<DocumentProps> document;

@property (readonly, nonatomic) id<HistoryOperationsRegistry> operationsRegistry;

@property (readonly, nonatomic) id<DocumentRevisionsProvider> revisionsProvider;

@end

/// History can be implemented as list of history items with pointer to current revision.
/// Making changes in the middle of history removes all the revisions after the revision that
/// changes were applied to.
@interface HistoryManager : NSObject <HistoryManager>

- (instancetype)initWithDocument:(id<DocumentProps>)document
               revisionsProvider:(id<DocumentRevisionsProvider>)revisionsProvider
            historyItemsProvider:(id<HistoryItemsProvider>)itemsProvider
                     itemsRunner:(id<ItemsRunner>)itemsRunner;

@end

NS_ASSUME_NONNULL_END
