#include <stdio.h>
#include<windows.h>
#include<conio.h>
 
#define MAX_SIZE 12
#define XPOS 50
#define YPOS 5
 
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
 
 int chr;
chr = GetKey();
    
    if (chr == 0 || chr == 0xe0) 
        chr = GetKey();
        switch (chr)
        {
            case UP: 
                break;
 
            case DOWN:
                break;
 
            case LEFT:   
                break;
 
            case RIGHT:          
                break;
        }
    }
                            
