// set up and initialization macro for X11 graphics. B. Wilkinson June 1, 2015
// x_resn, y_resn are the resolutions in x and y direction provided by programmer

#include <X11/Xlib.h>		// X11 library headers
#include <X11/Xutil.h>
#include <X11/Xos.h>

#define initX11(x_resn,y_resn) \
Window		win;\
unsigned int width, height,win_x,win_y,border_width,display_width, display_height,screen;\
char		*window_name = "N-body with X11 graphics, Nbody-G.c", *display_name = NULL;\
GC		gc;\
unsigned long	valuemask = 0;\
XGCValues	values;\
Display		*display;\
XSizeHints	size_hints;\
Pixmap		bitmap;\
XPoint		points[800];\
XSetWindowAttributes attr[1];\
if ((display = XOpenDisplay (display_name)) == NULL ) {  \
	fprintf (stderr, "drawon: cannot connect to X server %s\n",XDisplayName (display_name) );\
	exit (-1);\
}\
screen = DefaultScreen (display);\
display_width = DisplayWidth (display, screen);\
display_height = DisplayHeight (display, screen);\
width = x_resn;\
height = y_resn;\
win_x = 0;\
win_y = 0;\
border_width = 4;\
win = XCreateSimpleWindow (display, RootWindow (display, screen),win_x, win_y, width, height, border_width,BlackPixel (display, screen), WhitePixel(display, screen));\
size_hints.flags = USPosition|USSize;\
size_hints.x = win_x;\
size_hints.y = win_y;\
size_hints.width = width;\
size_hints.height = height;\
size_hints.min_width = 300;\
size_hints.min_height = 300;\
XSetNormalHints (display, win, &size_hints);\
XStoreName(display, win, window_name);\
gc = XCreateGC (display, win, valuemask, &values);\
XSetBackground (display, gc, WhitePixel (display, screen));\
XSetForeground (display, gc, BlackPixel (display, screen));\
XSetLineAttributes (display, gc, 1, LineSolid, CapRound, JoinRound);\
attr[0].backing_store = Always;\
attr[0].backing_planes = 1;\
attr[0].backing_pixel = BlackPixel(display, screen);\
XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);\
XMapWindow (display, win);\
XSync(display, 0);\
usleep(1000) 


#define BLACK (long) 0x000000
#define BLUE (long) 0x0000FF
#define BROWN (long) 0xA52A2A 
#define CYAN (long) 0x00FFFF
#define GRAY (long) 0xBEBEBE
#define GREEN (long) 0x00FF00
#define MAGENTA (long) 0xFF00FF
#define ORANGE (long) 0xFFA500
#define PINK (long) 0xFFC0CB
#define PURPLE (long) 0xA020F0
#define RED (long) 0xFF0000
#define TURQUOISE (long) 0x40E0D0
#define VIOLET (long) 0xEE82EE
#define WHITE (long) 0xFFFFFF
#define YELLOW (long) 0xFFFF00


