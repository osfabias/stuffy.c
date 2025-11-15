#import <Cocoa/Cocoa.h>

#import "stuffy/window.h"

#import "src/input/keyboard_state.h"
#import "src/input/mouse_state.h"
#import "src/macos/ContentView.h"
#import "src/macos/Window.h"

@implementation ContentView

@synthesize stuffyWindow;

- (instancetype)initWithWindow:(StuffyWindow *)window
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
  g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_LEFT ] = 1;
}

- (void)mouseDragged:(NSEvent *)event
{
  // Equivalent to moving the mouse for now
  [self mouseMoved:event];
}

- (void)mouseUp:(NSEvent *)event
{
  g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_LEFT ] = 0;
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
  g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_RIGHT ] = 1;
}

- (void)rightMouseDragged:(NSEvent *)event
{
  [self mouseMoved:event];
}

- (void)rightMouseUp:(NSEvent *)event
{
  g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_RIGHT ] = 0;
}

- (void)otherMouseDown:(NSEvent *)event
{
  g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_MIDDLE ] = 1;
}

- (void)otherMouseDragged:(NSEvent *)event
{
  [self mouseMoved:event];
}

- (void)otherMouseUp:(NSEvent *)event
{
  g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_MIDDLE ] = 0;
}

- (void)keyDown:(NSEvent *)event
{
  StuffyKey key                = [ContentView translateKeyCode:[event keyCode]];
  g_keyboard_state.keys[ key ] = 1;
}

