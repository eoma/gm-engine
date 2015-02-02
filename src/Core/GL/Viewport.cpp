#include <GM/Core/GL/Viewport.h>

using namespace GM::Core;

Viewport::Viewport(float x, float y, float width, float height, bool bind_on_construct, int index)
	: x(x), y(y), width(width), height(height), index(index)
{
	if(bind_on_construct)
	{
		bind();
	}
}

Viewport::Viewport(int x, int y, int width, int height, bool bind_on_construct, int index)
	: Viewport((float)x, (float)y, (float)width, (float)height, bind_on_construct, index)
{
}

Viewport::~Viewport()
{
}

void Viewport::reshape(float x, float y, float width, float height, int index)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->index = index;
	bind();
}

void Viewport::reshape(int x, int y, int width, int height, int index)
{
	reshape((float)x, (float)y, (float)width, (float)height, index);
}

void Viewport::bind()
{
	glViewportIndexedf(index, x, y, width, height);
}
