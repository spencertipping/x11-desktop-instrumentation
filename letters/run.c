/*
 * Here is the layout of this file:
 *
 * - Structures and constants
 * - Letter drawing routines
 * - Computational cycle
 * - Main method
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <unistd.h>

// Structures and constants
	#define letterheight 9
	#define letterwidth 7

	#define screenheight 1050
	#define screenwidth 1400

	#define charsx (screenwidth / (letterwidth + 1))
	#define charsy (screenheight / (letterheight + 1))

	typedef struct {
		int xletter;
		int yletter;
	} cursor;

	typedef struct {
		Display* disp;
		Drawable win;
		GC gc;
	} xstuff;

	#define xpixel(c) (c.xletter * (letterwidth + 1))
	#define ypixel(c) (c.yletter * (letterheight + 1))

	#define background 0x3355AA
	#define foreground 0x6688EE

	#define rect(xloc, y, w, h) XDrawRectangle(x.disp, x.win, x.gc, xoff + (xloc), yoff + (y), w, h);
	#define line(x1, y1, x2, y2) XDrawLine(x.disp, x.win, x.gc, xoff + (x1), yoff + (y1), xoff + (x2), yoff + (y2));
	#define point(xloc, y) XDrawPoint(x.disp, x.win, x.gc, xoff + (xloc), yoff + (y));

// Letter drawing routines
	void clearscreen(xstuff x) {
		//
		// This function clears the entire screen.
		//

		XSetForeground(x.disp, x.gc, background);
		XFillRectangle(x.disp, x.win, x.gc, 0, 0, 1400, 1050);
	}

	void clearspot(cursor c, xstuff x) {
		//
		// This function clears out the space referred to by c.
		//

		XSetForeground(x.disp, x.gc, background);
		XFillRectangle(x.disp, x.win, x.gc, xpixel(c), ypixel(c), letterwidth + 1, letterheight + 1);
	}

	void drawletter(cursor c, xstuff x, char l, int clearmode) {
		//
		// This function renders the given letter at the cursor
		// position with the given x stuff.
		//

		int xoff = xpixel(c);
		int yoff = ypixel(c);

		clearspot(c, x);

		XSetForeground(x.disp, x.gc, foreground);

#ifdef debug
		printf("Current cursor location is: %d, %d in pixels\n", xoff, yoff);
#endif

		switch (l) {
			case ' ':
				break;

			case 'a':
			case 'A':
				rect(1, 4, 4, 2)
				line(1, 2, 5, 2)
				point(5, 3)
				break;

			case 'b':
			case 'B':
				rect(1, 2, 4, 4)
				line(1, 0, 1, 1)
				break;

			case 'c':
			case 'C':
				line(1, 2, 1, 6)
				line(2, 2, 5, 2)
				line(2, 6, 5, 6)
				break;

			case 'd':
			case 'D':
				rect(1, 2, 4, 4)
				line(5, 0, 5, 1)
				break;

			case 'e':
			case 'E':
				rect(1, 2, 4, 2)
				line(1, 6, 5, 6)
				point(1, 5)
				break;

			case 'f':
			case 'F':
				line(3, 0, 3, 6)
				line(1, 3, 5, 3)
				line(3, 0, 5, 0)
				break;

			case 'g':
			case 'G':
				rect(1, 2, 4, 4)
				line(1, 8, 5, 8)
				point(5, 7)
				break;

			case 'h':
			case 'H':
				line(1, 0, 1, 6)
				line(1, 2, 5, 2)
				line(5, 3, 5, 6)
				break;

			case 'i':
			case 'I':
				line(3, 2, 3, 6)
				line(1, 6, 5, 6)
				point(2, 2)
				point(3, 0)
				break;

			case 'j':
			case 'J':
				line(3, 2, 3, 8)
				line(1, 8, 3, 8)
				break;

			case 'k':
			case 'K':
				line(1, 0, 1, 6)
				line(1, 4, 5, 4)
				line(4, 2, 4, 4)
				line(5, 4, 5, 6)
				break;

			case 'l':
			case 'L':
				line(3, 0, 3, 6)
				line(1, 6, 5, 6)
				break;

			case 'm':
			case 'M':
				line(1, 2, 1, 6)
				line(3, 2, 3, 6)
				line(5, 2, 5, 6)
				line(1, 2, 5, 2)
				break;

			case 'n':
			case 'N':
				line(1, 2, 1, 6)
				line(5, 2, 5, 6)
				line(1, 2, 5, 2)
				break;

			case 'o':
			case 'O':
				rect(1, 2, 4, 4)
				break;

			case 'p':
			case 'P':
				rect(1, 2, 4, 4)
				line(1, 7, 1, 8)
				break;

			case 'q':
			case 'Q':
				rect(1, 2, 4, 4)
				line(5, 7, 5, 8)
				break;

			case 'r':
			case 'R':
				line(1, 2, 1, 6)
				line(1, 2, 5, 2)
				break;

			case 's':
			case 'S':
				line(1, 2, 5, 2)
				line(1, 4, 5, 4)
				line(1, 6, 5, 6)
				point(1, 3)
				point(5, 5)
				break;

			case 't':
			case 'T':
				line(3, 0, 3, 6)
				line(1, 2, 5, 2)
				break;

			case 'u':
			case 'U':
				line(1, 2, 1, 6)
				line(5, 2, 5, 6)
				line(1, 6, 5, 6)
				break;

			case 'v':
			case 'V':
				line(1, 2, 1, 6)
				line(1, 6, 3, 6)
				line(5, 2, 5, 4)
				point(4, 5)
				break;

			case 'w':
			case 'W':
				line(1, 2, 1, 6)
				line(3, 2, 3, 6)
				line(5, 2, 5, 6)
				line(1, 6, 5, 6)
				break;

			case 'x':
			case 'X':
				line(1, 2, 5, 6)
				line(5, 2, 1, 6)
				break;

			case 'y':
			case 'Y':
				line(1, 2, 1, 5)
				line(5, 2, 5, 8)
				line(1, 5, 5, 5)
				line(1, 8, 5, 8)
				break;

			case 'z':
			case 'Z':
				line(1, 2, 5, 2)
				line(1, 6, 5, 6)
				line(1, 6, 5, 2)
				break;
				
			case '1':
				line(1, 6, 5, 6)
				line(3, 0, 3, 6)
				line(1, 0, 3, 0)
				break;

			case '2':
				line(1, 0, 5, 0)
				line(1, 3, 5, 3)
				line(1, 6, 5, 6)
				line(5, 0, 5, 3)
				line(1, 3, 1, 6)
				break;

			case '3':
				line(1, 0, 5, 0)
				line(1, 3, 5, 3)
				line(1, 6, 5, 6)
				line(5, 0, 5, 6)
				break;

			case '4':
				line(1, 0, 1, 3)
				line(5, 0, 5, 6)
				line(1, 3, 5, 3)
				break;

			case '5':
				line(1, 0, 5, 0)
				line(1, 3, 5, 3)
				line(1, 6, 5, 6)
				line(5, 3, 5, 6)
				line(1, 0, 1, 3)
				break;

			case '6':
				rect(1, 3, 4, 3)
				line(1, 0, 1, 3)
				line(1, 0, 5, 0)
				break;

			case '7':
				line(1, 0, 5, 0)
				line(5, 0, 5, 6)
				break;

			case '8':
				rect(1, 0, 4, 6)
				line(1, 3, 5, 3)
				break;

			case '9':
				rect(1, 0, 4, 3)
				line(5, 3, 5, 6)
				break;

			case '0':
				rect(1, 0, 4, 6)
				point(3, 3)
				break;

			case '-':
				line(1, 3, 5, 3)
				break;

			case '.':
				point(3, 6)
				break;

			case ',':
				line(3, 6, 3, 8)
				break;

			case '\'':
				line(3, 0, 3, 2)
				break;

			case '\"':
				line(2, 0, 2, 2)
				line(4, 0, 4, 2)
				break;

			case '?':
				line(2, 0, 5, 0)
				line(5, 0, 5, 3)
				line(3, 3, 5, 3)
				point(3, 6)
				break;

			case '!':
				line(3, 0, 3, 3)
				point(3, 6)
				break;

			case ':':
				point(3, 3)
				point(3, 6)
				break;

			case ';':
				point(3, 3)
				line(3, 6, 3, 8)
				break;

			case '[':
				line(1, 0, 4, 0)
				line(1, 0, 1, 6)
				line(1, 6, 4, 6)
				break;

			case ']':
				line(2, 0, 5, 0)
				line(5, 0, 5, 6)
				line(2, 6, 5, 6)
				break;

			case '/':
				line(5, 2, 1, 6)
				break;

			case '_':
				line(1, 6, 5, 6)
				break;
				
		}
	}

	void drawstring(cursor* c, xstuff x, char* s) {
		//
		// This function draws a string, automatically advancing the cursor
		// and applying newlines when necessary. Note: This function does
		// not detect edge-of-screen issues!
		//

		int i = 0;

		while (s[i] != '\0') {
			switch (s[i]) {
				case '\n':
					// Erase the rest of the line
					XSetForeground(x.disp, x.gc, background);
					XFillRectangle(x.disp, x.win, x.gc, xpixel((*c)), ypixel((*c)), 1400, letterheight + 1);

					c -> yletter++;
					
				case '\r':
					c -> xletter = 0;
					break;
					
				case 0x02:
					clearscreen(x);
				case 0x01:
					c -> xletter = 0;
					c -> yletter = 0;
					
					break;

				case '~':
					// Erase the rest of the screen.

					if (c -> xletter != 0) {
						c -> xletter = 0;
						c -> yletter++;
					}

					XSetForeground(x.disp, x.gc, background);
					XFillRectangle(x.disp, x.win, x.gc, xpixel((*c)), ypixel((*c)), 1400, 1050);
					
					break;

				case '`':
#ifdef debug
					printf("Update\n");
#endif
					XFlushGC(x.disp, x.gc);
					XFlush(x.disp);
//					XSync(x.disp, 0);
					break;

				default:
					drawletter(*c, x, s[i], 0);
					c -> xletter++;

			}

			i++;
		}
	}

// Main function
	int main(int argc, char** argv) {
		//
		// This routine listens for characters from stdin and
		// prints them to the screen.
		//
		// \001 is the "reset screen" command, and ` (backtick)
		// is the "update" command.
		//
		cursor c;
		cursor* pc = (&c);
		xstuff x;
		int width = 1400 / (letterwidth + 1);
		int height = 1050 / (letterheight + 1);

		x.disp = XOpenDisplay(":0.0");
		x.win = XRootWindow(x.disp, 0);
		x.gc = XDefaultGC(x.disp, 0);

		c.xletter = 0;
		c.yletter = 0;

#ifdef buffered
		char *input = malloc(sizeof(char) * 8192);
		input[8191] = '\0';
		int index = 0;
#else
		char* pin = malloc(sizeof(char) * 2);
		pin[1] = '\0';
#endif
		
		while (! feof(stdin)) {
#ifdef buffered
			if (index < 8191) {
				input[index++] = fgetc(stdin);
				input[index] = '\0';
			}

			if (input[index - 1] == '`') {
				index = 0;
				input[index] = '\0';
				drawstring(pc, x, input);
			}
#else
			pin[0] = fgetc(stdin);
			drawstring(pc, x, pin);
#endif
		
#ifdef debug
			printf("Got a character\n");
#endif
		}

		XCloseDisplay(x.disp);
		return 0;
	}
