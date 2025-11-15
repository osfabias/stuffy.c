#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>

#import "stuffy/window.h"

#import "src/macos/Window.h"
#import "src/macos/WindowDelegate.h"

static float stuffy__transform_y (float value)
{
  return CGDisplayBounds (CGMainDisplayID ( )).size.height - value - 1;
}

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
    const StuffyWindowRect rect = {
      .x      = (int32_t)content_rect.origin.x,
      .y      = (int32_t)stuffy__transform_y (content_rect.origin.y + content_rect.size.height - 1),
      .width  = (uint32_t)content_rect.size.width,
      .height = (uint32_t)content_rect.size.height,
    };
    self.stuffyWindow->resize_callback (self.stuffyWindow, rect);
  }
}

- (void)windowDidMove:(NSNotification *)notification
{
  if (self.stuffyWindow && self.stuffyWindow->move_callback)
  {
    const NSRect content_rect = [self.stuffyWindow->ns_window contentRectForFrameRect:[self.stuffyWindow->ns_window frame]];
    const StuffyWindowRect rect = {
      .x      = (int32_t)content_rect.origin.x,
      .y      = (int32_t)stuffy__transform_y (content_rect.origin.y + content_rect.size.height - 1),
      .width  = (uint32_t)content_rect.size.width,
      .height = (uint32_t)content_rect.size.height,
    };
    self.stuffyWindow->move_callback (self.stuffyWindow, rect);
  }
}

@end

