#include "Player.h"

#include <iostream>

using namespace MINX;
using namespace MINX::Graphics;
using namespace MINX::Input;

using namespace MINX_GEMGAME_NS;

void screenWrap(Rectangle* rect);

Player::Player(Rectangle rect, Keyboard* keyboard, GamePad* gamePad)
{
	this->rect = rect;
	this->keyboard = keyboard;
	this->gamePad = gamePad;
	this->speed = 500.0f;
}

void Player::HandleInput(GameTime* gameTime)
{
	Vector2 velocity(0,0);
	velocity.X += (keyboard->GetKey(Keys::KEY_LEFT).GetState() || keyboard->GetKey(Keys::KEY_A).GetState()) ? -1 : 0;
	velocity.X += (keyboard->GetKey(Keys::KEY_RIGHT).GetState() || keyboard->GetKey(Keys::KEY_D).GetState()) ? 1 : 0;
	velocity.Y += (keyboard->GetKey(Keys::KEY_UP).GetState() || keyboard->GetKey(Keys::KEY_W).GetState()) ? -1 : 0;
	velocity.Y += (keyboard->GetKey(Keys::KEY_DOWN).GetState() || keyboard->GetKey(Keys::KEY_S).GetState()) ? 1 : 0;
	velocity *= gameTime->GetDeltaTimeSeconds() * speed;


	rect.X += velocity.X;
	rect.Y += velocity.Y;

	screenWrap(&rect);

}

void Player::Draw(TextureBatch* textureBatch)
{
	textureBatch->DrawPrimitiveRectangle(rect, Color(0, 0, 0));
}

void screenWrap(Rectangle* rect)
{
	if(rect->X < 0)
		rect->X = GameWindow::GetWidth() - rect->Width;
	if(rect->X > GameWindow::GetWidth() - rect->Width)
		rect->X = 0;
	if(rect->Y < 0)
		rect->Y = GameWindow::GetHeight() - rect->Height;
	if(rect->Y > GameWindow::GetHeight() - rect->Height)
		rect->Y = 0;
}
