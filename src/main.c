#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>


#include "include.h"
#include "label.h"
#include "App.h"
#include "logs.h"
#include "drawing.h"
#include "points.h"
#include "path.h"
#include "menu.h"



APP *app;
LABEL *point_text;

MENU *menu;
LABEL *menu_up;
LABEL *menu_down;
LABEL *menu_left;
LABEL *menu_right;

float x = 100, y = 100;

void *F_menu_up(void *m) {
        y -= 20;
        return m;
}
void *F_menu_down(void *m) {
        y += 20;
        return m;
}
void *F_menu_left(void *m) {
        x -= 20;
        return m;
}
void *F_menu_right(void *m) {
        x += 20;
        return m;
}

SDL_Texture *background_texture = NULL;
SDL_Texture *point_texture = NULL;


bool mouse_pressed = 0;
bool shift_pressed = 0;

bool points_changed = 1;
PArray points = {
        {
                {{100, 100}, none},
                {{100, 500}, none},
                {{150, 200}, none},
                {{450, 500}, none},
                {{500, 300}, none},
                {{200, 100}, none},
                {{300, 270}, none},
                {{70, 277}, none}
        },
        NULL,
        4
};


int setup(APP *app) {
        SDL_SetRenderDrawColor(app->Renderer, 160, 160, 160, 255);

        SDL_Surface *tmp_surf = IMG_Load("images/point.png");
        if ( NULL == tmp_surf ) {
                LogError("setup", "IMG_Load failed: %s", SDL_GetError());
                return 0;
        }

        point_texture = SDL_CreateTextureFromSurface(app->Renderer, tmp_surf);
        SDL_DestroySurface(tmp_surf);
        if ( NULL == point_texture ) {
                LogError("setup", "SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
                return 0;
        }
        
        tmp_surf = IMG_Load("images/ground.png");
        if ( NULL == tmp_surf ) {
                LogError("setup", "IMG_Load failed: %s", SDL_GetError());
                return 0;
        }

        background_texture = SDL_CreateTextureFromSurface(app->Renderer, tmp_surf);
        SDL_DestroySurface(tmp_surf);
        if ( NULL == background_texture ) {
                LogError("setup", "SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
                return 0;
        }

        point_text = Label_New(app->Renderer, "fonts/ArialBlackPrimer.ttf", "a", TEXT_SIZE, TEXT_COLOR_FG, LABEL_PARAM_BORDER, TEXT_PARAMS);
        if ( NULL == point_text ) {
                LogError("setup", "Label_New failed");
                return 0;
        }

        menu = Menu_New(app->Renderer, 100, 100, 
                MENU_BG, 2, MENU_BORDER_COLOR);
        if ( NULL == menu ) {
                LogError("setup", "Menu_New failed");
                return 0;
        }

        menu_down = Label_New(app->Renderer, "fonts/ArialBlackPrimer.ttf", "down", 20, TEXT_COLOR_FG, 0, LABEL_VOID_PARAMS);
        if ( NULL == menu_down ) return 0;

        menu_up = Label_New(app->Renderer, "fonts/ArialBlackPrimer.ttf", "up", 20, TEXT_COLOR_FG, 0, LABEL_VOID_PARAMS);
        if ( NULL == menu_down ) return 0;

        menu_left = Label_New(app->Renderer, "fonts/ArialBlackPrimer.ttf", "left", 20, TEXT_COLOR_FG, 0, LABEL_VOID_PARAMS);
        if ( NULL == menu_down ) return 0;

        menu_right = Label_New(app->Renderer, "fonts/ArialBlackPrimer.ttf", "right", 20, TEXT_COLOR_FG, 0, LABEL_VOID_PARAMS);
        if ( NULL == menu_down ) return 0;

        menu->active = 1;
        
        Menu_SetButton(menu, 0, 90, 20, 5, 4, MENU_BG, 1, MENU_BORDER_COLOR, menu_down, F_menu_down);
        Menu_SetButton(menu, 1, 90, 20, 5, 28, MENU_BG, 1, MENU_BORDER_COLOR, menu_up, F_menu_up);
        Menu_SetButton(menu, 2, 90, 20, 5, 52, MENU_BG, 1, MENU_BORDER_COLOR, menu_left, F_menu_left);
        Menu_SetButton(menu, 3, 90, 20, 5, 76, MENU_BG, 1, MENU_BORDER_COLOR, menu_right, F_menu_right);


        SDL_SetTextureColorMod(point_texture, 0, 0, 0);

        SDL_RenderClear(app->Renderer);
        SDL_RenderTexture(app->Renderer, background_texture, NULL, NULL);

        RenderPath(app->Renderer, point_texture, &points, point_text);

        Menu_Draw(menu, 100, 100);
        
        SDL_RenderPresent(app->Renderer);

        return 1;
}


int render(APP *app) {
        if ( points_changed == 0 )
                return 1;
        points_changed = 0;
        
        SDL_RenderClear(app->Renderer);
        SDL_RenderTexture(app->Renderer, background_texture, NULL, NULL);

        RenderPath(app->Renderer, point_texture, &points, point_text);

        Menu_Draw(menu, x, y);
        
        SDL_RenderPresent(app->Renderer);
        return 1;
}


int Tick(APP *app) {
        SDL_Event event;
        static double mouse_x = 0;
        static double mouse_y = 0;

        while ( SDL_PollEvent(&event) ) {
                switch (event.type) {
                        case SDL_EVENT_QUIT:
                                app->is_running = 0;
                                return 0;
                                break;
                        case SDL_EVENT_MOUSE_MOTION:
                                mouse_x = event.motion.x;
                                mouse_y = event.motion.y;
                                break;
                        case SDL_EVENT_MOUSE_BUTTON_DOWN:
                                if ( event.button.button == 1 )
                                        mouse_pressed = 1;
                                break;
                        case SDL_EVENT_MOUSE_BUTTON_UP:
                                if ( event.button.button == 1 )
                                        mouse_pressed = 0;
                                break; 
                        case SDL_EVENT_KEY_DOWN: 
                                switch (event.key.scancode) {
                                        case SDL_SCANCODE_ESCAPE:
                                                app->is_running = 0;
                                                return 0;
                                                break;
                                        case SDL_SCANCODE_LSHIFT:
                                                shift_pressed = 1;
                                                break;
                                        default:
                                                break;
                                }
                                break;
                        case SDL_EVENT_KEY_UP:
                                switch ( event.key.scancode ) {
                                        case SDL_SCANCODE_LSHIFT:
                                                shift_pressed = 0;
                                                break;
                                        default:
                                                break;
                                }
                                break;
                        default:
                                break;
                }
        }

        points_changed = points_changed || CheckMousePos(&points, mouse_x, mouse_y, mouse_pressed, shift_pressed);

        static uint64_t update_timer = 0;
        if ( SDL_GetTicksNS() - update_timer > 500000000) {
                points_changed = points_changed || Menu_CheckUpdate(menu, mouse_x, mouse_y, mouse_pressed);
                update_timer = SDL_GetTicksNS();
        }

        return 1;
}



int main() {
        Logs_SetLogLevel(LOG_LEVEL_INFO);
        Logs_EnableColors(1);

        if ( 0==SDL_Init(SDL_INIT_FLAGS) ) {
                LogError("main", "SDL_Init failed: %s", SDL_GetError());
                return 0;
        }

        if ( 0==TTF_Init() ) {
                LogError("setup/TTF_Init", "TTF_Init failed: %s", SDL_GetError());
                return 0;
        }   

        app = AppNew("Планировщик маршрута", APP_WIDTH, APP_HEIGHT, 0, NULL);
        if ( NULL==app ) {
                LogError("main", "AppNew failed");
                goto app_quit;
        }
 
        if ( 0==setup(app) ) {
                LogError("main", "setup failed");
                goto app_quit;
        }

        AppSetTick(app, Tick);
        AppSetRendererTick(app, render);

        AppSetTps(app, TPS);
        AppSetFps(app, FPS);

        AppMainloop(app);

        app_quit:
        Label_Free(&point_text);

        Label_Free(&menu_down);
        Label_Free(&menu_left);
        Label_Free(&menu_right);
        Label_Free(&menu_up);
        Menu_Free(menu);

        SDL_DestroyTexture(point_texture);
        SDL_DestroyTexture(background_texture);
        AppQuit(&app);
        TTF_Quit();
        SDL_Quit();
        return 0;
}