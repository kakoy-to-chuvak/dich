#ifndef __MENU_H__
#define __MENU_H__


#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "logs.h"
#include "label.h"


typedef struct MENU_BUTTON {
        int id;

        float x;
        float y;
        float w;
        float h;

        uint32_t border_radius;
        SDL_Color border_color;
        SDL_Color bg_color;

        LABEL *label;
        void* (*function)(void *menu);

        struct MENU_BUTTON *next;
        struct MENU_BUTTON *prev;
} MENU_BUTTON; 


typedef struct MENU {
        SDL_Renderer *renderer;

        bool active;

        float x;
        float y;
        float w;
        float h;
        
        uint32_t border_radius;
        SDL_Color border_color;
        SDL_Color bg_color;
        
        MENU_BUTTON *buttons;
} MENU;


MENU *Menu_New( SDL_Renderer *renderer, 
                uint32_t width, uint32_t height, 
                SDL_Color background, 
                uint32_t border_radius, SDL_Color border_color );
                
void Menu_Draw(MENU *menu, float x, float y);
void Menu_Free(MENU *menu);


MENU_BUTTON *Menu_SetButton( MENU *menu, int id,
                    uint32_t width, uint32_t height, 
                    int32_t x, int32_t y, 
                    SDL_Color background, 
                    uint32_t border_radius, SDL_Color border_color,
                    LABEL *label, void* (*function)(void *menu) );

bool Menu_DelButton(MENU *menu, MENU_BUTTON *button);



bool Menu_CheckUpdate(MENU *menu, float mouse_x, float mouse_y, bool mouse_down);





#endif // __MENU_H__