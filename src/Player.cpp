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
#include "Player.h"

using namespace MINX_GEMGAME;

using namespace MINX;

MINX_GEMGAME::Player::Player(int X, int Y):rect(new Rectangle(X,Y,16,16))
{
	color = new Color(255,255,255,0);
	velocity = new Vector2(0,0);
	loc = new Vector2(0,0);
	speedMultiplier=5;
}
void Player::Update(GameTime * gametime, Keyboard* keyboard, vector<Gem*> * gems, int * score)
{
	velocity->X =0;
	velocity->Y =0;
	if(keyboard->getButton(SDLK_UP).state)
	{
		velocity->Y +=-1;
	}
	if(keyboard->getButton(SDLK_DOWN).state)
	{
		velocity->Y +=1;
	}
	if(keyboard->getButton(SDLK_LEFT).state)
	{
		velocity->X +=-1;
	}
	if(keyboard->getButton(SDLK_RIGHT).state)
	{
		velocity->X +=1;
	}
	*velocity = velocity->normalize();
	*velocity = *velocity * speedMultiplier;
	loc->X += velocity->X;
	loc->Y += velocity->Y;
	if(loc->X < 192)
	{
		loc->X = 816;
	}
	if(loc->X > 816)
	{
		loc->X = 192;
	}
	if(loc->Y < 144)
	{
		loc->Y = 608;
	}
	if(loc->Y > 608)
	{
		loc->Y = 144;
	}
	rect->X = loc->X;
	rect->Y = loc->Y;
	int i = 0;
#ifdef _WIN32
	for(vector<Gem*>::iterator it = gems->begin(); it < gems->end(); ++it)
	{
		// Gem * gem = *it;
		if(rect->intersects((*it)->rect))
		{
			//gems->at(0) = NULL;
			gems->erase(gems->begin() + i);
			(*score)++;
			i--;
		}
		i++;
	}
#else
	for(Gem* gem : *gems)
	{
		if(rect->intersects(gem->rect))
		{
			//gems->at(0) = NULL;
			gems->erase(gems->begin() + i);
			(*score)++;
			i--;
		}
		i++;
	}
#endif
}
void Player::Draw(GameTime * gametime, SDL_Surface* screen)
{
	//Put stuff here to draw your game each frame.
	Graphics::Primitives::drawRectangle(color, rect->X, rect->Y, rect->Width, rect->Height, screen);
}
