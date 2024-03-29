#ifndef TETRIS_H_
#define TETRIS_H_

#include <stdio.h>
#include <chrono>
#include <cmath>
#include <SDL.h>
#include <SDL_Image.h>

#define PI 3.14159
#define CAT_COL 4
#define CAT_ROW 21
#define NUM_OF_FRAMES ((CAT_ROW<<2) - 5)
#define CAT_FRAMES 8

class TEXTURE{

    private:

    uint16_t IMG_Width;
    uint16_t IMG_Height;

    SDL_Texture * cTexture;

    public:

    void free();
    void LoadTexture(const char * PTH);
    void RenderFlipTexture(uint16_t x, uint16_t y, SDL_Rect * clip, const double angle, const SDL_Point * center);
    void render(uint16_t x, uint16_t y, SDL_Rect * clip);
    
    int Width(){ return IMG_Width; }
    int Height(){ return IMG_Height; }
};

enum Tet_pieces{

    TETRIS_T_PIECE,

    T_CENTRAL,
    T_RIGHT,
    T_LEFT,
    T_UP,

    BACKGROUND,

    MAP_N_BORDER,

    GAME_BORDER,

    NUM_OF_TETRIS_TEXTURES
};
enum animations{
    ORANGE_CAT
};
enum rotations{
    MAIN,
    SIDE_LEFT,
    UPSIDE_DOWN,
    SIDE_RIGHT
};

#endif