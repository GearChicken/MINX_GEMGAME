#ifndef PLAYER_H
#define PLAYER_H

#include <MINX/Input/Keyboard.h>
#include <MINX/Input/GamePad.h>
#include <MINX/Rectangle.h>
#include <MINX/Graphics/TextureBatch.h>
#include <MINX/Graphics/GameWindow.h>
#include <MINX/GameTime.h>

namespace MINX_GEMGAME_NS
{
	class Player
	{
	public:
		Player(MINX::Rectangle rect, MINX::Input::Keyboard* keyboard, MINX::Input::GamePad* gamePad);
		void HandleInput(MINX::GameTime* gameTime);
		void Draw(MINX::Graphics::TextureBatch* textureBatch);
		inline MINX::Rectangle CollisionRect() { return rect; }
		inline void SlowDown() { speed *= .95; }
	private:
		MINX::Rectangle rect;
		MINX::Input::Keyboard* keyboard;
		MINX::Input::GamePad* gamePad;
		double speed;
	};
}

#endif
