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
                SDL_Color background, int32_t border_radius,
                int32_t border, SDL_Color border_color ) 
{

        MENU *menu = malloc(sizeof(MENU));
        if ( NULL == menu )
                return NULL;

        menu->h = 0;
        menu->w = 0;
        menu->x = 0;
        menu->y = 0;
        
        menu->bg_color = background;
        menu->border_color = border_color;
        menu->border_width = border;
        menu->border_radius = border_radius;

        menu->renderer = renderer;
        menu->active = 0;
        menu->buttons = NULL;

        menu->button_h = 0;
        menu->button_w = 0;

        menu->w = 0;
        menu->h = 0;

        menu->button_indent_h = 0;
        menu->button_indent_w = 0;

        menu->text_indent_h = 0;
        menu->text_indent_w = 0;

        menu->button_radius = 0;

        return menu;
}


void Menu_SetupButtons( MENU *menu, float radius,
                        int32_t width, int32_t height,
                        SDL_Color background, SDL_Color trigger_color,
                        int32_t indent_w, int32_t indent_h,
                        int32_t text_indent_w, int32_t text_indent_h) 
{
        if ( menu == NULL )
                return;

        menu->button_bg_color = background;
        menu->trigger_color = trigger_color;

        menu->button_h = height;
        menu->button_w = width;

        menu->w = width + 2 * indent_w;
        menu->h = menu->h - 2 * menu->button_indent_h + 2 * indent_h;

        menu->button_indent_h = indent_h;
        menu->button_indent_w = indent_w;

        menu->text_indent_h = text_indent_h;
        menu->text_indent_w = text_indent_w;

        menu->button_radius = radius;

}

// Render functions
void __Menu_DrawButton(MENU *menu, int32_t x, int32_t y, SDL_Color color, LABEL *label) {
        SDL_FRect rect = (SDL_FRect){
                x, y,
                menu->button_w,
                menu->button_h
        };

        SDL_SetRenderDrawColor( menu->renderer, 
                                color.r, color.g,
                                color.b, color.a );

        Render_RounderRect(menu->renderer, rect, menu->button_radius);

        if ( label == NULL )
                return;

        float label_h = menu->button_h - 2 * menu->text_indent_h;
        float label_w = label_h * label->rect.w / label->rect.h;
        
        SDL_FRect label_rect = (SDL_FRect){
                rect.x + menu->text_indent_w,
                rect.y + menu->text_indent_h,
                label_w,
                label_h
        };

        Label_Draw(label, NULL, &label_rect);
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
                x - menu->border_width,
                y - menu->border_width,
                rect.w + 2 * menu->border_width,
                rect.h + 2 * menu->border_width
        };

        SDL_SetRenderDrawColor( menu->renderer, menu->border_color.r, menu->border_color.g,
                                     menu->border_color.b, menu->border_color.a );

        Render_RounderRect(menu->renderer, border, menu->border_radius);

        SDL_SetRenderDrawColor( menu->renderer, menu->bg_color.r, menu->bg_color.g,
                                     menu->bg_color.b, menu->bg_color.a );
        Render_RounderRect(menu->renderer, rect, menu->border_radius);

        MENU_BUTTON *now = menu->buttons;
        x = x + menu->button_indent_w;
        y = y + menu->button_indent_h;

        while ( now ) {
                if ( now->hide )
                        goto next;
                
                SDL_Color color = now->triggered ? menu->trigger_color : menu->button_bg_color;
                __Menu_DrawButton(menu, x, y, color, now->label);
                y += menu->button_h;

                next:
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



MENU_BUTTON *Menu_SetButton(    MENU *menu, 
                                int id,
                                LABEL *label,
                                void* (*function)(void*) 
                        )
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
                menu->h += menu->button_h;
        }
        
        button->triggered = 0;
        button->hide = 0;

        button->function =  function;
        button->label = label;   

        return button;
}



bool Menu_DelButton(MENU *menu, MENU_BUTTON *button) {
        if ( NULL == menu || NULL == button ) 
                return 0;
        
        if ( button->prev )
                button->prev->next = button->next;
        else 
                menu->buttons = button->next;

        if ( button->next )
                button->next->prev = button->prev;

        menu->h -= menu->button_h;

        free(button);

        return 1;
}

void Menu_ButtonHide(MENU *menu, MENU_BUTTON *button, bool hide) {
        if ( button == NULL )
                return;

        if ( button->hide == 0 && hide == 1 ) {
                menu->h -= menu->button_h;
        } else if ( button->hide == 1 && hide == 0 ) {
                menu->h += menu->button_h;
        }

        button->hide = hide;
}

bool Menu_MouseOut(MENU *menu, int32_t mouse_x, int32_t mouse_y) {
        return  mouse_x < menu->x - menu->border_width || 
                mouse_y < menu->y - menu->border_width || 
                mouse_x > menu->x + menu->w + menu->border_width || 
                mouse_y > menu->y + menu->h + menu->border_width;
}


bool Menu_CheckUpdate(MENU *menu, float mouse_x, float mouse_y, bool click) {
        if ( NULL == menu || menu->active == 0 ) 
                return 0;
        
        
        bool res = 0;

        bool mouse_out_menu = Menu_MouseOut(menu, mouse_x, mouse_y);
        if ( mouse_out_menu && click ) {
                menu->active = 0;
                res = 1;
        }

        int32_t now_y = menu->y + menu->button_indent_h;
        int32_t button_x = menu->x + menu->button_indent_w;
        int32_t button_h = menu->button_h;
        int32_t button_w = menu->button_w;

        MENU_BUTTON *now = menu->buttons;
        while ( now ) {
                if ( mouse_out_menu || now->hide == 1 ) {
                        if ( now->triggered )
                                res = 1;
                        now->triggered = 0;
                        now = now->next;
                        continue;
                }

                if ( mouse_x > button_x && mouse_x < button_x + button_w &&
                     mouse_y > now_y && mouse_y < now_y + button_h ) {
                        if ( now->triggered == 0 ) {
                                res = 1;
                                now->triggered = 1;
                        }

                        if ( click ) {
                                res = 1;
                                if ( now->function )
                                        now->function(menu);
                                menu->active = 0;
                        }
                } else {
                        if ( now->triggered == 1 ) {
                                res = 1;
                                now->triggered = 0;
                        }
                }

                now = now->next;
                now_y += menu->button_h;
        }

        return res;
}



void Menu_Move(MENU *menu, float x, float y, float window_w, float window_h) {
        if ( menu == NULL ) 
                return;

        menu->x = fix_cord(x, menu->border_width + menu->w, window_w);
        menu->y = fix_cord(y, menu->border_width + menu->h, window_h);
}