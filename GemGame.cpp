/*
    MINX - A C++ Graphics and Input Wrapper Library
    Copyright (C) 2013  MINX Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	*/
#include "GemGame.h"
#include "Point.h"
#include "Gem.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL_image.h"
#include "Player.h"
#include "Rectangle.h"
#include "Graphics/Color.h"
#include "stdlib.h"
#include "Input/Keyboard.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
using namespace MINX_GEMGAME;
using namespace MINX;
using namespace MINX::Graphics;
using namespace std;

vector<Gem*> * gems;
vector<Color*> * colors;
Player* player;
int gemsCollected=0;
int timelimit = 0;
int roundID = 0;
int gemCount = 0;
TTF_Font* font;

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}
void newRound(int roundID)
{
	gemCount = 15 + 5 * roundID*roundID;
	timelimit = (int)((roundID + 5) * 3.5)*1000 - 50*(roundID-4/(roundID+1.3)) + gemCount*10/(roundID*roundID+2);
	for(int i =0; i < gemCount; i++)
	{
		gems->push_back(new Gem(rand() % 624, rand() % 464, colors->at((int)(rand() % colors->size()))));
	}
}
string intToBinary(int number)
{
	string result;
	do result.push_back('0' + (number & 1));
	while (number >>=1);
	
	reverse(result.begin(), result.end());
	return result;
}
void drawScore(string scoreString, Point drawingPoint, GameWindow* gameWindow)
{
	for(string::size_type i = 0; i < scoreString.size(); ++i)
	{
		if(scoreString[i] == *"0")
		{
			Graphics::Primitives::drawOutlineRectangle(new Graphics::Color(255,255,255,255), drawingPoint.X + i*10, drawingPoint.Y, 5, 10, gameWindow->screen);
		}
		if(scoreString[i] == *"1")
		{
			Graphics::Primitives::drawRectangle(new Graphics::Color(255,255,255,255), drawingPoint.X + i*10, drawingPoint.Y, 5, 10, gameWindow->screen);
		}
	}
}
void drawScores(GameWindow* gameWindow, GameTime* gameTime)
{
	stringstream ss;
	ss << roundID;
	//drawScore(intToBinary(roundID), Point(50,440), gameWindow);
	apply_surface(50,440, TTF_RenderText_Solid(font, ss.str().c_str(), {255,255,255}), gameWindow->screen);
	ss.str(string());
	ss << timelimit/1000;
	//drawScore(intToBinary(timelimit), Point(50,50), gameWindow);
	apply_surface(50,50, TTF_RenderText_Solid(font, ss.str().c_str(), {255,255,255}), gameWindow->screen);
}
GemGame::GemGame()
{
	desiredFPS = 120;
	srand(time(NULL));
	//This is the constructor. Put stuff here that should happen when the Game is created.
	gems = new vector<Gem*>();
	colors = new vector<Color*>();	
	isRunning = true;
}

void GemGame::Initialize()
{
	//Put stuff here that should happen when the Game is initialized.
	Game::Initialize();
	colors->push_back(new Color(255,0,0,0));
	colors->push_back(new Color(0,255,0,0));
	colors->push_back(new Color(0,0,255,0));
	colors->push_back(new Color(255,255,0,0));
	colors->push_back(new Color(255,0,255,0));
	colors->push_back(new Color(255,255,0,0));
	colors->push_back(new Color(0,255,255,0));
	cout << "Game inited!\n";
	newRound(0);
	player = new Player(50,50);
	keyboard= new Input::Keyboard(this);
	cout << "gems made!\n";
}

void GemGame::LoadContent()
{
	//Put stuff here that loads content for your game.
	font = content->loadTTFFont("Ubuntu-B.ttf", 24);
	Game::LoadContent();
}

void GemGame::UnloadContent()
{
	//Put stuff here that unloads content from your game.
	Game::UnloadContent();
}

void GemGame::Update(GameTime * gameTime)
{
	//Put stuff here to update the logic in your game each tick.
	Game::Update(gameTime);
	cout << "game update!\n";
	player->Update(gameTime, keyboard, gems, &gemsCollected);
	if(gems->size() <= 0)
	{
		roundID++;
		newRound(roundID);
	}
	timelimit -= gameTime->getDeltaTime();
	if(timelimit <= 0)
	{
		isRunning = false;
	}
	//SDL_Delay(50);
}

void GemGame::Draw(GameTime * gameTime)
{

	SDL_FillRect(gameWindow->screen, NULL, 0x000000);
	//Put stuff here to draw your game each frame.
	for(Gem* gem : *gems)
	{
		gem->Draw(gameTime, gameWindow->screen);
	}
	player->Draw(gameTime, gameWindow->screen);
	drawScores(gameWindow, gameTime);
	Game::Draw(gameTime);
}
