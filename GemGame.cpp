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

using namespace MINX_GEMGAME;


GemGame::GemGame()
{
	//This is the constructor. Put stuff here that should happen when the Game is created.
	isRunning = true;
}

void GemGame::Initialize()
{
	//Put stuff here that should happen when the Game is initialized.
	Game::Initialize();
}

void GemGame::LoadContent()
{
	//Put stuff here that loads content for your game.
	Game::LoadContent();
}

void GemGame::UnloadContent()
{
	//Put stuff here that unloads content from your game.
	Game::UnloadContent();
}

void GemGame::Update(GameTime * gametime)
{
	//Put stuff here to update the logic in your game each tick.
	Game::Update(gametime);
}

void GemGame::Draw(GameTime * gametime)
{
	//Put stuff here to draw your game each frame.
	Game::Draw(gametime);
}
