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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#include "lnclk.h"
#include "config.h"

xcb_info xcb_empty = {
    .connection = NULL,
    .screen = NULL,
    .ewmh = NULL
};
xcb_info *xcb = &xcb_empty;

int main () {
    xcb_init();

    parse_config_file();

    time_t timestamp;
    struct tm *timeinfo;
    time (&timestamp);
    timeinfo = localtime(&timestamp);

    xcb_point_t fg_points[] = {{0,0},{0,0}};
    xcb_point_t bg_points[] = {{0,0},{(int16_t)xcb->screen->width_in_pixels,0}};

    xcb_ewmh_connection_t ewmh = {
        .connection = xcb->connection,
        .screens = &xcb->screen,
        .nb_screens = 1,
    };
    ewmh_create_atoms ( xcb->connection, &ewmh );
    xcb_atom_t winType = ewmh._NET_WM_WINDOW_TYPE_DOCK;
    xcb_ewmh_set_wm_window_type(&ewmh, xcb->window, 1, &winType);

    x11_disable_decoration( xcb->connection, xcb->window );                     

    set_context_color(xcb->foreground, config.foreground);
    set_context_color(xcb->background, config.background);
    /* Map the window on the screen */
    xcb_map_window (xcb->connection, xcb->window);

    xcb_flush (xcb->connection);

    xcb_generic_event_t *event;
    int loop=1;
    while (loop) {
        time(&timestamp);
        timeinfo = localtime(&timestamp);
        fg_points[1].x = get_x_progress(timeinfo);
        bg_points[0].x = get_x_progress(timeinfo);
        xcb_poly_line (xcb->connection, XCB_COORD_MODE_ORIGIN, xcb->window, xcb->foreground, 2, fg_points);
        xcb_poly_line (xcb->connection, XCB_COORD_MODE_ORIGIN, xcb->window, xcb->background, 2, bg_points);
        xcb_flush(xcb->connection);
        sleep(1);
        event = xcb_poll_for_event (xcb->connection);
        if(event) {
            switch (event->response_type & ~0x80) {
                case XCB_EXPOSE:
                    /* We draw the polygonal line */
                    xcb_poly_line (xcb->connection, XCB_COORD_MODE_ORIGIN, xcb->window, xcb->foreground, 2, fg_points);
                    xcb_poly_line (xcb->connection, XCB_COORD_MODE_ORIGIN, xcb->window, xcb->background, 2, bg_points);
                    xcb_flush(xcb->connection);
                    break;
                case XCB_DESTROY_NOTIFY:
                    loop = 0;    
                    break;
                default: 
                    /* Unknown event type, ignore it */
                    break;
            }
        }
        free(event);

    }

    xcb_disconnect (xcb->connection);

    return 0;
}

void x11_disable_decoration ( xcb_connection_t *connection, xcb_window_t window )
{
    // Flag used to indicate we are setting the decoration type.
    const uint32_t MWM_HINTS_DECORATIONS = ( 1 << 1 );
    // Motif property data structure
    struct MotifWMHints
    {
        uint32_t flags;
        uint32_t functions;
        uint32_t decorations;
        int32_t  inputMode;
        uint32_t state;
    };

    struct MotifWMHints hints;
    hints.flags       = MWM_HINTS_DECORATIONS;
    hints.decorations = 0;
    hints.functions   = 0;
    hints.inputMode   = 0;
    hints.state       = 0;
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom ( connection, 0, strlen("_MOTIF_WM_HINTS"), "_MOTIF_WM_HINTS" );
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply ( connection, cookie, NULL );
    if ( reply ) {
        xcb_atom_t ha = reply->atom;
        xcb_change_property ( connection, XCB_PROP_MODE_REPLACE, window, ha, ha, 32, 5, &hints );
    }
}

void ewmh_create_atoms ( xcb_connection_t *connection, xcb_ewmh_connection_t *ewmh ) {
    xcb_intern_atom_cookie_t *cookie = xcb_ewmh_init_atoms ( connection, ewmh );
    xcb_generic_error_t      *errors = NULL;
    xcb_ewmh_init_atoms_replies ( ewmh, cookie, &errors );
    if ( errors ) {
        printf( "That's embarrassing, we failed to create EWMH atoms...\n" );
        free ( errors );
    }
}

void xcb_init () {
    /* Open the connection to the X server */
    xcb->connection = xcb_connect (NULL, NULL);

    /* Get the first screen */
    const xcb_setup_t      *setup  = xcb_get_setup (xcb->connection);
    xcb_screen_iterator_t   iter   = xcb_setup_roots_iterator (setup);
    xcb->screen = iter.data;

    /* Create Foreground Graphic Context */
    xcb->window = xcb->screen->root;
    xcb->foreground = xcb_generate_id(xcb->connection);
    uint32_t        mask       = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t        values[2]  = {xcb->screen->white_pixel, 0};

    xcb_create_gc (xcb->connection,
            xcb->foreground,
            xcb->window,
            mask,
            values);

    /* Create Background Graphic Context */
    xcb->background = xcb_generate_id(xcb->connection);
    values[0]  = xcb->screen->black_pixel;

    xcb_create_gc (xcb->connection,
            xcb->background,
            xcb->window,
            mask,
            values);

    /*Create the window*/
    xcb->window = xcb_generate_id (xcb->connection);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = xcb->screen->black_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE ;
    xcb_create_window (xcb->connection,                    /* Connection          */
            XCB_COPY_FROM_PARENT,          /* depth (same as root) */
            xcb->window,                        /* window Id           */
            xcb->screen->root,                  /* parent window       */
            0, 0,                          /* x, y                */
            xcb->screen->width_in_pixels, 1,                      /* width, height       */
            0,                            /* border_width        */
            XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
            xcb->screen->root_visual,           /* visual              */
            mask, values );                     /* masks, not used yet */


    /* Initiate colormap as the default one */
    xcb->colormap = xcb->screen->default_colormap;
}

void set_context_color (xcb_gcontext_t gcontext, color col) {
   xcb_alloc_color_reply_t *reply = xcb_alloc_color_reply (xcb->connection,
                                                           xcb_alloc_color (xcb->connection, xcb->colormap, col.red, col.green, col.blue),
                                                           NULL);
    if (reply) {
        xcb_change_gc(xcb->connection,
                      gcontext,
                      XCB_GC_FOREGROUND,
                      &reply->pixel);
    }
}

int get_x_progress (struct tm *timeinfo) {
    int x = 0;
    switch (config.unit) {
        case SEC:
            x = (int)(((double)timeinfo->tm_sec/60)*xcb->screen->width_in_pixels);
            break;
        case MIN:
            x = (int)(((double)timeinfo->tm_min/60)*xcb->screen->width_in_pixels);
            break;
        case HOUR:
            x = (int)(((double)timeinfo->tm_hour/24)*xcb->screen->width_in_pixels);
            break;
    }
    return x;
}
