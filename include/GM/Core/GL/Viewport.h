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
			Viewport(float x_offset, float y_offset, float width, float height, bool bind_on_construct = true, int index = 0);
			Viewport(int x_offset, int y_offset, int width, int height, bool bind_on_construct = true, int index = 0);
			~Viewport();

			void reshape(float x_offset, float y_offset, float width, float height, int index = 0);
			void reshape(int x_offset, int y_offset, int width, int height, int index = 0);
			void bind();
	
		private:
			float x_offset;
			float y_offset;
			float width;
			float height;

			int index;
		};
	}
}
