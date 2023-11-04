# Melon Documentation

This provides some documentation for the Melon library.

## Current modules

Currently, the following modules are supported:

 * `alloc` for memory allocations
 * `args` for argument parsing
 * `bitmap` for bitmap images and drawing
 * `error` for error handling
 * `file` for easy file access (WIP)
 * `hash` for basic string hashing
 * `log` for logging
 * `maths` for mathematical structures (vectors, matricies)
 * `rand` for random numbers (XORShift)
 * `storage` for filesystem access (WIP)
 * `stream` for generic memory stream
 * `string` (alias `str`) for string utilities (Backwards-compatible refactoring in progress)
 * `table` for assocative arrays
 * `thread` for multi-threading (NB: plans to refactor)
 * `time` for time utilities
 * `types` for some cores types (not used often)
 * `window` for basic windowing support

### Internal modules

The following modules are internal modules and shouldn't be included directly.

 * `surface_implementation` for some functions relating to the `surface` module

### Old and deprecated modules

 * `fs` for file system access (deprecated, but still widely used)
 * `surface` for 3D bezier surfaces
 * `script` for lua scripting interface

## Possible future and unimplemented modules

 * `algebra` for a basic computer algebra system
 * `compute` for access to hetrogeneous compute (OpenCL, Vulkan Compute, etc.)
 * `geometry` for expanded 3D model support
 * `graph` for scene graphs
 * `cryptography` for cryptographic functions
 * `network` for networking support
 * `http` for http support
 * `image` for vector and basic raster graphics support
 * `compress` for compression support

### Low priority ideas

 * `draw` for a high-level graphics API (OpenGL, Vulkan Graphics, DirectX, etc.)
 * `main` for a main loop manager
 * `resman` for resource manegement

