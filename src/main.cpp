#include <GL/glew.h> // first - handles loading extensions and includes the OpenGL headers
#include <GLFW/glfw3.h> // second

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// macro for assertion
#define ASSERT(x) if (!(x)) __debugbreak();
// macro for logging and clearing errors in opengl errors
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

////
// clearing all OpenGL errors
static void GLClearError()
{
    // clear all the errors
    while (glGetError() != GL_NO_ERROR);
}

////
// catching all errors
static bool GLLogCall(const char* function, const char* file, int line)
{
    // while errors exist
    while (GLenum error = glGetError())
    {
        std::cout << "ALL ERRORS: " << std::endl;
        std::cout << "OpenGL Error: " << error << ", Function: " << function << ", " << "Line number: " << line << std::endl;
        return false;
    }

    return true;
}

// allowing returning 2 strings by use fo struct
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

////
//// parse the vertex and fragment shaders
static ShaderProgramSource ParseShader(const std::string& filePath)
{
    // open the file
    std::ifstream stream(filePath);

    // check mode
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];    // one for vertex one for fragment
    ShaderType type = ShaderType::NONE; // default shader type is none

    // go through the file and decide which shader it is
    while (getline(stream, line)) {
        //more lines to read
        // if it finds shader
        if (line.find("#shader") != std::string::npos) {
            // if found vertex shader
            if (line.find("vertex") != std::string::npos) {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
                // if found fragment shader
            else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        } else if (type != ShaderType::NONE) {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

////
//// compiling the shader
// type - type of shader to make - vertex, fragment
// source - the actual shader information
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // id of shader is the type of shader wanted
    GLCall(unsigned int id = glCreateShader(type));
    // return pointer cant be changed - pointer to beginning of data
    const char* src = source.c_str();
    // specifies th source of the shader
    GLCall(glShaderSource(id, 1, &src, nullptr));
    // compile the shader

    GLCall(glCompileShader(id));

    // error handling
    int result;
    // get info about shader
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    // check if it compiles or nah
    if (result == GL_FALSE)
    {
        // get error message
        int length;
        // get shader info
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        // alloca allows to alloc on the stack dynamically
        char* message = static_cast<char *>(alloca(length * sizeof(char)));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        // tell which shader didn't compile - can be extended for other shaders
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

////
//// creating a shader
//  take the shaders to create
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // provide both shaders - provide opengl with a string/can be a file
    //
    GLCall(unsigned int program = glCreateProgram());
    // create 2 shader objects
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attach shaders to program
    // vertex shader
    GLCall(glAttachShader(program, vs));
    // fragment shader
    GLCall(glAttachShader(program, fs));
    // link program
    GLCall(glLinkProgram(program));
    // validate program
    GLCall(glValidateProgram(program));

    // delete intermediates - because they stored in the program now
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

////
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
    float positions[] = {
        // each line a vertex - can contain more than position
        // vertex - the whole blob of data that makes that vertex - pos, tex coords, normals etc.
        // x  -  y
        // unique vertices only - use index buffer - no duplicate vertices
        -0.5f, -0.5f,    // 0
         0.5f, -0.5f,    // 1
         0.5f, 0.5f,     // 2
        -0.5f, 0.5f,     // 3
    };

    // index into the vertex buffer - index buffer
    unsigned int indices[] = {
        // using the index buffer allows for drawing vertices multiple times - better for memory
        0,1,2,    // first triangle using index buffer
        2,3,0,    // second tringle using index buffer
    };

    ////
    //// vertex buffer things
    unsigned int buffer;
    // define a vertex buffer using the memory pool (buffer)
    GLCall(glGenBuffers(1, &buffer));
    // now we have an ID (buffer)
    // selecting the buffer - (buffer) - this is the state and what will be drawn
    // buffer of memory - gl_array_buffer
    // id of buffer (buffer)
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    //  index of the vert index to be enabled - enable the buffer
    // specify the data for the buffer - 6 vertices
    // array buffer
    // size of data - in bytes
    // copying positions into the buffer (* to positions)
    // static draw - draw something from buffer update every frame
    // send to gpu
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    ////
    //// setting up vertex attr and layout
    GLCall(glEnableVertexAttribArray(0));
    // tell opengl what the actual layout of the buffer is
    // index - what index attribute at in buffer
    // size - count of floats - 2 floats per vertex (atm)
    // normalized - to be between 0 and 1 from like 0-255
    // stride - amount of bytes between each vertex - use macro to automate (offsetof)
    /// pointer - pointer into the actual attribute (amount of bytes between actual attributes) - use (offsetoff)
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

    ////
    // index buffer
    // (ibo) index buffer object
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    // send data to gpu
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ////
    // read the shader file in
    ShaderProgramSource source = ParseShader("../res/shaders/Basic.shader");
    // vertex
    // ShaderProgramSource sourceVert = ParseShader("../res/shaders/bbyblue.vert");
    // ShaderProgramSource sourceFrag = ParseShader("../res/shaders/bbyblue.frag");

    std::cout << "Vertex shader loaded: " << !source.VertexSource.empty() << std::endl;
    std::cout << "Fragment shader loaded: " << !source.FragmentSource.empty() << std::endl;
    std::cout << "Vertex source:\n" << source.VertexSource << std::endl;
    std::cout << "Fragments source:\n" << source.FragmentSource << std::endl;

    // make shader by creating process
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    // bind shader
    GLCall(glUseProgram(shader));

    ////
    while (!glfwWindowShouldClose(window))
    {
        // clear buffers to preset values
        // indicates the buffers currently enabled for color writing
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // issue a draw call for the buffer
        // GL_DRAW_ARRAYS - if you do not have an index buffer
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // GL_DRAW_ELEMENTS - if you have an index buffer
        // glcall is checking for opengl errors and asserting whether there is error or nah
        // its also giving information about the error
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        // swaps the front and back biffers of a specified window
        glfwSwapBuffers(window);

        // process the events that are already in the queue
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));
    // destroys all remaining windows and cursors - restores anything modified or allocated
    glfwTerminate();
    return 0;
}
