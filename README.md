# Ray Tracer

<p align="center"><img src="./images/demo.png" width="900"/></p>
<p align="center">Sample Render</p>

A very simple WIP hobby CPU ray tracer with minimal UI elements.   
  
Heavily based on *Ray Tracing In One Weekend*, *Ray Tracing from the ground up*, *PBRT* 

## Features (WIP)
- Multiple tracers: Path Tracing, Whitted Ray Tracing
- Materials: Matte, Phong, Reflective, Transparent
- Lights: Coming Soon
- 3D meshes in .obj format
- BVH Optimization
- Multithreaded rendering
- GUI

## External dependencies
- [Dear ImGui](https://github.com/ocornut/imgui) + OpenGL 3 backend - for immediate mode GUI elements
- [spng](https://github.com/randy408/libspng/) + [miniz](https://github.com/richgel999/miniz) - for PNG encoding and decoding
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) for 3D meshes 

## Notes
*TODO*
## Roadmap
- ~~3D Meshes~~
- Affine transformations
- Lights
- Normal ray tracing
- Blinn-Phong shading
- Highly efficient bucket rendering

## Gallery
<p align="center"><img src="./images/spheres1.png" width="700"/></p>
<br>
<p align="center"><img src="./images/spheres2.png" width="700"/></p>

## References
- [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html), Peter Shirley. 2020
- Ray Tracing from the Ground Up, Kevin Suffern. 2007. 