/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Windowing access
 * 
 * @warning Not good! I *just* wanted it to work.
 * 
 * @see https://benedicthenshaw.com/soft_render_sdl2.html
 */

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

#include "log.h"
#include "maths.h"
#include "bitmap.h"
#include "error.h"
#include "memory.h"
#include "library.h"

#include "window.h"

DgWindow *gDefaultWindow;

// Windowing under linux, it's hell
#ifdef __linux__

#if defined(DG_ENABLE_X11) && defined(DG_ENABLE_WAYLAND)
	#define SELECT(SYM, ...) {\
		switch (this->backend) { \
			case DG_WINDOW_WAYLAND: \
				result = SYM ## _Wayland(__VA_ARGS__)\
				break;\
			\
			case DG_WINDOW_X11:\
				result = SYM ## _X11(__VA_ARGS__)\
				break;\
			\
			default: DgLog(DG_LOG_WARNING, "Unknown window backend: %d", this->backend); break;\
		}\
	}
#elif defined(DG_ENABLE_WAYLAND)
	#define SELECT(SYM, ...) result = SYM ## _Wayland(__VA_ARGS__);
#elif defined(DG_ENABLE_X11)
	#define SELECT(SYM, ...) result = SYM ## _X11(__VA_ARGS__);
#else
	#define SELECT(SYM, ...)
#endif

// Wayland specific functions
#ifdef DG_ENABLE_WAYLAND
#include <wayland-client-protocol.h>

DgError DgWindowInit_Wayland(DgWindow *this, const char *title, DgVec2I size) {
	return DG_ERROR_NOT_IMPLEMENTED;
}

int DgWindowFree_Wayland(DgWindow *this) {
	return 0;
}

bool DgWindowUpdate_Wayland(DgWindow *this) {
	return false;
}

void *DgWindowGetNativeDisplayHandleForEGL_Wayland(DgWindow *this) {
	return (void *) this->wl.display;
}

void *DgWindowGetNativeWindowHandleForEGL_Wayland(DgWindow *this) {
	return (void *) this->wl.surface;
}
#endif

// X11 specific functions
#ifdef DG_ENABLE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <signal.h>

#define X11_SYM(RET, SYM, SIG) RET (*SYM)SIG = DgLibraryGetSymbol(&this->x11.lib, #SYM)

DgError DgWindowInit_X11(DgWindow *this, const char *title, DgVec2I size) {
	DgError error = DgLibraryInit(&this->x11.lib, "libX11.so");
	
	if (error) {
		return DG_ERROR_FAILED;
	}
	
	Display *(*XOpenDisplay)(const char *) = DgLibraryGetSymbol(&this->x11.lib, "XOpenDisplay");
	int (*XDefaultScreen)(Display *) = DgLibraryGetSymbol(&this->x11.lib, "XDefaultScreen");
	Window (*XRootWindow)(Display *, int) = DgLibraryGetSymbol(&this->x11.lib, "XRootWindow");
	Visual *(*XDefaultVisual)(Display *, int) = DgLibraryGetSymbol(&this->x11.lib, "XDefaultVisual");
	Colormap (*XCreateColormap)(Display *, Window, Visual *, int) = DgLibraryGetSymbol(&this->x11.lib, "XCreateColormap");
	int (*XFreeColormap)(Display *, Colormap) = DgLibraryGetSymbol(&this->x11.lib, "XFreeColormap");
	int (*XCreateWindow)(Display *, Window, int, int, unsigned, unsigned, unsigned, int, int, Visual *, unsigned long, XSetWindowAttributes *) = DgLibraryGetSymbol(&this->x11.lib, "XCreateWindow");
	int (*XMapWindow)(Display *, Window) = DgLibraryGetSymbol(&this->x11.lib, "XMapWindow");
	int (*XStoreName)(Display *, Window, const char *) = DgLibraryGetSymbol(&this->x11.lib, "XStoreName");
	int (*XDefaultDepth)(Display *, int) = DgLibraryGetSymbol(&this->x11.lib, "XDefaultDepth");
	
	if (!XOpenDisplay || !XDefaultScreen || !XRootWindow || !XDefaultVisual || !XCreateColormap || !XFreeColormap || !XCreateWindow || !XMapWindow || !XStoreName) {
		return DG_ERROR_FAILED;
	}
	
	this->x11.display = XOpenDisplay(NULL);
	
	if (!this->x11.display) {
		return DG_ERROR_FAILED;
	}
	
	int screen = XDefaultScreen(this->x11.display);
	Window root = XRootWindow(this->x11.display, screen);
	Visual *visual = XDefaultVisual(this->x11.display, screen);
	Colormap colourmap = XCreateColormap(this->x11.display, root, visual, AllocNone);
	
	XSetWindowAttributes attributes;
	attributes.colormap = colourmap;
	attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | StructureNotifyMask;
	
	int default_depth = XDefaultDepth(this->x11.display, screen);
	this->x11.window = XCreateWindow(this->x11.display, root, 0, 0, size.x, size.y, 0, default_depth, InputOutput, visual, CWColormap | CWEventMask, &attributes);
	
	XFreeColormap(this->x11.display, colourmap);
	
	XMapWindow(this->x11.display, this->x11.window);
	XStoreName(this->x11.display, this->x11.window, title);
	
	if (!this->x11.window) {
		return DG_ERROR_FAILED;
	}
	
	this->size = size;
	this->should_close = false;
	this->backend = DG_WINDOW_X11;
	
	return DG_ERROR_SUCCESSFUL;
}

