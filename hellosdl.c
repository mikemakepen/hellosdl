#include "SDL/SDL.h"


void game_on()
{
    while(1)
    {
        handle_userinput();
        fresh_video();
    }
}

void handle_userinput()
{
}

void fresh_video()
{}

int display_bmp(char* file_name, SDL_Surface* screen)
{
    SDL_Surface *image = NULL;
    image = SDL_LoadBMP(file_name);
    if (NULL == image)
    {
        printf("can not load bmp %s\n", file_name);
        return -1;
    }

    if (image->format->palette && screen->format->palette)
    {
        SDL_SetColors(screen, image->format->palette->colors, 0,
                image->format->palette->ncolors);
    }

    SDL_Rect src_rect, dst_rect, screen_rect;
    memset(&src_rect, 0, sizeof(src_rect));
    memset(&dst_rect, 0, sizeof(dst_rect));
    memset(&screen_rect, 0, sizeof(screen_rect));
    src_rect.w = image->w;
    src_rect.h = image->h;
    screen_rect.w = screen->w;
    screen_rect.h = screen->h;
    int vx, vy;
    vx = 3;
    vy = 3;
    while (1)
    {
        if (SDL_BlitSurface(image, &src_rect, screen, &dst_rect) < 0)
        {
            printf("blit error\n");
        }

        SDL_UpdateRect(screen, dst_rect.x, dst_rect.y, image->w, image->h);
        SDL_Delay(100);
        if (dst_rect.x + vx > screen->w
                || dst_rect.x + vx < 0)
        { 
            vx = 0 - vx;
        }
        if (dst_rect.x + vx < screen->h 
                || dst_rect.y + vy < 0)
        {
            vy = 0 - vy;
        }


        dst_rect.x += vx;
        dst_rect.y += vy;
        SDL_FillRect(screen, NULL, 222);

    }
    

    SDL_FreeSurface(image);

                 
    return 0;
}
int main(int argc, char** argv)
{
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (0 != ret)
    {
        printf("init sdl error\n");
    }
    printf("sdl system init success\n");
    SDL_Surface * screen = NULL;
    screen = SDL_SetVideoMode(320, 240, 8, SDL_SWSURFACE);
    if (NULL == screen)
    {
        printf("set videoMode fail\n");
    }
    game_on();
    display_bmp("lala.bmp", screen);

    SDL_Quit();
    printf("sdl system quit now\n");
    return 0;
}
