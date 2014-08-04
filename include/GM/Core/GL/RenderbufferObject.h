#pragma once

#include <GL/gl3w.h>

#include <memory>
#include <vector>

namespace GM {
namespace Core {
	class RenderbufferObject;
	typedef std::shared_ptr<RenderbufferObject> RenderbufferObjectPtr;
	
	class RenderbufferObject
	{
	public:
		RenderbufferObject(unsigned int type, unsigned int w, unsigned int h);
		~RenderbufferObject();

		void bind();
		void unbind();

		unsigned int get_handle() const { return handle; }
		unsigned int get_type() const { return type; }

		unsigned int get_width() const { return w; }
		unsigned int get_height() const { return h; }

	private:
		unsigned int handle;
		unsigned int type;

		unsigned int w;
		unsigned int h;
	};
}
}
