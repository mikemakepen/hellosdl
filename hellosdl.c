#include "SDL2/SDL.h"

void keep_fps();
void load_resource();
void game_on();
//void handle_active(SDL_ActiveEvent* active);
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
SDL_Texture *load_texture_from_bmp(char* file_path);
void speed_up(int *speed);
void speed_down(int *speed);


int g_running = 0;
int g_count = 0;
char g_mode = 'm';
double reduction = 0.8;
double acceleration = 1.5;

SDL_Texture *g_image = NULL;
SDL_Rect g_image_rect;
int g_ball_speed_x = 50;
int g_ball_speed_y = 50;


SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;

SDL_Surface *g_screen = NULL;
SDL_Rect g_screen_rect;

SDL_Texture *g_background = NULL;

#define GAME_FPS 144
#define BMP_FILE "media/lala.bmp"
#define BACKGROUND_FILE "media/background_black.bmp"


void keep_fps()
{
    int interval = 1000 / GAME_FPS;
    if (g_mode == 'm')
    {
        interval = 40;
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

SDL_Texture *load_texture_from_bmp(char* file_path)
{
	SDL_Surface *image = SDL_LoadBMP(file_path);
    if (NULL == image)
    {
        printf("can not load bmp %s\n", file_path);
    }

	SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, image);
	if (NULL == tex)
	{
		printf("create texture from surface fail: %s", SDL_GetError());
	}
	SDL_FreeSurface(image);
	return tex;
}

void load_resource()
{
	/*
	g_image = SDL_LoadBMP(BMP_FILE);
    if (NULL == g_image)
    {
        printf("can not load bmp %s\n", BMP_FILE);
    }*/
	g_image = load_texture_from_bmp(BMP_FILE);


/*
	g_background = SDL_LoadBMP(BACKGROUND_FILE);
    if (NULL == g_background)
    {
        printf("can not load bmp %s\n", BMP_FILE);
    }*/

	g_background = load_texture_from_bmp(BACKGROUND_FILE);

	/*
    if (g_image->format->palette && g_screen->format->palette)
    {
        SDL_SetColors(g_screen, g_image->format->palette->colors, 0,
                g_image->format->palette->ncolors);
    }*/
	int w = 0;
	int h = 0;
	SDL_QueryTexture(g_image, NULL, NULL, &w, &h);
	memset(&g_image_rect, 0, sizeof(g_image_rect));

	g_image_rect.w = w;
	g_image_rect.h = h;
	printf("g_image_rect, w: %d, h: %d\n", g_image_rect.w, g_image_rect.h);

	w = 0;
	h = 0;
	SDL_QueryTexture(g_background, NULL, NULL, &w, &h);
	memset(&g_screen_rect, 0, sizeof(g_screen_rect));
	g_screen_rect.w = w;
	g_screen_rect.h = h;
	printf("g_screen_rect, w: %d, h: %d\n", g_screen_rect.w, g_screen_rect.h);
}

int display_smallball()
{

}

void release_resource()
{
	SDL_DestroyTexture(g_image);
	g_image = NULL;
	SDL_DestroyTexture(g_background);
	g_background = NULL;
	SDL_DestroyRenderer(g_renderer);
}
void game_on()
{
    while(!g_running)
    {
        handle_userinput();
        // fflush(stdout);
		refresh_video();
		// printf("end of refresh\n");
		fflush(stdout);
		keep_fps();
    }
	printf("sdl system quit now\n");
}

void speed_up(int *speed)
{
	
	if (*speed == 0)
	{
		*speed = 2;
	}
	*speed = *speed * acceleration;
}

void speed_down(int *speed)
{
	if (*speed == 0)
	{
		*speed = 2;
	}
	*speed = *speed * reduction;
}

void handle_keydown(SDL_KeyboardEvent * key)
{
	printf("key down: %d\n", key->keysym.sym);
	switch (key->keysym.sym)
	{
		case SDLK_ESCAPE:
			g_running = 1;
			printf("user input escape set g_running = 1\n");
			break;
		case SDLK_UP:
			speed_up(&g_ball_speed_y);
			break;
		case SDLK_DOWN:
			speed_up(&g_ball_speed_y);
			break;
		case SDLK_LEFT:
			speed_up(&g_ball_speed_x);
			break;
		case SDLK_RIGHT:
			speed_up(&g_ball_speed_x);
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
			printf("unknown key down: %d\n", key->keysym.sym);
			break;
	}
	printf("end of key down: %d\n", key->keysym.sym);
}

/*
void handle_active(SDL_ActiveEvent * active)
{
}*/
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
            /*case SDL_ACTIVEEVENT:
                handle_active(&event.active);
                break;*/
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
				SDL_Log("unknown event: %d\n", event.type);
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
	
	/*
	if (SDL_BlitSurface(g_background, NULL, g_screen, NULL) < 0)
	{
		printf("blit error\n");
	}*/

	SDL_RenderCopy(g_renderer, g_background, NULL, NULL);
	
	//SDL_UpdateRect(g_screen, 0, 0, 0, 0);
	
	return 0;
}

