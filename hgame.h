#ifndef HGAME_H_INCLUDED
#define HGAME_H_INCLUDED
#include "hconsole.h"
#include <vector>
using namespace std;

const float GRAVITY = 0.36;
const int PLAYER_JUMP = 7.5;

struct s_position
{
    int x; int y; int w; int h;
};

class csprite
{
    public:
        csprite(cs_image_type type, int x, int y, int w, int h);
        void sp_Move_Sprite(int x, int y);
    protected:
        s_position position;
        cs_image_type type;

        friend bool onCollision(csprite& sprite1, csprite& sprite2);
};

class cplayer : public csprite
{
    public:
        cplayer(cs_image_type type, int x, int y, int w, int h) : csprite(type, x, y, w, h) {}
        void pl_Render_Player(bool isNormal);
        void pl_Spawn_Player();
        void pl_Run();
        void pl_Jump();
        bool pl_Fall_Outside();
    private:
        float posY;
        float v;

};

class cground : public csprite
{
    public:
        cground(cs_image_type type, int x, int y, int w, int h) : csprite(type, x, y, w, h) {}
        void gr_Run();
        void gr_Render_Ground(int space_obstacle);
        bool gr_isOutside();
        bool gr_isNearPlayer();
        bool gr_isOverPlayer();
    private:

};

class cgame
{
    public:
        cgame();
        bool ga_Init_Game();
        void ga_Prepare_Game();
        void ga_Event_Handle();
        void ga_Draw_Image();
        void ga_Close_Game();
        bool ga_IsRunning;
        bool ga_OnGame;
    private:
        cs_input_type input_type;
        vector<cground> ground;
        vector<cplayer> player;
        void Spawn_Obstacle();
        void Effect_Dead();

        int space_obstacle;
        int count_obstacle;
        bool onMatch;
        bool isAlive;
        bool isWait;
        bool isDie;
        int score;
        int MaxScore;

        int ph_Player_Dead;
        uint32_t ti_Player_Dead;
        uint32_t ti_isWait;
        bool bo_isWait;
};

int random(int MIN, int MAX);
bool onCollision(csprite& sprite1, csprite& sprite2);

#endif // HGAME_H_INCLUDED
