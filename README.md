This project is meant to follow the tutorial based from [learnopengl.com](learnopengl.com)

### Understanding the different libraries used by this project 

**OpenGL** is an API. it defines some functions and effects. Each vendor (NVIDIA, AMD, Intel, ...) implements those functions inside their drivers. Since the locations of the header and .so files are different for different OSs and vendors, you would need to create a lot of function pointers at runtime to be able to use those functions in your code. 

>[!NOTE]
> If you want a better understanding of how the OpenGl specification works, you can check out the [wikipedia article about it](https://en.wikipedia.org/wiki/OpenGL)\
> The *original* open gl header (in my case defined by the mesa3d graphics driver), is located in the path  `/usr/include/GL/gl.h`

The library **GLAD** appears to solve this problem. Basically, it dinamically creates the function pointers for you and make them available when you include the libraryusing the `#include <glad/glad.h>`. This is the reason, by the way, why you dont have to explicitly include the library gl.h.

> [!NOTE] 
> To understand more about glad and Loading Libraries, refer to the OpenGL [wiki](https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library)

The last library in the recipe is *GLEW*. Glew is basically an extension library to OpenGL. It defines some functions that help your use of the library functions, like creating windows, detecting input and others. This [stack exchange post answer](https://softwareengineering.stackexchange.com/a/345211) has a great explanation:

>*"Different platforms (such as Windows, Linux/X, OSX, iOS, Android) provide different mechanisms for creating contexts. For example Windows has its own function calls that allow the creation of OpenGL contexts that are very different from how OSX creates a context. Additionally Windows provides functions to support things like mouse events. This is where GLFW comes in: it provides a cross-platform library to handle the things that OpenGL doesn't handle, like creating contexts and handling mouse events. By and large it's not a wrapper library on top of OpenGL though it does provide some a small amount of functionality like this: like automatically building mip-map levels."*

### Things I haven`t undestood yet:

- I have zero idea why the -lglfw works but the -lglfw3 dont work on my machine. Even tough I`m farely sure I dont have the glfw (without 3) installed.
