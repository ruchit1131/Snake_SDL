#include<iostream>
#include<cstdlib>
#include<vector>
#include "SDL_ttf.h"
#include "SDL.h"
#undef main

bool compareRect(SDL_Rect a, SDL_Rect b)
{
	if (a.h == b.h && a.w == b.w && a.x == b.x && a.y == b.y)
		return true;
	return false;
}

int main()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	//Create a window
	int window_w = 500;
	int window_h = 500;
	SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_w, SDL_WINDOW_SHOWN);
	//Create a renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	bool isRunning = true;
	
	// enum for Direction
	enum eDirection :unsigned char {
		DSTOP,
		DUP,
		DDOWN,
		DLEFT,
		DRIGHT
	};

	eDirection direction = DSTOP;
	bool changeColor = false;
	bool add_node = false;
	//Speed can be 2, 4, 5, 6, 8, 10 i.e can divide 1000
	//!The speed will also decrease the size of the rect.You need to have the rect 10 by 10, and still have smaller speed
	//For that, you need to write a fucntion that touches the apples and consumes it.
	int speed = 5;

	SDL_Color backgroundColor = { 0, 0, 0, 255 };
	SDL_Rect head = { window_w / 2, window_h / 2, speed, speed };

	//Using a deque is better as it has O(1) Time Complexity to get front element.
	std::vector<SDL_Rect> snake = { head };
	std::vector<SDL_Rect> apples;

	for (int i = 0; i < 50; i++)
		apples.push_back({ (rand() % (window_w)/speed) * speed, (rand() % (window_h) / speed) * speed, speed, speed });
	
	srand(NULL);

	while (isRunning)
	{

		SDL_Event event;
		//We need to make the display refresh after every key press inside this while only, because 2 very fast key presses may change the direction vairable only once.

		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				//When Window is closed
			case SDL_QUIT:
			{
				isRunning = false;
				break;
			}
			case SDL_KEYDOWN:
			{
				if (event.key.keysym.sym == SDLK_DOWN)
					direction = direction == DUP ? DUP : DDOWN;
				if (event.key.keysym.sym == SDLK_UP)
					direction = direction == DDOWN ? DDOWN : DUP;
				if (event.key.keysym.sym == SDLK_LEFT)
					direction = direction == DRIGHT ? DRIGHT : DLEFT;
				if (event.key.keysym.sym == SDLK_RIGHT)
					direction = direction == DLEFT ? DLEFT : DRIGHT;
				if (event.key.keysym.sym == SDLK_ESCAPE)
					isRunning = false;
			}
			case SDL_TEXTINPUT:
			{
				//Press c to change color
				if (event.key.keysym.sym == SDLK_c)
					changeColor = true;
				//Press p to pause, NEEDto implement this corectly by having a direction_before_stop variable .
				//if (event.key.keysym.sym == SDLK_p)
					//direction = DSTOP;
				//press a to add node without eating apples
				if (event.key.keysym.sym == SDLK_a)
					add_node = true;
				break;
			}
			default:
				break;
			}

		}
			//Setting color of whole window
			if (changeColor)
			{
				backgroundColor.r = rand() % 256;
				backgroundColor.g = rand() % 256;
				backgroundColor.b = rand() % 256;
				backgroundColor.a = rand() % 256;
				changeColor = false;
			}
			SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
			SDL_RenderClear(renderer);

			//MOVE the head
			if (direction != DSTOP)
			{
				switch (direction)
				{
				case DUP:
				{
					head.y -= speed;
					break;
				}
				case DDOWN:
				{
					head.y += speed;
					break;
				}
				case DLEFT:
				{
					head.x -= speed;
					break;
				}
				case DRIGHT:
				{
					head.x += speed;
					break;
				}
				default:
					break;
				}
			}

			//Check out of bounds 
			if (head.y < 0)
				head.y = window_h;
			else if (head.y > window_h)
				head.y = 0;
			if (head.x < 0)
				head.x = window_w;
			else if (head.x > window_w)
				head.x = 0;

			if (add_node)
			{
				add_node = false;
				if(direction != DSTOP)
				snake.insert(snake.begin(), head);
			}
			else
			{
				//Move Snake
				if (direction != DSTOP)
				{
					SDL_Rect front = snake.back();
					snake.pop_back();
					front = head;
					snake.insert(snake.begin(), front);
				}
			}

			//Draw Apples
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
			for (int i = 0; i < apples.size(); i++)
				SDL_RenderFillRect(renderer, &apples[i]);

			//Draw Snake
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			for (int i = 0; i < snake.size(); i++)
				SDL_RenderFillRect(renderer, &snake[i]);

			//Check collision of snake head with its body
			for (int i = 1; i < snake.size(); i++)
			{
				if (compareRect(snake[i], snake[0]))
					isRunning = false;
			}

			//Check Collision of snake head with apples
			for (int i = 0; i < apples.size(); i++)
			{
				if (compareRect(snake[0], apples[i]))
				{
					add_node = true;
					apples.erase(apples.begin() + i);
				}
			}
			SDL_RenderPresent(renderer);
		
		SDL_Delay(30);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
