#include "hgame.h"
#include "hconsole.h"
#include <stdlib.h>
#include <ctime>

#include <iostream>
using namespace std;



// ****************************** CGAME CLASS ***********************************
cgame::cgame()
{
    count_obstacle = 0;
    ga_OnGame = true;
    MaxScore = 0;
}

bool cgame::ga_Init_Game()
{
    if (cs_Init_Console())
    {
        if (cs_Load_Media())
        {
            cs_Update_MaxScore(MaxScore);
            return true;
        }
        else cout << "Could not load media\n";
    }
    else cout << "Could not init console\n";
    return false;
}

void cgame::ga_Prepare_Game()
{
    ga_IsRunning = true;
    onMatch = false;
    isAlive = true;
    isWait = true;
    isDie = false;
    ti_isWait = cs_Get_StartTime();
    bo_isWait = true;
    cs_Update_Text("- Press J to Jump -");
    score = 0;
    space_obstacle = 150;
    cs_Update_Score(score);
    ph_Player_Dead = 0;
    cs_Prepare_Console();
    srand(time(NULL));
    ground.clear();
    player.clear();
    ground.reserve(15);
    player.reserve(3);
    ground.push_back(cground(GRASS, 0, 0, SCREEN_WIDTH, 35));
    ground.push_back(cground(GRASS, 0, SCREEN_HEIGHT - 35, SCREEN_WIDTH, 35));
    player.push_back(cplayer(PLAYER1, SCREEN_WIDTH / 2 - space_obstacle, SCREEN_HEIGHT / 2, 40, 30));
    player[0].pl_Spawn_Player();
    cs_Play_BackgroundMusic();
}

void cgame::ga_Event_Handle()
{
    // Input
    input_type = cs_Get_Input();
    switch (input_type)
    {
        case GAME_QUIT:
            ga_IsRunning = false;
            ga_OnGame = false;
            break;
        case BUTTON_FIRE:
            if (onMatch)
            {
                player[0].pl_Jump();
                cs_Play_Music(JUMP);
            }
            break;
        case BUTTON_CONSOLE:
            onMatch = !onMatch;
            break;
        default:
            break;
    }
    if (onMatch)
    {
        // Collision
        for (int i = 2; i < ground.size(); i++)
        {
            if (ground[i].gr_isOutside())
            {
                ground.erase(ground.begin() + i);
                i--;
            }
        }
        for (int i = 2; i < ground.size(); i++)
        {
            if (ground[i].gr_isNearPlayer())
            {
                if (onCollision(ground[i], player[0]))
                {
                    isAlive = false;
                    onMatch = false;
                    cs_Play_Music(HIT);
                }
                else if (i % 2 == 0 && ground[i].gr_isOverPlayer())
                {
                    score++;
                    cs_Update_Score(score);
                    if (score > MaxScore)
                    {
                        MaxScore = score;
                        cs_Update_MaxScore(MaxScore);
                    }
                    if (score % 10 == 0)
                    {
                        cs_Play_Music(LEVEL);
                    }
                    else cs_Play_Music(SCORE);
                    if (score % 5 == 0)
                    {
                        if (space_obstacle > 120) space_obstacle -= 10;
                    }
                }
            }
        }
        if (player[0].pl_Fall_Outside())
        {
            isAlive = false;
            onMatch = false;
            cs_Play_Music(HIT);
        }
        // Run
        if (isAlive)
        {
            for (int i = 2; i < ground.size(); i++)
            {
                ground[i].gr_Run();
            }
            for (int i = 0; i < player.size(); i++)
            {
                player[i].pl_Run();
            }
            if (count_obstacle == 150)
            {
                Spawn_Obstacle();
                count_obstacle = 0;
            }
            else count_obstacle++;
        }
    }
    else
    {
        if (isAlive)
        {
            if (input_type == BUTTON_FIRE)
            {
                onMatch = true;
                isWait = false;
                cs_Play_Music(SCORE);
            }
            else
            {
                if (cs_Check_RunningTime(ti_isWait, 800))
                {
                    bo_isWait = !bo_isWait;
                    ti_isWait = cs_Get_StartTime();
                }
            }
        }
        else Effect_Dead();
    }
}

void cgame::ga_Draw_Image()
{
    cs_Draw_Background(onMatch);
    for (int i = 2; i < ground.size(); i++)
    {
        ground[i].gr_Render_Ground(space_obstacle);
    }
    for (int i = 0; i < player.size(); i++)
    {
        player[i].pl_Render_Player(isWait);
    }
    if (isWait)
    {
        if (bo_isWait) cs_Draw_Text();
    }
    else cs_Draw_Score();
    if (isDie) cs_Draw_Text();
    cs_Present_Image();
}

