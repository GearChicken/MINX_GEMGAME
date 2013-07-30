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
#include "Game.h"
#include "Gem.h"
#include "GameTime.h"
#include "Vector2.h"
#include "Rectangle.h"
#include "Graphics/Color.h"
#include "Graphics/Primitives.h"
#include "Input/Keyboard.h"
#ifndef PLAYER_H_
#define PLAYER_H_

using namespace MINX;
using namespace MINX::Graphics;
using namespace MINX::Input;

namespace MINX_GEMGAME
{
	class Player
	{
		public:
			Player(int X, int Y);
			void Update(GameTime * gametime, Keyboard* keyboard, vector<Gem*> * gems, int * score);
			void Draw(GameTime * gametime, SDL_Surface* screen);
		private:
			Rectangle* rect;
			Color* color;
			Vector2* velocity;
	};
}

#endif
