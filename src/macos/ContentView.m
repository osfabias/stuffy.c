#import <Cocoa/Cocoa.h>

#import "stuffy/window.h"

#import "src/input/keyboard_state.h"
#import "src/input/mouse_state.h"
#import "src/macos/ContentView.h"
#import "src/macos/Window.h"

@implementation ContentView

@synthesize stuffyWindow;

- (instancetype)initWithWindow:(Window *)window
{
  self = [super init];
  if (self)
  {
    self.stuffyWindow = window;
    ns_window         = window->ns_window;
  }

  return self;
}

- (BOOL)canBecomeKeyView
{
  return YES;
}

- (BOOL)acceptsFirstResponder
{
  return YES;
}

- (BOOL)wantsUpdateLayer
{
  return YES;
}

- (BOOL)acceptsFirstMouse:(NSEvent *)event
{
  return YES;
}

- (void)mouseDown:(NSEvent *)event
{
  g_mouse_state.buttons[ MOUSE_BUTTON_LEFT ] = 1;
}

- (void)mouseDragged:(NSEvent *)event
{
  // Equivalent to moving the mouse for now
  [self mouseMoved:event];
}

- (void)mouseUp:(NSEvent *)event
{
  g_mouse_state.buttons[ MOUSE_BUTTON_LEFT ] = 0;
}

- (void)mouseMoved:(NSEvent *)event
{
  const NSPoint pos = [event locationInWindow];

  // Need to invert Y on macOS, since origin is bottom-left.
  // Also need to scale the mouse position by the device pixel
  // ratio so screen lookups are correct.
  NSSize windowSize = self.stuffyWindow->metal_layer.drawableSize;
  g_mouse_state.x   = (int32_t)(pos.x * self.stuffyWindow->metal_layer.contentsScale);

  g_mouse_state.y = (int32_t)(windowSize.height - (pos.y * self.stuffyWindow->metal_layer.contentsScale));
}

- (void)rightMouseDown:(NSEvent *)event
{
  g_mouse_state.buttons[ MOUSE_BUTTON_RIGHT ] = 1;
}

- (void)rightMouseDragged:(NSEvent *)event
{
  [self mouseMoved:event];
}

- (void)rightMouseUp:(NSEvent *)event
{
  g_mouse_state.buttons[ MOUSE_BUTTON_RIGHT ] = 0;
}

- (void)otherMouseDown:(NSEvent *)event
{
  g_mouse_state.buttons[ MOUSE_BUTTON_MIDDLE ] = 1;
}

- (void)otherMouseDragged:(NSEvent *)event
{
  [self mouseMoved:event];
}

- (void)otherMouseUp:(NSEvent *)event
{
  g_mouse_state.buttons[ MOUSE_BUTTON_MIDDLE ] = 0;
}

- (void)keyDown:(NSEvent *)event
{
  Key key                = [ContentView translateKeyCode:[event keyCode]];
  g_keyboard_state.keys[ key ] = 1;
}

- (void)keyUp:(NSEvent *)event
{
  Key key                = [ContentView translateKeyCode:[event keyCode]];
  g_keyboard_state.keys[ key ] = 0;
}

- (void)scrollWheel:(NSEvent *)event
{
  g_mouse_state.scroll = (float)[event scrollingDeltaY];
}

- (void)insertText:(id)string replacementRange:(NSRange)replacementRange
{
}

- (void)setMarkedText:(id)string
        selectedRange:(NSRange)selectedRange
     replacementRange:(NSRange)replacementRange
{
}

- (void)unmarkText
{
}

- (NSRange)selectedRange
{
  NSRange emptyRange = {NSNotFound, 0};
  return emptyRange;
}

- (NSRange)markedRange
{
  NSRange emptyRange = {NSNotFound, 0};
  return emptyRange;
}

- (BOOL)hasMarkedText
{
  return NO;
}

- (nullable NSMutableAttributedString *)
  attributedSubstringForProposedRange:(NSRange)range
                          actualRange:(nullable NSRangePointer)actualRange
{
  return nil;
}

- (NSArray<NSAttributedStringKey> *)validAttributesForMarkedText
{
  return [NSArray array];
}

- (NSRect)firstRectForCharacterRange:(NSRange)range
                         actualRange:(nullable NSRangePointer)actualRange
{
  return NSMakeRect (0, 0, 0, 0);
}

- (NSUInteger)characterIndexForPoint:(NSPoint)point
{
  return 0;
}

