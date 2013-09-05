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
#include "Graphics/Font.h"
#include "Gem.h"
#include <stdio.h>
#include <stdlib.h>
#include "Player.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <iostream>
using namespace MINX_GEMGAME;
using namespace MINX;
using namespace MINX::Graphics;
using namespace std;
// How many frames time values to keep
// The higher the value the smoother the result is...
// Don't make it 0 or less :)
#define FRAME_VALUES 10

// An array to store frame times:
Uint32 frametimes[FRAME_VALUES];

// Last calculated SDL_GetTicks
Uint32 frametimelast;

// total frames rendered
Uint32 framecount;

// the value you want
float framespersecond;

// This function gets called once on startup.
void fpsinit() {

        // Set all frame times to 0ms.
        memset(frametimes, 0, sizeof(frametimes));
        framecount = 0;
        framespersecond = 0;
        frametimelast = SDL_GetTicks();

}

void fpsthink() {

        Uint32 frametimesindex;
        Uint32 getticks;
        Uint32 count;
        Uint32 i;

        // frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
        // This value rotates back to 0 after it hits FRAME_VALUES.
        frametimesindex = framecount % FRAME_VALUES;

        // store the current time
        getticks = SDL_GetTicks();

        // save the frame time value
        frametimes[frametimesindex] = getticks - frametimelast;

        // save the last frame time for the next fpsthink
        frametimelast = getticks;

        // increment the frame count
        framecount++;

        // Work out the current framerate

        // The code below could be moved into another function if you don't need the value every frame.

        // I've included a test to see if the whole array has been written to or not. This will stop
        // strange values on the first few (FRAME_VALUES) frames.
        if (framecount < FRAME_VALUES) {

                count = framecount;

        } else {

                count = FRAME_VALUES;

        }

        // add up all the values and divide to get the average frame time.
        framespersecond = 0;
        for (i = 0; i < count; i++) {

                framespersecond += frametimes[i];

        }

        framespersecond /= count;

        // now to make it an actual frames per second value...
        framespersecond = 1000.f / framespersecond;
	//cout << "Fps: " << framespersecond << "\n";
}
vector<Gem*> * gems;
vector<Color*> * colors;
Player* player;
int gemsCollected=0;
int timelimit = 15;
int roundID = 0;
int gemCount = 0;
TTF_Font* font;
Texture2D* gem;
bool loseCondition= false;
string loseString = "Welcome! Press Spacebar to Play!";
void newRound(int roundID)
{
	srand(time(NULL));
	gemCount = 15 + 5 * roundID*roundID;
	timelimit = (int)((roundID + 5) * 3.5)*1000 - 50*(roundID-4/(roundID+1.3)) + gemCount*10/(roundID*roundID+2);
#ifdef _WIN32
	for(vector<Gem*>::iterator it = gems->begin(); it != gems->end(); ++it)
	{
		delete *it;
	}
#else
	for(Gem * gem : *gems)
	{
		delete gem;
	}
#endif
	for(int i =0; i < gemCount; i++)
	{
		gems->push_back(new Gem(rand() % 624 + 192, rand() % 464 + 144, colors->at((int)(rand() % colors->size())) , gem));
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
	windowWidth = 1024;
	windowHeight = 768;
	windowFlags = SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_SRCALPHA|SDL_HWACCEL;
#ifdef _WIN32
	Game::setVideoOptions(120,1024,768,32,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_SRCALPHA|SDL_HWACCEL);
#else
	Game::setVideoOptions(120,1024,768,32,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_SRCALPHA|SDL_HWACCEL);
#endif
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
	SDL_WM_SetCaption("Gem Game","Gem Game");
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

	fpsinit();
}

void GemGame::LoadContent()
{
	//Put stuff here that loads content for your game.
	font = TTF_OpenFont(((string)"Ubuntu-B.ttf").c_str(), 24);
	gem = Content::loadTexture("gem.png",gameWindow);
	newRound(0);
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
		newRound(roundID);
		player->speedMultiplier *= .85;
	}
	if(timelimit <= 0)
	{
		loseString = "You Lose! Press Spacebar to play again!";
		loseCondition = true;
	}
	else
	{
		timelimit -= abs(gameTime->getDeltaTime());
	}
	if(loseCondition && keyboard->getButton(SDLK_SPACE).state)
	{
	gems = NULL;
	gems = new vector<Gem*>();
#ifdef _WIN32
		for(vector<Gem*>::iterator it = gems->begin(); it < gems->end(); ++it)
		{
			gems->erase(gems->begin());
		}
#else
		for(Gem* gem : *gems)
		{
			gems->erase(gems->begin());
		}
#endif
		loseCondition = false;
		gemsCollected = 0;
		roundID = 0;
		newRound(roundID);
		player->speedMultiplier = 5;
	}
	if(keyboard->getButton(SDLK_ESCAPE).state)
	{
		isRunning = false;
	}
}

void GemGame::Draw(GameTime * gameTime)
{

	SDL_FillRect(gameWindow->screen, NULL, 0x000000);
	//Put stuff here to draw your game each frame.
	if(loseCondition)
	{
		
		DrawString(50,100, loseString, gameWindow->screen, font);

	}
	else
	{
#ifdef _WIN32
		for(vector<Gem*>::iterator it = gems->begin(); it < gems->end(); ++it)
		{
			if(typeid(*it).name() == typeid(Gem*).name())
			{
				(*it)->Draw(gameTime, gameWindow->screen);
			}
		}
#else
		for(Gem* gem : *gems)
		{
			gem->Draw(gameTime, gameWindow->screen);
		}
#endif
		player->Draw(gameTime, gameWindow->screen);
	DrawString(50,50, "Time: " + ToString(timelimit,3), gameWindow->screen, font);
	}
	DrawString(50,650, "Gems Collected: " + ToString(gemsCollected), gameWindow->screen, font);
	DrawString(50,720, "Round: " + ToString(roundID), gameWindow->screen, font);
	DrawString(850,50, "FPS: " + ToString(framespersecond), gameWindow->screen, font);
	Primitives::drawOutlineRectangle(new Color(255,255,255,0), 192, 144, 640, 480, gameWindow->screen);
	Game::Draw(gameTime);
	fpsthink();
}
