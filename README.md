# DFGame
DFGame is a collection of libraries designed to remove much of the boilerplate
involved in "from scratch" game development.

## Features
DFGame's various modules provide a variety of helpful features:
- Basic delegate implementation
- Dynamic arrays and hashmaps
- Simple "main loop" implementation
- Action-based input
- Message logging
- Sprite, mesh, and text rendering
- Matrix and vector math
- Transform handling
- Audio preloading, streaming, and playback
- Post-processing

...and probably more that I've forgotten to list.

## Building DFGame
To build DFGame, you'll need the following tools:
- meson
- ninja
- pkg-config (optional)
and the following libraries:
- libFreeType
- libJPEG (optional)
- libPNG (optional)
- libTIFF (optional)
- libTGA (optional)
- OpenAL
- libOgg
- libVorbisfile
- libXml
- ZLib

DFGame will pull in the following libraries on its own:
- glew
- glfw

You can build and install DFGame using the following steps.
```bash
# Create build directory and generate project files
mkdir build && cd build
meson ..

# Compile and install
ninja
ninja install
```
