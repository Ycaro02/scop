# SCOP 3D Object Renderer

This project is a small program designed to display a 3D object, conceived with a modeling program like Blender, from a .obj file. The program is responsible for parsing the .obj file to obtain the requested rendering.

## Features

- The 3D object is displayed in perspective, meaning objects further away appear smaller.
- The object rotates on its own around its main symmetrical axis.
- The object can be moved along three axes, in both directions.
- The object can be colored differently on various sides for distinction.
- A texture can be applied to the object by pressing a dedicated key. Pressing the same key will revert the object back to its original colors. A soft transition between the two states is implemented.

## Technical Constraints

- You're free to use any languages (C / C++ / Rust preferred).
- You're free to choose between OpenGL, Vulkan, Metal, and the MinilibX.
- A classic Makefile is required.
- External libraries (other than OpenGL, Vulkan, or Metal) can only be used to manage the windows and the events.
- No libraries are allowed to load the 3D object, nor to make your matrices, or to load the shaders.

## Technical Choices

- The project is implemented using the C programming language.
- OpenGL is used for graphics rendering.
- GLFW3 is used for window management and event handling.

## Demonstration

A video demonstration of the project can be found [here](rsc/helper/Scop.mp4).

## Usage

To run the program, follow these steps:

1. Clone the repository.
2. Navigate to the project directory.
3. Run `make` to compile the program.
4. Run the program with `./scop <model_path> [texture_path]`.
