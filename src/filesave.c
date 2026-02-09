#include "filesave.h"


static const SDL_DialogFileFilter dialog_filters[4] = {
    { "JSON (*.json)",    "json" },
    { "TXT (*.txt)",      "txt"  },
    { "CSV (*.csv)",      "csv"  },
    { "All files (*.*)",  "*"    }
};



FILESAVE_FORMAT DefineFileFormat(const char *_File_name) {
        char *extension = strrchr(_File_name, '.');
        if ( extension == NULL ) {
                return FILE_FORMAT_UNDEFINED;
        }

        extension++;

        if ( strcmp(extension, "csv") == 0 ) {
                return FILE_FORMAT_CSV;
        } else if ( strcmp(extension, "json") == 0 ) {
                return FILE_FORMAT_JSON;
        } else if ( strcmp(extension, "txt") == 0 ) {
                return FILE_FORMAT_TXT;
        } else {
                return FILE_FORMAT_UNDEFINED;
        }
}


void SavePoints(PArray* _Points) {
        if ( _Points == NULL ) {
                LogNotice("SavePoints", "No points to save");
                return;
        }

        if ( _Points->format == FILE_FORMAT_UNDEFINED ) {
                LogNotice("SavePoints", "Undefined file format");
                return;
        }

        FILE *file = fopen(_Points->file_name, "w");
        if ( file == NULL ) {
                LogNotice("SavePoints", "Error on opening file: %s", _Points->file_name);
                return;
        }

        char *print_format = "%.4f %.4f %.10f\n";

        switch (_Points->format) {
                case FILE_FORMAT_JSON:
                        print_format = "{\"x\":%.4f,\"y\":%.4f,\"angle\":%.10f},";
                        fputc('[', file);
                        if ( _Points->points == NULL ) {
                                fputc(' ', file);
                        }
                        break;
                case FILE_FORMAT_CSV:
                        print_format = "%.4f,%.4f,%.10f\n";
                        fputs("x,y,angle\n", file);
                        break;
                default:
                        break;
        }

        Point *now = _Points->points;
        while ( now ) {
                fprintf(file, print_format, now->cords.x, now->cords.y, now->angle);
                now = now->next;
        }

        if ( _Points->format == FILE_FORMAT_JSON ) {
                fseek(file, -1, SEEK_CUR);
                fputc(']', file);
        }

        fclose(file);
}


static void SDLCALL __SaveFileDialogCallback(void* userdata, const char* const* filelist, int filter) {
        if ( filelist == NULL ) {
                LogNotice("ShowSaveFIleDialog (__SaveFileDialogCallback)", "An error occured: %s", SDL_GetError());
                return;
        }

        if ( *filelist == NULL ) {
                LogNotice("ShowSaveFIleDialog (__SaveFileDialogCallback)", "No files selected");
                return;
        }

        FileSaveArgs *args = (FileSaveArgs*)userdata;
        if ( args == NULL || args->points == NULL || args->parametrs == NULL ) {
                LogNotice("ShowSaveFIleDialog (__SaveFileDialogCallback)", "No args received");
                return;
        }

        char *extension = strrchr(*filelist, '.');

        FILESAVE_FORMAT format = FILE_FORMAT_UNDEFINED;
        if ( filter >= 0 && filter < 3 ) {
                format = (FILESAVE_FORMAT)filter;
        } else {
                format = DefineFileFormat(extension);
        }
        
        args->points->format = format;
        strcpy_s(args->points->file_name, MAX_PATH, *filelist);

        switch ( filter ) {
                case 0: 
                        // if extension is undefined or not equals ".json"
                        if ( extension == NULL || strcmp(extension, ".json") )
                                strcat_s(args->points->file_name, MAX_PATH, ".json");
                        break;
                case 1:
                        // if extension is undefined or not equals ".txt"
                        if ( extension == NULL || strcmp(extension, ".txt") )
                                strcat_s(args->points->file_name, MAX_PATH, ".txt");
                        break;
                case 2:
                        // if extension is undefined or not equals ".csv"
                        if ( extension == NULL || strcmp(extension, ".csv") )
                                strcat_s(args->points->file_name, MAX_PATH, ".csv");
                        break;
                default:
                        break;
        } 

        SavePoints(args->points);
}


void ShowSaveFIleDialog(SDL_Window *_Window, const char *_Default_location, FileSaveArgs *_Args) {
        SDL_ShowSaveFileDialog(__SaveFileDialogCallback, _Args, _Window, dialog_filters, 4, _Default_location);
}











