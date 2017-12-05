//#undef __WIN32__
//
//#ifdef __OBJC__
//#import <Cocoa/Cocoa.h>
//#endif
//
////#include "include.h"
////#include "window.h"
////#include "game.h"
//
////#include <SDL2/SDL.h>        //SDL
//
//#include <OpenGL/OpenGL.h>      //OpenGL
//#include <OpenGL/gl3.h>      //OpenGL
////    #include <OpenGL/gl.h>
//#include <OpenGL/glu.h>         //GLU
//
//#include "vector.hpp"
//#include "polyline.hpp"
//#include "primitives.hpp"
//
////#include <vector>
////#include <string>
////#include <iterator>
//
////class string4 {
////    char str[4];
////};
//
//int main(int argc, const char * argv[]) {
//    //    WINDOW window([[[NSBundle mainBundle] resourcePath] UTF8String]);
//    //    if (window.initiate() < 1){ return 0; }
//    //
//    //    GAME game(&window);
//    
//    VECTOR v = VECTOR(1,2);
//    VECTOR w = VECTOR(3,4);
//    
//    v.printNL();
//    w.printNL();
//    
//    printf("%.2f\n", v * w);
//    printf("%.2f\n", v.unit() * w.unit());
//    
////    std::string str = "fish";
////    printf("%s", str.c_str());
////    
////    std::string str2 = str;
////    printf("%s", str2.c_str());
//    
//    printf("sizeof(GLdouble) = %lu\n", sizeof(GLdouble));
//    printf("sizeof(VECTOR)   = %lu\n", sizeof(VECTOR));
//    
//    POLYLINE rectangle = rect(v, w);
//    rectangle.print();
//    printf("Area = %.2f\n", rectangle.area());
//    
////    long l = 101231231242342342;
////    printf("LONG: %li\n", l);
////    
////    char* str = new char(sizeof(long)/sizeof(char));
////    *str = l;
//    
////    printf("LONG: %s\n", ((string4)l).str );
////    printf("LONG: %s\n", str);
//    
//    
//    return NSApplicationMain(argc,  (const char **) argv);
////
////    for (int i = 0; i < sizeof(unsigned int)*8; i++){
////        
////    }
//    
//    //    window.
//    
//    //    printf("HERE");
//    
//    //glVertex2d(1, 1);
//    
//    //    wxEntry(argc, (char**)argv);
//    
//    //    while (true){
//    //        game.tick();
//    //    }
//    
////    return 0;
//}


// Link statically with GLEW
#define GLEW_STATIC

// Headers
#include <OpenGL/OpenGL.h>
//#include <OpenGL/gl>
//#include <glew.h>
//#include <GL/glew.h>
#include <SFML/Window.hpp>

// Vertex shader
const GLchar* vertexShaderSrc = R"glsl(
#version 150 core

in vec2 pos;
in vec3 color;
in float sides;

out vec3 vColor;
out float vSides;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
    vColor = color;
    vSides = sides;
}
)glsl";

// Geometry shader
const GLchar* geometryShaderSrc = R"glsl(
#version 150 core

layout(points) in;
layout(line_strip, max_vertices = 64) out;

in vec3 vColor[];
in float vSides[];
out vec3 fColor;

const float PI = 3.1415926;

void main()
{
    fColor = vColor[0];
    
    // Safe, GLfloats can represent small integers exactly
    for (int i = 0; i <= vSides[0]; i++) {
        // Angle between each side in radians
        float ang = PI * 2.0 / vSides[0] * i;
        
        // Offset from center of point (0.3 to accomodate for aspect ratio)
        vec4 offset = vec4(cos(ang) * 0.3, -sin(ang) * 0.4, 0.0, 0.0);
        gl_Position = gl_in[0].gl_Position + offset;
        
        EmitVertex();
    }
    
    EndPrimitive();
}
)glsl";

// Fragment shader
const GLchar* fragmentShaderSrc = R"glsl(
#version 150 core

in vec3 fColor;

out vec4 outColor;

void main()
{
    outColor = vec4(fColor, 1.0);
}
)glsl";

// Shader creation helper
GLuint createShader(GLenum type, const GLchar* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    
    sf::Window window(sf::VideoMode(800, 600, 32), "Circles", sf::Style::Titlebar | sf::Style::Close, settings);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    // Compile and activate shaders
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint geometryShader = createShader(GL_GEOMETRY_SHADER, geometryShaderSrc);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    // Create VBO with point coordinates
    GLuint vbo;
    glGenBuffers(1, &vbo);
    
    GLfloat points[] = {
        //  Coordinates     Color             Sides
        -0.45f,  0.45f, 1.0f, 0.0f, 0.0f,  4.0f,
        0.45f,  0.45f, 0.0f, 1.0f, 0.0f,  8.0f,
        0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
        -0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
    // Create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    
    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    
    GLint sidesAttrib = glGetAttribLocation(shaderProgram, "sides");
    glEnableVertexAttribArray(sidesAttrib);
    glVertexAttribPointer(sidesAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));
    
    bool running = true;
    while (running)
    {
        sf::Event windowEvent;
        while (window.pollEvent(windowEvent))
        {
            switch (windowEvent.type)
            {
                case sf::Event::Closed:
                    running = false;
                    break;
            }
        }
        
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Render frame
        glDrawArrays(GL_POINTS, 0, 4);
        
        // Swap buffers
        window.display();
    }
    
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);
    glDeleteShader(vertexShader);
    
    glDeleteBuffers(1, &vbo);
    
    glDeleteVertexArrays(1, &vao);
    
    window.close();
    
    return 0;
}
