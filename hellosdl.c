#include "SDL/SDL.h"

void keep_fps();
void load_resource();
void game_on();
void handle_keydown();
void handle_userinput();
void fresh_video();
void move_ball();
int display_bmp();
int display_smallball();
int display_background();


int g_running = 0;
int g_count = 0;

SDL_Surface *g_image = NULL;
SDL_Rect g_image_rect;
int g_ball_speed_x = 50;
int g_ball_speed_y = 50;



SDL_Surface *g_screen = NULL;
SDL_Rect g_screen_rect;

SDL_Surface *g_background = NULL;


#define BMP_FILE "media/lala.bmp"
#define BACKGROUND_FILE "media/background_black.bmp"


void keep_fps()
{
	SDL_Delay(30);
}

void load_resource()
{
	g_image = SDL_LoadBMP(BMP_FILE);
    if (NULL == g_image)
    {
        printf("can not load bmp %s\n", BMP_FILE);
    }
	
	g_background = SDL_LoadBMP(BACKGROUND_FILE);
    if (NULL == g_background)
    {
        printf("can not load bmp %s\n", BMP_FILE);
    }

    if (g_image->format->palette && g_screen->format->palette)
    {
        SDL_SetColors(g_screen, g_image->format->palette->colors, 0,
                g_image->format->palette->ncolors);
    }
	
	memset(&g_image_rect, 0, sizeof(g_image_rect));

	g_image_rect.w = g_image->w;
	g_image_rect.h = g_image->h;
	
	memset(&g_screen_rect, 0, sizeof(g_screen_rect));
	g_screen_rect.w = g_screen->w;
	g_screen_rect.h = g_screen->h;
}

int display_smallball()
{

}

void release_resource()
{
	SDL_FreeSurface(g_image);
	g_image = NULL;
	SDL_FreeSurface(g_screen);
	g_screen = NULL;
}
void game_on()
{
    while(!g_running)
    {
        handle_userinput();
        fresh_video();
		keep_fps();
    }
	printf("sdl system quit now\n");
}

void handle_keydown(SDL_KeyboardEvent * key)
{
	switch (key->keysym.sym)
	{
		case SDLK_ESCAPE:
			g_running = 1;
			printf("user input escape set g_running = 1\n");
			break;
		case SDLK_UP:
			g_ball_speed_y += 5;
			break;
		case SDLK_DOWN:
			g_ball_speed_y -= 5;
			break;
		case SDLK_LEFT:
			g_ball_speed_x -= 5;
			break;
		case SDLK_RIGHT:
			g_ball_speed_x += 5;
			break;
		default:
			printf("unknown key\n");
			break;
	}
	
}
void handle_userinput()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				handle_keydown(&event.key);
				break;
			default:
				printf("unknown envent\n");
				break;
		}
	}
	
}

int display_background()
{
	if (g_background == NULL)
	{
		printf("g_background can't ben NULL\n");
		return -1;
	}
	
	
	if (SDL_BlitSurface(g_background, NULL, g_screen, NULL) < 0)
	{
		printf("blit error\n");
	}

	//SDL_UpdateRect(g_screen, 0, 0, 0, 0);
	
	return 0;
}

void fresh_video()
{
	display_background();
	
	int ret = display_bmp();
	if (SDL_Flip(g_screen) == -1)
	{
		printf("flip screen fail\n");
	}

	printf("count %d \tdisplay_bmp return %d ball pos_x %d pos_y %d\n",
		g_count, ret, g_image_rect.x, g_image_rect.y);
	g_count++;
	
	move_ball();
	
}

void move_ball()
{
	if (g_image_rect.x + g_ball_speed_x + g_image_rect.w > g_screen->w
			|| g_image_rect.x + g_ball_speed_x < 0)
	{
		g_ball_speed_x = 0 - g_ball_speed_x;
	}
	if (g_image_rect.y + g_ball_speed_y + g_image_rect.h > g_screen->h 
			|| g_image_rect.y + g_ball_speed_y < 0)
	{
		g_ball_speed_y = 0 - g_ball_speed_y;
	}
	g_image_rect.x += g_ball_speed_x;
	g_image_rect.y += g_ball_speed_y;
}

int display_bmp()
{
    if (g_image == NULL)
	{
		printf ("g_image can't be NULL\n");
		return -1;
	}
	
	if (NULL == g_screen)
	{
		printf("g_screen can't be NULL\n");
		return -1;
	}
    
	if (SDL_BlitSurface(g_image, NULL, g_screen, &g_image_rect) < 0)
	{
		printf("blit error\n");
	}

	//SDL_UpdateRect(g_screen, g_image_rect.x, g_image_rect.y, g_image->w, g_image->h);
	
	
	//SDL_FillRect(g_screen, NULL, 222);
                 
    return 0;
}

int main(int argc, char** argv)
{
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (0 != ret)
    {
        printf("init sdl error\n");
        return ret;
    }
    printf("sdl system init success ^_^\n");
    g_screen = SDL_SetVideoMode(493, 300, 8, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (NULL == g_screen)
    {
        printf("set videoMode fail\n");
    }
	
	load_resource();
    game_on();
	release_resource();

    SDL_Quit();
    printf("sdl system quit now bye\n");
    return 0;
}
