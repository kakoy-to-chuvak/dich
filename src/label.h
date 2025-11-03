#ifndef __LABEL_H
#define __LABEL_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "logs.h"


#define LABEL_VOID_PARAMS ((LABEL_PARAMS){ (SDL_Color){0,0,0,0}, 5 })

#define LABEL_PARAM_BUBBLE ((int16_t)0b100)
#define LABEL_PARAM_BORDER ((int16_t)0b010)


typedef struct LABEL_PARAMS {
        // Bubble/Border text
        SDL_Color outer_color;
        uint32_t radius;
} LABEL_PARAMS;


typedef struct LABEL {
        LABEL_PARAMS params;
        int16_t params_flags;

        SDL_Renderer *renderer;
        TTF_Font *font;
        SDL_Texture *texture;
        SDL_FRect rect;
} LABEL;





LABEL *Label_New(SDL_Renderer *renderer, const char *font_name, const char *_Text, float ptsize, SDL_Color foreground, int16_t flags, LABEL_PARAMS params);
int Label_Free(LABEL **labelp);
bool Label_Draw(LABEL *label, SDL_FRect *srcrect, SDL_FRect *dstrect);
int Label_Update(LABEL *label, const char *_Text, SDL_Color fg);


#endif // __LABEL_H