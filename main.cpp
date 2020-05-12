#include "hgame.h"

int main(int argc, char* argv[])
{
    cgame* game = new cgame;
    if (game->ga_Init_Game())
    {
        while (game->ga_OnGame)
        {
            game->ga_Prepare_Game();
            while (game->ga_IsRunning)
            {
                game->ga_Event_Handle();
                game->ga_Draw_Image();
            }
        }
        game->ga_Close_Game();
    }
    delete game;
    return 0;
}
