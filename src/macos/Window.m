#import <AppKit/AppKit.h>
#import <QuartzCore/QuartzCore.h>
#include <stdlib.h>

#import "stuffy/window.h"

#import "src/macos/ContentView.h"
#import "src/macos/Window.h"
#import "src/macos/WindowDelegate.h"

inline static StuffyWindow *stuffy__alloc_window_struct(void);
inline static WindowDelegate *stuffy__alloc_window_delegate(StuffyWindow *);
inline static ContentView *stuffy__alloc_content_view(StuffyWindow *);
inline static NSWindow *stuffy__alloc_ns_window(const StuffyWindowConfig *);
inline static CAMetalLayer *stuffy__alloc_metal_layer(ContentView *);
static float stuffy__transform_y(float);

StuffyWindow *stuffy_window_open(const StuffyWindowConfig *config) {
  StuffyWindow *window = stuffy__alloc_window_struct();
  if (!window) {
    return NULL;
  }

  window->window_delegate = stuffy__alloc_window_delegate(window);
  if (!window->window_delegate) {
    goto FAILED_WINDOW_DELEGATE;
  }

  window->content_view = stuffy__alloc_content_view(window);
  if (!window->content_view) {
    goto FAILED_INIT_CONTENT_VIEW;
  }

  window->ns_window = stuffy__alloc_ns_window(config);
  if (!window->ns_window) {
    goto FAILED_INIT_NS_WINDOW;
  }
  [window->ns_window setDelegate:window->window_delegate];
  [window->ns_window setContentView:window->content_view];
  [window->ns_window makeFirstResponder:window->content_view];

  [[NSNotificationCenter defaultCenter]
      addObserver:window->window_delegate
         selector:@selector(windowDidResize:)
             name:NSWindowDidResizeNotification
           object:window->ns_window];
  [[NSNotificationCenter defaultCenter] addObserver:window->window_delegate
                                           selector:@selector(windowDidMove:)
                                               name:NSWindowDidMoveNotification
                                             object:window->ns_window];

  window->metal_layer = stuffy__alloc_metal_layer(window->content_view);

  return window;

FAILED_INIT_CONTENT_VIEW:
  [window->ns_window close];
  [window->ns_window release];

FAILED_INIT_NS_WINDOW:
  [window->window_delegate release];

FAILED_WINDOW_DELEGATE:
  free(window);

  return NULL;
}

void stuffy_window_close(StuffyWindow *window) {
  [window->ns_window orderOut:nil];

  [[NSNotificationCenter defaultCenter]
      removeObserver:window->window_delegate
                name:NSWindowDidResizeNotification
              object:window->ns_window];
  [[NSNotificationCenter defaultCenter]
      removeObserver:window->window_delegate
                name:NSWindowDidMoveNotification
              object:window->ns_window];

  [window->ns_window setContentView:nil];
  [window->content_view setLayer:nil];
  [window->content_view release];

  [window->ns_window setDelegate:nil];
  [window->window_delegate release];

  [window->ns_window close];
  [window->ns_window release];

  free(window);
}

bool stuffy_window_should_close(StuffyWindow *window) {
  return window->should_close == YES;
}

void stuffy_window_set_title(StuffyWindow *window, const char *title) {
  [window->ns_window
      setTitle:[NSString stringWithCString:title
                                  encoding:NSUTF8StringEncoding]];
}

const char *stuffy_window_get_title(StuffyWindow *window) {
  const char *const title =
      [window->ns_window.title cStringUsingEncoding:NSUTF8StringEncoding];
  return title ? title : "";
}

void stuffy_window_set_rect(StuffyWindow *window, StuffyWindowRect rect) {
  if (stuffy_window_get_state(window) != STUFFY_WINDOW_STATE_NORMAL) {
    return;
  }

  @autoreleasepool {
    NSRect ns_rect =
        [window->ns_window contentRectForFrameRect:[window->ns_window frame]];
    ns_rect.origin.x = rect.x;
    ns_rect.origin.y = stuffy__transform_y(rect.y + rect.height - 1);
    ns_rect.size.width = rect.width;
    ns_rect.size.height = rect.height;
    [window->ns_window
        setFrame:[window->ns_window frameRectForContentRect:ns_rect]
         display:NO];
  } // autoreleasepool
}

StuffyWindowRect stuffy_window_get_rect(StuffyWindow *window) {
  @autoreleasepool {
    const NSRect rect =
        [window->ns_window contentRectForFrameRect:[window->ns_window frame]];

    StuffyWindowRect window_rect = {
        .x = (int32_t)rect.origin.x,
        .y = (int32_t)stuffy__transform_y(rect.origin.y + rect.size.height - 1),
        .width = (uint32_t)rect.size.width,
        .height = (uint32_t)rect.size.height,
    };

    return window_rect;
  } // autoreleasepool
}

