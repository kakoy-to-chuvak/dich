#ifndef INCLUDE_H
#define INCLUDE_H


#define MAX_POINTS 256

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO)
#define APP_TITLE "Планировщик маршрута"
#define APP_WIDTH 806 
#define APP_HEIGHT 540

#define TPS 60
#define FPS 60

#define TEXT_SIZE (30)

#define TEXT_COLOR_FG ((SDL_Color){0, 0, 0, 255})
#define TEXT_BORDER_COLOR ((SDL_Color){255, 255, 255, 255})

#define TEXT_PARAMS ((LABEL_PARAMS){TEXT_BORDER_COLOR, 4})


#define MENU_BG ((SDL_Color){31, 31, 31, 255})
#define MENU_BORDER_COLOR ((SDL_Color){100, 100, 100, 255})
#define MENU_TRIGGER_COLOR ((SDL_Color){148, 148, 148, 255})


#define POINT_DIAMETR 31
#define POINT_RADIUS (POINT_DIAMETR / 2)






#endif  // INCLUDE_H