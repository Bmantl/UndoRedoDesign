// Copyright (c) 2016 Lightricks. All rights reserved.
// Created by Boris Talesnik.

#import "HistoryItem.h"
#import "HistoryItemAdaptor.h"
#import "Protocols.h"

NS_ASSUME_NONNULL_BEGIN

@interface ImageProcessor

@end

@interface ImageResource : NSObject

@end

@protocol ProcessorProvider

- (ImageProcessor *)processorForOperation:(NSString *)operationType;

@end

typedef void(^ParametersBlock)(NSDictionary<NSString *, id> *parameters);

@interface HistoryOperationParametersSet : NSObject <HistoryOperation>

- (instancetype)initWithParameters:(NSDictionary<NSString *, id> *)parameters
                 processorProvider:(id<ProcessorProvider>)provider;

- (void)applyOnImage:(ImageResource *)image;

@end

@interface HistoryOperationAdaptorImage : NSObject <HistoryOperationAdaptor>

- (instancetype)initWithProcessorProvider:(id<ProcessorProvider>)processorProvider;

@end

NS_ASSUME_NONNULL_END
