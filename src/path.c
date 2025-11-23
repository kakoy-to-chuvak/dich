#include "path.h"


#define SQ(x) ((x)*(x))
#define UNDER_MOUSE(cords, mouse_x, mouse_y) ( SQ(mouse_x-cords.x) + SQ(mouse_y-cords.y) <= SQ(POINT_DIAMETR>>1) ) 

#define FIX_CORD(x, app_x) (x < 0 ? 0 : (x > app_x ? app_x : x))
#define FIX_LABEL_CORD(x, app_x, label_x) ( x + label_x + POINT_RADIUS > app_x - 5 ? x - label_x - POINT_RADIUS : x + POINT_RADIUS)



static float start_x = 0;
static float start_y = 0;
static float mouse_point_x = 0;
static float mouse_point_y = 0;



void _RenderPointCords(Point point, LABEL *label) {
        if ( point.state == NONE_STATE ) 
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


void _RenderPoint(SDL_Renderer *renderer, Point point, SDL_Texture *point_texture, LABEL *point_label) {
        switch ( point.state ) {
                case UNDER_MOUSE:
                        SDL_SetTextureColorMod(point_texture, 40, 255, 40);
                        break;
                case SELECTED:
                        SDL_SetTextureColorMod(point_texture, 255, 40, 40);
                        break;
                default:
                        SDL_SetTextureColorMod(point_texture, 0, 0, 0);
                        break;
        }
        

        SDL_FRect pos = {       
                        point.cords.x-POINT_RADIUS, 
                        point.cords.y-POINT_RADIUS, 
                        POINT_DIAMETR, 
                        POINT_DIAMETR
                        };

        SDL_RenderTexture(renderer, point_texture, NULL, &pos);
        _RenderPointCords(point, point_label);
}


void _RenderLine(SDL_Renderer *renderer, Point *point, float line_r, float arrrow_base, SDL_FColor arrow_Fcolor) {
        Point *prev = point + 1;
        if ( point->line_state == UNDER_MOUSE ) {
                SDL_SetRenderDrawColor(renderer, 90, 255, 90, 255);
        } else if ( point->line_state == SELECTED ) {
                SDL_SetRenderDrawColor(renderer, 255, 90, 90, 255);
        }
        
        RenderLine(renderer, point->cords, prev->cords, line_r);
        RenderArrow(renderer, point->cords, prev->cords, arrrow_base, arrow_Fcolor);

        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); 
}


void RenderPath(SDL_Renderer *renderer, SDL_Texture *point_texture, PArray *points, LABEL *point_label) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);

        SDL_Color arrow_color = { 50, 50, 255, 255 };
        SDL_FColor arrow_Fcolor = (SDL_FColor) { arrow_color.r / 255.f, arrow_color.g / 255.f, arrow_color.b / 255.f, arrow_color.a / 255.f };

        for ( int32_t i = points->count - 1 ; i >= 0 ; i-- ) {
                Point *now_point = points->points + i;
                Point *next_point = now_point - 1;

                if ( i > 0 ) {
                      _RenderLine(renderer, next_point, LINE_RADIUS, 20, arrow_Fcolor);  
                }
                
                
                if ( now_point->state == NONE_STATE )
                        _RenderPoint(renderer, *now_point, point_texture, point_label);
        }

        if ( points->selected_point )
                _RenderPoint(renderer, *(points->selected_point), point_texture, point_label);
        
        SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
}




