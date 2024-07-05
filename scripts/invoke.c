/**
 * Run a function from a shared object.
 * 
 * clang -o invoke invoke.c -ldl
 */

#include <stdio.h>
#include <dlfcn.h>

typedef void (*InvokeFunction)(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("%s - start a function from a shared object\n\nUsage: %s <soname> <function> [args ...]\n", argv[0], argv[0]);
		return 127;
	}
	
	void *handle = dlopen(argv[1], RTLD_LAZY | RTLD_LOCAL);
	
	if (!handle) {
		printf("dlopen(): %s\n", dlerror());
		return 1;
	}
	
	InvokeFunction func = (InvokeFunction) dlsym(handle, argv[2]);
	
	if (!func) {
		printf("dlsym(): %s\n", dlerror());
		return 2;
	}
	
	func(argc - 3, argv + 3);
	
	dlclose(handle);
	
	return 0;
}
