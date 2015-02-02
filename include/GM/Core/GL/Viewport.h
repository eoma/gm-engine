#pragma once

#include <GL/gl3w.h>

#include <memory>
#include <vector>

namespace GM
{
	namespace Core
	{
		class Viewport;
		typedef std::shared_ptr<Viewport> ViewportPtr;
	
		class Viewport
		{
		public:
			Viewport(float x, float y, float width, float height, bool bind_on_construct = true, int index = 0);
			Viewport(int x, int y, int width, int height, bool bind_on_construct = true, int index = 0);
			~Viewport();

			void reshape(float x, float y, float width, float height, int index = 0);
			void reshape(int x, int y, int width, int height, int index = 0);
			void bind();
	
		private:

			float x, y, width, height;
			int index;
		};
	}
}
