#include "easy_json_parser.h"


#define NULL_TOKEN ((Token){NULL, TKNTP_UNDEFINED})
#define IS_SPACE(X)  ( X == ' ' || X == '\t' || X == '\n' )

enum TokenType {
        TKNTP_COMMA           = 0,
        TKNTP_COLON           = 1,
        TKNTP_OPEN_BRACKET    = 2,
        TKNTP_CLOSE_BRACKET   = 3,
        TKNTP_OPEN_F_BRACKET  = 4,
        TKNTP_CLOSE_F_BRACKET = 5,
        TKNTP_KEY             = 7,
        TKNTP_VALUE           = 8,
        TKNTP_ELEMENT         = 200,
        TKNTP_UNDEFINED       = -10
};

typedef struct Token {
        char *token;
        enum TokenType type;
        struct Token *prev;
        struct Token *next;
} Token;



void FreeToken(Token *_Token) {
        if ( _Token == NULL ) {
                return;
        }

        free(_Token->token);
        free(_Token);
}

void FreeTokens(Token *_Tokens) {
        while ( _Tokens ) {
                FreeToken(_Tokens);
                _Tokens = _Tokens->next;
        }
}




Token *_GetToken(char **_Buffer, enum TokenType _PrevType) {
        if ( **_Buffer == '\0' ) {
                return NULL;
        }

        char *now = *_Buffer;
        size_t len = 1;
        enum TokenType type = TKNTP_UNDEFINED;
        
        bool string_opened = 0;
        char *lf_space = NULL; // last-first space


        // trim token
        while ( IS_SPACE(*now) ) {
                if ( *now == '\0' ) {
                        return NULL;
                }
                now++;
        }

        *_Buffer = now;
        switch ( *now ) {
                case '\0':
                        return NULL;
                case ':':
                        type = TKNTP_COLON;
                        goto create_token;
                case '[':
                        type = TKNTP_OPEN_BRACKET;
                        goto create_token;
                case ']':
                        type = TKNTP_CLOSE_BRACKET;
                        goto create_token;
                case '{':
                        type = TKNTP_OPEN_F_BRACKET;
                        goto create_token;
                case '}':
                        type = TKNTP_CLOSE_F_BRACKET;
                        goto create_token;
                case ',':
                        type = TKNTP_COMMA;
                        goto create_token;
                case '\"':
                        string_opened = 1;
                        break;
                default:
                        break;
                        
        }
        type = TKNTP_ELEMENT;
        
        // parsing
        now++;
        bool prev_is_space = 0;
        while ( now ) {
                // check delimetrs
                if ( !string_opened && ( *now == ':' || *now == '[' || *now == ']' || *now == '{' || *now == '}' || *now == ',' ) ) {
                        break;
                }

                // check string ("")
                if ( *now == '\"' && now[-1] != '\\' ) {
                        string_opened = !string_opened;
                }

                // find last space for trim string
                if ( IS_SPACE(*now) ) {
                        if ( prev_is_space == 0 ) {
                                lf_space = now;
                                prev_is_space = 1;
                        }
                } else {
                        prev_is_space = 0;
                        lf_space = NULL;
                }

                // next char
                now++;
                len++;
        }


        create_token:
        // allocate memory for token object
        Token *new_token = calloc(1, sizeof(Token));
        if ( new_token == NULL ) {
                return NULL;
        }

        // define token type
        if ( type == TKNTP_ELEMENT ) {
                if ( _PrevType == TKNTP_COLON ) {
                        new_token->type = TKNTP_VALUE;
                } else {
                        new_token->type = TKNTP_KEY;
                }
        } else {
                new_token->type = type;
        }

        if ( string_opened ) {
                new_token->type = TKNTP_UNDEFINED;
        }


        // allocate memory for token string
        new_token->token = calloc(len + 1, sizeof(char));
        if ( new_token->token == NULL ) {
                return NULL;
        }

        // copy token string
        now = *_Buffer;
        for ( size_t i = 0 ; i < len ; i++ ) {
                new_token->token[i] = now[i];
        }

        // trim token
        if ( lf_space ) {
                new_token->token[lf_space-now] = '\0';
        }

        *_Buffer += len;
        return new_token;
}


