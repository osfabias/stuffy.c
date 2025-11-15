#import <Cocoa/Cocoa.h>

#import "stuffy/window.h"

#import "src/macos/Window.h"
#import "src/macos/WindowDelegate.h"

@implementation WindowDelegate

@synthesize stuffyWindow;

- (instancetype)initWithWindow:(StuffyWindow *)window
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
  self.stuffyWindow->state = STUFFY_WINDOW_STATE_ICONIFIED;
}

- (void)windowWillDeminiaturize:(NSNotification *)notification
{
  self.stuffyWindow->state = STUFFY_WINDOW_STATE_NORMAL;
}

- (void)windowWillEnterFullScreen:(NSNotification *)notification
{
  self.stuffyWindow->state = STUFFY_WINDOW_STATE_FULLSCREEN;
}

- (void)windowWillExitFullScreen:(NSNotification *)notification
{
  self.stuffyWindow->state = STUFFY_WINDOW_STATE_NORMAL;
}

- (void)windowDidResize:(NSNotification *)notification
{
  if (self.stuffyWindow && self.stuffyWindow->resize_callback)
  {
    const NSRect content_rect = [self.stuffyWindow->ns_window contentRectForFrameRect:[self.stuffyWindow->ns_window frame]];
    self.stuffyWindow->resize_callback (
      self.stuffyWindow,
      (uint32_t)content_rect.size.width,
      (uint32_t)content_rect.size.height
    );
  }
}

@end

