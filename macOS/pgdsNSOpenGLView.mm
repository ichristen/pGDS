#ifndef PGDSNSOPENGLVIEW_H
#define PGDSNSOPENGLVIEW_H
//
//#include <GLFW/glfw3.h>
//#define GLFW_EXPOSE_NATIVE_COCOA
//#include <GLFW/glfw3native.h>
//
////#import <objc/runtime.h>
//
//// Dummy class for supplying a fake mouseDownCanMoveWindow selector.
//@interface FakeView : NSView
//@end
//@implementation FakeView
//- (BOOL)fakeMouseDownCanMoveWindow { return YES; }
//@end
//
//void HackFullContentView(GLFWwindow* window)
//{
//    // For each new window we open, the following code needs to be run.
//    NSWindow* wnd = glfwGetCocoaWindow(window);
//    wnd.titlebarAppearsTransparent = YES;
//    wnd.titleVisibility = NSWindowTitleHidden;
//    wnd.backgroundColor = NSColor.blackColor;
//    
//    // We only need override/swizzle the selector once.
//    static bool run_once = true;
//    if (run_once)
//    {
//        run_once = false;
//        NSView* glView = [wnd contentView];
//        Method originalMethod = class_getInstanceMethod([glView class], @selector(mouseDownCanMoveWindow));
//        Method categoryMethod = class_getInstanceMethod(FakeView.class, @selector(fakeMouseDownCanMoveWindow));
//        method_exchangeImplementations(originalMethod, categoryMethod);
//    }
//}

//#include "window.hpp"
//
//#import <Cocoa/Cocoa.h>
//#import <Foundation/Foundation.h>
//
//#import <OpenGL/OpenGL.h>
////#import <OpenGL/gl3.h>
////#import <OpenGL/glu.h>
////#import <CoreVideo/CVDisplayLink.h>
//#include "polyline.hpp"
//#include "primitives.hpp"
//
//@interface pgdsNSOpenGLView : NSOpenGLView
//
//// Inherits:
////  NSWindow*               window;
////  NSOpenGLContext*        openGLContext
////  NSOpenGLPixelFormat*    pixelFormat;
//
//@property(nonatomic) WINDOW* window2;
//
//- (void)drawRect:(NSRect)dirtyRect;
//
//- (void)prepareOpenGL;
//
//- (void)reshape;
//- (void)update;
//
//- (void)mouseDown:      (NSEvent*)event;
//- (void)mouseMoved:     (NSEvent*)event;
//- (void)mouseUp:        (NSEvent*)event;
//
//- (void)rightMouseDown: (NSEvent*)event;
//- (void)rightMouseMoved:(NSEvent*)event;
//- (void)rightMouseUp:   (NSEvent*)event;
//
//- (void)scrollWheel:    (NSEvent*)event;
//@end

#endif
