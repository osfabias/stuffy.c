#import <Cocoa/Cocoa.h>

#import "stuffy/window.h"

@class WindowDelegate;

NS_ASSUME_NONNULL_BEGIN

@interface WindowDelegate : NSObject <NSWindowDelegate>

@property (nonatomic) StuffyWindow *stuffyWindow;

- (instancetype)initWithWindow:(StuffyWindow *)window;

@end

NS_ASSUME_NONNULL_END

