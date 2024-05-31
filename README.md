This is my attempt to learn the basics of OpenGL and getting started into graphics programming. For that, I`ll initially follow the path from [learnopengl.com](learnopengl.com) and as I get comfortable with the library and its capabilities I expect to explore more

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

### Some notes on binding 

So, about vertex arrays and vertex buffers. Basically, there are Buffer Objects (BO), which are just a general buffer, but defined inside the openGL contex. When you run glCreateBuffers(), you receive a  references one or more of those buffers. You can`t tough, manipulate a BO by itself. For that, you have to first bind it to a target (or, how I prefer to think about it, a bind point). 
You can think of a bind point like a linux mount point. When you bind a buffer to a bind point, you can then use the bind point to write data to the buffer using glBuffersData, specify the structure of the data inside the buffer and divide it into different attributes, using glVertexAttribPointer, and enable or disable certain attributes (similarly, when you mount a volume to a mount point, you can read, write or change the data inside of it through the mount point ).
Just to reinforce one more time. You can acess the buffer by itself, but you can bind it to a bind point and modify it through the bind point
Here\'s an example of the process of creating, storing data and defining the structure of a single  attribute inside the buffer:

```
    int buffer_id;
	glGenBuffers(1, (GLuint*)&bufferId);

    //Bind the buffer object to the bind point GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id); 
    
    //Stores the data inside the buffer currently bound to GL_ARRAY_BUFFER bind point
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData),vertexData,GL_STATIC_DRAW);
    
    //Specify the organization logic of a specific attribute
    //inside the buffer currently bound to GL_ARRAY_BUFFER bind point
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    //Enable attribute on bind point
	glEnableVertexAttribArray(0);
```

The next thing which confused me is the Vertex Array Objects (VAOs). By definition, VAOs are:

>*A Vertex Array Object (VAO) is an OpenGL Object that stores all of the state needed to supply vertex data (with one minor exception noted below). It stores the format of the vertex data as well as the Buffer Objects (see below) providing the vertex data arrays. Note that a VAO merely references the buffers, it does not copy or freeze their contents; if referenced buffers are modified later, those changes will be seen when using the VAO.*

So, basically, a VAO stores a reference to a buffer object and all the configuration done to it, so you dont need to reconfigure it everytime you use it.
VAO objects work in a similar way as buffer objects. You first get a reference and bind that reference. From that point on all the modifications you apply to a buffer object will be stored in the VAO. To unbind the VAO (and thus stop saving new configurations to it) you can run glBindVertexArray(0). 
Heres an example creating two different VAOs, each storing vertex data of one triangle:
```
	int bufferObjs[2], vaos[2];
	glGenBuffers(2, (GLuint*)&bufferObjs); //get references to buffer objs
	glGenVertexArrays(2,(GLuint*)&vaos); // get VAO references 
    
	glBindVertexArray(vaos[0]); //start storing configs and states in VAO 0

	glBindBuffer(GL_ARRAY_BUFFER, bufferObjs[0]);  //bind buffer to bind point
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData),vertexData,GL_STATIC_DRAW); //fill in buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //set attributes configuration 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(0); //enable attributes
	glEnableVertexAttribArray(1);

    //start storing configs and states in VAO 1. Binding this unbinds 
    //previous VAO 
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObjs[1]); 
	glBufferData(GL_ARRAY_BUFFER,sizeof(v2D),v2D,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
```

This idea of binding, editing and using seems to be the main philosophy of openGL, but I don`t quite get it fully yet. So I\'ll leave this link here, for further reading

[OpenGL object](https://www.khronos.org/opengl/wiki/OpenGL_Object)






### Things I haven`t undestood yet:

- I have zero idea why the -lglfw works but the -lglfw3 dont work on my machine. Even tough I`m farely sure I dont have the glfw (without 3) installed.


### Challenges
- Try to render more than one object at once
