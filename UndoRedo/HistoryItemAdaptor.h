// Copyright (c) 2016 Lightricks. All rights reserved.
// Created by Boris Talesnik.

#import <Foundation/Foundation.h>

#import "Document.h"
#import "HistoryItem.h"

NS_ASSUME_NONNULL_BEGIN

/// Each editing operation (feature) should provide adator which knows how to convert operation data
/// of given revision to \c HistoryItem instance and back
@protocol HistoryOperationAdaptor

- (id<HistoryOperation>)operationForRevision:(id<DocumentRevision>)revision
                                revisionData:(NSData *)data;

- (NSData *)dataForOperation:(id<HistoryOperation>)operation;

//- (NSData *)dataForOperationSnapshot:(id<HistoryOperation>)operation;

@property (readonly, nonatomic) NSString *operationType;

@end

NS_ASSUME_NONNULL_END