void refresh_video()
{
	SDL_RenderClear(g_renderer);
	// printf("clear render\n");
	display_background();
	// printf("display background\n");
	// fflush(stdout);
	/*
	int ret = display_bmp();
	if (SDL_Flip(g_screen) == -1)
	{
		printf("flip screen fail\n");
	}*/
	int ret = display_bmp();
	// printf("display bmp\n");
	SDL_RenderPresent(g_renderer);
	// printf("display now\n");
	// fflush(stdout);
	//printf("count %d \tdisplay_bmp return %d ball pos_x %d pos_y %d\n",
//		g_count, ret, g_image_rect.x, g_image_rect.y);
	g_count++;
	// printf("test g_mode: %c \n", g_mode);
    if (g_mode == 'a')
    {
		// printf("game mode a\n");
		// fflush(stdout);
        move_ball();
		// printf("end of move ball\n");
		// fflush(stdout);
    }
	
}

void move_ball_to(int x, int y)
{
    g_image_rect.x = x - g_image_rect.w / 2;
    g_image_rect.y = y - g_image_rect.h / 2;
}

void move_ball()
{
	// printf("start of g_ball_speed_x\n");
	// fflush(stdout);

	if (g_image_rect.x + g_ball_speed_x + g_image_rect.w > g_screen_rect.w
			|| g_image_rect.x + g_ball_speed_x < 0)
	{
		g_ball_speed_x = (0 - g_ball_speed_x) * reduction;
	}
	// printf("end of g_ball_speed_x\n");
	// fflush(stdout);
	if (g_image_rect.y + g_ball_speed_y + g_image_rect.h > g_screen_rect.h 
			|| g_image_rect.y + g_ball_speed_y < 0)
	{
		g_ball_speed_y = (0 - g_ball_speed_y) * reduction;
	}
	// printf("end of g_ball_speed_y\n");
	// fflush(stdout);
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
	/*
	if (NULL == g_screen)
	{
		printf("g_screen can't be NULL\n");
		return -1;
	}
    
	if (SDL_BlitSurface(g_image, NULL, g_screen, &g_image_rect) < 0)
	{
		printf("blit error\n");
	}*/

	int ret = SDL_RenderCopy(g_renderer, g_image, NULL, &g_image_rect);
	if (ret != 0) 
	{
		printf("copy ball fail %s\n", SDL_GetError());
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

	g_window = SDL_CreateWindow("ball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 493, 300, SDL_WINDOW_OPENGL);
	if (NULL == g_window) 
	{
		printf("create window fail\n");
	}

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
	if (NULL == g_renderer) 
	{
		printf("create renderer fail\n");
	}
	/*
    g_screen = SDL_SetVideoMode(493, 300, 8, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (NULL == g_screen)
    {
        printf("set videoMode fail\n");
    }*/
	
	load_resource();
    game_on();
	release_resource();

    SDL_Quit();
    printf("quit now bye\n");
    return 0;
}
