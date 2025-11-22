#include "menu.h"


float fix_cord(float x, float w, float app_x) {
        float d = x + w - app_x;
        if ( d > 0 ) {
                if ( d > 30 ) {
                        return x - w;
                } else {
                        return app_x - w;
                }
        } else {
                return x;
        }
}

MENU *Menu_New( SDL_Renderer *renderer, 
                int32_t width, int32_t height, 
                SDL_Color background, int32_t border_radius,
                int32_t border, SDL_Color border_color ) 
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
        menu->border = border;
        menu->border_radius = border_radius;

        menu->renderer = renderer;
        menu->active = 0;
        menu->buttons = NULL;

        return menu;
}



// Render functions
void __Menu_DrawButton(SDL_Renderer *renderer, float menu_x, float menu_y, MENU_BUTTON *button ) {
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
        if ( button->triggered ) {
                SDL_SetRenderDrawColor(renderer, button->trigger_color.r, button->trigger_color.g,
                                             button->trigger_color.b, button->trigger_color.a );
        } else {
                SDL_SetRenderDrawColor(renderer, button->bg_color.r, button->bg_color.g,
                                             button->bg_color.b, button->bg_color.a );
        }
        SDL_RenderFillRect(renderer, &rect);SDL_SetRenderDrawColor(renderer, button->bg_color.r, button->bg_color.g,
                                             button->bg_color.b, button->bg_color.a );

        Label_Draw(button->label, NULL, &label);
}

void __Menu_DrawBorder(SDL_Renderer *renderer, SDL_FRect border, int32_t border_width, int32_t border_radius) {
        SDL_FRect now_rect = {
                border.x,
                border.y + border_radius,
                border_width,
                border.h - 2 * border_radius
        };

        SDL_RenderFillRect(renderer, &now_rect);
        
        now_rect = (SDL_FRect){
                border.x + border_radius,
                border.y,
                border.w - 2 * border_radius,
                border_width + border_radius
        };

        SDL_RenderFillRect(renderer, &now_rect);

        now_rect = (SDL_FRect){
                border.x + border_radius,
                border.y,
                border.w - 2 * border_radius,
                border_width
        };

        SDL_RenderFillRect(renderer, &now_rect);

        now_rect = (SDL_FRect){
                border.x + border_radius,
                border.y + border.h - border_width - border_radius,
                border.w - 2 * border_radius,
                border_width
        };

        SDL_RenderFillRect(renderer, &now_rect);

        now_rect = (SDL_FRect){
                border.x + border.w - border_width,
                border.y + border_radius,
                border_width,
                border.w - 2 * border_radius
        };

        SDL_RenderFillRect(renderer, &now_rect);

        // Circle midpoint algoritm
        int32_t x_left = border.x + border_radius;
        int32_t y_up = border.y + border_radius;
        int32_t x_right = border.x + border.w - border_radius - 1;
        int32_t y_bottom = border.y + border.h - border_radius - 1;
       
        int32_t x = border_radius;
        int32_t y = 0;
        int32_t err = 0;

        SDL_RenderLine(renderer, x_left - x, y_up - y, x_right + x, y_up - y);
        SDL_RenderLine(renderer, x_left - x, y_bottom + y, x_right + x, y_bottom + y);

        while (x >= y)
        {

        	if (err <= 0)
        	{
        	    y += 1;
        	    err += 2*y + 1;
        	}

        	if (err > 0)
        	{
        	    x -= 1;
        	    err -= 2*x + 1;
        	}

                SDL_RenderLine(renderer, x_left - x, y_up - y, x_right + x, y_up - y);
                SDL_RenderLine(renderer, x_left - x, y_bottom + y, x_right + x, y_bottom + y);
                SDL_RenderLine(renderer, x_left - y, y_up - x, x_right + y, y_up - x);
                SDL_RenderLine(renderer, x_left - y, y_bottom + x, x_right + y, y_bottom + x);
        }
}

void Menu_Render(MENU *menu) {
        if ( NULL == menu || menu->active == 0) 
                return;

        float x = menu->x;
        float y = menu->y;
        
        
        

        SDL_FRect rect = (SDL_FRect){
                x, y, 
                menu->w, menu->h
        };

        SDL_FRect border = (SDL_FRect){
                x - menu->border,
                y - menu->border,
                rect.w + 2 * menu->border,
                rect.h + 2 * menu->border
        };

        SDL_SetRenderDrawColor( menu->renderer, menu->border_color.r, menu->border_color.g,
                                     menu->border_color.b, menu->border_color.a );

        Render_RounderRect(menu->renderer, border, menu->border_radius);

        SDL_SetRenderDrawColor( menu->renderer, menu->bg_color.r, menu->bg_color.g,
                                     menu->bg_color.b, menu->bg_color.a );
        Render_RounderRect(menu->renderer, rect, menu->border_radius);

        MENU_BUTTON *now = menu->buttons;
        while ( now ) {
                __Menu_DrawButton(menu->renderer, x, y, now);
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
                    int32_t width, int32_t height, 
                    int32_t x, int32_t y, 
                    SDL_Color background, SDL_Color trigger_color,
                    LABEL *label, void* (*function)(void*) ) 
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
        
        button->bg_color = background;
        button->trigger_color = trigger_color;
        button->triggered = 0;

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



bool Menu_CheckUpdate(MENU *menu, float mouse_x, float mouse_y, bool click) {
        if ( NULL == menu || menu->active == 0 ) 
                return 0;
        
        
        bool res = 0;

        bool mouse_out_menu = mouse_x < menu->x - menu->border || mouse_y < menu->y - menu->border || mouse_x > menu->x + menu->w + menu->border || mouse_y > menu->y + menu->h + menu->border;
        if ( mouse_out_menu && click ) {
                menu->active = 0;
                res = 1;
        }


        mouse_x -= menu->x;
        mouse_y -= menu->y;

        MENU_BUTTON *now = menu->buttons;
        while ( now ) {
                if ( mouse_out_menu  ) {
                        if ( now->triggered )
                                res = 1;
                        now->triggered = 0;
                        now = now->next;
                        continue;
                }

                if ( mouse_x > now->x && mouse_x < now->x + now->w &&
                     mouse_y > now->y && mouse_y < now->y + now->h ) {
                        if ( now->triggered == 0 ) {
                                res = 1;
                                now->triggered = 1;
                        }

                        if ( click ) {
                                res = 1;
                                now->function(menu);
                        }
                } else {
                        if ( now->triggered == 1 ) {
                                res = 1;
                                now->triggered = 0;
                        }
                }

                now = now->next;
        }

        return res;
}



void Menu_Move(MENU *menu, float x, float y, float window_w, float window_h) {
        if ( menu == NULL ) 
                return;

        menu->x = fix_cord(x, menu->border + menu->w, window_w);
        menu->y = fix_cord(y, menu->border + menu->h, window_h);
}