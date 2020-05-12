#ifndef HCONSOLE_H_INCLUDED
#define HCONSOLE_H_INCLUDED
#include <stdint.h>
#include <math.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
using namespace std;
const float PI = 3.1416;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

// ============================ ENUM DECLARATION
enum cs_image_type
{
    BACKGROUND, PLAYER1, PLAYER2, GRASS, DIRT, TOTAL
};

enum cs_input_type
{
    BUTTON_FIRE, BUTTON_CONSOLE,
    MOUSE_FIRE,
    GAME_QUIT,
    NOTHING
};

enum cs_music_type
{
    JUMP, DEAD, HIT, LEVEL, SCORE, M_TOTAL
};


// ============================ CONSOLE FUNCTION
bool cs_Init_Console();
bool cs_Load_Media();
void cs_Prepare_Console();
void cs_Get_ImageSize(cs_image_type type, int &width, int &height);
void cs_Render_Image(cs_image_type type, int x, int y, int w, int h);
void cs_Render_Image(cs_image_type type, int x, int y, int w, int h, double angle, bool isFlip);
cs_input_type cs_Get_Input();
void cs_Present_Image();
void cs_Close_Console();
void cs_Update_Score(int score);
void cs_Update_MaxScore(int MaxScore);
void cs_Draw_Score();
void cs_Update_Text(string text);
void cs_Draw_Text();
void cs_Play_Music(cs_music_type type);
void cs_Play_BackgroundMusic();
void cs_Stop_Music();

void cs_Draw_Background(bool& onMatch);
void cs_Draw_Ground(int x, int y, int space_obstacle);

uint32_t cs_Get_StartTime();
bool cs_Check_RunningTime(uint32_t& startTime, int waitTime);


#endif // HCONSOLE_H_INCLUDED