void cgame::ga_Close_Game()
{
    cs_Close_Console();
}

void cgame::Spawn_Obstacle()
{
    int temp = random(105, 615 - space_obstacle);
    ground.push_back(cground(DIRT, SCREEN_WIDTH, 0, 70, temp));
    ground.push_back(cground(GRASS, SCREEN_WIDTH, temp + space_obstacle, 70, SCREEN_HEIGHT - temp - space_obstacle));
}

void cgame::Effect_Dead()
{
    if (ph_Player_Dead == 0)
    {
        ti_Player_Dead = cs_Get_StartTime();
        cs_Update_Text("- Game Over -");
        cs_Stop_Music();
        ph_Player_Dead++;
    }
    else if (ph_Player_Dead == 1)
    {
        if (cs_Check_RunningTime(ti_Player_Dead, 500))
        {
            ph_Player_Dead++;
            cs_Play_Music(DEAD);
        }
    }
    else if (ph_Player_Dead == 2)
    {
        player[0].pl_Jump();
        ti_Player_Dead = cs_Get_StartTime();
        ph_Player_Dead++;
    }
    else if (ph_Player_Dead == 3)
    {
        player[0].pl_Run();
        if (cs_Check_RunningTime(ti_Player_Dead, 50))
        {
            if (onCollision(player[0], ground[1]))
            {
                ph_Player_Dead++;
                ti_Player_Dead = cs_Get_StartTime();
            }
        }
    }
    else if (ph_Player_Dead == 4)
    {
        if (cs_Check_RunningTime(ti_Player_Dead, 500))
        {
            isDie = true;
            ph_Player_Dead++;
        }
    }
    else if (ph_Player_Dead == 5)
    {
        if (input_type == BUTTON_FIRE) ga_IsRunning = false;
    }
}

// ===================================== CSPRITE CLASS
csprite::csprite(cs_image_type type, int x, int y, int w, int h)
{
    this->type = type;
    this->position.x = x;
    this->position.y = y;
    this->position.w = w;
    this->position.h = h;
}
// Move
void csprite::sp_Move_Sprite(int x, int y)
{
    position.x += x;
    position.y += y;
}

// ====================================== CGROUND CLASS
void cground::gr_Run()
{
    position.x -= 2;
}
// Render
void cground::gr_Render_Ground(int space_obstacle)
{
    if (type == DIRT)
    {
        cs_Draw_Ground(position.x, position.y + position.h, space_obstacle);
    }
}
// Check if go outside
bool cground::gr_isOutside()
{
    if (position.x < -70) return true;
    else return false;
}
// Is near player ?
bool cground::gr_isNearPlayer()
{
    if (position.x > 300 && position.x < 450)
    {
        return true;
    }
    else return false;
}
// Is over player ?
bool cground::gr_isOverPlayer()
{
    if (position.x == 320) return true;
    else return false;
}

// ====================================== CPLAYER CLASS
// Spawn
void cplayer::pl_Spawn_Player()
{
    posY = position.y;
    v = -PLAYER_JUMP;
}
// Render
void cplayer::pl_Render_Player(bool isNormal)
{
    float angle = 0.0;
    if (!isNormal)
    {
        if (v == 0) angle = 0;
        else
        {
            angle = atan(-10 / v) - PI / 2;
            if (v > 0) angle += PI;
        }
        angle = angle * 180 / PI;
    }
    cs_Render_Image(type, position.x, position.y, position.w, position.h, angle, false);
}
// Run
void cplayer::pl_Run()
{
    posY = posY + (v + GRAVITY / 2);
    v = v + GRAVITY;
    position.y = posY;
}
// Jump
void cplayer::pl_Jump()
{
    v = -PLAYER_JUMP;
}
// Check fall outside
bool cplayer::pl_Fall_Outside()
{
    if (position.y <= 35 || position.y >= 655) return true;
    else return false;
}

// ======================================   ASSISTANT FUNCTION
int random(int MIN, int MAX)
{
	return MIN + rand() % (MAX - MIN + 1);
}

bool onCollision(csprite& sprite1, csprite& sprite2)
{
    if (sprite1.position.x + sprite1.position.w < sprite2.position.x) return false;
    if (sprite1.position.y + sprite1.position.h < sprite2.position.y) return false;
    if (sprite2.position.x + sprite2.position.w < sprite1.position.x) return false;
    if (sprite2.position.y + sprite2.position.h < sprite1.position.y) return false;
    return true;
}
