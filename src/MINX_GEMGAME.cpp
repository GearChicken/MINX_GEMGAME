#include "MINX_GEMGAME.h"

#include <MINX/Graphics/ShaderFactory.h>
#include <MINX/Graphics/TextureBatch.h>
#include <MINX/Graphics/GameWindow.h>
#include <MINX/Graphics/Font.h>
#include <MINX/Input/Keyboard.h>
#include <MINX/Input/GamePad.h>
#include <MINX/Media/SoundFile.h>

#include <vector>
#include <iostream>
#include <sstream>

#include "Gem.h"
#include "Player.h"

using namespace MINX_GEMGAME_NS;

using namespace MINX;
using namespace MINX::Graphics;
using namespace MINX::Input;
using namespace MINX::Media;

TextureBatch* textureBatch;
Keyboard* keyboard;
GamePad* gamePad;
SoundFile* blip;
Font* font;

vector<Gem*> gemList;
vector<Color> colorList;
Player* player;

int playerScore;
int roundNumber;

double roundTimer;

void newRound();

enum class GameState
{
	MAIN_MENU,
	GAMEPLAY,
	GAME_OVER
};

GameState gameState;

MINX_GEMGAME::MINX_GEMGAME() : Game::Game()
{
	//This is the constructor. Put stuff here that should happen when the Game is created.
	srand(time_t(NULL));
	isRunning = true;
	Game::SetVideoOptions(640, 480, true, "MINX_GEMGAME");
}

void MINX_GEMGAME::Initialize()
{
	//Put stuff here that should happen when the Game is initialized
	keyboard = new Keyboard(this);
	gamePad = new GamePad(0, this);
	player = new Player(Rectangle(100.0, 100.0, 16.0, 16.0), keyboard, gamePad);

	colorList.push_back(Color(255, 0, 0));
	colorList.push_back(Color(0, 255, 0));
	colorList.push_back(Color(0, 0, 255));

	Game::Initialize();
}

void MINX_GEMGAME::LoadContent()
{
	//Put stuff here that loads content for your game.
	Game::LoadContent();

	textureBatch = new TextureBatch(ShaderFactory::GetInstance()->GetShaderAtIndex(0));

	blip = new SoundFile("../content/blip.mp3", this);
	font = new Font(freeTypeLibrary, "../content/Ubuntu-B.ttf", ShaderFactory::GetInstance()->GetShaderAtIndex(1));

	newRound();
}

void MINX_GEMGAME::UnloadContent()
{
	//Put stuff here that unloads content from your game.
	Game::UnloadContent();
}

void MINX_GEMGAME::Update(GameTime * gameTime)
{
	//Put stuff here to update the logic in your game each tick.

	Button keyState = keyboard->GetKey(Keys::KEY_SPACE);
	Button butState = gamePad->GetButton(0);

	switch(gameState)
	{
		case GameState::MAIN_MENU:
		{
			if(keyState.state && !keyState.prevState || butState.state && !butState.prevState)
			{
				gameState = GameState::GAMEPLAY;
				roundNumber = 0;
				playerScore = 0;
				newRound();
			}
		}
		break;
		case GameState::GAMEPLAY:
		{
			// For now if the user presses space enter a new round
		
			if(!gemList.size())
			{
				newRound();
			}
	
			player->HandleInput(gameTime);
		
			// Erase Gems That Intersect With The Player
			for(int i = 0; i < gemList.size(); ++i)
			{
				if(gemList.at(i)->CheckIntersection(player->CollisionRect()))
				{
					blip->Play();
					gemList.erase(gemList.begin()+i);
					++playerScore;
					--i;
				}
			}
		
			roundTimer -= gameTime->GetDeltaTimeSeconds();
	
			if(roundTimer <= 0)
			{
				gameState = GameState::GAME_OVER;
			}
		}
		break;
		case GameState::GAME_OVER:
		{
			if(keyState.state && !keyState.prevState || butState.state && !butState.prevState)
			{
				gameState = GameState::MAIN_MENU;
			}
		}
		break;
	}

	gameTime->LimitFPS(60);
	Game::Update(gameTime);
}

void MINX_GEMGAME::Draw(GameTime * gameTime)
{
	gameWindow->Clear();

	switch(gameState)
	{
		case GameState::MAIN_MENU:
		{
			std::stringstream menuText;
			menuText << "Welcome to the Gem Game! ";
			menuText << "Press Spacebar to Begin!";
			font->RenderText(menuText.str(), 10, 10, 16);
		}
		break;
		case GameState::GAMEPLAY:
		{
			for(int i = 0; i < gemList.size(); ++i)
			{
				gemList.at(i)->Draw(textureBatch);
			}
		
			player->Draw(textureBatch);
	
			textureBatch->DrawLoadedTextures();
			std::stringstream infoText;
			infoText << "Round #: " << roundNumber << "  ";
			infoText << "Total Score: " << playerScore << " ";
			infoText << "Time Remaining: " << roundTimer;
			font->RenderText(infoText.str(), 10, 10, 16);
		}
		break;
		case GameState::GAME_OVER:
		{
			std::stringstream gameOverText;
			gameOverText << "Game Over! ";
			gameOverText << "You collected " << playerScore << " gems! ";
			gameOverText << "in " << roundNumber << " rounds!";
			font->RenderText(gameOverText.str(), 10, 10, 16);
			gameOverText.str(std::string());
			gameOverText << "Press Spacebar to return to the main menu!";
			font->RenderText(gameOverText.str(), 10, 25, 16);
		}
		break;
	}
	//Put stuff here to draw your game each frame.
	gameTime->LimitFPS(60);
	Game::Draw(gameTime);
}

// adds new gems to the gemlist and resets the round timer
void newRound()
{
	gemList.clear();
	++roundNumber;
	player->SlowDown();
	roundTimer = 15 + roundNumber * 1.5;
	int count = 10 + roundNumber * 4;
	for(int i = 0; i < count; ++i)
	{
		int x = rand() % (GameWindow::GetWidth() - 8);
		int y = rand() % (GameWindow::GetHeight() - 8);
		Rectangle rect (x, y, 8, 8);
		gemList.push_back(new Gem(rect, colorList.at(rand() % colorList.size())));
	}
}
