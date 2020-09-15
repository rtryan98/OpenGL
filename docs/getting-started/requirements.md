---
sort: 1
---

# Requirements
Creating a program with OpenGL requires atleast one dependency (The OpenGL loader library, in our case *GLAD*). We'll be using a few more out of convinience and to get results fast.

## CMake
```note
While fully optional for following most of this tutorial, it is required to build the examples.
```
[CMake](https://cmake.org/) is an open-source, cross-plattform toolset used to build IDE files for projects, so that the project doesn't depend on a single IDE. It is also useful to manage dependencies with git submodules, because most C/C++ projects already depend on it.
Even if you don't want to build your project files with CMake, it's good practice to build libraries yourself.
If you're not familiar with git, the following tutorial will make a quick introduction to it including CMake.

## GLFW
[GLFW](https://www.glfw.org/) is a windowing library written in C, targeted at OpenGL but also usable for other APIs such as Vulkan or even Direct3D.
It is available as prebuilt binaries or as source to build for yourself.
GLFW will abstract all of the OS-specific windowing and input away from us and leave us with an easy to read and easy to understand API, which will jumpstart our OpenGL journey a lot.

## GLAD
By default, Windows and possible other operating systems don't offer support for OpenGL beyond 2.0 or even lower, for that reason, we require an *OpenGL loader library* to make newer versions available to use for us.
The loader library which we will use is [GLAD](https://github.com/Dav1dde/glad). You can either [generate](https://glad.dav1d.de/) your OpenGL loader to your wishes or you can download my premade [CMake version](https://github.com/rtryan98/glad).
If you wish to generate it yourself, simply select your version, the core profile and the extensions you wish to use. Then click generate and download it! All you have to do now is add the `glad.c` file somewhere into your source folder and the `glad/` and `KHR/` directories into your include directory.

## GLM
[GLM](https://glm.g-truc.net/0.9.9/index.html) is a header-only mathematics library specifically designed to be used with OpenGL. Like GLFW, we will be able to get results much quicker and don't have to write our own math library.