void __ParseCSV(PArray *_Points, FILE *_Stream, Parametrs *_Parametrs) {
        float x = 0;
        float y = 0;
        float angle = 0;

        char buffer[1024] = "";

        if ( fgets(buffer, sizeof(buffer), _Stream) == 0 ) {
                LogNotice("ShowOpenFIleDialog (LoadPoints)", "couldn`t read file");  
        }

        int n = strlen(buffer) - 1;
        if ( buffer[n] == '\n' ) 
                buffer[n] = '\0';

        if ( strcmp(buffer, "x,y,angle") ) {
                LogNotice("ShowOpenFIleDialog (LoadPoints)", "wrong format for <.csv>: \"%s\" expected \"x,y,angle\"", buffer);
                return;
        }

        while ( fgets(buffer, sizeof(buffer), _Stream) ) {
                int n = sscanf(buffer, "%f,%f,%f", &x, &y, &angle);
                if ( n == 3 ) {
                        AddPoint(_Points, (SDL_FPoint){x,y}, &angle, NULL, _Parametrs);
                } else {
                        n = strlen(buffer) - 1;
                        if ( buffer[n] == '\n' ) 
                                buffer[n] = '\0';
                        LogNotice("ShowOpenFIleDialog (LoadPoints)", "wrong format for <.csv>: \"%s\"", buffer);
                }
        }
}

void __ParseTXT(PArray *_Points, FILE *_Stream, Parametrs *_Parametrs) {
        float x = 0;
        float y = 0;
        float angle = 0;

        char buffer[1024] = "";

        while ( fgets(buffer, sizeof(buffer), _Stream) ) {
                int n = sscanf(buffer, "%f %f %f", &x, &y, &angle);
                if ( n == 3 ) {
                        AddPoint(_Points, (SDL_FPoint){x,y}, &angle, NULL, _Parametrs);
                } else {
                        n = strlen(buffer) - 1;
                        if ( buffer[n] == '\n' ) 
                                buffer[n] = '\0';
                        LogNotice("ShowOpenFIleDialog (LoadPoints)", "wrong format for <.txt>: \"%s\"", buffer);
                }
        }
}


void LoadPoints(PArray* _Points, Parametrs *_Parametrs) {
        if ( _Points == NULL ) {
                LogNotice("SavePoints", "Wrong pointer to [PArray]");
                return;
        }

        if ( _Parametrs == NULL ) {
                LogNotice("SavePoints", "Wrong pointer to [Parametrs]");
                return;
        }

        if ( _Points->format == FILE_FORMAT_UNDEFINED ) {
                LogNotice("SavePoints", "Undefined file format");
                return;
        }

        FreePoints(_Points);
        _Points->points = NULL;

        FILE *file = fopen(_Points->file_name, "r");
        
        switch ( _Points->format ) {
                case FILE_FORMAT_CSV:
                        __ParseCSV(_Points, file, _Parametrs);
                        break;
                case FILE_FORMAT_JSON:
                        __ParseJSON(_Points, file, _Parametrs);
                        break;
                default:
                        __ParseTXT(_Points, file, _Parametrs);
        }

        _Points->changed = 1;
}


static void SDLCALL __OpenFileDialogCallback(void* userdata, const char* const* filelist, int filter) {
        if ( filelist == NULL ) {
                LogNotice("ShowOpenFIleDialog (__OpenFileDialogCallback)", "An error occured: %s", SDL_GetError());
                return;
        }

        if ( *filelist == NULL ) {
                LogNotice("ShowOpenFIleDialog (__OpenFileDialogCallback)", "No files selected");
                return;
        }

        FileSaveArgs *args = (FileSaveArgs*)userdata;
        if ( args == NULL || args->points == NULL || args->parametrs == NULL ) {
                LogNotice("ShowSaveFIleDialog (__SaveFileDialogCallback)", "No args received");
                return;
        }

        FILESAVE_FORMAT format = FILE_FORMAT_UNDEFINED;
        if ( filter >= 0 && filter < 3 ) {
                format = (FILESAVE_FORMAT)filter;
        } else {
                format = DefineFileFormat(*filelist);
        }

        args->points->format = format;
        strcpy_s(args->points->file_name, MAX_PATH, *filelist);

        LoadPoints(args->points, args->parametrs);
}


void ShowOpenFIleDialog(SDL_Window *_Window, const char *_Default_location, FileSaveArgs *_Args) {
        SDL_ShowOpenFileDialog(__OpenFileDialogCallback, _Args, _Window, dialog_filters, 4, _Default_location, 0);
}