int DgWindowFree_X11(DgWindow *this) {
	void (*XDestroyWindow)(Display *, Window) = DgLibraryGetSymbol(&this->x11.lib, "XDestroyWindow");
	void (*XCloseDisplay)(Display *) = DgLibraryGetSymbol(&this->x11.lib, "XCloseDisplay");
	
	XDestroyWindow(this->x11.display, this->x11.window);
	XCloseDisplay(this->x11.display);
	
	DgLibraryFree(&this->x11.lib);
	
	return 0;
}

bool DgWindowUpdate_X11(DgWindow *this) {
	X11_SYM(int, XPending, (Display *));
	X11_SYM(void, XNextEvent, (Display *, XEvent *));
	
	while (XPending(this->x11.display)) {
		XEvent event;
		
		XNextEvent(this->x11.display, &event);
		
		if (event.type == KeyPress) {
			// this->should_close = true;
		}
		else if (event.type == MotionNotify) {
			this->mouse_pos.x = event.xmotion.x;
			this->mouse_pos.y = event.xmotion.y;
		}
		else if (event.type == DestroyNotify) {
			this->should_close = true;
		}
	}
	
	return true;
}

void *DgWindowGetNativeDisplayHandleForEGL_X11(DgWindow *this) {
	return (void *) this->x11.display;
}

void *DgWindowGetNativeWindowHandleForEGL_X11(DgWindow *this) {
	return (void *) this->x11.window;
}
#endif
#endif

DgError DgWindowInit(DgWindow *this, const char *title, DgVec2I size) {
	/**
	 * Initialise and create a window
	 * 
	 * @param this Window object
	 * @return Zero on success, non-zero on failure
	 */
	
	DgMemoryZero(this, sizeof *this);
	
	DgError error = DG_ERROR_NOT_IMPLEMENTED;
	
#ifdef DG_ENABLE_WAYLAND
	if (error) {
		error = DgWindowInit_Wayland(this, title, size);
	}
#endif
#ifdef DG_ENABLE_X11
	if (error) {
		error = DgWindowInit_X11(this, title, size);
	}
#endif
	
	return error;
}

void DgWindowFree(DgWindow *this) {
	/**
	 * Free resources assocaited with a window
	 * 
	 * @param this Window object
	 */
	
	int result; // unused
	
	SELECT(DgWindowFree, this);
}

bool DgWindowUpdate(DgWindow *this) {
	/**
	 * Display new changes to a window.
	 * 
	 * @param this Window object
	 * @return true on success, or false if there was some kind of error
	 */
	
	bool result = false;
	
	SELECT(DgWindowUpdate, this);
	
	return result;
}

DgVec2I DgWindowGetMouseLocation(DgWindow * restrict this) {
	/**
	 * Return the current mouse position in window coordinates.
	 * 
	 * @param this Window object
	 * @return Cursor position relative to window in window coordinates
	 */
	
	return this->mouse_pos;
}

bool DgWindowGetMouseDown(DgWindow * restrict this) {
	/**
	 * Get if the left mouse button is down or not.
	 * 
	 * @param this Window object (not needed using SDL backend)
	 * @return Cursor position relative to window
	 */
	
	return false;
}

DgVec2I DgWindowGetSize(DgWindow * restrict this) {
	/**
	 * Get the current size of the window.
	 * 
	 * @param this Window object
	 * @return Window size in pixels
	 */
	
	return this->size;
}

