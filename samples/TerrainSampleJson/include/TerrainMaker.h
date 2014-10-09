#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace GM {
	namespace Application {
		class Main; typedef std::shared_ptr<Main> MainPtr;
	}

	namespace Framework {
		class RawImage; typedef std::shared_ptr<RawImage> RawImagePtr;
	}
}

struct TerrainVertex {
	glm::vec3 position;
	glm::vec3 normal;
};

void create_triangle_mesh(const GM::Application::MainPtr &app, unsigned int width, unsigned int height, bool use_tesselation);

void build_terrain_indices_triangle_strip(std::vector<unsigned int> &indices, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

void build_terrain_indices_triangles(std::vector<unsigned int> &indices, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

void build_terrain_vertices(std::vector<TerrainVertex> &vertices, const GM::Framework::RawImagePtr &img, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

void build_terrain_normals(std::vector<TerrainVertex> &verts, unsigned int width, unsigned int height);
