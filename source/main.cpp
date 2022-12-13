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

TEXTURE _tetris_map[MAP_N_BORDER];
SDL_Rect TetrisPieceMap[MAP_N_BORDER];

inline void init(){

    const char * WindowIcon = "assets/Title.png";

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    cWindow = SDL_CreateWindow  (title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, nScreenWidth, nScreenHeight, SDL_WINDOW_SHOWN);
    cRender = SDL_CreateRenderer(cWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface * cSurface = IMG_Load(WindowIcon);
  	SDL_SetWindowIcon(cWindow, cSurface);
  	SDL_FreeSurface(cSurface);
}

void TEXTURE::free(){
    SDL_DestroyTexture(cTexture);
    IMGW = 0;
    IMGH = 0;
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
        rec.w = clip->w * 0.5;
        rec.h = clip->h * 0.5;
    }

    SDL_RenderCopy(cRender, cTexture, clip, &rec);
}

inline void LoadDims(){

    const uint16_t IMGWMAP = 382;
    const uint16_t IMGHMAP = 700;

    const uint16_t BACKGROUND_WIDTH = 640;
    const uint16_t BACKGROUND_HEIGHT = 480;

    _tetris_map[ MAP_N_BORDER ].LoadTexture("assets/Board/Board.png");
    TetrisPieceMap[ MAP_N_BORDER ] = {NULL, NULL, IMGWMAP, IMGHMAP};

    _tetris_map[ BACKGROUND ].LoadTexture("assets/background.png");
    TetrisPieceMap[ BACKGROUND ] = {NULL, NULL, ((BACKGROUND_WIDTH<<2) - BACKGROUND_WIDTH), ((BACKGROUND_HEIGHT<<2) - BACKGROUND_HEIGHT)};

    _tetris_T[ TETRIS_T_PIECE ].LoadTexture("assets/single/Blue.png");
    TetrisPieceT[ TETRIS_T_PIECE ][ T_CENTRAL ] = {NULL, NULL, IMGW, IMGH};
    TetrisPieceT[ TETRIS_T_PIECE ][ T_RIGHT   ] = {NULL, NULL, IMGW, IMGH};
    TetrisPieceT[ TETRIS_T_PIECE ][ T_LEFT    ] = {NULL, NULL, IMGW, IMGH};
    TetrisPieceT[ TETRIS_T_PIECE ][ T_UP      ] = {NULL, NULL, IMGW, IMGH};
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

    IMGW *= 0.5;
    IMGH *= 0.5;

    // resizing map and initial tetris position on map

    TetrisPieceMap[ MAP_N_BORDER ].w = (TetrisPieceMap[ MAP_N_BORDER ].w * 1.80  );
    TetrisPieceMap[ MAP_N_BORDER ].h = (TetrisPieceMap[ MAP_N_BORDER ].h * 1.80  ); 
    BORDER_IMGW                      = (TetrisPieceMap[ MAP_N_BORDER ].w         );
    BORDER_IMGH                      = (TetrisPieceMap[ MAP_N_BORDER ].h         );
    uint16_t MAP_X                   = ((nScreenWidth -  BORDER_IMGW * 0.5) * 0.5);
    uint16_t MAP_Y                   = ((nScreenHeight - BORDER_IMGH * 0.5) * 0.5);
    
    // Initial position for the tetris pieces
    uint16_t POS_X                   = MAP_X + (MAP_X>>1);
    uint16_t POS_Y                   = MAP_Y;

    // The positions relative to the center block
    uint16_t POS_X_UP                = POS_X;
    uint16_t POS_Y_UP                = (POS_Y - IMGH);
    uint16_t POS_X_LEFT              = (POS_X - IMGW);
    uint16_t POS_Y_LEFT              = POS_Y;
    uint16_t POS_X_RIGHT             = (POS_X + IMGW);
    uint16_t POS_Y_RIGHT             = POS_Y;
    float NudgeVelocity              = BORDER_IMGW / 12; // 12 is the number of tiles in the map
    float speed                      = (IMGH - (IMGH * 0.5));
                 POS_X               = MAP_X + (((int)NudgeVelocity<<1) + (int)NudgeVelocity);
                 POS_Y               = NudgeVelocity; // One tile down



    printf("Window position -> [%d][%d]", MAP_X, MAP_Y);

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

            if     (input[ SDL_SCANCODE_V ] || input[ SDL_SCANCODE_L ]     ) key = 'U';
            else if(input[ SDL_SCANCODE_UP ]|| input[ SDL_SCANCODE_W ]     ) key = 'V';
            else if(input[ SDL_SCANCODE_A ] || input[ SDL_SCANCODE_LEFT ]  ) key = 'L';
            else if(input[ SDL_SCANCODE_D ] || input[ SDL_SCANCODE_RIGHT ] ) key = 'R';
            else if(input[ SDL_SCANCODE_ESCAPE ]                           ) key = '~';
            else                                                             key = ' ';
        }
