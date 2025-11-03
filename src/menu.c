#include "menu.h"



MENU *Menu_New( SDL_Renderer *renderer, 
                uint32_t width, uint32_t height, 
                SDL_Color background, 
                uint32_t border_radius, SDL_Color border_color ) 
{

        MENU *menu = malloc(sizeof(MENU));
        if ( NULL == menu )
                return NULL;

        menu->h = height;
        menu->w = width;
        menu->x = 0;
        menu->y = 0;
        
        menu->bg_color = background;
        menu->border_color = border_color;
        menu->border_radius = border_radius;

        menu->renderer = renderer;
        menu->active = 0;
        menu->buttons = NULL;

        return menu;
}



void Menu_DrawButton(SDL_Renderer *renderer, float menu_x, float menu_y, MENU_BUTTON *button ) {
        SDL_FRect rect = (SDL_FRect){
                menu_x + button->x, menu_y + button->y, 
                button->w, button->h
        };

        SDL_FRect label = (SDL_FRect){
                rect.x + (rect.w - button->label->rect.w) / 2,
                rect.y + (rect.h - button->label->rect.h) / 2 ,
                button->label->rect.w,
                button->label->rect.h
        };


        SDL_FRect border = (SDL_FRect){
                rect.x - button->border_radius,
                rect.y - button->border_radius,
                rect.w + 2 * button->border_radius,
                rect.h + 2 * button->border_radius
        };

        SDL_SetRenderDrawColor(renderer, button->border_color.r, button->border_color.g,
                                     button->border_color.b, button->border_color.a );
        SDL_RenderFillRect(renderer, &border);

        SDL_SetRenderDrawColor(renderer, button->bg_color.r, button->bg_color.g,
                                     button->bg_color.b, button->bg_color.a );
        SDL_RenderFillRect(renderer, &rect);

        Label_Draw(button->label, NULL, &label);
}


void Menu_Draw(MENU *menu, float x, float y) {
        if ( NULL == menu || menu->active == 0) 
                return;
        
        menu->x = x;
        menu->y = y;

        SDL_FRect rect = (SDL_FRect){
                x, y, 
                menu->w, menu->h
        };

        SDL_FRect border = (SDL_FRect){
                x - menu->border_radius,
                y - menu->border_radius,
                rect.w + 2 * menu->border_radius,
                rect.h + 2 * menu->border_radius
        };

        SDL_SetRenderDrawColor( menu->renderer, menu->border_color.r, menu->border_color.g,
                                     menu->border_color.b, menu->border_color.a );
        SDL_RenderFillRect(menu->renderer, &border);

        SDL_SetRenderDrawColor( menu->renderer, menu->bg_color.r, menu->bg_color.g,
                                     menu->bg_color.b, menu->bg_color.a );
        SDL_RenderFillRect(menu->renderer, &rect);

        MENU_BUTTON *now = menu->buttons;
        while ( now ) {
                Menu_DrawButton(menu->renderer, x, y, now);
                now = now->next;
        }
}


void Menu_Free(MENU *menu) {
        if ( NULL == menu ) 
                return;

        MENU_BUTTON *now = menu->buttons;
        while ( now ) {
                MENU_BUTTON *tmp = now->next;
                free(now);
                now = tmp;
        }

        free(menu);
}


MENU_BUTTON *Menu_GetButton(MENU *menu, int id) {
        MENU_BUTTON *now = menu->buttons;
        while ( now ) {
                if ( now->id == id )
                        return now;
                now = now->next;
        }

        return NULL;
}



MENU_BUTTON *Menu_SetButton( MENU *menu, int id,
                    uint32_t width, uint32_t height, 
                    int32_t x, int32_t y, 
                    SDL_Color background, 
                    uint32_t border_radius, SDL_Color border_color,
                    LABEL *label, void* (*function)(void *menu) ) 
{       
        MENU_BUTTON *button = Menu_GetButton(menu, id);

        if ( button == NULL) {
                button = malloc(sizeof(MENU_BUTTON));
                if ( button == NULL)
                        return NULL;

                if ( menu->buttons ) 
                        menu->buttons->prev = button;
                button->prev = NULL;
                button->next = menu->buttons;
                menu->buttons = button;

                button->id = id;
        }

        button->border_color = border_color;
        button->border_radius = border_radius;
        
        button->bg_color = background;
        button->function =  function;
        button->label = label;

        button->h = height;
        button->w = width;
        button->x = x;
        button->y = y;      

        return button;
}



bool Menu_DelButton(MENU *menu, MENU_BUTTON *button) {
        if ( NULL == menu || NULL == button ) 
                return 0;
        
        button->prev->next = button->next;
        button->next->prev = button->prev;

        free(button);

        return 1;
}



bool Menu_CheckUpdate(MENU *menu, float mouse_x, float mouse_y, bool mouse_down) {
        if ( NULL == menu || 0 == mouse_down /* ||*menu->active == 0*/ ) 
                return 0;

        if (    mouse_x < menu->x || mouse_y < menu->y ||
                mouse_x > menu->x + menu->w || mouse_y > menu->y + menu->h 
        ) 
                return 0;

        bool res = 0;

        mouse_x -= menu->x;
        mouse_y -= menu->y;

        MENU_BUTTON *now = menu->buttons;
        while ( now ) {
                if ( mouse_x > now->x && mouse_x < now->x + now->w &&
                     mouse_y > now->y && mouse_y < now->y + now->h ) {
                        res = 1;
                        now->function(menu);
                }

                now = now->next;
        }

        return res;
}