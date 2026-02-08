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
        TKNTP_ELEMENT         = 6,
        TKNTP_KEY             = 7,
        TKNTP_VALUE           = 8,
        TKNTP_UNDEFINED       = -1
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


        // trimming token
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
        
        now++;
        bool prev_is_space = 0;
        while ( now ) {
                if ( *now == '[' || *now == ']' || *now == '{' || *now == '}' || *now == ',' ) {
                        break;
                }

                if ( *now == '\"' && now[-1] != '\\' ) {
                        string_opened = !string_opened;
                }

                if ( *now == ':' && string_opened == 0 ) {
                        break;
                }

                if ( IS_SPACE(*now) ) {
                        if ( prev_is_space == 0 ) {
                                lf_space = now;
                                prev_is_space = 1;
                        }
                } else {
                        prev_is_space = 0;
                        lf_space = NULL;
                }

                now++;
                len++;
        }


        create_token:
        Token *new_token = calloc(1, sizeof(Token));
        if ( new_token == NULL ) {
                return NULL;
        }

        // defining token type
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


        // allocating memory
        new_token->token = calloc(len + 1, sizeof(char));
        if ( new_token->token == NULL ) {
                return NULL;
        }

        // copy token string
        now = *_Buffer;
        for ( size_t i = 0 ; i < len ; i++ ) {
                new_token->token[i] = now[i];
        }

        // trimming token
        if ( lf_space ) {
                new_token->token[lf_space-now] = '\0';
        }

        *_Buffer += len;
        return new_token;
}




void __ParseJSON(PArray *_Points, FILE *_Stream, Parametrs *_Parametrs) {
        float x = 0;
        float y = 0;
        float angle = 0;

        // get file size
        fseek(_Stream, 0 , SEEK_END);
        size_t file_size = ftell(_Stream);
        fseek(_Stream, 0, SEEK_SET);

        if ( file_size > 134217728 ) {
                LogNotice("ShowOpenFIleDialog (LoadPoints)", "File too large (128MB)");
                return;
        }

        char *buffer = (char*)malloc(file_size + 1);

        size_t buffer_size = fread(buffer, 1, file_size, _Stream);
        if (  buffer_size == 0 ) {
                LogNotice("ShowOpenFIleDialog (LoadPoints)", "couldn`t read file");
                free(buffer);
                return;
        }

        buffer[buffer_size] = '\0';

        printf("%f %f %f %p %p\n\n\n", x, y, angle, _Points, _Parametrs);

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

        token = array;
        while ( token ) {
                printf("%i %s\n", token->type, token->token);

                token = token->next;
        }
        

        FreeTokens(array);
        puts("\n\n");



        free(buffer);
}