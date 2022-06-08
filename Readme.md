# Melon Software Library

**Melon** is currently the main core library for some game(-like) projects by Knot126. It is mainly focused on making it simple to create a game engine or other type of simulation which supports procedureal content generation, advanced physics and "demosceney" things.

## Support

This library is **not currently supported** very well, and there is a lot of messy code. It probably shouldn't be used in seroius projects until it evolves more.

There is some documentation in the [docs](docs) folder. Most documentation is inline at the start of the implementation of each function.

## Including in Projects

To include in projects, add the `.c` files in the `src` folder to the projects files, likely under the `util` subfolder.

For example:

```bash
cp -r ./melon/src ./hello/util
gcc -o hello ./hello/hello.c $(ls ./hello/util/*.c) -I./hello
```

```c
#include "util/log.h"

int main(const int argc, char *argv[]) {
	DgLog(DG_LOG_INFO, "Hello, world!");
	return 0;
}
```
