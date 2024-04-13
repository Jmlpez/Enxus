## OpenGL Test Menu App

A SandBox app to test some OpenGL feature by creating an isolated test.

The App have been designed as an Core Engine with abstractions of OpenGL buffers and shaders etc.
For example to load a model you can type:

```cpp
Model box("path/to/box.obj");
```

### Vendor dependencies
- *Assimp*: To import/export models with several formats from softwares like blender, 3DMax etc.
- *ImGui*: To create the UI.
- *stb_image*: A header-only library to handle images with several formats.
- *glm*: To handle math (vectors, matrices, etc)
- *GLFW*: To handle windows and openGL context 
- *GLEW*: To call the OpenGL functions according to your drivers and/or OS.



