// Copyright (c) 2016 Lightricks. All rights reserved.
// Created by Boris Talesnik.

#import "HistoryItem.h"
#import "Protocols.h"

NS_ASSUME_NONNULL_BEGIN

typedef void(^HistoryActionComplete)(id<Cancelable> token, id<DocumentRevision> revisionApplied);

@protocol HistoryManagerAsync

/// since undo/redo operations are async it is HistoryOperation responsibility to synchronize the
/// actuall editing task or run it on correct thread.
/// (Which means that its interface should async too).
- (id<Cancelable>)undoWithCompletion:(HistoryActionComplete)completion;

- (id<Cancelable>)redoWithCompletion:(HistoryActionComplete)completion;

- (id<Cancelable>)addOperation:(id<HistoryOperation>)operation
              withPrerequisite:(nullable id<DocumentRevision>)revision
                    completion:(HistoryActionComplete)completion;

- (id<Cancelable>)applyRevision:(id<DocumentRevision>)revision
                      withCompletion:(HistoryActionComplete)completion;

- (void)commit;

@property (readonly, nonatomic) BOOL canUndo;
@property (readonly, nonatomic) BOOL canRedo;

/// gets current revision and applies its operation, setter should be async and implemented as
/// applyRevision method.
@property (readonly, nonatomic) id<DocumentRevision> currentRevision;

@property (readonly, nonatomic) id<DocumentProps> document;

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
