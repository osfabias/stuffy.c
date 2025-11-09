#import <Cocoa/Cocoa.h>

#import "stuffy/window.h"

#import "src/macos/Window.h"
#import "src/macos/WindowDelegate.h"

@implementation WindowDelegate

@synthesize stuffyWindow;

- (instancetype)initWithWindow:(Window *)window
{
  self = [super init];
  if (self) { self.stuffyWindow = window; }

  return self;
}

- (BOOL)windowShouldClose:(id)sender
{
  self.stuffyWindow->should_close = YES;
  return YES;
}

- (void)windowWillMiniaturize:(NSNotification *)notification
{
  self.stuffyWindow->state = WINDOW_STATE_ICONIFIED;
}

- (void)windowWillDeminiaturize:(NSNotification *)notification
{
  self.stuffyWindow->state = WINDOW_STATE_NORMAL;
}

- (void)windowWillEnterFullScreen:(NSNotification *)notification
{
  self.stuffyWindow->state = WINDOW_STATE_FULLSCREEN;
}

- (void)windowWillExitFullScreen:(NSNotification *)notification
{
  self.stuffyWindow->state = WINDOW_STATE_NORMAL;
}

@end

