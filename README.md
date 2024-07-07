## Enxus Game Engine

A game Engine created for the sake of learning 😄. Heavily inspired by [The Cherno](https://www.youtube.com/@TheCherno). 

I used OpenGL as the graphics library but this may change in the future. I mainly start learning in [learnopengl.com](learnopengl.com) 
and also in [scratchpixel](https://www.scratchapixel.com/)

## Build
I'm a linux user so I can't use Visual Studio (like many windows game engine developers) so I use makefiles for the compilation/execution process
If you don't know what make or makefiles are I highly recommend to check it [here](https://makefiletutorial.com/). It has a learning curve (as everything) but it totally worth it. 

So, first navigate to the project root folder and compile the engine :

```
make enxus
```
Note: (it must be from the root folder because the internal paths used by the engine are relative to root folder).

Next you have two options: 
- Terrain Generator App
- The OpenGL features menu 


### Terrain Generator App

An application to generate terrain using several types of noise. I used [FastNoiseLite](https://github.com/Auburn/FastNoiseLite) library.

It has several features besides the noise generation so check it out.

Once the engine is built then type

```
make terrain
make run-terrain
```
The first line is to create terrain-app from scratch.
The second one to actually running it.

I will post some pictures when it is a little more finished.


## The OpenGL features menu 
It is a little app that allows me to test some OpenGL features before I including them in the Terrain Generator App.

Some Test are:
- [x] Model Loading
- [x] Mesh animation (simple)
- [x] Blending
- [x] Depth&Stencils buffers
- [x] Basic lighting

Those test are mostly from [learnopengl.com](learnopengl.com)



### Vendor dependencies
Right now the project can't compile because no vendor dependency is included or has instructions about how to set it correctly :). Soon I will include how to get those libraries. 
- *Assimp*: To import/export models with several formats from softwares like blender, 3DMax etc.
- *ImGui*: To create the UI.
- *stb_image*: A header-only library to handle images with several formats.
- *glm*: To handle math (vectors, matrices, etc)
- *GLFW*: To handle windows and openGL context 
- *GLEW*: To call the OpenGL functions according to your drivers and/or OS.