+ (Key)translateKeyCode:(unsigned short)keyCode
{
  switch (keyCode)
  {
  case 0x52 :
    return KEY_NUMPAD0;
  case 0x53 :
    return KEY_NUMPAD1;
  case 0x54 :
    return KEY_NUMPAD2;
  case 0x55 :
    return KEY_NUMPAD3;
  case 0x56 :
    return KEY_NUMPAD4;
  case 0x57 :
    return KEY_NUMPAD5;
  case 0x58 :
    return KEY_NUMPAD6;
  case 0x59 :
    return KEY_NUMPAD7;
  case 0x5B :
    return KEY_NUMPAD8;
  case 0x5C :
    return KEY_NUMPAD9;

  case 0x12 :
    return KEY_1;
  case 0x13 :
    return KEY_2;
  case 0x14 :
    return KEY_3;
  case 0x15 :
    return KEY_4;
  case 0x17 :
    return KEY_5;
  case 0x16 :
    return KEY_6;
  case 0x1A :
    return KEY_7;
  case 0x1C :
    return KEY_8;
  case 0x19 :
    return KEY_9;
  case 0x1D :
    return KEY_0;

  case 0x00 :
    return KEY_A;
  case 0x0B :
    return KEY_B;
  case 0x08 :
    return KEY_C;
  case 0x02 :
    return KEY_D;
  case 0x0E :
    return KEY_E;
  case 0x03 :
    return KEY_F;
  case 0x05 :
    return KEY_G;
  case 0x04 :
    return KEY_H;
  case 0x22 :
    return KEY_I;
  case 0x26 :
    return KEY_J;
  case 0x28 :
    return KEY_K;
  case 0x25 :
    return KEY_L;
  case 0x2E :
    return KEY_M;
  case 0x2D :
    return KEY_N;
  case 0x1F :
    return KEY_O;
  case 0x23 :
    return KEY_P;
  case 0x0C :
    return KEY_Q;
  case 0x0F :
    return KEY_R;
  case 0x01 :
    return KEY_S;
  case 0x11 :
    return KEY_T;
  case 0x20 :
    return KEY_U;
  case 0x09 :
    return KEY_V;
  case 0x0D :
    return KEY_W;
  case 0x07 :
    return KEY_X;
  case 0x10 :
    return KEY_Y;
  case 0x06 :
    return KEY_Z;

  case 0x27 :
    return KEY_APOSTROPHE;
  case 0x2A :
    return KEY_BACKSLASH;
  case 0x2B :
    return KEY_COMMA;
  case 0x18 :
    return KEY_EQUAL;  // Equal/Plus
  case 0x32 :
    return KEY_GRAVE;
  case 0x21 :
    return KEY_LBRACKET;
  case 0x1B :
    return KEY_MINUS;
  case 0x2F :
    return KEY_PERIOD;
  case 0x1E :
    return KEY_RBRACKET;
  case 0x29 :
    return KEY_SEMICOLON;
  case 0x2C :
    return KEY_SLASH;
  case 0x0A :
    return KEY_COUNT;  // ?

  case 0x33 :
    return KEY_BACKSPACE;
  case 0x39 :
    return KEY_CAPSLOCK;
  case 0x75 :
    return KEY_DELETE;
  case 0x7D :
    return KEY_DOWN;
  case 0x77 :
    return KEY_END;
  case 0x24 :
    return KEY_ENTER;
  case 0x35 :
    return KEY_ESCAPE;
  case 0x7A :
    return KEY_F1;
  case 0x78 :
    return KEY_F2;
  case 0x63 :
    return KEY_F3;
  case 0x76 :
    return KEY_F4;
  case 0x60 :
    return KEY_F5;
  case 0x61 :
    return KEY_F6;
  case 0x62 :
    return KEY_F7;
  case 0x64 :
    return KEY_F8;
  case 0x65 :
    return KEY_F9;
  case 0x6D :
    return KEY_F10;
  case 0x67 :
    return KEY_F11;
  case 0x6F :
    return KEY_F12;
  case 0x69 :
    return KEY_PRINT;
  case 0x6B :
    return KEY_F14;
  case 0x71 :
    return KEY_F15;
  case 0x6A :
    return KEY_F16;
  case 0x40 :
    return KEY_F17;
  case 0x4F :
    return KEY_F18;
  case 0x50 :
    return KEY_F19;
  case 0x5A :
    return KEY_F20;
  case 0x73 :
    return KEY_HOME;
  case 0x72 :
    return KEY_INSERT;
  case 0x7B :
    return KEY_LEFT;
  case 0x3A :
    return KEY_LALT;
  case 0x3B :
    return KEY_LCONTROL;
  case 0x38 :
    return KEY_LSHIFT;
  case 0x37 :
    return KEY_LSUPER;
  case 0x6E :
    return KEY_COUNT;  // Menu
  case 0x47 :
    return KEY_NUMLOCK;
  case 0x79 :
    return KEY_PAGEDOWN;
  case 0x74 :
    return KEY_PAGEUP;
  case 0x7C :
    return KEY_RIGHT;
  case 0x3C :
    return KEY_RSHIFT;
  case 0x36 :
    return KEY_RSUPER;
  case 0x31 :
    return KEY_SPACE;
  case 0x30 :
    return KEY_TAB;
  case 0x7E :
    return KEY_UP;

  case 0x45 :
    return KEY_ADD;
  case 0x41 :
    return KEY_DECIMAL;
  case 0x4B :
    return KEY_DIVIDE;
  case 0x4C :
    return KEY_ENTER;
  case 0x51 :
    return KEY_NUMPAD_EQUAL;
  case 0x43 :
    return KEY_MULTIPLY;
  case 0x4E :
    return KEY_SUBTRACT;

  default :
    return KEY_COUNT;
  }
}

@end

