#include "window.hpp"

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }
    
    
    
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    
    
    
    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
    
    
    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}

WINDOW::WINDOW() {
    screen = BOUNDINGBOX(VECTOR(0,0), VECTOR(XRES, YRES));
    rotation = 0;
    
    bb = BOUNDINGBOX(VECTOR(-1.5,-1), VECTOR(1.5, 1));
    
    focus = nullptr;
    
    init();
}

int WINDOW::init() {
    if(!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    
    
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    
    // Open a window and create its OpenGL context
     // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow( XRES, YRES, "pGDS", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    glfwSwapInterval(1);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
//    static const GLfloat g_vertex_buffer_data[] = {
//        -1.0f, -1.0f,
//        1.0f, -1.0f,
//        0.0f,  1.0f,
//    };
//
////    glColor3ub(0, 255, 255);
//
////    GLuint vertexbuffer;
//    glGenBuffers(1, &vertexbuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    MATERIAL::shaders = LoadShaders("/Users/i/Documents/pGDS/test.vert", "/Users/i/Documents/pGDS/test.frag");
    
//    shaders = LoadShaders("/Users/i/Documents/pGDS/test.vert", "/Users/i/Documents/pGDS/test.frag");
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
//    int tick = 0;
    
//    printf("JOYSTICK: %i", glfwJoystickPresent(GLFW_JOYSTICK_1));
    
//    auto func = [](GLFWwindow* w, double, double)
//    {
//        static_cast<WINDOW*>(glfwGetWindowUserPointer(w))->mouseButtonPressed( /* ... */ );
//    }
    
//    HackFullContentView(window);
    
    glfwSetWindowUserPointer(window, this);
    
    glfwSetScrollCallback(window, scroll);
    glfwSetFramebufferSizeCallback(window, frame);
    getScreen();
//    glfwSetMouseButtonCallback(window, mousebutton);
    
    return 0;
}

void WINDOW::getScreen() {
    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
//    int left, top, right, bottom;
//    glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
    
//    printf("[ %i, %i ] - [ %i, %i ]\n", left, bottom, right, top);
    
    screen = BOUNDINGBOX(xpos, xpos + width, ypos + height, ypos);
}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
    WINDOW* w = (WINDOW*)glfwGetWindowUserPointer(window);
    
    bool zoom = glfwGetKey(window, GLFW_KEY_LEFT_ALT) || glfwGetKey(window, GLFW_KEY_RIGHT_ALT);
    
    if (zoom) {
        VECTOR screenCoords;
        glfwGetCursorPos(window, &screenCoords.x, &screenCoords.y);
        
//        screenCoords.printNL();
        screenCoords.y = w->screen.height() - screenCoords.y;
        
//        VECTOR realCoords = AFFINE(w->screen, w->bb) * screenCoords;
        VECTOR realCoords = w->bb.center();
        
//        realCoords.printNL();
//        w->screen.print();
//        w->bb.print();
//        (AFFINE(-realCoords) * w->bb).print();
//        (AFFINE( realCoords) * w->bb).print();
//        printf("\n");
        
        GLdouble scale = (1 + max(-.5, -.1*yoffset*.25));
        
        w->bb = AFFINE(realCoords / scale) * ( (AFFINE(-realCoords) * w->bb) * scale );
    } else {
//        GLdouble mult = w->bb.width()/w->screen.width();
//
//        printf("WID=%f, MULT=%f\n", w->bb.width(), mult);
//        printf("WID=%f, MULT=%f\n", mult);
        
        GLdouble mult = 0.03;
        
        VECTOR dv = VECTOR(xoffset * mult, -yoffset * mult);
        
//        dv.printNL();
        
        w->bb.ll += dv;
        w->bb.ur += dv;
        
//        w->bb.print();
    }
}

void frame(GLFWwindow* window, int width, int height) {
    WINDOW* w = (WINDOW*)glfwGetWindowUserPointer(window);
//    BOUNDINGBOX screen = BOUNDINGBOX(VECTOR(), VECTOR(width, height));
    
    BOUNDINGBOX oldScreen = w->screen;
    printf("oldScreen = ");
    oldScreen.print();
    
    w->getScreen();
    
    BOUNDINGBOX newScreen = w->screen;
    printf("newScreen = ");
    newScreen.print();
    
    printf("w->bb before = ");
    w->bb.print();
    
    GLdouble cameraWidth = w->bb.width() * (newScreen.width() / oldScreen.width());
    GLdouble cameraHeight = cameraWidth * (newScreen.height() / newScreen.width());
    
    w->bb = BOUNDINGBOX(w->bb.center(), cameraWidth, cameraHeight);
    
//    printf("AFFINE = \n");
//    AFFINE(oldScreen, newScreen).print();
//
//    AFFINE m = AFFINE(oldScreen, newScreen);
//    m.e = 0;
//    m.f = 0;
//
//    w->bb = m * w->bb;
    
    printf("w->bb after = ");
    w->bb.print();
}

void WINDOW::loop() {
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(5.0f);
    
    POLYLINE c = circle(2);
    
    glfwWaitEventsTimeout(0.1);
    
    do{
        //        tick++;
        //        glClearColor(0, (rand() % 100)/200., 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
//        if (glfwGetMouseButton(window, 1)) { bb *= .99; }
//        if (glfwGetMouseButton(window, 0)) { bb /= .99; }
        
//        if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT)) { bb = BOUNDINGBOX(VECTOR(-1.5,-1), VECTOR(1.5, 1)); }
        if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT)) { bb = focus.device->bb; }
        
        GLfloat scalex = 2./bb.width();
        GLfloat scaley = 2./bb.height();
        
        const GLfloat m[] = {
            scalex,         0.0f,           0.0f,   0.0f,
            0.0f,           scaley,         0.0f,   0.0f,
            0.0f,           0.0f,           1.0f,   0.0f,
            (GLfloat)bb.center().x,  (GLfloat)bb.center().y,  0.0f,   1.0f
        };
        
        AFFINE camera = AFFINE(scalex, 0, 0, scaley, bb.center().x, bb.center().y);
        
//        camera.print();
        
//        const GLfloat red[] =     { 1.0, 0.0, 0.0 };
//        const GLfloat green[] =   { 0.0, 1.0, 0.0 };
        
//        GLuint colorID = glGetUniformLocation(shaders, "c");
//        glUniform4f(colorID, 1, 0, 0, 1);
//
        GLuint MatrixID = glGetUniformLocation(MATERIAL::shaders, "m");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, m);
//
//        glUseProgram(shaders);
        
//        focus->polylines[0].outline();
//        focus->polylines[0].fillOutline();
//        focus->polylines[0].print();
        
        focus.render(camera, false);
        
//        c.fillOutline();
        
//        focus->render(shaders);
        
//        glEnableVertexAttribArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glDisableVertexAttribArray(0);
//
//        {
//            GLuint colorID = glGetUniformLocation(shaders, "c");
//            glUniform4f(colorID, 1, 1, 1, .5);
//
//            GLuint MatrixID = glGetUniformLocation(shaders, "m");
//            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, m);
//
//            glUseProgram(shaders);
//
////            focus->polylines[0].outline();
//
//            glEnableVertexAttribArray(0);
//            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
//            glDrawArrays(GL_LINE_LOOP, 0, 3);
//            glDisableVertexAttribArray(0);
//        }
        
        glfwSwapBuffers(window);
        glfwWaitEvents();
        
    } while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
}