- (void)keyUp:(NSEvent *)event
{
  StuffyKey key                = [ContentView translateKeyCode:[event keyCode]];
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

+ (StuffyKey)translateKeyCode:(unsigned short)keyCode
{
  switch (keyCode)
  {
  case 0x52 :
    return STUFFY_KEY_NUMPAD0;
  case 0x53 :
    return STUFFY_KEY_NUMPAD1;
  case 0x54 :
    return STUFFY_KEY_NUMPAD2;
  case 0x55 :
    return STUFFY_KEY_NUMPAD3;
  case 0x56 :
    return STUFFY_KEY_NUMPAD4;
  case 0x57 :
    return STUFFY_KEY_NUMPAD5;
  case 0x58 :
    return STUFFY_KEY_NUMPAD6;
  case 0x59 :
    return STUFFY_KEY_NUMPAD7;
  case 0x5B :
    return STUFFY_KEY_NUMPAD8;
  case 0x5C :
    return STUFFY_KEY_NUMPAD9;

  case 0x12 :
    return STUFFY_KEY_1;
  case 0x13 :
    return STUFFY_KEY_2;
  case 0x14 :
    return STUFFY_KEY_3;
  case 0x15 :
    return STUFFY_KEY_4;
  case 0x17 :
    return STUFFY_KEY_5;
  case 0x16 :
    return STUFFY_KEY_6;
  case 0x1A :
    return STUFFY_KEY_7;
  case 0x1C :
    return STUFFY_KEY_8;
  case 0x19 :
    return STUFFY_KEY_9;
  case 0x1D :
    return STUFFY_KEY_0;

  case 0x00 :
    return STUFFY_KEY_A;
  case 0x0B :
    return STUFFY_KEY_B;
  case 0x08 :
    return STUFFY_KEY_C;
  case 0x02 :
    return STUFFY_KEY_D;
  case 0x0E :
    return STUFFY_KEY_E;
  case 0x03 :
    return STUFFY_KEY_F;
  case 0x05 :
    return STUFFY_KEY_G;
  case 0x04 :
    return STUFFY_KEY_H;
  case 0x22 :
    return STUFFY_KEY_I;
  case 0x26 :
    return STUFFY_KEY_J;
  case 0x28 :
    return STUFFY_KEY_K;
  case 0x25 :
    return STUFFY_KEY_L;
  case 0x2E :
    return STUFFY_KEY_M;
  case 0x2D :
    return STUFFY_KEY_N;
  case 0x1F :
    return STUFFY_KEY_O;
  case 0x23 :
    return STUFFY_KEY_P;
  case 0x0C :
    return STUFFY_KEY_Q;
  case 0x0F :
    return STUFFY_KEY_R;
  case 0x01 :
    return STUFFY_KEY_S;
  case 0x11 :
    return STUFFY_KEY_T;
  case 0x20 :
    return STUFFY_KEY_U;
  case 0x09 :
    return STUFFY_KEY_V;
  case 0x0D :
    return STUFFY_KEY_W;
  case 0x07 :
    return STUFFY_KEY_X;
  case 0x10 :
    return STUFFY_KEY_Y;
  case 0x06 :
    return STUFFY_KEY_Z;

  case 0x27 :
    return STUFFY_KEY_APOSTROPHE;
  case 0x2A :
    return STUFFY_KEY_BACKSLASH;
  case 0x2B :
    return STUFFY_KEY_COMMA;
  case 0x18 :
    return STUFFY_KEY_EQUAL;  // Equal/Plus
  case 0x32 :
    return STUFFY_KEY_GRAVE;
  case 0x21 :
    return STUFFY_KEY_LBRACKET;
  case 0x1B :
    return STUFFY_KEY_MINUS;
  case 0x2F :
    return STUFFY_KEY_PERIOD;
  case 0x1E :
    return STUFFY_KEY_RBRACKET;
  case 0x29 :
    return STUFFY_KEY_SEMICOLON;
  case 0x2C :
    return STUFFY_KEY_SLASH;
  case 0x0A :
    return STUFFY_KEY_COUNT;  // ?

  case 0x33 :
    return STUFFY_KEY_BACKSPACE;
  case 0x39 :
    return STUFFY_KEY_CAPSLOCK;
  case 0x75 :
    return STUFFY_KEY_DELETE;
  case 0x7D :
    return STUFFY_KEY_DOWN;
  case 0x77 :
    return STUFFY_KEY_END;
  case 0x24 :
    return STUFFY_KEY_ENTER;
  case 0x35 :
    return STUFFY_KEY_ESCAPE;
  case 0x7A :
    return STUFFY_KEY_F1;
  case 0x78 :
    return STUFFY_KEY_F2;
  case 0x63 :
    return STUFFY_KEY_F3;
  case 0x76 :
    return STUFFY_KEY_F4;
  case 0x60 :
    return STUFFY_KEY_F5;
  case 0x61 :
    return STUFFY_KEY_F6;
  case 0x62 :
    return STUFFY_KEY_F7;
  case 0x64 :
    return STUFFY_KEY_F8;
  case 0x65 :
    return STUFFY_KEY_F9;
  case 0x6D :
    return STUFFY_KEY_F10;
  case 0x67 :
    return STUFFY_KEY_F11;
  case 0x6F :
    return STUFFY_KEY_F12;
  case 0x69 :
    return STUFFY_KEY_PRINT;
  case 0x6B :
    return STUFFY_KEY_F14;
  case 0x71 :
    return STUFFY_KEY_F15;
  case 0x6A :
    return STUFFY_KEY_F16;
  case 0x40 :
    return STUFFY_KEY_F17;
  case 0x4F :
    return STUFFY_KEY_F18;
  case 0x50 :
    return STUFFY_KEY_F19;
  case 0x5A :
    return STUFFY_KEY_F20;
  case 0x73 :
    return STUFFY_KEY_HOME;
  case 0x72 :
    return STUFFY_KEY_INSERT;
  case 0x7B :
    return STUFFY_KEY_LEFT;
  case 0x3A :
    return STUFFY_KEY_LALT;
  case 0x3B :
    return STUFFY_KEY_LCONTROL;
  case 0x38 :
    return STUFFY_KEY_LSHIFT;
  case 0x37 :
    return STUFFY_KEY_LSUPER;
  case 0x6E :
    return STUFFY_KEY_COUNT;  // Menu
  case 0x47 :
    return STUFFY_KEY_NUMLOCK;
  case 0x79 :
    return STUFFY_KEY_PAGEDOWN;
  case 0x74 :
    return STUFFY_KEY_PAGEUP;
  case 0x7C :
    return STUFFY_KEY_RIGHT;
  case 0x3C :
    return STUFFY_KEY_RSHIFT;
  case 0x36 :
    return STUFFY_KEY_RSUPER;
  case 0x31 :
    return STUFFY_KEY_SPACE;
  case 0x30 :
    return STUFFY_KEY_TAB;
  case 0x7E :
    return STUFFY_KEY_UP;

  case 0x45 :
    return STUFFY_KEY_ADD;
  case 0x41 :
    return STUFFY_KEY_DECIMAL;
  case 0x4B :
    return STUFFY_KEY_DIVIDE;
  case 0x4C :
    return STUFFY_KEY_ENTER;
  case 0x51 :
    return STUFFY_KEY_NUMPAD_EQUAL;
  case 0x43 :
    return STUFFY_KEY_MULTIPLY;
  case 0x4E :
    return STUFFY_KEY_SUBTRACT;

  default :
    return STUFFY_KEY_COUNT;
  }
}

@end