StuffyExtent2D stuffy_window_get_framebuffer_size(StuffyWindow *window) {
  @autoreleasepool {
    const NSRect content_rect =
        [window->ns_window contentRectForFrameRect:[window->ns_window frame]];
    const NSSize backing_size =
        [window->content_view convertSizeToBacking:content_rect.size];

    StuffyExtent2D extent = {
        .width = (uint32_t)backing_size.width,
        .height = (uint32_t)backing_size.height,
    };

    return extent;
  } // autoreleasepool
}

void stuffy_window_set_state(StuffyWindow *window, StuffyWindowState state) {
  if ((state == STUFFY_WINDOW_STATE_FULLSCREEN) !=
      (bool)(window->ns_window.styleMask & NSWindowStyleMaskFullScreen)) {
    [window->ns_window toggleFullScreen:nil];
  }

  window->state = state;

  switch (state) {
  case STUFFY_WINDOW_STATE_ICONIFIED:
    [window->ns_window miniaturize:nil];
    break;
  case STUFFY_WINDOW_STATE_NORMAL:
    [window->ns_window deminiaturize:nil];
    break;
  case STUFFY_WINDOW_STATE_FULLSCREEN:
    break;
  default:
    return;
  }
}

StuffyWindowState stuffy_window_get_state(StuffyWindow *window) {
  return window->state;
}

void stuffy_window_set_resize_callback(StuffyWindow *window,
                                       StuffyWindowResizeCallback callback) {
  if (window == NULL) {
    return;
  }
  window->resize_callback = callback;
}

void stuffy_window_set_move_callback(StuffyWindow *window,
                                     StuffyWindowMoveCallback callback) {
  if (window == NULL) {
    return;
  }
  window->move_callback = callback;
}

StuffyWindow *stuffy__alloc_window_struct(void) {
  StuffyWindow *const window = malloc(sizeof(StuffyWindow));
  if (!window) {
    return NULL;
  }

  *window = (StuffyWindow){
      .state = STUFFY_WINDOW_STATE_NORMAL,
      .resize_callback = NULL,
      .move_callback = NULL,
  };

  return window;
}

WindowDelegate *stuffy__alloc_window_delegate(StuffyWindow *window) {
  WindowDelegate *delegate = [WindowDelegate alloc];
  delegate = [delegate initWithWindow:window];
  if (!delegate) {
    return NULL;
  }

  return delegate;
}

NSWindow *stuffy__alloc_ns_window(const StuffyWindowConfig *config) {
  @autoreleasepool {
    const NSRect ns_window_rect = NSMakeRect(
        config->rect.x,
        stuffy__transform_y(config->rect.y + config->rect.height - 1),
        config->rect.width, config->rect.height);

    NSWindow *ns_window = [NSWindow alloc];
    ns_window =
        [ns_window initWithContentRect:ns_window_rect
                             // window style mask and ns window
                             // style mask are compatible
                             styleMask:(NSWindowStyleMask)config->style_mask
                               backing:NSBackingStoreBuffered
                                 defer:NO];
    if (!ns_window) {
      return NULL;
    }

    [ns_window setTitle:[NSString stringWithCString:config->title
                                           encoding:NSUTF8StringEncoding]];

    [ns_window makeKeyAndOrderFront:nil];
    [ns_window setAcceptsMouseMovedEvents:YES];
    [ns_window
        setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
    [ns_window setLevel:NSNormalWindowLevel];
    [ns_window setRestorable:NO];

    return ns_window;
  } // autoreleasepool
}

ContentView *stuffy__alloc_content_view(StuffyWindow *window) {
  ContentView *view = [ContentView alloc];
  view = [view initWithWindow:window];
  if (!view) {
    return NULL;
  }

  [view setWantsLayer:YES];

  return view;
}

CAMetalLayer *stuffy__alloc_metal_layer(ContentView *view) {
  CAMetalLayer *layer = [CAMetalLayer layer];

  /* Handle content scaling for various fidelity displays (i.e. Retina) */
  [layer setBounds:view.bounds];

  /**
   * It's important to set the drawableSize to the actual backing pixels. When
   * rendering full-screen, we can skip the macOS compositor if the size matches
   * the display size.
   */
  const NSSize drawable_size = [view convertSizeToBacking:view.bounds.size];
  [layer setDrawableSize:drawable_size];

  /**
   * The contentsScale is set to match the window's backing scale factor,
   * which is important for proper rendering on high-DPI displays (e.g., Retina).
   */
  [layer setContentsScale:view.window.backingScaleFactor];

  [view setLayer:layer];

  /**
   * This is set to NO by default, but is also important to ensure we can bypass
   * the compositor in full-screen mode NOTE: See "Direct to Display"
   * http://metalkit.org/2017/06/30/introducing-metal-2.html.
   */
  [layer setOpaque:YES];

  return layer;
}

static float stuffy__transform_y(float value) {
  return CGDisplayBounds(CGMainDisplayID()).size.height - value - 1;
}

void *stuffy_window_get_metal_layer(StuffyWindow *window) {
  if (!window) {
    return NULL;
  }
  return (__bridge void *)window->metal_layer;
}
