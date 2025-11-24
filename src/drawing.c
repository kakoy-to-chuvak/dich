#include "drawing.h"


#define FIX_CORD(x, app_x) ( x < 0 ? 0 : ( x > app_x ? app_x : x) )


void RenderLine(SDL_Renderer *renderer, SDL_FPoint P0, SDL_FPoint P1, int32_t radius) {
        int32_t x = radius;
        int32_t y = 0; 
        int32_t err = 0;

        int32_t x0 = FIX_CORD(P0.x, APP_WIDTH);
        int32_t y0 = FIX_CORD(P0.y, APP_HEIGHT);
        int32_t x1 = FIX_CORD(P1.x, APP_WIDTH);
        int32_t y1 = FIX_CORD(P1.y, APP_HEIGHT);

        while ( x >= y ) {
                SDL_RenderLine(renderer, x0 + x, y0 + y, x1 + x, y1 + y);
                SDL_RenderLine(renderer, x0 + x, y0 - y, x1 + x, y1 - y);
                SDL_RenderLine(renderer, x0 - x, y0 + y, x1 - x, y1 + y);
                SDL_RenderLine(renderer, x0 - x, y0 - y, x1 - x, y1 - y);
                SDL_RenderLine(renderer, x0 + y, y0 + x, x1 + y, y1 + x);
                SDL_RenderLine(renderer, x0 + y, y0 - x, x1 + y, y1 - x);
                SDL_RenderLine(renderer, x0 - y, y0 + x, x1 - y, y1 + x);
                SDL_RenderLine(renderer, x0 - y, y0 - x, x1 - y, y1 - x);

                if (err <= 0) {
        	    y += 1;
        	    err += 2*y + 1;
        	} else {
        	    x -= 1;
        	    err -= 2*x + 1;
        	}
        }
}
                        

void RenderArrow(SDL_Renderer *renderer, SDL_FPoint P0, SDL_FPoint P1, double base, SDL_FColor arrow_color) {
        SDL_FPoint Pa = Point_Norm( Point_Sub(P1, P0) );

        P1 = Point_Sub(P1, Point_Mult(Pa, POINT_RADIUS + base));

        SDL_Vertex vertex[3];

        vertex[0].color = arrow_color;
        vertex[1].color = arrow_color;
        vertex[2].color = arrow_color;

        Pa = Point_Mult(Pa, base);
        vertex[0].position = Point_Sum(P1, Pa);

        Pa = Point_Rotate90(Pa);
        vertex[1].position = Point_Sum(P1, Pa);

        Pa = Point_Invert(Pa);
        vertex[2].position = Point_Sum(P1, Pa);


        SDL_RenderGeometry(renderer, NULL, vertex, 3, NULL, 0);
}


void RenderCircle(SDL_Renderer *renderer, float x0, float y0, float radius) {
        float x = 0;
        float y = radius;
        float p = 1 - radius;

        SDL_RenderPoint(renderer, x0 + x, y0 + y);
        SDL_RenderPoint(renderer, x0 + y, y0 + x);
        SDL_RenderPoint(renderer, x0 - y, y0 + x);
        SDL_RenderPoint(renderer, x0 - x, y0 + y);
        SDL_RenderPoint(renderer, x0 - x, y0 - y);
        SDL_RenderPoint(renderer, x0 - y, y0 - x);
        SDL_RenderPoint(renderer, x0 + y, y0 - x);
        SDL_RenderPoint(renderer, x0 + x, y0 - y);

        while (x < y)
        {       
                
                if (p < 0){
                        p += 2 * x + 1;
                        x++;
                }else{
                        p += 2 * (x - y) + 1;
                        y--;
                        x++;
                }

                SDL_RenderPoint(renderer, x0 + x, y0 + y);
        	SDL_RenderPoint(renderer, x0 + y, y0 + x);
        	SDL_RenderPoint(renderer, x0 - y, y0 + x);
        	SDL_RenderPoint(renderer, x0 - x, y0 + y);
        	SDL_RenderPoint(renderer, x0 - x, y0 - y);
        	SDL_RenderPoint(renderer, x0 - y, y0 - x);
        	SDL_RenderPoint(renderer, x0 + y, y0 - x);
        	SDL_RenderPoint(renderer, x0 + x, y0 - y);

                
        }
}


void Render_RounderRect(SDL_Renderer *renderer, SDL_FRect rect, float radius) {
        
        SDL_FRect now_rect = {
                rect.x,
                rect.y + radius,
                rect.w,
                rect.h - 2 * radius
        };

        SDL_RenderFillRect(renderer, &now_rect);

        if ( radius )
                radius--;
                
        float x_l = rect.x + radius;
        float y_u = rect.y + radius;
        float x_r = rect.x + rect.w - radius - 1;
        float y_b = rect.y + rect.h - radius - 1;

        float x = 0;
        float y = radius;
        float p = 1 - radius;

        float y_next = p < 0 ? y : y - 1;
        float p_next = p < 0 ? p + 2 * x + 1 : p + 2 * ( x - y ) + 1;
        
        SDL_RenderLine(renderer, x_l - y, y_u - x, x_r + y, y_u - x);
        SDL_RenderLine(renderer, x_l - y, y_b + x, x_r + y, y_b + x);
        if ( y != y_next ) {      
                SDL_RenderLine(renderer, x_l - x, y_u - y, x_r + x, y_u - y);
                SDL_RenderLine(renderer, x_l + x, y_b - y, x_r + x, y_b + y);
        }
                
        while (x < y)
        {
        	p = p_next;
                y = y_next;
                x++;

                y_next = p < 0 ? y : y - 1;
                p_next = p < 0 ? p + 2 * x + 1 : p + 2 * ( x - y ) + 1;

                if ( x != y ) {
                        SDL_RenderLine(renderer, x_l - y, y_u - x, x_r + y, y_u - x);
                        SDL_RenderLine(renderer, x_l - y, y_b + x, x_r + y, y_b + x);
                }

                if ( y != y_next ) {
                        SDL_RenderLine(renderer, x_l - x, y_u - y, x_r + x, y_u - y);
                        SDL_RenderLine(renderer, x_l - x, y_b + y, x_r + x, y_b + y);
                }
                
        }
}


