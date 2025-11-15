#import <AppKit/AppKit.h>

#import "stuffy/app.h"

#import "src/macos/AppDelegate.h"

inline static AppDelegate *stuffy__alloc_app_delegate (void);

static struct
{
  AppDelegate *app_delegate;
  int          initialized;
} g_app_state = {0};

int32_t stuffy_app_init (void)
{
  if (g_app_state.initialized)
  {
    return 1;
  }

  // Create shared application instance (further 'NSApp')
  [NSApplication sharedApplication];

  g_app_state.app_delegate = stuffy__alloc_app_delegate ( );
  if (!g_app_state.app_delegate) { return 1; }
  [NSApp setDelegate:g_app_state.app_delegate];

  if (![[NSRunningApplication currentApplication] isFinishedLaunching]) { [NSApp run]; }

  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
  [NSApp activateIgnoringOtherApps:YES];

  g_app_state.initialized = 1;

  return 0;
}

void stuffy_app_update (void)
{
  @autoreleasepool
  {
    NSEvent *event;

    while (1)
    {
      event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                 untilDate:[NSDate distantPast]
                                    inMode:NSDefaultRunLoopMode
                                   dequeue:YES];
      if (!event) { break; }
      [NSApp sendEvent:event];
    }
  }
}

void stuffy_app_deinit (void)
{
  if (!g_app_state.initialized)
  {
    return;
  }

  [NSApp setDelegate:nil];
  [g_app_state.app_delegate release];

  g_app_state.initialized = 0;
}

AppDelegate *stuffy__alloc_app_delegate (void)
{
  AppDelegate *delegate = [AppDelegate alloc];
  delegate = [delegate init];
  if (!delegate)
  {
    return NULL;
  }

  return delegate;
}

