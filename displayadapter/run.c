/*
 * Here is the layout of this file:
 *
 * - Format description
 * - Global data
 * - Error-handling routines
 * - Parsing routines
 * - Main function
 */

/*
 * Format description
 * ------------------
 *
 * There are several basic commands. When they take parameters, those parameters
 * will be in the form of hexadecimal digits, one after another, either in bytes
 * or words.
 *
 * Each command is made up of a single character. Here are the commands that are
 * currently supported:
 *
 * C rrggbb: Change color
 * P xxxxyyyy: Set pixel
 * L xxxxyyyyxxxxyyyy: Draw line
 * R xxxxyyyywwwwhhhh: Draw rectangle
 * F xxxxyyyywwwwhhhh: Fill rectangle
 *
 * U: Update display
 * Q: Quit application
 *
 * All hexadecimal digits must be lower-case.
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <unistd.h>

/*
 * This define enables messages in the program code that can be used to trace
 * execution. It does not change the execution of the program beyond these messages.
 */

//#define DEBUG

// Global data
  static const char HexadecimalDigits[] = {'0', '1', '2', '3', '4', '5', '6', '7',
             '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

// Error-handling routines
  void errorBadByteValue(void) {
    fprintf(stderr, "A bad byte value was found.\n");
  }

  void errorBadWordValue(void) {
    fprintf(stderr, "A bad word value was found.\n");
  }

  void errorBadOperation(void) {
    fprintf(stderr, "A bad operation code was found.\n");
  }

// Parsing routines
  int locationIn(char c, const char digits[], int numberofdigits) {
    // This function returns the position of C in the digit array.

    int i = 0;

    while (i < numberofdigits) {
      if (c == digits[i]) {
        return i;
      }

      i++;
    }

    return -1;
  }

  int parseHexadecimal(FILE* input, int numberofdigits) {
    // This routine creates a number out of the hexadecimal
    // digits on the input stream. It reads the number of digits
    // and creates the total from those digits. -1 is returned
    // if any character from the input stream is invalid.

    int total    = 0;
    int multiplier    = 0x1 << (numberofdigits * 4 - 4);
    int immediate    = 0;
    int character    = 0;

    while ((multiplier > 0) && ! feof(input)) {
      // We use the immediate variable to handle exceptional
      // cases of non-hexadecimal characters.
      character = fgetc(input);

      if (character != ' ' && character != '\n') {
        immediate = locationIn(character, HexadecimalDigits, 16);

        // -1 is the sentinel value for a non-hex character.
        if (immediate == -1) {
          return -1;
        } else {
          // Add the value to the total.
          total += multiplier * immediate;
        }

        #ifdef DEBUG
          printf("    Hex converter: total = %x, multiplier = %x, immediate = %x\n", total, multiplier, immediate);
        #endif

        // Downshift the multiplier to be one hex-digit (half-byte)
        // less so that it is accurate for the next position.
        multiplier >>= 4;
      }
    }

    #ifdef DEBUG
      printf("Converted hex value of %x\n", total);
    #endif

    return total;
  }

  int parseByte(FILE* input) {
    // A shortcut for a two-digit hexadecimal parse.
    return parseHexadecimal(input, 2);
  }

  int parseWord(FILE* input) {
    // A shortcut for a four-digit hexadecimal parse.
    return parseHexadecimal(input, 4);
  }

  void parseChangeColor(FILE* input, Display* pDisplay, GC TheGC) {
    // Here, we read three bytes for the R, G, and B values, and then
    // total them up into one big int value.

    int red = parseByte(input);
    int green = parseByte(input);
    int blue = parseByte(input);

    #ifdef DEBUG
      printf("Received change-color operator\n");
    #endif

    // While we could have done something like
    // if ((red | green | blue) & (1 << 31) != 0),
    // I think that this is far more readable.
    if ((red == -1) || (green == -1) || (blue == -1)) {
      errorBadByteValue();
    } else {
      // Set the color appropriately.
      XSetForeground(pDisplay, TheGC, (red << 16) + (green << 8) + blue);
    }
  }

  void parsePixel(FILE* input, Display* pDisplay, Drawable Win, GC TheGC) {
    // Here, we read two words for the X and Y values and then fill
    // a pixel.

    int x = parseWord(input);
    int y = parseWord(input);

    #ifdef DEBUG
      printf("Received pixel operator\n");
    #endif

    if ((x == -1) || (y == -1)) {
      errorBadWordValue();
    } else {
      // Render the pixel.
      XDrawPoint(pDisplay, Win, TheGC, x, y);
    }
  }

  void parseLine(FILE* input, Display* pDisplay, Drawable Win, GC TheGC) {
    // We need four values here. They are, in order, X1, Y1, X2, and Y2.

    int x1 = parseWord(input);
    int y1 = parseWord(input);
    int x2 = parseWord(input);
    int y2 = parseWord(input);

    #ifdef DEBUG
      printf("Received line operator\n");
    #endif

    if ((x1 == -1) || (y1 == -1) || (x2 == -1) || (y2 == -1)) {
      errorBadWordValue();
    } else {
      // Draw the line.
      XDrawLine(pDisplay, Win, TheGC, x1, y1, x2, y2);
    }
  }

  void parseRectangle(FILE* input, Display* pDisplay, Drawable Win, GC TheGC) {
    // Here, we also need four values. They are X, Y, width, and height.

    int x = parseWord(input);
    int y = parseWord(input);
    int w = parseWord(input);
    int h = parseWord(input);

    #ifdef DEBUG
      printf("Received rectangle operator\n");
    #endif
    
    if ((x == -1) || (y == -1) || (w == -1) || (h == -1)) {
      errorBadWordValue();
    } else {
      // Draw the rectangle.
      XDrawRectangle(pDisplay, Win, TheGC, x, y, w, h);
    }
  }

  void parseFillRectangle(FILE* input, Display* pDisplay, Drawable Win, GC TheGC) {
    // And again, we need four values. They are the same ones that we need
    // to draw a rectangle (parseRectangle()).

    int x = parseWord(input);
    int y = parseWord(input);
    int w = parseWord(input);
    int h = parseWord(input);

    #ifdef DEBUG
      printf("Received fill rectangle operator\n");
    #endif

    if ((x == -1) || (y == -1) || (w == -1) || (h == -1)) {
      errorBadWordValue();
    } else {
      // Fill the rectangle.
      XFillRectangle(pDisplay, Win, TheGC, x, y, w, h);
    }
  }

  int parseOperation(FILE* input, Display* pDisplay, Pixmap Background, Drawable Win, GC TheGC) {
    // Here, we read a character and figure out which operation
    // it represents. The table is listed above.
    // If the program should exit, then we return a nonzero number.

    switch (fgetc(input)) {
      case 'C':
        parseChangeColor(input, pDisplay, TheGC);
        break;

      case 'P':
        parsePixel(input, pDisplay, Background, TheGC);
        break;

      case 'L':
        parseLine(input, pDisplay, Background, TheGC);
        break;

      case 'R':
        parseRectangle(input, pDisplay, Background, TheGC);
        break;

      case 'F':
        parseFillRectangle(input, pDisplay, Background, TheGC);
        break;

      case 'U':
        #ifdef DEBUG
          printf("Received update operator\n");
        #endif

        XSetWindowBackgroundPixmap(pDisplay, Win, Background);
        XCopyArea(pDisplay, Background, Win, TheGC, 0, 0, 1280, 800, 0, 0);
        
        XFlushGC(pDisplay, TheGC);
        XFlush(pDisplay);
        break;

      case 'Q':
        return -1;

      case '\n':
      case ' ':
        // Do nothing. We allow newlines in our input.
        break;

      default:
        errorBadOperation();
        break;

    }

    return 0;
  }

// Main function
  int main(int argc, char** argv) {
    //
    // This routine listens for characters on the standard input
    // and processes them as they come along, with no buffering to
    // speak of.
    //

    Display*  pDisplay  = NULL;
    Drawable  Win;
    Pixmap    Background;
    GC    TheGC;
    int    quit    = 0;
    int    window_id;

    // Step 1. Initialize the X stuff so that we can commence
    // g drawing operations.
    pDisplay = XOpenDisplay(getenv("DISPLAY"));

    if (argc > 1)
      window_id  = atoi (argv[1]);
    else
      window_id = XRootWindow (pDisplay, 0);
    
    printf ("Using window id %d\n", window_id);

    Win = window_id;
    Background = XCreatePixmap(pDisplay, Win, 1280, 800, 24);
    TheGC = XDefaultGC(pDisplay, 0);
    
    XSetWindowBackground(pDisplay, Win, Background);
    
    // Step 2. Begin the processing loop.
    while (! feof(stdin) && ! quit) {
      quit = parseOperation(stdin, pDisplay, Background, Win, TheGC);
    }

    // Step 3. Clean up by closing the display; then, return non-error status.
    XFreePixmap(pDisplay, Background);
    XCloseDisplay(pDisplay);
    return 0;
  }
