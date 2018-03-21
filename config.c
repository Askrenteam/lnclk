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
#include "config.h"

configuration config = {
    .foreground = {65000,65000,65000},
    .background = {0,0,0},
    .unit  = SEC
};

void parse_config_file () {
    char *home = getenv("HOME");
    const char *file_path = "/.config/lnclk/config";
    char *config_path;
    FILE *file = NULL;
    if(home){
        config_path = strcat(home,file_path);
        file = fopen(config_path, "r");
    }
    if (file) {
        char *line;
        size_t len = 0;
        while(getline(&line,&len,file) != -1){
           line = strip_whitespaces(line);
           if(strlen(line)>1 && line[0]!='#'){
               char *key = strtok(line,"=");
               char *val = strtok(NULL,"=");
               if(strcmp(key, "fg") == 0) {
                   config.foreground = hex_to_color (val); 
               } else if (strcmp(key, "bg") == 0) {
                   config.background = hex_to_color (val); 
               } else if (strcmp(key, "tu") == 0) {
                   if (strcmp(val, "seconds") == 0) {
                       config.unit = SEC;
                   } else if (strcmp(val, "minutes") == 0) {
                       config.unit = MIN;
                   } else if (strcmp(val, "hours") == 0) {
                       config.unit = HOUR;
                   }
               } else {
                    printf("Unknown key : %s\n",key);
               }
           }
        }
    } else {
        printf("No config file found.\n");
    }
}

color hex_to_color (char *hex) {
    color col = {0,0,0};
    if ( strlen(hex) == 7 ) {
        char r[5] = {hex[1],'0',hex[2],'0','\0'};
        unsigned short rv = strtol(r, NULL, 16);
        char g[5] = {hex[3],'0',hex[4],'0','\0'};
        unsigned short gv = strtol(g, NULL, 16);
        char b[5] = {hex[5],'0',hex[6],'0','\0'};
        unsigned short bv = strtol(b, NULL, 16);
        col = (color){rv, gv, bv};
    } else {
        printf("Error parsing color %s\n",hex);
    }   
    return col;
}

char* strip_whitespaces(char *in) {
    char *res = strdup(in);
    int j=0;
    for( unsigned int i=0 ; i<strlen(in) ; i++){
        if(in[i]!=' ' && in[i]!='\t' && in[i]!='\n') {
           res[j] = in[i]; 
           j++;
        }
    }
    res[j]='\0';
    return res;
}
