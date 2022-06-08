# Melon Documentation

This provides some documentation for the Melon library.

## Modules

Currently, the following modules are supported:

 * `alloc` for memory allocations
 * `args` for argument parsing
 * `bitmap` for bitmap images and drawing
 * `hash` for basic string hashing
 * `log` for logging
 * `maths` for mathematical structures (vectors, matricies)
 * `rand` for random numbers (XORShift)
 * `script` for lua scripting interface
 * `stream` for generic memory stream
 * `string` (or `str`) for string utilities
 * `surface` for 3D bezier surfaces
 * `thread` for multi-threading
 * `time` for time utilities
 * `types` for some cores types (not used often)
 * `window` for basic windowing support

### Internal modules

The following modules are internal modules and shouldn't be included directly.

 * `surface_implementation` for some functions relating to the `surface` module

### Old, incomplete and deprecated modules

 * `fs` for file system access (deprecated, but still widely used)
 * `value` for generic or typeless arrays (deprecated, needs replacement, never used)
 * `obj` for basic `.obj` file loading (deprecated, melon is focused on dynamically generated content)

### Possible future and unimplemented modules

 * `algebra` for a basic computer algebra system
 * `compute` for access to hetrogeneous compute (OpenCL, Vulkan Compute, etc.)
 * `draw` for a high-level graphics API (OpenGL, Vulkan Graphics, DirectX, etc.)
 * `file` for file access
 * `geometry` for expanded 3D model support
 * `storage` for filesystem access
