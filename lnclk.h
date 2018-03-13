/* Copyright © 2018, R. Meurisse 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished 
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or 
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. In no event shall the 
 * authors or copyright holders be liable for any claim, damages or other 
 * liability, whether in an action of contract, tort or otherwise, arising from, 
 * out of or in connection with the software or the use or other dealings in the Software.
 *
*/

#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

typedef struct xcb_info {
    xcb_connection_t        *connection;
    xcb_screen_t            *screen;
    xcb_window_t            window;
    xcb_ewmh_connection_t   *ewmh;
    xcb_gcontext_t          foreground;
    xcb_gcontext_t          background;
    xcb_colormap_t          colormap;
} xcb_info;
typedef struct color color;
//Pointer to global var containing all xcb-related stuff
extern xcb_info *xcb;

//Disable window decorations by setting MOTIF_WM_HINTS
void x11_disable_decoration ( xcb_connection_t *connection, xcb_window_t window );

//Create EWMH atoms used to set _NET_WM_WINDOW_TYPE hints to dock
void ewmh_create_atoms ( xcb_connection_t *connection, xcb_ewmh_connection_t *ewmh );

//Create the xcb_connection and screen
void xcb_init ( );

//Set the foreground color of a context
void set_context_color (xcb_gcontext_t gcontext,color col);

//Get the x for a given time
int get_x_progress (struct tm *timeinfo);
