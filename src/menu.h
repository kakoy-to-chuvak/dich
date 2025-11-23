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

        bool hide;
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
        
        int32_t border_width;
        int32_t border_radius;
        
        SDL_Color border_color;
        SDL_Color bg_color;

        int32_t buttons_count;
        MENU_BUTTON *buttons;
        
        SDL_Color button_bg_color;
        SDL_Color trigger_color;

        int32_t text_indent_w;
        int32_t text_indent_h;

        int32_t button_w;
        int32_t button_h;
        int32_t button_indent_w;
        int32_t button_indent_h;
        float button_radius;
} MENU;


MENU *Menu_New( SDL_Renderer *renderer, 
                SDL_Color background, int32_t border_radius,
                int32_t border, SDL_Color border_color );
         
void Menu_SetupButtons( MENU *menu, float radius,
                        int32_t width, int32_t height,
                        SDL_Color background, SDL_Color trigger_color,
                        int32_t indent_w, int32_t indent_h,
                        int32_t text_indent_w, int32_t text_indent_h);
                
void Menu_Render(MENU *menu);
void Menu_Free(MENU *menu);
bool Menu_MouseOut(MENU *menu, int32_t mouse_x, int32_t mouse_y);
bool Menu_CheckUpdate(MENU *menu, float mouse_x, float mouse_y, bool click);
void Menu_Move(MENU *menu, float x, float y, float window_w, float window_h);


MENU_BUTTON *Menu_SetButton(    MENU *menu, int id,
                                LABEL *label, void* (*function)(void*) );


bool Menu_DelButton(MENU *menu, MENU_BUTTON *button);
void Menu_ButtonHide(MENU *menu, MENU_BUTTON *button, bool hide);






#endif // __MENU_H__