#ifndef PGDSNSOPENGLVIEW_H
#define PGDSNSOPENGLVIEW_H

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

#import <OpenGL/OpenGL.h>
//#import <OpenGL/gl3.h>
//#import <OpenGL/glu.h>
//#import <CoreVideo/CVDisplayLink.h>

#include "polyline.hpp"
#include "primitives.hpp"
#include "window.hpp"

@interface pgdsNSOpenGLView : NSOpenGLView

// Inherits:
//  NSWindow*               window;
//  NSOpenGLContext*        openGLContext
//  NSOpenGLPixelFormat*    pixelFormat;

@property(nonatomic) WINDOW* window2;

- (void)drawRect:(NSRect)dirtyRect;

- (void)prepareOpenGL;

- (void)reshape;
- (void)update;

- (void)mouseDown:      (NSEvent*)event;
- (void)mouseMoved:     (NSEvent*)event;
- (void)mouseUp:        (NSEvent*)event;

- (void)rightMouseDown: (NSEvent*)event;
- (void)rightMouseMoved:(NSEvent*)event;
- (void)rightMouseUp:   (NSEvent*)event;

- (void)scrollWheel:    (NSEvent*)event;
@end

#endif
