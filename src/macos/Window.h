#pragma once

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#import "stuffy/window.h"

#import "src/macos/ContentView.h"
#import "src/macos/WindowDelegate.h"

struct StuffyWindow
{
  NSWindow          *ns_window;
  ContentView       *content_view;
  WindowDelegate    *window_delegate;
  CAMetalLayer      *metal_layer;
  StuffyWindowState state;
  BOOL               should_close;
  void (*resize_callback) (struct StuffyWindow *, StuffyWindowRect);
};

