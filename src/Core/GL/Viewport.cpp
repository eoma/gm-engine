#include <GM/Core/GL/Viewport.h>

using namespace GM::Core;

Viewport::Viewport(float x_offset, float y_offset, float width, float height, bool bind_on_construct, int index)
	: x_offset(x_offset)
	, y_offset(y_offset)
	, width(width)
	, height(height)
	, index(index)
{
	if(bind_on_construct)
	{
		bind();
	}
}

Viewport::Viewport(int x_offset, int y_offset, int width, int height, bool bind_on_construct, int index)
	: Viewport((float)x_offset, (float)y_offset, (float)width, (float)height, bind_on_construct, index)
{
}

Viewport::~Viewport()
{
}

void Viewport::reshape(float x_offset, float y_offset, float width, float height, int index)
{
	this->x_offset = x_offset;
	this->y_offset = y_offset;
	this->width = width;
	this->height = height;
	this->index = index;
	bind();
}

void Viewport::reshape(int x_offset, int y_offset, int width, int height, int index)
{
	reshape((float)x_offset, (float)y_offset, (float)width, (float)height, index);
}

void Viewport::bind()
{
	glViewportIndexedf(index, x_offset, y_offset, width, height);
}
