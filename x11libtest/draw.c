/*
 * A simple test program to see how the draw-to-window operations work...
 */

#include <X11/Xlib.h>
#include <X11/X.h>
#include <stdio.h>

void testgraphics(Window target, Display* display) {
	//
	// Let's try to draw a line.
	//
	
	GC gc;
	long mask = 0;
	int i = 0;

	XRectangle rect;
	XRectangle* prect = (&rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = 1400;
	rect.height = 1050;

	printf("%d\n", prect -> width);

	for (i = 1; i < 19; i++) {
		mask |= (1 << i);
	}

	printf("OK so far!\n");
	fflush(stdout);

	GC def = XDefaultGC(display, 0);
/*	printf("%d\n", XCopyGC(display, def, GCFunction | GCPlaneMask | GCForeground | GCBackground | GCLineWidth | GCLineStyle | GCCapStyle | GCJoinStyle | GCFillStyle | GCFillRule, gc));

	printf("Still OK\n");
	fflush(stdout);

	printf("%d\n", XSetClipRectangles(display, gc, 0, 0, prect, 1, 1));

	printf("OK\n");
	fflush(stdout);
	
	XSetForeground(display, gc, 0xFFFFFF);
	XSetBackground(display, gc, 0x000000);
//	XSetFunction(display, gc, GXcopy);*/
	
	XDrawLine(display, target, def, 0, 0, 1000, 1000);

	//
	// Now try to fill a rectangle.
	//
	
	XFillRectangle(display, target, def, 0, 0, 100, 100);
	XFlushGC(display, def);
//	XFreeGC(display, gc);
 }

int main(int argc, char** argv) {
	//
	// First, load in the display.
	//

	Display* pd = XOpenDisplay(":0.0");

	//
	// Next, find the desktop window.
	//

	Window pw = XRootWindow(pd, 0);

	//
	// Now for the fun part. We get to do some graphics operations!
	//

	testgraphics(pw, pd);
	XCloseDisplay(pd);

	return 0;
}
