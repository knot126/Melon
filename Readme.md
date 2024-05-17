# Melon Software Library

**Melon** is a replacement core library for the C language. It is aimed to be a general purpose library with some extra bias for games.

Please note that Melon is currently only available under the same terms as the Microsoft Reference Source License, or the terms of the [Melon Alternate License](LICENCE.alt).

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