void MovePoint(Point *point, double x, double y, bool shift_pressed) {
        x += mouse_point_x;
        y += mouse_point_y;

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


bool TouchLine(SDL_FPoint P1, SDL_FPoint P2, SDL_FPoint M, float line_r, float point_r) {
        SDL_FPoint A = Point_Sub(M, P2);
        SDL_FPoint B = Point_Sub(M, P1);
        SDL_FPoint C = Point_Sub(P1, P2);

        float SqAbs_A = Point_SqAbs(A);
        float SqAbs_B = Point_SqAbs(B);
        float SqAbs_C = Point_SqAbs(C);
        float sq_point_r = point_r * point_r;

        if ( SqAbs_A >= SqAbs_C || SqAbs_B >= SqAbs_C || SqAbs_A <= sq_point_r || SqAbs_B <= sq_point_r )
                return 0;

        double p_scl = Point_SclMult(A, C);

        double h2 = Point_SqAbs(A) - ( p_scl * p_scl ) / SqAbs_C;
        
        return h2 <= line_r * line_r;

}


bool CheckLine(PArray *points, Point *point, double mouse_x, double mouse_y, bool mouse_pressed, bool prev_mouse_state) {
        PState new = NONE_STATE;

        if (    
                TouchLine(point->cords, (point+1)->cords, (SDL_FPoint){mouse_x, mouse_y}, LINE_RADIUS, POINT_RADIUS)
        ) {
                if ( mouse_pressed && prev_mouse_state == 0 ) {
                        new = SELECTED;
                } else {
                        new = UNDER_MOUSE;
                }
                points->selected_line = point;
        }

        if ( point->line_state != new ) {
                point->line_state = new;
                
                return 1;
        }
        
        return 0;
}


bool CheckPoint(PArray *points, Point *point, double mouse_x, double mouse_y, bool mouse_pressed, bool prev_mouse_state) {
        PState new = NONE_STATE;

        if ( UNDER_MOUSE(point->cords, mouse_x, mouse_y) ) {
                if ( mouse_pressed && prev_mouse_state == 0 ) {
                        new = SELECTED;
                        start_x = point->cords.x;
                        start_y = point->cords.y;

                        mouse_point_x = start_x - mouse_x;
                        mouse_point_y = start_y - mouse_y;
                } else {
                        new = UNDER_MOUSE;
                }
                points->selected_point = point;
        }


        if ( point->state != new ) {
                point->state = new;
                return 1;
        }

        return 0;
}


bool CheckMousePos(PArray *points, double mouse_x, double mouse_y, bool mouse_pressed, bool prev_mouse_state, bool shift_pressed) {
        bool points_changed = 0;

        if ( points->selected_point ) {
                points->selected_line = NULL;
                if ( points->selected_point->state == SELECTED && mouse_pressed ) {
                        MovePoint(points->selected_point, mouse_x, mouse_y, shift_pressed);
                } else {
                        points->selected_point->state = NONE_STATE;
                        points->selected_point = NULL;
                } 
                points_changed = 1;
                points->selected_line = NULL;
        }

        if ( points->selected_line ) {
                points->selected_point = NULL;
                if ( mouse_pressed && points->selected_line->line_state == SELECTED ) {
                        points->selected_line->line_state = SELECTED;
                } else {
                        points->selected_line->line_state = NONE_STATE;
                        points->selected_line = NULL;
                        points_changed = 1;
                }
        }

        for (int32_t i = 0 ; i < points->count ; i++ ) {
                Point *now_point = points->points + i;
                
                if ( points->selected_point == NULL && points->selected_line == NULL ) {
                        points_changed |= CheckPoint(points, now_point, mouse_x, mouse_y, mouse_pressed, prev_mouse_state);
                }

                if ( points->selected_point == NULL && points->selected_line == NULL && i + 1 < points->count ) {
                        points_changed |= CheckLine(points, now_point, mouse_x, mouse_y, mouse_pressed, prev_mouse_state);
                }
        }

        if ( points->selected_point ) {
                points->selected_line = NULL;
        }

        
        SDL_SetCursor(SDL_CreateSystemCursor(points->selected_point ? points->selected_point->state == UNDER_MOUSE ? SDL_SYSTEM_CURSOR_POINTER : SDL_SYSTEM_CURSOR_MOVE : SDL_SYSTEM_CURSOR_DEFAULT ));

        return points_changed;
} 


// void AddPoint(PArray *points, double mouse_x, double mouse_y) {
        
// }
