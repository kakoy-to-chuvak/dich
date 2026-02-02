#ifndef __FILESAVE_H__
#define __FILESAVE_H__

#include <SDL3/SDL_dialog.h>
#include <stdlib.h>
#include <string.h>
#include "path.h"
#include "logs.h"



void SavePoints(PArray* _Points);
void ShowSaveFIleDialog(SDL_Window *_Window, const char *_Default_location, PArray *_Points);

void LoadPoints(PArray* _Points);
void ShowOpenFIleDialog(SDL_Window *_Window, const char *_Default_location, PArray *_Points);








#endif //__FILESAVE_H__