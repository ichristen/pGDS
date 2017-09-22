#import "pgdsNSOpenGLView.h"

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif

@implementation pgdsNSOpenGLView


- (void)drawRect:(NSRect)dirtyRect {
    [[self openGLContext] makeCurrentContext];
    
//    [self window2]->render();
    
    //    CGSize size = [self window].frame.size;
    
    //    BOOL setCtx = [NSOpenGLContext currentContext] != glContext;
    //
    //    [glContext update];
    //
//    glViewport( 0, 0, 100, 100); //(GLint) size.width, (GLint) size.height );
//    
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity();
//    
//    glOrtho(0, 10, 10, 0, 10, -10); // ( 40.0, size.width / size.height, 1.0f, 1000.0f );
//    
//    glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
//    glLoadIdentity();                           // Reset The Modelview Matrix
//    
//    glClearColor(0.0, 0.0, 0.0, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT);
//    
//    //    glBegin(GL_TRIANGLES);
//    //
//    //    glEnd();
//    
//    
//    glLineWidth(2.5);
//    
//    glColor3ub(255, 255, 255);
//    
//    VECTOR v = VECTOR(1,2);
//    VECTOR w = VECTOR(3,4);
//    
//    POLYLINE rectangle = rect(v, w);
//    rectangle.print();
////    rectangle.outline();
//    
//    glColor3f(1.0, 0.0, 0.0);
//    glBegin(GL_LINES);
//    glVertex3f(0.0, 0.0, 0.0);
//    glVertex3f(15, 0, 0);
//    glEnd();
//    
//    glColor3ub(0, 0, 255);
//    glBegin(GL_LINE);
//    glVertex2d(0, 0);
//    glVertex2d(200, 200);
//    glEnd();
//    
//    glBegin(GL_TRIANGLE_STRIP);
//    glVertex2f(100, -100);
//    glVertex2f(-100, -100);
//    glVertex2f(-100, 100);
//    glVertex2f(100, 100);
//    glEnd();
//    
//    glBegin(GL_TRIANGLE_STRIP);
//    glVertex2f(-100, 100);
//    glVertex2f(-100, -100);
//    glVertex2f(100, -100);
//    glVertex2f(100, 100);
//    glEnd();
    
    //    glFlush();
    
    [[self openGLContext] flushBuffer];
}

- (void)prepareOpenGL {
    
}

- (void)reshape {
    [self window2]->width =     [self bounds].size.width;
    [self window2]->height =    [self bounds].size.height;
}
- (void)update {
//    [self window2]->render();
}

- (void)mouseDown:      (NSEvent*)event {
    
}
- (void)mouseMoved:     (NSEvent*)event {
    [event allTouches];
}
- (void)mouseUp:        (NSEvent*)event {
    
}

- (void)rightMouseDown: (NSEvent*)event {
    
}
- (void)rightMouseMoved:(NSEvent*)event {
    
}
- (void)rightMouseUp:   (NSEvent*)event {
    
}

- (void)scrollWheel:    (NSEvent*)event {
//    [self window2]->scale;
}

@end
