#include "hconsole.h"





// ============================= GLOBAL VARIABLES
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gImageTexture[TOTAL] = {NULL};
SDL_Event event;
TTF_Font* gFont = NULL;
SDL_Texture* ScoreTexture = NULL;
SDL_Texture* MaxScoreTexture = NULL;
SDL_Texture* TextTexture = NULL;
Mix_Chunk* gSample[M_TOTAL] = {NULL};
Mix_Music* gMusic = NULL;

int dr_Background = 0;
int dr_Ground = 0;

// ============================= FIRST DECLARE ASSISTANT FUNCTION
SDL_Texture* loadImageTexture(string path);
SDL_Texture* loadFontTexture(string text, SDL_Color color);

// ============================== CONSOLE FUNCTION

// Init console
bool cs_Init_Console()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        cout << "Could not init SDL\n";
        return false;
    }
    else
    {
        gWindow = SDL_CreateWindow("Spaceship", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            cout << "Could not create Window\n";
            return false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                cout << "Could not create Renderer\n";
                return false;
            }
            else
            {
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    cout << "Could not init SDL image\n";
                    return false;
                }
                if (TTF_Init() == -1)
                {
                    cout << "Could not init font\n";
                    return false;
                }
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    cout << "Could not init SDL mixer\n";
                    return false;
                }
                else return true;
            }
        }
    }
}
// Load media
bool cs_Load_Media()
{
    bool success = true;
    gImageTexture[BACKGROUND] = loadImageTexture("image/background.png");
    if (gImageTexture[BACKGROUND] == NULL) success = false;
    gImageTexture[PLAYER1] = loadImageTexture("image/player1.png");
    if (gImageTexture[PLAYER1] == NULL) success = false;
    gImageTexture[PLAYER2] = loadImageTexture("image/player2.png");
    if (gImageTexture[PLAYER2] == NULL) success = false;
    gImageTexture[GRASS] = loadImageTexture("image/grass.png");
    if (gImageTexture[GRASS] == NULL) success = false;
    gImageTexture[DIRT] = loadImageTexture("image/dirt.png");
    if (gImageTexture[DIRT] == NULL) success = false;
    gFont = TTF_OpenFont("font/joystix.ttf", 40);
    if (gFont == NULL) success = false;
    gSample[JUMP] = Mix_LoadWAV("music/jump.wav");
    if (gSample[JUMP] == NULL)
    {
        success = false;
    }
    gSample[DEAD] = Mix_LoadWAV("music/dead.wav");
    if (gSample[DEAD] == NULL)
    {
        success = false;
    }
    gSample[HIT] = Mix_LoadWAV("music/hit.wav");
    if (gSample[HIT] == NULL)
    {
        success = false;
    }
    gSample[LEVEL] = Mix_LoadWAV("music/level.wav");
    if (gSample[LEVEL] == NULL)
    {
        success = false;
    }
    gSample[SCORE] = Mix_LoadWAV("music/score.wav");
    if (gSample[SCORE] == NULL)
    {
        success = false;
    }
    gMusic = Mix_LoadMUS("music/background.wav");
    if (gMusic == NULL)
    {
        success = false;
    }
    return success;
}
// Prepare
void cs_Prepare_Console()
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
}
// Get width and height
void cs_Get_ImageSize(cs_image_type type, int &width, int &height)
{
    SDL_QueryTexture(gImageTexture[type], NULL, NULL, &width, &height);
}
// Render image
void cs_Render_Image(cs_image_type type, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(gRenderer, gImageTexture[type], NULL, &rect);
}
// Render image EX
void cs_Render_Image(cs_image_type type, int x, int y, int w, int h, double angle, bool isFlip)
{
    SDL_RendererFlip flip = isFlip ? SDL_FLIP_VERTICAL: SDL_FLIP_NONE;
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopyEx(gRenderer, gImageTexture[type], NULL, &rect, angle, NULL, flip);
}
// Get input type
cs_input_type cs_Get_Input()
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                return GAME_QUIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_j:
                        return BUTTON_FIRE;
                        break;
                    case SDLK_p:
                        return BUTTON_CONSOLE;
                        break;
                    default:
                        break;
                }

                break;
            default:
                break;
        }
    }
    return NOTHING;
}
// Show image to screen
void cs_Present_Image()
{
    SDL_RenderPresent(gRenderer);
}
// Close console
void cs_Close_Console()
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyTexture(ScoreTexture);
    ScoreTexture = NULL;
    SDL_DestroyTexture(MaxScoreTexture);
    MaxScoreTexture = NULL;
    SDL_DestroyTexture(TextTexture);
    TextTexture = NULL;
    TTF_CloseFont(gFont);
    gFont = NULL;
    for (int i = 0; i < TOTAL; i++)
    {
        SDL_DestroyTexture(gImageTexture[i]);
        gImageTexture[i] = NULL;
    }
    for (int i = 0; i < M_TOTAL; i++)
    {
        Mix_FreeChunk(gSample[i]);
        gSample[i] = NULL;
    }
    Mix_FreeMusic(gMusic);
    gMusic = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
}
// Draw running background
void cs_Draw_Background(bool& onMatch)
{
    cs_Render_Image(BACKGROUND, dr_Background, 0, 1836, 720);
    cs_Render_Image(BACKGROUND, dr_Background + 1836, 0, 1836, 720);
    dr_Background--;
    if (dr_Background == -1836) dr_Background = 0;
    int temp = dr_Ground;
    while (temp < SCREEN_WIDTH)
    {
        cs_Render_Image(GRASS, temp, 0, 40, 40, 0, true);
        cs_Render_Image(GRASS, temp, SCREEN_HEIGHT - 40, 40, 40);
        temp += 40;
    }
    if (onMatch)
    {
        dr_Ground -= 2;
        if (dr_Ground == -40) dr_Ground = 0;
    }
}
// Draw ground
void cs_Draw_Ground(int x, int y, int space_obstacle)
{
    int temp = y - 70;
    cs_Render_Image(GRASS, x, temp, 70, 70, 0, true);
    while (temp > 0)
    {
        temp -= 70;
        cs_Render_Image(DIRT, x, temp, 70, 70);
    }
    temp = y + space_obstacle;
    cs_Render_Image(GRASS, x, temp, 70, 70);
    temp += 70;
    while (temp < SCREEN_HEIGHT)
    {
        cs_Render_Image(DIRT, x, temp, 70, 70);
        temp += 70;
    }
}
// Get start time
uint32_t cs_Get_StartTime()
{
    return SDL_GetTicks();
}
// Check running time
bool cs_Check_RunningTime(uint32_t& startTime, int waitTime)
{
    if (SDL_GetTicks() - startTime >= waitTime) return true;
    else return false;
}

