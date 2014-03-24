#ifndef GEM_H
#define GEM_H

#include <MINX/Rectangle.h>
#include <MINX/Graphics/TextureBatch.h>

namespace MINX_GEMGAME_NS
{
	class Gem
	{
	public:
		Gem(MINX::Rectangle gemRect, MINX::Graphics::Color tintColor);
		bool CheckIntersection(MINX::Rectangle otherRect);
		void Draw(MINX::Graphics::TextureBatch* texBatch);
	private:
		MINX::Rectangle gemRect;
		MINX::Graphics::Color tintColor;
	};
}

#endif
