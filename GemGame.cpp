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
#include "Graphics/Font.h"
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
bool loseCondition= false;
void newRound(int roundID, Content * content)
{
	srand(time(NULL));
	gemCount = 15 + 5 * roundID*roundID;
	timelimit = (int)((roundID + 5) * 3.5)*1000 - 50*(roundID-4/(roundID+1.3)) + gemCount*10/(roundID*roundID+2);
	for(Gem * gem : *gems)
	{
		delete gem;
	}
	delete gems;
	gems = new vector<Gem*>();
	for(int i =0; i < gemCount; i++)
	{
		gems->push_back(new Gem(rand() % 624, rand() % 464, colors->at((int)(rand() % colors->size())) , content->textures->at("gem")));
	}
}
string ToString(int val)
{
	stringstream ss;
	ss << val;
	return ss.str();
}
string ToString(int val, int decimal)
{
	stringstream ss;
	ss << val;
	string s = ss.str();
	if(s.size() > decimal -1)
	{
		s.insert(s.end() - decimal, '.');
	}
	return s;
}
GemGame::GemGame()
{
	desiredFPS = 120;
	windowFlags = SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_SRCALPHA|SDL_HWACCEL;
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
	player = new Player(50,50);
	keyboard= new Input::Keyboard(this);
	cout << "gems made!\n";
}

void GemGame::LoadContent()
{
	//Put stuff here that loads content for your game.
	font = content->loadTTFFont("Ubuntu-B.ttf", 24, "font");
	content->loadTexture("gem.png", "gem");
	newRound(0, content);
	cout << "content loaded!\n";
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
	player->Update(gameTime, keyboard, gems, &gemsCollected);
	if(gems->size() <= 0)
	{
		roundID++;
		newRound(roundID, content);
		player->speedMultiplier *= .85;
	}
	if(timelimit <= 0)
	{
		
		loseCondition = true;
	}
	else
	{
		timelimit -= gameTime->getDeltaTime();
	}
	if(loseCondition && keyboard->getButton(SDLK_SPACE).state)
	{
		for(Gem* gem : *gems)
		{
			gems->erase(gems->begin());
		}
		loseCondition = false;
		gemsCollected = 0;
		roundID = 0;
		newRound(roundID, content);
		player->speedMultiplier = 5;
	}
	if(keyboard->getButton(SDLK_ESCAPE).state)
	{
		isRunning = false;
	}
	//SDL_Delay(50);
}

void GemGame::Draw(GameTime * gameTime)
{

	SDL_FillRect(gameWindow->screen, NULL, 0x000000);
	//Put stuff here to draw your game each frame.
	if(loseCondition)
	{
		
		DrawString(50,195, "You Lose! Press Spacebar to play again!", gameWindow->screen, font);

	}
	else
	{
		for(Gem* gem : *gems)
		{
			gem->Draw(gameTime, gameWindow->screen);
		}
		player->Draw(gameTime, gameWindow->screen);
	DrawString(50,50, "Time: " + ToString(timelimit,3), gameWindow->screen, font);
	}
	DrawString(50,390, "Gems Collected: " + ToString(gemsCollected), gameWindow->screen, font);
	DrawString(50,440, "Round: " + ToString(roundID), gameWindow->screen, font);
	Game::Draw(gameTime);
}