// Update score
void cs_Update_Score(int score)
{
    SDL_DestroyTexture(ScoreTexture);
    SDL_Color color = {0x5A, 0x17, 0x5D};
    ScoreTexture = loadFontTexture(to_string(score), color);
}
// Update Max Score
void cs_Update_MaxScore(int MaxScore)
{
    SDL_DestroyTexture(MaxScoreTexture);
    SDL_Color color = {0xFF, 0xBE, 0x00};
    MaxScoreTexture = loadFontTexture("Max Score: " + to_string(MaxScore), color);
}
// Draw score
void cs_Draw_Score()
{
    SDL_Rect rect = {SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 6, 40, 80};
    SDL_RenderCopy(gRenderer, ScoreTexture, NULL, &rect);
    rect = {SCREEN_WIDTH * 2 / 3 + 30, 40, SCREEN_WIDTH / 3 - 40, 40};
    SDL_RenderCopy(gRenderer, MaxScoreTexture, NULL, &rect);
}
// Update Text
void cs_Update_Text(string text)
{
    SDL_DestroyTexture(TextTexture);
    if (text != "")
    {
        SDL_Color color = {0xC6, 0xE2, 0xFF};
        TextTexture = loadFontTexture(text, color);
    }
    else TextTexture = NULL;

}
// Draw text
void cs_Draw_Text()
{
    if (TextTexture != NULL)
    {
        int w, h;
        SDL_QueryTexture(TextTexture, NULL, NULL, &w, &h);
        SDL_Rect rect = {SCREEN_WIDTH*3/4 - w/2, SCREEN_HEIGHT/2, SCREEN_WIDTH / 2, h};
        SDL_RenderCopy(gRenderer, TextTexture, NULL, &rect);
    }
}
// Play music
void cs_Play_Music(cs_music_type type)
{
    Mix_PlayChannel(-1, gSample[type], 0);
}
// Play Background Music
void cs_Play_BackgroundMusic()
{
    if (Mix_PlayingMusic() == 0)
    {
        Mix_PlayMusic(gMusic, -1);
    }
}
// Stop music
void cs_Stop_Music()
{
    Mix_HaltMusic();
}

// ============================== ASSISTANT FUNCTION
// Load PNG file
SDL_Texture* loadImageTexture(string path)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = NULL;
    loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        cout << "Could not load image " << path << endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            cout << "Could not create Texture from " << path << endl;
        }
    }
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}
// Load font texture
SDL_Texture* loadFontTexture(string text, SDL_Color color)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = NULL;
    loadedSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    if (loadedSurface == NULL)
    {
        cout << "Could not load surface from text " << text << endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            cout << "Could not create texture from text " << text << endl;
        }
    }
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}
