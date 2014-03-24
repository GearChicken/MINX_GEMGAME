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
	if(gamePad->IsConnected())
	{
		velocity.X += gamePad->GetAxis(0).val;
		velocity.Y += gamePad->GetAxis(1).val;
		if(velocity.Length() < 0.25)
			velocity = Vector2(0,0);
	}
	if(velocity == Vector2(0,0))
	{
		velocity.X += (keyboard->GetKey(Keys::KEY_LEFT).state || keyboard->GetKey(Keys::KEY_A).state) ? -1 : 0;
		velocity.X += (keyboard->GetKey(Keys::KEY_RIGHT).state || keyboard->GetKey(Keys::KEY_D).state) ? 1 : 0;
		velocity.Y += (keyboard->GetKey(Keys::KEY_UP).state || keyboard->GetKey(Keys::KEY_W).state) ? -1 : 0;
		velocity.Y += (keyboard->GetKey(Keys::KEY_DOWN).state || keyboard->GetKey(Keys::KEY_S).state) ? 1 : 0;
	}
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
