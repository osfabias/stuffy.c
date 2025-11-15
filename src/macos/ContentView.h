#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

#import "stuffy/input/keyboard.h"
#import "stuffy/window.h"

@class ContentView;

NS_ASSUME_NONNULL_BEGIN

@interface ContentView : NSView <NSTextInputClient> {
  NSWindow *ns_window;
}

@property (nonatomic) StuffyWindow *stuffyWindow;

- (instancetype)initWithWindow:(StuffyWindow *)window;

+ (StuffyKey)translateKeyCode:(unsigned short)key_code;

@end

NS_ASSUME_NONNULL_END

