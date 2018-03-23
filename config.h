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

#ifndef _CONFIG
#define _CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct color {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
} color;

typedef enum time_unit {SEC, MIN, HOUR} time_unit;

typedef struct configuration {
    color foreground;
    color background;
    time_unit unit;
    char *config_file;
} configuration;

extern configuration config ;

//Parse config file located at ~/.config/lnclk/config
void parse_config_file ();

//Convert a #rrggbb color to a color struct with 16-bit channels
color hex_to_color (char *hex);

//Strip spaces, tabs, and newlines from a string
char* strip_whitespaces(char *in);

//Parse command-line options
void get_opts (int argc, char **argv);

//TODO : alpha channel ?

#endif