// EITHER CHECK WHICH POSITION IT IS IN AND ASSIGN X, Y ACCORDINGLY, OR USE ALGORITHM TO COMPUTE THEIR POSITIONS DYNAMICALLY
        
        switch(key){
            case 'U':
                ++CURRENT_DEGREE;
                printf("rotation[%d]\nPOS_X[%d]\nPOS_Y[%d]\n\n\n", CURRENT_DEGREE, POS_X, POS_Y);
                SDL_Delay(50);
            break;
            case 'V':
                --CURRENT_DEGREE;
                printf("rotation[%d]\nPOS_X[%d]\nPOS_Y[%d]\n\n\n", CURRENT_DEGREE, POS_X, POS_Y);
                SDL_Delay(50);
            break;
            case 'L':
            printf("rotation[%d]\nPOS_X[%d]\nPOS_Y[%d]\n\n\n", CURRENT_DEGREE, POS_X, POS_Y);
                POS_X = 
                (POS_X - ((int)NudgeVelocity>>1) <= (MAP_X + NudgeVelocity)) && CURRENT_DEGREE%4==0 ? (MAP_X + NudgeVelocity          )
            :   (POS_X - ((int)NudgeVelocity>>1) <= (MAP_X + NudgeVelocity)) && CURRENT_DEGREE%4==1 ? (MAP_X + NudgeVelocity          )
            :   (POS_X - ((int)NudgeVelocity>>1) <= (MAP_X + NudgeVelocity)) && CURRENT_DEGREE%4==2 ? (MAP_X + NudgeVelocity          )
            :   (POS_X - ((int)NudgeVelocity>>1) <= (MAP_X + NudgeVelocity)) && CURRENT_DEGREE%4==3 ? (MAP_X + ((int)NudgeVelocity>>1))
            :   POS_X - ((int)NudgeVelocity>>1);
            break;
            case 'R':
            printf("rotation[%d]\nPOS_X[%d]\nPOS_Y[%d]\n\n\n", CURRENT_DEGREE, POS_X, POS_Y);
                POS_X = 
                (POS_X + ((int)NudgeVelocity>>1) >= ((int)nScreenWidth>>1) + ((int)NudgeVelocity<<1)) - IMGW && CURRENT_DEGREE%4==0 ? (((int)nScreenWidth>>1) + ((int)NudgeVelocity<<1)) - IMGW
            :   (POS_X + ((int)NudgeVelocity>>1) >= ((int)nScreenWidth>>1) + ((int)NudgeVelocity<<1))        && CURRENT_DEGREE%4==1 ? (((int)nScreenWidth>>1) + ((int)NudgeVelocity<<1))
            :   (POS_X + ((int)NudgeVelocity>>1) >= ((int)nScreenWidth>>1) + ((int)NudgeVelocity<<1)) - IMGW && CURRENT_DEGREE%4==2 ? (((int)nScreenWidth>>1) + ((int)NudgeVelocity<<1)) - IMGW
            :   (POS_X + ((int)NudgeVelocity>>1) >= ((int)nScreenWidth>>1) + ((int)NudgeVelocity<<1)) - IMGW && CURRENT_DEGREE%4==3 ? (((int)nScreenWidth>>1) + ((int)NudgeVelocity<<1)) - IMGW
            :   POS_X + ((int)NudgeVelocity>>1);
            break;
            case '~':
                close();
                running = false;
            break;
        }
        // a check for each iteration and their computation
        switch(CURRENT_DEGREE%4){
            case 0:

            POS_X_UP    = POS_X; POS_Y_UP             = (POS_Y - IMGH) + (IMGH>>1);
            POS_X_LEFT  = (POS_X - IMGW); POS_Y_LEFT  = POS_Y + (IMGH>>1);
            POS_X_RIGHT = (POS_X + IMGW); POS_Y_RIGHT = POS_Y + (IMGH>>1);

            POS_Y       = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==0 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1)) : (POS_Y + speed);
            POS_Y_UP    = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==0 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1)) :        POS_Y_UP;
            POS_Y_LEFT  = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==0 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1)) :      POS_Y_LEFT;
            POS_Y_RIGHT = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==0 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1)) :     POS_Y_RIGHT;
                
            break;
            case 1:

            POS_X_UP    = (POS_X - IMGW); POS_Y_UP = POS_Y + (IMGH>>1);
            POS_X_LEFT  = POS_X; POS_Y_LEFT        = (POS_Y + IMGH) + (IMGH>>1);
            POS_X_RIGHT = POS_X; POS_Y_RIGHT       = (POS_Y - IMGH) + (IMGH>>1);
            
            POS_Y       = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==1 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1)) : (POS_Y + speed);
            POS_Y_UP    = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==1 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1)) :        POS_Y_UP;
            POS_Y_LEFT  = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==1 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1)) :      POS_Y_LEFT;
            POS_Y_RIGHT = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==1 ? (nScreenHeight - (IMGH*3 ) - ((int)NudgeVelocity>>1)) :     POS_Y_RIGHT;
            
            break;
            case 2:

            POS_X_UP    = POS_X; POS_Y_UP             = (POS_Y + IMGH) + (IMGH>>1);
            POS_X_LEFT  = (POS_X + IMGW); POS_Y_LEFT  = POS_Y + (IMGH>>1);
            POS_X_RIGHT = (POS_X - IMGW); POS_Y_RIGHT = POS_Y + (IMGH>>1);

            POS_Y       = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==2 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1)) : (POS_Y + speed);
            POS_Y_UP    = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==2 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1)) :        POS_Y_UP;
            POS_Y_LEFT  = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==2 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1)) :      POS_Y_LEFT;
            POS_Y_RIGHT = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==2 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1)) :     POS_Y_RIGHT;
            
            break;
            case 3:

            POS_X_UP    = (POS_X + IMGW); POS_Y_UP = POS_Y + (IMGH>>1);
            POS_X_LEFT  = POS_X; POS_Y_LEFT        = (POS_Y - IMGH) + (IMGH>>1);
            POS_X_RIGHT = POS_X; POS_Y_RIGHT       = (POS_Y + IMGH) + (IMGH>>1);

            POS_Y       = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==3 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1)) : (POS_Y + speed);
            POS_Y_UP    = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==3 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1)) :        POS_Y_UP;
            POS_Y_LEFT  = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==3 ? (nScreenHeight - (IMGH*3 ) - ((int)NudgeVelocity>>1)) :      POS_Y_LEFT;
            POS_Y_RIGHT = (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==3 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1)) :     POS_Y_RIGHT;
            
            break;
        }

        // ((int)NudgeVelocity>>1) -> This represents a single tile unit of the tetris map

        /////////////////////////DEFINITIONS FOR THE BOUNDARIES AND LOGIC OF THE MOVING TETIS PIECES///////////////////////////////
