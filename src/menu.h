#ifndef __MENU_H__
#define __MENU_H__


#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdarg.h>

#include "logs.h"
#include "label.h"
#include "drawing.h"


typedef struct MENU_BUTTON {
        int id;

        float x;
        float y;
        float w;
        float h;

        SDL_Color bg_color;
        SDL_Color trigger_color;

        bool triggered;

        LABEL *label;
        void* (*function)(void *args);

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
        
        int32_t border;
        int32_t border_radius;
        SDL_Color border_color;

        SDL_Color bg_color;
        
        MENU_BUTTON *buttons;
} MENU;


MENU *Menu_New( SDL_Renderer *renderer, 
                int32_t width, int32_t height, 
                SDL_Color background, int32_t border_radius,
                int32_t border, SDL_Color border_color );
         
                
void Menu_Render(MENU *menu);
void Menu_Free(MENU *menu);
bool Menu_CheckUpdate(MENU *menu, float mouse_x, float mouse_y, bool click);
void Menu_Move(MENU *menu, float x, float y, float window_w, float window_h);


MENU_BUTTON *Menu_SetButton( MENU *menu, int id,
                    int32_t width, int32_t height, 
                    int32_t x, int32_t y, 
                    SDL_Color background, SDL_Color trigger_color,
                    LABEL *label, void* (*function)(void *menu) );


bool Menu_DelButton(MENU *menu, MENU_BUTTON *button);






#endif // __MENU_H__