#include <GL/glew.h> // first - handles loading extensions and includes the OpenGL headers
#include <GLFW/glfw3.h> // second

#include <iostream>

//// writing the vertex shader - pos of indices
    const std::string vertexShader =
        // gsl
        "#version 330 core\n"
        "\n"
        // index of the attribute
        // needs to be a vec4 in future
        "layout(location = 0)   in vec4 position;\n"
        "\n"
        // main func for shader
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

// writing the fragment shader
// deciding what colour the pixels should be
const std::string fragmentShader =
    // gsl
    "#version 330 core\n"
    "\n"
    // outputting a color
    // needs to be a vec4 in future
    "layout(location = 0)   out vec4 color;\n"
    "\n"
    // main func for shader
    "void main()\n"
    "{\n"
        // try baby blue - non hdr
    "   color = vec4(0.635, 0.816, 0.996, 1.0);\n"
    "}\n";

//// compiling the shader
// type - type of shader to make - vertex, fragment
// source - the actual shader information
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // id of shader is the type of shader wanted
    unsigned int id = glCreateShader(type);
    // return pointer cant be changed - pointer to beginning of data
    const char* src = source.c_str();
    // specifies th source of the shader
    glShaderSource(id, 1, &src, nullptr);
    // compile the shader
    glCompileShader(id);

    // error handling
    int result;
    // get info about shader
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    // check if it compiles or nah
    if (result == GL_FALSE)
    {
        // get error message
        int length;
        // get shader info
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // alloca allows to alloc on the stack dynamically
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        // tell which shader didn't compile - can be extended for other shaders
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

//// creating a shader
//  take the shaders to create
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // provide both shaders - provide opengl with a string/can be a file
    //
    unsigned int program = glCreateProgram();
    // create 2 shader objects
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attach shaders to program
    // vertex shader
    glAttachShader(program, vs);
    // fragment shader
    glAttachShader(program, fs);
    // link program
    glLinkProgram(program);
    // validate program
    glValidateProgram(program);

    // delete intermediates - because they stored in the program now
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main()
{
    // init GLFW before to get OPENGL context
    if (!glfwInit())
        return -1;

    // Creates a window and its associated OpenGL context - uses window hits too
    GLFWwindow* window = glfwCreateWindow(640, 480, "Window", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // makes the OpenGL context of the specified window current on the calling thread
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);    // vsync 60hz etc

    // check if GLEW working after GLFW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
    }
    // if working show GPU information
    std::cout << glGetString(GL_VERSION) << std::endl;

    // data for buffer - the indices of the triangle
    float positions[6] = {
        // each line a vertex - can contain more than position
        // vertex - the whole blob of data that makes that vertex - pos, tex coords, normals etc.
        // x  -  y
        -0.5f, -0.5f,
         0.0f, 0.5f,
         0.5f, -0.5f
    };

    //// vertex buffer things
    unsigned int buffer;
    // define a vertex buffer using the memory pool (buffer)
    glGenBuffers(1, &buffer);
    // now we have an ID (buffer)
    // selecting the buffer - (buffer) - this is the state and what will be drawn
    // buffer of memory - gl_array_buffer
    // id of buffer (buffer)
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //  index of the vert index to be enabled - enable the buffer
    // specify the data for the buffer
    // array buffer
    // size of data - in bytes
    // copying positions into the buffer (* to positions)
    // static draw - draw something from buffer update every frame
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    //// setting up vertex attr and layout
    glEnableVertexAttribArray(0);
    // tell opengl what the actual layout of the buffer is
    // index - what index attribute at in buffer
    // size - count of floats - 2 floats per vertex (atm)
    // normalized - to be between 0 and 1 from like 0-255
    // stride - amount of bytes between each vertex - use macro to automate (offsetof)
    /// pointer - pointer into the actual attribute (amout of bytes betyween actual attributes) - use (offsetoff)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    // make shader by creating process
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    // bind shader
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))
    {
        // clear buffers to preset values
        // indicates the buffers currently enabled for color writing
        glClear(GL_COLOR_BUFFER_BIT);

        // issue a draw call for the buffer
        // GL_DRAW_ARRAYS - if you do not have an index buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // GL_DRAW_ELEMENTS - if you have an index buffer
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        // swaps the front and back biffers of a specified window
        glfwSwapBuffers(window);

        // process the events that are already in the queue
        glfwPollEvents();
    }

    // destroys all remaining windows and cursors - restores anything modified or allocated
    glfwTerminate();
    return 0;
}
