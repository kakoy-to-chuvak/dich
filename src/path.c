#include "path.h"


#define SQ(x) ((x)*(x))
#define UNDER_MOUSE(cords, mouse_x, mouse_y) (SQ(mouse_x-cords.x) + SQ(mouse_y-cords.y) <= SQ(POINT_DIAMETR>>1))

#define FIX_CORD(x, app_x) (x < 0 ? 0 : (x > app_x ? app_x : x))
#define FIX_LABEL_CORD(x, app_x, label_x) ( x + label_x + POINT_RADIUS > app_x - 5 ? x - label_x - POINT_RADIUS : x + POINT_RADIUS)




void RenderPointCords(Point point, LABEL *label) {
        if ( point.state == none ) 
                return;

        char point_text[32];
        sprintf_s(point_text, sizeof(point_text), "(%.0f, %.0f)", point.cords.x, point.cords.y);
        Label_Update(label, point_text, TEXT_COLOR_FG);

        SDL_FRect label_rect = {
                FIX_LABEL_CORD(point.cords.x, APP_WIDTH, label->rect.w / 2 ), 
                FIX_LABEL_CORD(point.cords.y, APP_HEIGHT, label->rect.h / 2 ),
                label->rect.w / 2, label->rect.h / 2 
        };

        Label_Draw(label, NULL, &label_rect);
}

void RenderPoint(SDL_Renderer *renderer, Point point, SDL_Texture *point_texture, LABEL *point_label) {
        if ( point.state == under ) 
                SDL_SetTextureColorMod(point_texture, 40, 255, 40);
        if ( point.state == selected )
                SDL_SetTextureColorMod(point_texture, 255, 40, 40);
        
        SDL_FRect pos = {       
                        point.cords.x-POINT_RADIUS, 
                        point.cords.y-POINT_RADIUS, 
                        POINT_DIAMETR, 
                        POINT_DIAMETR
                        };

        SDL_RenderTexture(renderer, point_texture, NULL, &pos);

        SDL_SetTextureColorMod(point_texture, 0, 0, 0);

        RenderPointCords(point, point_label);
}

void RenderPath(SDL_Renderer *renderer, SDL_Texture *point_texture, PArray *points, LABEL *point_label) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);

        SDL_Color arrow_color = { 50, 50, 255, 255 };
        SDL_FColor arrow_Fcolor = (SDL_FColor) { arrow_color.r / 255.f, arrow_color.g / 255.f, arrow_color.b / 255.f, arrow_color.a / 255.f };

        for ( int32_t i = 0 ; i < points->count ; i++ ) {
                Point next_point = points->points[i+1];
                Point now_point = points->points[i];
                // if ( next_point.state == under_mouse && now_point.state == under_mouse)
                //         SDL_SetRenderDrawColor(renderer, 255, 90, 90, 255);
                if ( i+1 != points->count ) {
                        RenderLine(renderer, now_point.cords, next_point.cords, 3);
                        RenderArrow(renderer, now_point.cords, next_point.cords, 20, arrow_Fcolor);
                }
                // SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
                

                RenderPoint(renderer, now_point, point_texture, point_label);

        }
        
        SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
}



void MovePoint(Point *point, double start_x, double start_y, double x, double y, bool shift_pressed) {
        x = FIX_CORD(x, APP_WIDTH);
        y = FIX_CORD(y, APP_HEIGHT);

        if ( shift_pressed ) {
                if ( fabs(x-start_x) > fabs(y-start_y) )
                        y = start_y;
                else
                        x = start_x;
        } 

        point->cords.x = x;
        point->cords.y = y;
}



bool CheckMousePos(PArray *points, double x, double y, bool mouse_pressed, bool shift_pressed) {
        bool points_changed = 0;
        bool mouse_on_point = 0;
        Point *now_point = points->points;

        SDL_SystemCursor cursor = SDL_SYSTEM_CURSOR_DEFAULT;

        static float start_x = 0;
        static float start_y = 0;
        

        if ( points->selected_point ) {
                points_changed = true;
                
                if ( mouse_pressed ) {
                        MovePoint(points->selected_point, start_x, start_y, x, y, shift_pressed);
                        cursor = SDL_SYSTEM_CURSOR_MOVE;
                        mouse_on_point = true;
                } else {
                        points->selected_point->state = UNDER_MOUSE(points->selected_point->cords, x, y) ? under : none;
                        points->selected_point = NULL;
                }
        }

        for (int32_t i = 0; i < points->count ; i++ ) {
                now_point = points->points + i;
                PState new_state = selected;
                bool under_mouse = UNDER_MOUSE(now_point->cords, x, y);
                if ( under_mouse && !mouse_on_point ) {
                        mouse_on_point = true;
                        if ( mouse_pressed ) {
                                new_state = selected;
                                points->selected_point = now_point;
                                start_x = now_point->cords.x;
                                start_y = now_point->cords.y;
                        } else {
                                new_state = under;
                                cursor = SDL_SYSTEM_CURSOR_POINTER;
                        } 
                } else if ( now_point->state != selected ) {
                        new_state = none;
                }
                
                if ( now_point->state != new_state ) {
                        points_changed = true;
                        now_point->state = new_state;
                }
        }

        SDL_SetCursor(SDL_CreateSystemCursor(cursor));
        return points_changed;
}