/*
            POS_Y = 
            (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==0 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==1 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==2 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==3 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1))
        :   (POS_Y + speed);

            POS_Y_UP = 
            (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==0 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==1 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==2 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==3 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1))
        :   POS_Y_UP;

            POS_Y_LEFT = 
            (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==0 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==1 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==2 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==3 ? (nScreenHeight - (IMGH*3 ) - ((int)NudgeVelocity>>1))
        :   POS_Y_LEFT;

            POS_Y_RIGHT = 
            (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==0 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==1 ? (nScreenHeight - (IMGH*3 ) - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==2 ? (nScreenHeight - (IMGH<<1) - ((int)NudgeVelocity>>1))
        :   (POS_Y + (IMGH + ((int)NudgeVelocity>>1))) + speed >= nScreenHeight && CURRENT_DEGREE%4==3 ? (nScreenHeight - IMGH      - ((int)NudgeVelocity>>1))
        :   POS_Y_RIGHT;
*/
        SDL_Delay((int)speed << 2);

        //Resource rendering for background and map
        _tetris_map[ BACKGROUND   ].render(NULL,  NULL,  &TetrisPieceMap[ BACKGROUND   ]);
        _tetris_map[ MAP_N_BORDER ].render(MAP_X, MAP_Y, &TetrisPieceMap[ MAP_N_BORDER ]);
    
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