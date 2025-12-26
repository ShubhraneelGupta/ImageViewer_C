# SDL2 PPM Image Viewer

A simple C program that displays PPM (Portable Pixmap) images using SDL2. This image viewer supports binary PPM (P6 format) files and renders them pixel by pixel.

## Features

- Loads and displays PPM (P6 format) image files
- Supports images with various maximum color values (not just 255)
- Simple keyboard controls (press 'Q' to quit)
- Cross-platform compatibility (Windows, Linux, macOS)
- Clean CPU-based rendering workflow

## Requirements

- SDL2 library
- C compiler (GCC, Clang, MSVC, etc.)
- PPM image files (P6 binary format)

## Installation

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libsdl2-dev
```

### macOS (using Homebrew)
```bash
brew install sdl2
```

### Windows
Download SDL2 development libraries from [libsdl.org](https://www.libsdl.org/) and set up your build environment accordingly.

## Building

Compile the program using:
```bash
gcc -o ppm_viewer iv.c `sdl2-config --cflags --libs`
```

Or with more detailed options:
```bash
gcc -Wall -Wextra -o ppm_viewer iv.c `sdl2-config --cflags --libs`
```

## Usage

Run the program with a PPM file as argument:
```bash
./ppm_viewer image.ppm
```

### Controls
- **Q** - Quit the application
- **Window Close Button** - Quit the application

## Supported File Format

Currently only supports **P6 format PPM files** (binary format). This is indicated by the "P6" magic number at the beginning of the file.

### PPM File Format Details
- Magic number: `P6`
- Binary color data (RGB triplets)
- Header format: `P6\nwidth height\nmaxval\n`
- Color values scaled automatically if `maxval` is not 255

## Program Structure

1. **Command-line Argument Processing**
   - Validates input filename
   - Extracts and checks file extension (.ppm)

2. **PPM File Parsing**
   - Reads P6 header information
   - Handles comments and whitespace
   - Extracts width, height, and maximum color value

3. **SDL2 Initialization**
   - Creates window with dimensions matching the image
   - Sets up surface for pixel manipulation

4. **Image Rendering**
   - Reads RGB values from PPM file
   - Scales colors if `maxval` is not 255
   - Draws each pixel individually using SDL_FillRect

5. **Event Loop**
   - Handles keyboard input ('Q' to quit)
   - Manages window events
   - ivtains display until user exits

## Limitations

- Only supports P6 (binary) PPM format
- No image scaling or resizing
- Basic rendering (no hardware acceleration)
- No support for other image formats (PNG, JPEG, etc.)
- Single image viewing at a time

## Code Notes

- The program uses CPU-based rendering which is simple but may be slow for large images
- For hardware-accelerated rendering, see the commented GPU workflow section
- Memory is properly cleaned up on exit
- Event polling is required for proper window management on Wayland and X11

## License

This code is provided as-is for educational purposes. Modify and distribute as needed.

## Troubleshooting

1. **"Only supports .ppm for now"** - Ensure you're using a PPM file with .ppm extension
2. **"Only P6 format PPM files are supported"** - Convert your PPM file to binary (P6) format
3. **SDL2 errors** - Verify SDL2 is properly installed and linked
4. **Black screen** - Check that the PPM file is not empty and has valid RGB data

## Example

To view a sample PPM image:
```bash
./ppm_viewer test.ppm
```

The program will display a 100x100 pixel gradient from red to blue.