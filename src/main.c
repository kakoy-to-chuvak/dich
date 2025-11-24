#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
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

SDL_Texture *background_texture = NULL;
SDL_Texture *point_texture = NULL;


bool lmb_pressed = 0;
bool rmb_pressed = 0;
bool shift_pressed = 0;

bool changes = 1;
PArray points = {
        {
                {{100, 100}, NONE_STATE, NONE_STATE},
                {{100, 500}, NONE_STATE, NONE_STATE},
                {{150, 200}, NONE_STATE, NONE_STATE},
                {{450, 500}, NONE_STATE, NONE_STATE},
                {{500, 300}, NONE_STATE, NONE_STATE},
                {{200, 100}, NONE_STATE, NONE_STATE},
                {{300, 270}, NONE_STATE, NONE_STATE},
                {{70, 277}, NONE_STATE, NONE_STATE}
        },
        NULL,
        NULL,
        6
};




int render(APP *app) {
        if ( changes == 0 )
                return 1;
        changes = 0;
        
        LogDebug("render", "render");
        
        SDL_RenderClear(app->Renderer);

        SDL_RenderTexture(app->Renderer, background_texture, NULL, NULL);

        RenderPath(app->Renderer, point_texture, &points, point_text);

        Menu_Render(menu);
        
        SDL_RenderPresent(app->Renderer);
        return 1;
}


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



        menu = Menu_New(app->Renderer, SDL_PIXELFORMAT_RGBA32,
                MENU_BG, 7, 3, MENU_BORDER_COLOR);
        if ( NULL == menu ) {
                LogError("setup", "Menu_New failed");
                return 0;
        }

        Menu_SetupButtons(menu, 2, 90, 20, MENU_BG, MENU_TRIGGER_COLOR, 5, 10, 5, 1);      
        
        SDL_SetRenderDrawBlendMode(app->Renderer, SDL_BLENDMODE_BLEND);

        render(app);

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
                                if ( event.button.button == 1 ) {
                                        lmb_pressed = 1;
                                } else if ( event.button.button == 3 ) {
                                        rmb_pressed = 1;
                                }
                                break;
                        case SDL_EVENT_MOUSE_BUTTON_UP:
                                if ( event.button.button == 1 ) {
                                        lmb_pressed = 0;
                                } else if ( event.button.button == 3 ) { 
                                        rmb_pressed = 0;
                                }
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

        static bool prev_lmb_state = 0;
        bool lmb_clicked = lmb_pressed && prev_lmb_state == 0;

        static bool prev_rmb_state = 0;
        bool rmb_clicked = rmb_pressed && prev_rmb_state == 0;
        if ( rmb_clicked && ( menu->active == 0 || Menu_MouseOut(menu, mouse_x, mouse_y) ) ) {
                Menu_Move(menu, mouse_x, mouse_y, APP_WIDTH, APP_HEIGHT);
                menu->active = 1;
                changes = 1;
        }

        changes = CheckMousePos(&points, mouse_x, mouse_y, lmb_pressed, prev_lmb_state, shift_pressed) || changes;

        changes = Menu_CheckUpdate(menu, mouse_x, mouse_y, lmb_clicked | rmb_clicked) || changes;

        prev_lmb_state = lmb_pressed;
        prev_rmb_state = rmb_pressed;

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
        Label_Free(point_text);
        Menu_Free(menu);

        SDL_DestroyTexture(point_texture);
        SDL_DestroyTexture(background_texture);
        AppQuit(app);
        TTF_Quit();
        SDL_Quit();
        return 0;
}