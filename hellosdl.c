#include "SDL/SDL.h"

void keep_fps();
void load_resource();
void game_on();
void handle_active(SDL_ActiveEvent* active);
void handle_keydown(SDL_KeyboardEvent * key);
void handle_mouse_motion(SDL_MouseMotionEvent *motion);
void handle_mouse_button(SDL_MouseButtonEvent *button);
void handle_quit();
void handle_userinput();
void refresh_video();
void move_ball_to(int x, int y);
void move_ball();
void stop_ball();
void resume_ball();
int display_bmp();
int display_smallball();
int display_background();


int g_running = 0;
int g_count = 0;
char g_mode = 'm';

SDL_Surface *g_image = NULL;
SDL_Rect g_image_rect;
int g_ball_speed_x = 50;
int g_ball_speed_y = 50;



SDL_Surface *g_screen = NULL;
SDL_Rect g_screen_rect;

SDL_Surface *g_background = NULL;

#define GAME_FPS 30
#define BMP_FILE "media/lala.bmp"
#define BACKGROUND_FILE "media/background_black.bmp"


void keep_fps()
{
    int interval = 1000 / GAME_FPS;
    if (g_mode == 'm')
    {
        interval = 5;
    }
    SDL_Delay(interval);
}

void stop_ball()
{
    g_mode = 'm';
    g_ball_speed_y = 0;
    g_ball_speed_x = 0;
}

void resume_ball()
{
    g_mode = 'a';
    g_ball_speed_x = 50;
    g_ball_speed_y = 50;
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
        refresh_video();
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
        case SDLK_s:
            stop_ball();
            break;
        case SDLK_g:
            resume_ball();
            break;
        case SDLK_m:
            stop_ball();
            break;
        case SDLK_a:
            resume_ball();
            break;
		default:
			printf("unknown key\n");
			break;
	}
	
}

void handle_active(SDL_ActiveEvent * active)
{
}
void handle_mouse_motion(SDL_MouseMotionEvent *motion)
{
    //if left button is pressed then move ball
    if (motion->state & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        move_ball_to(motion->x, motion->y);
    }
}
void handle_mouse_button(SDL_MouseButtonEvent *button)
{
    if (button->type == SDL_MOUSEBUTTONDOWN)
    {
        move_ball_to(button->x, button->y);
    }
}
void handle_quit()
{
    g_running = 1;
    printf("user X out the window!!!\n");
}

void handle_userinput()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
            case SDL_QUIT:
                handle_quit();
                break;
            case SDL_ACTIVEEVENT:
                handle_active(&event.active);
                break;
			case SDL_KEYDOWN:
				handle_keydown(&event.key);
				break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION:
                handle_mouse_motion(&event.motion);
                break;
            case SDL_MOUSEBUTTONDOWN:
                handle_mouse_button(&event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                handle_mouse_button(&event.button);
                break;
			default:
				printf("unknown event\n");
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

void refresh_video()
{
	display_background();
	
	int ret = display_bmp();
	if (SDL_Flip(g_screen) == -1)
	{
		printf("flip screen fail\n");
	}

	//printf("count %d \tdisplay_bmp return %d ball pos_x %d pos_y %d\n",
//		g_count, ret, g_image_rect.x, g_image_rect.y);
	g_count++;
	
    if (g_mode == 'a')
    {
        move_ball();
    }
	
}

void move_ball_to(int x, int y)
{
    g_image_rect.x = x - g_image_rect.w / 2;
    g_image_rect.y = y - g_image_rect.h / 2;
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
    printf("quit now bye\n");
    return 0;
}
