#import <Cocoa/Cocoa.h>

#import "stuffy/window.h"

@class WindowDelegate;

NS_ASSUME_NONNULL_BEGIN

@interface WindowDelegate : NSObject <NSWindowDelegate>

@property (nonatomic) Window *stuffyWindow;

- (instancetype)initWithWindow:(Window *)window;

@end

NS_ASSUME_NONNULL_END