bool DgWindowShouldClose(DgWindow * restrict this) {
	/**
	 * Return if the window should close
	 * 
	 * @param this Window object
	 * @return true if the window should close, false if the window should keep
	 * running
	 */
	
	return this->should_close;
}

DgWindowBackend DgWindowGetBackend(DgWindow *this) {
	/**
	 * Get the name of the backend for this window.
	 */
	
	return this->backend;
}

void *DgWindowGetNativeDisplayHandleForEGL(DgWindow * restrict this) {
	/**
	 * Get the native display handle used with EGL.
	 * 
	 * @param this Window object
	 * @return Handle to the native display object 
	 */
	
	void *result = NULL;
	
	SELECT(DgWindowGetNativeDisplayHandleForEGL, this);
	
	return result;
}

void *DgWindowGetNativeWindowHandleForEGL(DgWindow * restrict this) {
	/**
	 * Get the native window handle used with EGL.
	 * 
	 * @param this Window object
	 * @return Handle to the native window object 
	 */
	
	void *result = NULL;
	
	SELECT(DgWindowGetNativeWindowHandleForEGL, this);
	
	return result;
}

#if defined(_WIN32)

#include <windows.h>

const char DG_WINDOW_CLASS_NAME[] = "Melon Library Window";
bool DG_WINDOW_WANTS_TO_QUIT = false;

static LRESULT CALLBACK DgWindow_NTProcessWindowEvent(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_CLOSE || message == WM_DESTROY || message == WM_QUIT || message == WM_NCDESTROY) {
		DG_WINDOW_WANTS_TO_QUIT = true;
	}
	
	return DefWindowProc(window_handle, message, wparam, lparam);
}

DgError DgWindowInit(DgWindow *this, const char *title, DgVec2I size) {
	/**
	 * Create a window (Windows NT)
	 * 
	 * @param this Handle to the window object
	 * @param title Title of the window
	 * @param size Size of the window
	 */
	
	// Setup window class
	memset(&this->window_class, 0, sizeof this->window_class);
	this->window_class.lpfnWndProc = &DgWindow_NTProcessWindowEvent; // Default callback
	this->window_class.hInstance = GetModuleHandle(NULL); // It seems this can be NULL and it will take care of things.
	this->window_class.lpszClassName = DG_WINDOW_CLASS_NAME;
	
	// Register the class
	if (RegisterClass(&this->window_class) == 0) {
		DgLog(DG_LOG_VERBOSE, "RegisterClass: error code = %d", GetLastError());
		return DG_ERROR_FAILED;
	}
	
	this->window_handle = CreateWindowEx(
		0,
		DG_WINDOW_CLASS_NAME,
		title,
		WS_POPUP, // HACK when using WS_OVERLAPPEDWINDOW windows needs extra size added to the window
		CW_USEDEFAULT, CW_USEDEFAULT,
		size.x, size.y,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	
	if (this->window_handle == NULL) {
		DgLog(DG_LOG_VERBOSE, "CreateWindowEx: error code = %d", GetLastError());
		return DG_ERROR_FAILED;
	}
	
	// Pop up the window
	ShowWindow(this->window_handle, SW_SHOW);
	
	return DG_ERROR_SUCCESS;
}

void DgWindowFree(DgWindow *this) {
	return;
}

DgWindowStatus DgWindowUpdate(DgWindow *this) {
	/**
	 * Update the window contents with to use the given bitmap (or NULL if using
	 * the assocaited bitmap).
	 * 
	 * @param this Window object
	 * @param bitmap Bitmap to take image from
	 * @return Error code
	 */
	
	// Handle messages
	MSG message;
	
	// Invalidate the rectange so we can draw to it.
	InvalidateRect(this->window_handle, NULL, FALSE);
	
	while (PeekMessage(&message, this->window_handle, 0, 0, PM_NOREMOVE) == 1) {
		// Remove message
		PeekMessage(&message, this->window_handle, 0, 0, PM_REMOVE);
		
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	
	// HACK: We need to use a global variable to check if the window would like
	// to quit becuase Windows does not use PostMessage to push the WM_QUIT or
	// similar messages and instead calls the callback directly, which is not
	// compatible with our architecture.
	if (DG_WINDOW_WANTS_TO_QUIT) {
		return 1;
	}
	
	return 0;
}

DgVec2I DgWindowGetMouseLocation(DgWindow *this) {
	return (DgVec2I) {0, 0};
}

bool DgWindowGetMouseDown(DgWindow *this) {
	return false;
}
#endif