bool _CheckKey(char *_Key) {
        if ( _Key == NULL || *_Key != '\"' ) {
                return 0;
        }

        _Key++;
        while ( *_Key ) {
                if ( *_Key == '\"' && _Key[-1] != '\\' && _Key[1] != '\0' ) {
                        return 0;
                }
                _Key++;
        }

        return 1;
}

bool _GetValues(PArray *_Points, Token *_Tokens, Parametrs *_Parametrs) {
        int opened_brackets = 0;
        int opened_f_brackets = 0;


        SDL_FPoint cords = {
                -1, -1
        };
        float angle = 0;

        if ( _Tokens->type != TKNTP_OPEN_BRACKET ) {
                return false;
        }

        while ( _Tokens ) {

                switch ( _Tokens->type ) {
                        case TKNTP_COLON:
                                if (    _Tokens->prev == NULL || _Tokens->next == NULL ||
                                        _Tokens->prev->type != TKNTP_KEY || _Tokens->next->type != TKNTP_VALUE ) 
                                {
                                        goto wrong_syntax;
                                }
                                char *key = _Tokens->prev->token + 1;
                                char *value = _Tokens->next->token;

                                if ( strcmp(key, "x\"") == 0 ) {
                                        cords.x = atof(value);      
                                } else if ( strcmp(key, "y\"") == 0 ) {
                                        cords.y = atof(value);
                                } else if ( strcmp(key, "angle\"") == 0 ) {
                                        angle = atof(value);
                                } else {
                                        goto wrong_syntax;
                                }

                                if ( errno == ERANGE ) {
                                        goto wrong_syntax;
                                }
                                break;
                        case TKNTP_OPEN_BRACKET:
                                opened_brackets++;
                                break;
                        case TKNTP_OPEN_F_BRACKET:
                                opened_f_brackets++;
                                break;
                        case TKNTP_CLOSE_BRACKET:
                                opened_brackets--;
                                break;
                        case TKNTP_CLOSE_F_BRACKET:
                                opened_f_brackets--;
                                break;
                        case TKNTP_KEY:
                                if ( _CheckKey( _Tokens->token ) == 0 ) {
                                        goto wrong_syntax;
                                }
                                break;
                        case TKNTP_VALUE:
                                break;
                        case TKNTP_COMMA:
                                
                                break;
                        
                        default:
                                goto wrong_syntax;
                }




                if (    opened_brackets > 1 || opened_brackets < 0 
                        || opened_f_brackets > 1 || opened_f_brackets < 0 ) 
                {       
                        goto wrong_syntax;
                }

                if ( _Tokens->type == TKNTP_CLOSE_F_BRACKET ) {
                        if ( cords.x == -1 || cords.y == -1 ) {
                                goto wrong_syntax;
                        }

                        AddPoint(_Points, cords, &angle, NULL, _Parametrs);
                        cords = (SDL_FPoint){
                                -1, -1
                        };
                        angle = 0;
                }

                _Tokens = _Tokens->next;
        }


        if ( opened_brackets != 0 || opened_f_brackets != 0 ) {
                goto wrong_syntax;
        }

        return 1;

        wrong_syntax:
        FreePoints(_Points);
        return 0;
}


void __ParseJSON(PArray *_Points, FILE *_Stream, Parametrs *_Parametrs) {
        // get file size
        fseek(_Stream, 0 , SEEK_END);
        size_t file_size = ftell(_Stream);
        fseek(_Stream, 0, SEEK_SET);

        if ( file_size > 134217728 ) {
                LogNotice("ShowOpenFIleDialog (LoadPoints)", "File too large (128MB)");
                return;
        }

        // allocate buffer
        char *buffer = (char*)malloc(file_size + 1);

        // read file
        size_t buffer_size = fread(buffer, 1, file_size, _Stream);
        if (  buffer_size == 0 ) {
                LogNotice("ShowOpenFIleDialog (LoadPoints)", "couldn`t read file");
                free(buffer);
                return;
        }

        buffer[buffer_size] = '\0';

        char *token_bufffer = buffer;
        Token *array = _GetToken(&token_bufffer, TKNTP_UNDEFINED);
        array->prev = NULL;
        Token *token = array;
        while ( token ) {
                token->next = _GetToken(&token_bufffer, token->type);
                if ( token->next == NULL ) {
                        break;
                }
                token->next->prev = token;
                token = token->next;
        }

        _GetValues(_Points, array, _Parametrs);

        FreeTokens(array);

        free(buffer);
}