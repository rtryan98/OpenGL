---
sort: 1
---

# The first Triangle

Drawing something with OpenGL requires some code upfront. It might seem like a lot, but in ~100 lines of code you can already have your first window showing a triangle on screen!
As a quick overview, we'll need to do the following:
1. Create a window we can draw to
2. Initialize our OpenGL loader
3. Setup a debug callback so we can directly see an error when it happens and trace it back
4. Create the data required to draw the triangle and send it to the GPU
5. Create a shader program that runs on the gpu
6. Run the program and see a triangle :)
