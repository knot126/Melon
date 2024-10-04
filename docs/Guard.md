# Error handling: Guard

Melon's guard functions provide a way to get exception-like error handling in C.

## Rasing an error

To raise an error, you can simply use the macro `DgRaise(TYPE, MESSAGE)`. For example:

```c
DgRaise("NullStringError", "One or both strings are NULL");
```

## Basic exmaple with DgTry

A simple macro, `DgTry`, very closely replicates the usage of a `try { ... } catch { ... }` statement with the guard facilities:

```c
void SomethingThatRaisesAnError(void) {
	DgRaise("ExampleError", "Pretend like something went wrong here and we need to raise an error");
}

int main() {
	DgTry({
		SomethingThatRaisesAnError();
	}, error_info, {
		DgLog(DG_LOG_INFO, "Caught an error: %s", error_info->type);
	})
}
```

The first parameter of the `DgTry` macro is the expression or block to try excuting. The second is the name of the pointer to the `DgErrorInfo` structure. The third is the block to execute if there was an error - essentially the `catch` block.

It is recommended that use use DgTry

## How it works

Every time you call `DgGuard` or use `DgTry`, a new *guard frame* is pushed onto the *guard stack*. This frame contains information about how to jump back to your function. In the case of an error, we pop the latest frame off of the guard stack and use it to return control to your function (of course, within the DgGuard macro, which handles getting the error info after jumping back).

This of course means if there is *not* an error, you will have to manually remove the frame from the stack using DgUnguard so you don't accidently catch any future errors.

## Using DgGuard directly

You can also use the full DgGuard interface directly:

```c
// The first time DgGuard is used, it returns NULL. This indicates a guard has
// been pushed onto the stack of guards, and we can now safely run our code and
// catch errors. If it returns a non-NULL value, then an error occured and we
// need to handle it.
DgErrorInfo *error = DgGuard();

if (!error) {
	// Code to try running goes here...
	
	// If successful, we should unguard it so we don't get any future exceptions
	DgUnguard();
}
else {
	// Oops! There was an error. Do something nice with it...
	
	// If it turns out to be an error we can't handle nicely, we can use
	// DgReraise to pass the current error along to the next guard.
	if (/* ... */) {
		DgReraise();
	}
	
	// Note that the guard frame was already pushed off the stack so we don't
	// need to call DgUnguard here.
}
```

## Internal workings

The guard utilities use `setjmp` and `longjmp` from `setjmp.h` of the C standard library to implement a basic form of exception handling.
