// Copyright (c) 2016 Lightricks. All rights reserved.
// Created by Boris Talesnik.

#import "HistoryItem.h"
#import "HistoryItemAdaptor.h"

NS_ASSUME_NONNULL_BEGIN

/// Editing operation that provides snapshot.
@protocol EditingOpertionSnapshotProvider <NSObject>

- (id<HistoryOperation>)snapshot;

@end

@protocol Cancelable

/// caller of cancelable operation should set cancelRequested to YES and check for error in
/// completion block to see if operation was indeed canceled.
@property (nonatomic) BOOL cancelRequested;

@property (strong, nonatomic) NSError *error;

@end

typedef void(^ItemCompletion)(id<Cancelable> token, id<HistoryItem> item);

typedef NSArray<id<DocumentRevision>> RevisionsArray;
typedef void(^RevisionsArrayCompletion)(id<Cancelable> token, RevisionsArray *revisions);

typedef NSArray<id<DocumentProps>> DocumentsArray;
typedef void(^DocumentArrayCompletion)(id<Cancelable> token, DocumentsArray *documents);

typedef NSArray<id<HistoryItem>> HistoryItemsArray;
typedef void(^HistoryItemsCompletion)(id<Cancelable> token, HistoryItemsArray *items);

typedef void(^CancelableCompletion)(id<Cancelable> token);

/// Make convertion of operation data representation to application logic implemented in
/// HistoryOperation objects
@protocol HistoryOperationsRegistry

- (void)addOperationAdaptor:(id<HistoryOperationAdaptor>)adaptor;

- (void)removeOperationAdaptor:(id<HistoryOperationAdaptor>)adaptor;

- (void)adaptorForOperation:(NSString *)operationType;

@end

/// load/store item for revision from revisions db. handles converting stored data to actuall
/// operation implementations.
@protocol HistoryItemsProvider <HistoryOperationsRegistry, NSObject>

- (id<Cancelable>)itemForRevision:(id<DocumentRevision>)revision
                       completion:(ItemCompletion)completion;

/// Saves the item to persistant DB assigning item operation to item revision.
- (id<Cancelable>)saveItem:(id<HistoryItem>)item completion:(CancelableCompletion)completion;

- (id<Cancelable>)itemsForRevisions:(RevisionsArray *)revisions
                         completion:(HistoryItemsCompletion)completion;

- (id<Cancelable>)commitUpTo:(id<DocumentRevision>)revision
                  completion:(RevisionsArrayCompletion)completion;

@end

typedef void(^PagedRevisionsArrayCompletion)(id<Cancelable> token, RevisionsArray *revisions,
                                             BOOL last);

/// Provides revisions from storage.
@protocol DocumentRevisionsProvider

- (id<Cancelable>)loadLastRevisions:(NSUInteger)maxCount
                         completion:(RevisionsArrayCompletion)completion
                        forDocument:(NSString *)documentUID;

/// Loads \c count revisions from start (newest) back in history.
- (id<Cancelable>)loadRevisions:(id<DocumentRevision>)start count:(NSUInteger)count
                     completion:(RevisionsArrayCompletion)completion;

- (id<Cancelable>)removeRevisions:(id<DocumentRevision>)start count:(NSUInteger)count
                       completion:(RevisionsArrayCompletion)completion;

- (id<Cancelable>)lastRevisionForDocument:(NSString *)documentUID
                               completion:(RevisionsArrayCompletion)completion;

/// more functions may be needed like paged load of revisions from/to given revisions.
/// this way if we synchronize with state on server we don't need to bring all the difference maybe
/// we need to load only until the common parent of last revisions.
- (id<Cancelable>)loadRevisions:(id<DocumentRevision>)start until:(id<DocumentRevision>)end
                pagedCompletion:(PagedRevisionsArrayCompletion)completion;


- (NSUInteger)revisionsCountForDocument:(NSString *)documentUID;

@end

/// Manages documents storage.
@protocol DocumentStorage

- (id<DocumentProps>)documentProps:(NSString *)documentUID;

- (NSUInteger)documentsCount;

- (id<Cancelable>)loadLastDocumentsWithCount:(NSUInteger)maxCount
                                  completion:(DocumentArrayCompletion)completion;

- (id<DocumentProps>)createFromULR:(NSURL *)url;

@end



NS_ASSUME_NONNULL_END
