// Copyright (c) 2016 Lightricks. All rights reserved.
// Created by Boris Talesnik.

#import "Driver.h"

#import "HistoryManager.h"
#import "ImageUndoRedo.h"
#import "Protocols.h"

NS_ASSUME_NONNULL_BEGIN

@interface HistoryManagerFactory

/// creates all the providers according to persistane storage and application model.
+ (id<HistoryManager>)managerWithDocument:(id<DocumentProps>)document;

@end

@interface EditingOperation : NSObject <EditingOpertionSnapshotProvider, HistoryOperationAdaptor>
@end

/// Model has to have some state of current document content that is passed to editing operations
/// this way EditingOperation snapshot (HistoryOperation) can apply its changes to current state.
@interface Model

@property (readonly, nonatomic) EditingOperation *reshape;

@property (readonly, nonatomic) EditingOperation *toneMapping;

@property (strong, nonatomic) EditingOperation *activeOperation;

@end

@implementation Driver

- (void)test {
  Model *model;
  
  id<DocumentStorage> storage;
  id<DocumentProps> doc = [storage documentProps:@"vasya"];
  
  id<HistoryManager> manager = [HistoryManagerFactory  managerWithDocument:doc];
  [manager.operationsRegistry addOperationAdaptor:model.reshape];
  [manager.operationsRegistry addOperationAdaptor:model.toneMapping];
  
  /// Example of usage
  
  model.activeOperation = model.reshape;
  /// change parameters and apply changes to image.
  /// ....
  
  id<DocumentRevision> reshapeRevision = [manager addOperation:[model.activeOperation snapshot]
                                              withPrerequisite:nil];
  
  model.activeOperation = model.toneMapping;
  /// change parameters and apply changes to image.
  /// ....
  
  [manager addOperation:[model.activeOperation snapshot] withPrerequisite:reshapeRevision];
  /// change parameters and apply changes to image.

  [manager addOperation:[model.activeOperation snapshot] withPrerequisite:reshapeRevision];

  [manager undo];
}

@end

NS_ASSUME_NONNULL_END
