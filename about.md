Learning graphics programming - implementing it into an engine
Also learning opengl
And all things included
(WIP!)

using resources like chern0, scratchapixel, learnopengl, etc.

OpenGL works as a state machine - tell what to do uses state

1. to build a window context
   * init glfw
   * create window
   * make the window the current context
2. to init opengl funcs
   * init glew - to work with gpu
3. check window context still valid in loop
   * clear buffer
   * swap buffers
   * poll for events
4. terminate the window context

5. Vertex buffer
   * A memory buffer - array of bytes of memory
   * Is in the GPU
   * Define a bunch of data that represents a triangle - put it into the vram 0 issue a draw call
   * Once you issue the draw call - layout the data in specified way and draw
   * send data from CPU to GPU
   
6. Vertex attributes
   * tells OpenGl the layout of memory
   * 12 bytes 3 floats - 4 bytes per float -> position
   * 8 bytes -> texture coords

7. shaders
   * a program (code) that works on the GPU works with the vertex buffer
   * shader must match vertex attributes
   * Utilise the GPU to render graphics on the screen
   * pos, texture, lighting, etc
   * Vertex shaders
     * data -> pos etc -> gpu -> draw call -> vertex shader -> fragment shader -> result
     * code of vertex shader gets called for all vertices
     * tell opengl where you want the vertex (pos) to be in the screen space
     * gives the data to the fragment shader
     * called 3 times - called for each vertex
     * can pass data from the vertex to the fragment
   * tesselation shaders -
   * compute shaders -
   * fragment shaders - pixel shader
     * run once for each pixel that gets rasterized
     * decide which color the pixel is supposed to be
     * so the pixel can be shaded in the right colour
     
8. writing shaders

9. ParseShader function
    parses the shader file for fragment or vertex - when found keeps processing it
    basically parses the info needed fo each type
    * takes the file path
    * opens an input file stream
    * checks which shader type w/ enum
    * takes a line which it uses to read line by line
    * takes strinstream with 0 for vertex 1 for frag
    * define shader type none
   * read every line until #shader
     * continues until vertex or fragment
       * sets the mode accordingly
     * otherwise keep readding

10. Index Buffers
   * allows for the reuse of existing vertices
   * put on the cpu with arr - then give them to the gpu to use
   * should be used for pretty much everything

11. dealing with errors in OpenGL
    * glgeterror - whenever func call if error occur a flag is set - gives back the flag of error
      * call in a loop to get all errors
      * debugbreak - allows to stop on the line with the problems
      * using macro to asser if error is true or nah
      * macro to call errors handle them and clear them
        * error, function, line number

    * gldebugmessagecallback - specify func pointer to opengl - opengl calls when error
    
12. uniforms
    * a way to get data from the cpu into the shader
    * set per draw - can set it up before drawing
    * 

- attributes are per vertex but same concept

13. vertex arrays
    * A way to bind vertex buffers with a certain specification for layout of that vertex buffer
    * for multiple objects
    * contains state - of objects

- if using core - must make vao
- either global vao or separate vao for each unique geometry
- 

- doesn't really exist in other graphics libs

- tips
    * use c++ types - important when using multiple API's etc
    * both vertex and fragment shaders in same file and parse them for info respectively
    * if vert and frag in separate files -> parse separately and use
    * all buffers must be unsigned - (unsigned int)

- VBO - vertex buffer data
- IBO - index buffer data
- VAO - layout holder
- 

    * GL_DRAW_ELEMENTS
      * will draw any indices - including batch indices
      * this way you can set up your scene - singular or batch rendering
      * This is how you make for example a level or scene
