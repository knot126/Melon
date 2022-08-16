# Melon Software Library

**Melon** is a core library for projects by Knot126. It is mostly focused on simplifying the creation of simulations and their engines which use procedureal (content) generation, advanced physics and "demosceney" or exprimental things.

## Support

This library is **not well supported for thirdparty projects**, though it is MIT licenced. There is a lot of messy code.

There is some documentation in the [docs](docs) folder. Most documentation is inline at the start of the implementation of each function.

It would probably be more fun for you to build your own tech, anyways.

## Including in Projects

To include in projects, add the `.c` files in the `src` folder to the projects files, likely under the `util` subfolder.

For example:

<details>
  <summary><code>build.sh</code></summary>
  
  ```bash
  cp -r ./melon/src ./hello/util
  gcc -o hello ./hello/hello.c $(ls ./hello/util/*.c) -I./hello
  ```
</details>

<details>
  <summary><code>hello/hello.c</code></summary>
  
  ```c
  #include "util/log.h"
  
  int main(const int argc, char *argv[]) {
  	DgLog(DG_LOG_INFO, "Hello, world!");
  	return 0;
  }
  ```
</details>
