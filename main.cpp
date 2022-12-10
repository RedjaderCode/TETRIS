// Tetris Main char

#include "tetris.h"

const char * title = "tetris just for you";
const uint16_t nScreenWidth = 960;
const uint16_t nScreenHeight = 630;

SDL_Window * cWindow;
SDL_Renderer * cRender;
SDL_Event e;

uint8_t IMGW;
uint8_t IMGH;

uint16_t BORDER_IMGW;
uint16_t BORDER_IMGH;

TEXTURE _tetris_T[ NUM_OF_TETRIS_TEXTURES * NUM_OF_TETRIS_TEXTURES ];
SDL_Rect TetrisPieceT[ NUM_OF_TETRIS_TEXTURES ][ NUM_OF_TETRIS_TEXTURES ];

inline void init(){

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    cWindow = SDL_CreateWindow  (title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, nScreenWidth, nScreenHeight, SDL_WINDOW_SHOWN);
    cRender = SDL_CreateRenderer(cWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void TEXTURE::free(){
    SDL_DestroyTexture(cTexture);
    IMG_Width  = 0;
    IMG_Height = 0;
}

void TEXTURE::LoadTexture(const char * PTH){

    free();

    SDL_Surface * IMG = IMG_Load(PTH);
    SDL_SetColorKey(IMG, SDL_TRUE, SDL_MapRGB(IMG->format, 0x00, 0x00, 0x00));
    cTexture = SDL_CreateTextureFromSurface(cRender, IMG);

    IMG_Width  = IMG->w;
    IMG_Height = IMG->h;
    IMGW       = IMG->w;
    IMGH       = IMG->h;

    SDL_FreeSurface(IMG);
}

void TEXTURE::render(uint16_t x, uint16_t y, SDL_Rect * clip){

    SDL_Rect rec = {x, y, IMG_Width, IMG_Height};

    if(cTexture != NULL){

        rec.w = clip->w;
        rec.h = clip->h;
    }

    SDL_RenderCopy(cRender, cTexture, clip, &rec);
}

inline void LoadDims(){

    _tetris_T[ MAP_N_BORDER ].LoadTexture("assets/Board/Board.png");
    TetrisPieceT[ MAP_N_BORDER ][ GAME_BORDER ] = {NULL, NULL, IMGW, IMGH};

    BORDER_IMGW = TetrisPieceT[ MAP_N_BORDER ][ GAME_BORDER ].w;
    BORDER_IMGH = TetrisPieceT[ MAP_N_BORDER ][ GAME_BORDER ].h;

    _tetris_T[ TETRIS_T_PIECE ].LoadTexture("assets/single/Blue.png");
    TetrisPieceT[ TETRIS_T_PIECE ][ T_CENTRAL ] = {NULL, NULL, IMGW, IMGH};
    TetrisPieceT[ TETRIS_T_PIECE ][ T_RIGHT ]   = {NULL, NULL, IMGW, IMGH};
    TetrisPieceT[ TETRIS_T_PIECE ][ T_LEFT ]    = {NULL, NULL, IMGW, IMGH};
    TetrisPieceT[ TETRIS_T_PIECE ][ T_UP ]      = {NULL, NULL, IMGW, IMGH};
}

inline void close(){

    SDL_DestroyWindow(cWindow);
    SDL_DestroyRenderer(cRender);

    for(int i=0; i<NUM_OF_TETRIS_TEXTURES * NUM_OF_TETRIS_TEXTURES; i++)
        _tetris_T[i].free();

    IMGW = NULL;
    IMGH = NULL;
}

//inline uint16_t rotation(int i1, int i2, int i3);

int main(int s, char * z[]){

    const uint8_t * input = SDL_GetKeyboardState(NULL);
    bool running          = true;
    unsigned char key     = ' ';
    uint16_t CURRENT_DEGREE    = 0;

    init();
    LoadDims();

    uint16_t POS_X       = ((nScreenWidth - IMGW * 0.5) * 0.5);
    uint16_t POS_Y       = ((nScreenHeight - IMGH * 0.5) * 0.5);
    uint16_t POS_X_UP    = POS_X;
    uint16_t POS_Y_UP    = (POS_Y - IMGH);
    uint16_t POS_X_LEFT  = (POS_X - IMGW);
    uint16_t POS_Y_LEFT  = POS_Y;
    uint16_t POS_X_RIGHT = (POS_X + IMGW);
    uint16_t POS_Y_RIGHT = POS_Y;

    while(running){

        SDL_SetRenderDrawColor(cRender, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(cRender);

        while(SDL_PollEvent(&e)){

            switch(e.type){

                case SDL_QUIT:
                    close();
                    running = false;
                    break;
            }

            if(input[ SDL_SCANCODE_UP ] || input[ SDL_SCANCODE_W ] || input[ SDL_SCANCODE_U ]) key = 'U';
            else if(input[ SDL_SCANCODE_V ]) key = 'V';
            else if(input[ SDL_SCANCODE_ESCAPE ]) key = '~';
            else key = ' ';
        }
// EITHER CHECK WHICH POSITION IT IS IN AND ASSIGN X, Y ACCORDINGLY, OR USE ALGORITHM TO COMPUTE THEIR POSITIONS DYNAMICALLY
        
        switch(key){
            case 'U':
                ++CURRENT_DEGREE;
                printf("rotation[%d]\nPOS_X[%d]\nPOS_Y[%d]\n\n\n", CURRENT_DEGREE, POS_X, POS_Y);
                switch(CURRENT_DEGREE%4){
                    case 0: 
                        POS_X_UP    = POS_X; POS_Y_UP             = (POS_Y - IMGH);
                        POS_X_LEFT  = (POS_X - IMGW); POS_Y_LEFT  = POS_Y;
                        POS_X_RIGHT = (POS_X + IMGW); POS_Y_RIGHT = POS_Y;
                    break;
                    case 1: 
                        POS_X_UP    = (POS_X - IMGW); POS_Y_UP = POS_Y;
                        POS_X_LEFT  = POS_X; POS_Y_LEFT        = (POS_Y + IMGH);
                        POS_X_RIGHT = POS_X; POS_Y_RIGHT       = (POS_Y - IMGH);
                    break;
                    case 2: 
                        POS_X_UP    = POS_X; POS_Y_UP             = (POS_Y + IMGH);
                        POS_X_LEFT  = (POS_X + IMGW); POS_Y_LEFT  = POS_Y;
                        POS_X_RIGHT = (POS_X - IMGW); POS_Y_RIGHT = POS_Y;
                    break;
                    case 3: 
                        POS_X_UP    = (POS_X + IMGW); POS_Y_UP = POS_Y;
                        POS_X_LEFT  = POS_X; POS_Y_LEFT        = (POS_Y - IMGH);
                        POS_X_RIGHT = POS_X; POS_Y_RIGHT       = (POS_Y + IMGH);
                    break;
                }
                SDL_Delay(50);
            break;
            case 'V':
                --CURRENT_DEGREE;
                printf("rotation[%d]\nPOS_X[%d]\nPOS_Y[%d]\n\n\n", CURRENT_DEGREE, POS_X, POS_Y);
                switch(CURRENT_DEGREE%4){
                    case 0: 
                        POS_X_UP    = POS_X; POS_Y_UP             = (POS_Y - IMGH);
                        POS_X_LEFT  = (POS_X - IMGW); POS_Y_LEFT  = POS_Y;
                        POS_X_RIGHT = (POS_X + IMGW); POS_Y_RIGHT = POS_Y;
                    break;
                    case 1: 
                        POS_X_UP    = (POS_X - IMGW); POS_Y_UP = POS_Y;
                        POS_X_LEFT  = POS_X; POS_Y_LEFT        = (POS_Y + IMGH);
                        POS_X_RIGHT = POS_X; POS_Y_RIGHT       = (POS_Y - IMGH);
                    break;
                    case 2: 
                        POS_X_UP    = POS_X; POS_Y_UP             = (POS_Y + IMGH);
                        POS_X_LEFT  = (POS_X + IMGW); POS_Y_LEFT  = POS_Y;
                        POS_X_RIGHT = (POS_X - IMGW); POS_Y_RIGHT = POS_Y;
                    break;
                    case 3: 
                        POS_X_UP    = (POS_X + IMGW); POS_Y_UP = POS_Y;
                        POS_X_LEFT  = POS_X; POS_Y_LEFT        = (POS_Y - IMGH);
                        POS_X_RIGHT = POS_X; POS_Y_RIGHT       = (POS_Y + IMGH);
                    break;
                }
                SDL_Delay(50);
            break;
            case '~':
                close();
                running = false;
            break;
        }
    
        //Tetris Pieces initial
        _tetris_T[ TETRIS_T_PIECE ].render(POS_X,       POS_Y,       &TetrisPieceT[ TETRIS_T_PIECE ][ T_CENTRAL ]);
        _tetris_T[ TETRIS_T_PIECE ].render(POS_X_UP,    POS_Y_UP,    &TetrisPieceT[ TETRIS_T_PIECE ][ T_UP      ]);
        _tetris_T[ TETRIS_T_PIECE ].render(POS_X_LEFT,  POS_Y_LEFT,  &TetrisPieceT[ TETRIS_T_PIECE ][ T_LEFT    ]);
        _tetris_T[ TETRIS_T_PIECE ].render(POS_X_RIGHT, POS_Y_RIGHT, &TetrisPieceT[ TETRIS_T_PIECE ][ T_RIGHT   ]);

        SDL_RenderPresent(cRender);
        SDL_Delay(50);
    }

    return 0;
}