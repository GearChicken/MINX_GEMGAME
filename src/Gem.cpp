#include "Gem.h"

using namespace MINX_GEMGAME_NS;
using namespace MINX;
using namespace MINX::Graphics;

Gem::Gem(Rectangle gemRect, Color tintColor)
{
	this->gemRect = gemRect;
	this->tintColor = tintColor;
}

bool Gem::CheckIntersection(Rectangle otherRect)
{
	return otherRect.Intersects(&gemRect);
}

void Gem::Draw(TextureBatch* texBatch)
{
	texBatch->DrawPrimitiveRectangle(gemRect, tintColor);
}
