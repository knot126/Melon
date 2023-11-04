# Melon Software Library

**Melon** is a replacement core library for the C language. It is aimed to be a general purpose library with some extra bias for games.

## Support

Melon will accept bugs from third party projects and we aim to proivde support for them. It is licenced in such a way that you can *probably* use it, though [you should check the licence](LICENCE) first. Melon is **not** free software or open source and has restrictions on who can use it.

There is some documentation in the [docs](docs) folder, though most documentation is inline at the start of the implementation of each function and at the start of each file.

I don't really recommend that you use it if you find something better - and it would probably be more fun for you to build your own tech, if you don't find something else.

## Usage

To include in projects, add the `.c` files in the `src` folder to the projects files, likely under the `melon` subfolder.

For example:

<details>
  <summary><code>build.sh</code></summary>
  
  ```bash
  cp -r ./melon/src ./hello/melon
  cc -o hello ./hello/hello.c $(ls ./hello/util/*.c) -I./hello
  ```
</details>

<details>
  <summary><code>hello/hello.c</code></summary>
  
  ```c
  #include "melon/melon.h"
  
  int main(const int argc, char *argv[]) {
  	DgLog(DG_LOG_INFO, "Hello, world!");
  	return 0;
  }
  ```
</details>
