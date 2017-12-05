//#define USE_FT_FONT 1
#define USE_GL_RENDER 1

#include "testdevices.hpp"
#include "diamonddevices.hpp"
#include "eodevices.hpp"

int main(int argc, const char * argv[]) {
//    mainDevice(1);
//    mainDiamondDevice(1);
    mainEODevice(1);
//    symmetricallyCoupledFilter();
    
//    auto start = std::chrono::steady_clock::now();
//
//    DEVICE* toReturn =  getDevice("main");
//
//    DEVICE* rect2 =      getDevice("rect");
//
//    CONNECTION c = CONNECTION(VECTOR(.5,2), VECTOR(0,1), .5, "top");
//
//    rect2->add(rect(VECTOR(0,0), VECTOR(1,2)));
//    rect2->add(c);
//
//    toReturn->add(rect2, AFFINE(TAU/8, -10,0), 'a');
//    toReturn->add(rect2, AFFINE(10,0), 'b');
//
//    toReturn->printConnectionNames();
//
//    connectThickenAndAdd(toReturn, toReturn->connections["topa"], toReturn->connections["topb"], CIRCULAR);
//
//    auto end = std::chrono::steady_clock::now();
//
//    printf("Devices were created in\n\t%f milliseconds.\n", std::chrono::duration <double, std::milli> (end - start).count());
//    printf("Devices have area %f um^2 = %f mm^2.\n", toReturn->area(), toReturn->area()/1e6);
//
//    toReturn->exportLibraryGDS("/Users/i/Desktop/Sandia/GDS/test.gds");

}

//#include <SFML/Graphics.hpp>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }
//
//    return 0;
//}


//
//// Link statically with GLEW
//#define GLEW_STATIC
//
//// Headers
//#include <OpenGL/OpenGL.h>
////#include <OpenGL/gl>
////#include <glew.h>
////#include <GL/glew.h>
//#include <SFML/Window.hpp>
////#include <SFML/>
//
//// Vertex shader
//const GLchar* vertexShaderSrc = R"glsl(
//#version 150 core
//
//in vec2 pos;
//in vec3 color;
//in float sides;
//
//out vec3 vColor;
//out float vSides;
//
//void main()
//{
//    gl_Position = vec4(pos, 0.0, 1.0);
//    vColor = color;
//    vSides = sides;
//}
//)glsl";
//
//// Geometry shader
//const GLchar* geometryShaderSrc = R"glsl(
//#version 150 core
//
//layout(points) in;
//layout(line_strip, max_vertices = 64) out;
//
//in vec3 vColor[];
//in float vSides[];
//out vec3 fColor;
//
//const float PI = 3.1415926;
//
//void main()
//{
//    fColor = vColor[0];
//    
//    // Safe, GLfloats can represent small integers exactly
//    for (int i = 0; i <= vSides[0]; i++) {
//        // Angle between each side in radians
//        float ang = PI * 2.0 / vSides[0] * i;
//        
//        // Offset from center of point (0.3 to accomodate for aspect ratio)
//        vec4 offset = vec4(cos(ang) * 0.3, -sin(ang) * 0.4, 0.0, 0.0);
//        gl_Position = gl_in[0].gl_Position + offset;
//        
//        EmitVertex();
//    }
//    
//    EndPrimitive();
//}
//)glsl";
//
//// Fragment shader
//const GLchar* fragmentShaderSrc = R"glsl(
//#version 150 core
//
//in vec3 fColor;
//
//out vec4 outColor;
//
//void main()
//{
//    outColor = vec4(fColor, 1.0);
//}
//)glsl";
//
//// Shader creation helper
//GLuint createShader(GLenum type, const GLchar* src) {
//    GLuint shader = glCreateShader(type);
//    glShaderSource(shader, 1, &src, nullptr);
//    glCompileShader(shader);
//    return shader;
//}
//
//int main()
//{
//    sf::ContextSettings settings;
//    settings.depthBits = 24;
//    settings.stencilBits = 8;
//    
//    sf::Window window(sf::VideoMode(800, 600, 32), "Circles", sf::Style::Titlebar | sf::Style::Close, settings);
//    
////    // Initialize GLEW
////    glewExperimental = GL_TRUE;
////    glewInit();
//    
//    // Compile and activate shaders
//    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSrc);
//    GLuint geometryShader = createShader(GL_GEOMETRY_SHADER, geometryShaderSrc);
//    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
//    
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, geometryShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    glUseProgram(shaderProgram);
//    
//    // Create VBO with point coordinates
//    GLuint vbo;
//    glGenBuffers(1, &vbo);
//    
//    GLfloat points[] = {
//        //  Coordinates     Color             Sides
//        -0.45f,  0.45f, 1.0f, 0.0f, 0.0f,  4.0f,
//        0.45f,  0.45f, 0.0f, 1.0f, 0.0f,  8.0f,
//        0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
//        -0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f
//    };
//    
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
//    
//    // Create VAO
//    GLuint vao;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//    
//    // Specify the layout of the vertex data
//    GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
//    glEnableVertexAttribArray(posAttrib);
//    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
//    
//    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
//    glEnableVertexAttribArray(colAttrib);
//    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
//    
//    GLint sidesAttrib = glGetAttribLocation(shaderProgram, "sides");
//    glEnableVertexAttribArray(sidesAttrib);
//    glVertexAttribPointer(sidesAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));
//    
//    bool running = true;
//    while (running)
//    {
//        sf::Event windowEvent;
//        while (window.pollEvent(windowEvent))
//        {
//            switch (windowEvent.type)
//            {
//                case sf::Event::Closed:
//                    running = false;
//                    break;
//            }
//        }
//        
//        // Clear the screen to black
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        
//        // Render frame
//        glDrawArrays(GL_POINTS, 0, 4);
//        
//        // Swap buffers
//        window.display();
//    }
//    
//    glDeleteProgram(shaderProgram);
//    glDeleteShader(fragmentShader);
//    glDeleteShader(geometryShader);
//    glDeleteShader(vertexShader);
//    
//    glDeleteBuffers(1, &vbo);
//    
//    glDeleteVertexArrays(1, &vao);
//    
//    window.close();
//    
//    return 0;
//}





