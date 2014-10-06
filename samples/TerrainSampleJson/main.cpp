#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Core/Utilities/ShaderConstants.h"
#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Samples/SamplesComponentSerializer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>


using namespace GM;
using namespace Application;

struct TerrainVertex {
	glm::vec3 position;
	glm::vec3 normal;
};

void build_terrain_indices_triangle_strip(std::vector<unsigned int> &indices, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	// 0    c1      n1      3
	// 4    u1      6       7
	// 8    9       10      c2
	// n2   13      14      u2
	// 16   17      18      19

	//convert the (x,y) to a single index value which represents the "current" index in the triangle strip
	//represented by "c" in the figure above
	unsigned int current_index = y*width + x;

	//find the vertex index in the grid directly under the current index
	//represented by "u" in the figure above
	unsigned int under_index = 0;
	if (y < height - 1)
		under_index = (y + 1)*width + x;
	else
		return; //This is the last row, which has already been covered in the previous row with triangle strips in mind

	indices.push_back(current_index);
	indices.push_back(under_index);

	//degenerate triangle technique at end of each row, so that we only have one dip call for the entire grid
	//otherwise we'd need one triangle strip per row. We only want one triangle strip for the entire grid!
	if (x < width - 1)
		return;

	if (y >= height - 2)
		return;

	//find the next vertex index in the grid from the current index
	//represented by "n" in the figure above
	//We already know that the current index is the last in the current row and that it's not the last row in the grid,
	//so no need to make any if-checks here.
	unsigned int next_index = (y + 1)*width;

	//Add an invisible degeneration here to bind with next row in triangle strip
	indices.push_back(under_index);
	indices.push_back(next_index);
}

void build_terrain_indices_triangles(std::vector<unsigned int> &indices, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	// 0    c1      n1      3
	// 4    u1      v1      7
	// 8    9       c2      n2
	// 12   13      u2      v2
	// 16   17      18      19

	// Triangluation order
	// 0 - 1
	// | \ |
	// 3 - 2
	//

	// If at end of row
	if (x >= width - 1)
		return;

	// If at end of column
	if (y >= height - 2)
		return;

	//convert the (x,y) to a single index value which represents the "current" index in the triangle strip
	//represented by "c" in the figure above
	unsigned int current_index = y*width + x;

	//find the vertex index in the grid directly under the current index
	//represented by "u" in the figure above
	unsigned int under_index = 0;
	if (y < height - 1)
		under_index = (y + 1)*width + x;
	else
		return; //This is the last row, which has already been covered in the previous row with triangles in mind

	//find the next vertex index in the grid from the current index
	//represented by "n" in the figure above
	unsigned int next_index = y*width + x + 1;

	//find the vertex index in the grid directly under the current index
	//represented by "v" in the figure above
	unsigned int next_under_index = 0;
	if (y < height - 1)
		next_under_index = (y + 1)*width + x + 1;
	else
		return; //This is the last row, which has already been covered in the previous row with triangles in mind

	indices.push_back(next_under_index);
	indices.push_back(next_index);
	indices.push_back(current_index);

	indices.push_back(current_index);
	indices.push_back(under_index);
	indices.push_back(next_under_index);
}

/**
 * Construct the vertices. Sets position and a normal that points upwards.
 */
void build_terrain_vertices(std::vector<TerrainVertex> &vertices, const GM::Framework::RawImagePtr &img, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	const std::vector<unsigned char> &img_data = img->get_data();
	unsigned int pixel_coordinate = y*width + x;

	// A channel is a colour component, eg. red is channel 0, green is channel 1, blue is channel 2 etc.
	// Heightmaps are usually in greyscale, and often only one channel.
	unsigned int channel = 0; // channel must be lesser than img->get_num_channels()

	// Normalized terrain height
	float terrain_height = (img_data[img->get_num_channels() * pixel_coordinate + channel]) / 255.f;

	//Add one x,y,z vertex for each x,y in the grid
	vertices.push_back({
		glm::vec3(x / (float)width, terrain_height, y / (float)height), // position
		glm::vec3( 0, 1, 0 ) // normal
	});
}

//////////////////////////////
//
//       a---b---c
// -1 ^  |  /|  /|
//    |  |/  |/  |
//  0 z  d---e---f
//    |  |  /|  /|
// +1 v  |/  |/  |
//       g---h---i
//
//         <-x->
//       -1  0 +1
///////////////////////////////
void build_terrain_normals(std::vector<TerrainVertex> &verts, unsigned int width, unsigned int height)
{
	glm::vec3 v1,v2,v3,v4,v5,v6; // direction vectors
	glm::vec3 n,n1,n2,n3,n4,n5,n6; // normal vectors created from direction vectors

	for(unsigned int z = 0; z < height; z++)
	{
		for(unsigned int x = 0; x < width; x++)
		{
			// back left corner - 1 tri 2 vertices
			if(z == 0 && x == 0)
			{
				v1 = verts[((z+1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				v2 = verts[(  (z)*width + (x+1))].position - verts[((z)*width + (x))].position;
				n = glm::cross(v1,v2);
			}
			// left edge - 3 tri 4 vertices
			else if((z > 0 && z < (height-1)) && x == 0)
			{
				v1 = verts[(  (z)*width + (x+1))].position - verts[((z)*width + (x))].position;
				v2 = verts[((z-1)*width + (x+1))].position - verts[((z)*width + (x))].position;
				v3 = verts[((z-1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				v4 = verts[((z+1)*width + (x+1))].position - verts[((z)*width + (x))].position;
				n1 = glm::cross(v1,v2); n2 = glm::cross(v2,v3); n3 = glm::cross(v3,v4);
				n = (n1+n2+n3)/3.0f;
			}
			// front left corner - 2 tri 3 vertices
			else if(z == (height-1) && x == 0)
			{
				v1 = verts[(  (z)*width + (x+1))].position - verts[((z)*width + (x))].position;
				v2 = verts[((z-1)*width + (x+1))].position - verts[((z)*width + (x))].position;
				v3 = verts[((z-1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				n1 = glm::cross(v1,v2); n2 = glm::cross(v2,v3);
				n = (n1+n2)/2.0f;
			}
			// front edge - 3 tri 4 vertices
			else if(z == (height-1) && (x > 0 && x < (width-1)))
			{
				v1 = verts[(  (z)*width + (x+1))].position - verts[((z)*width + (x))].position;
				v2 = verts[((z-1)*width + (x+1))].position - verts[((z)*width + (x))].position;
				v3 = verts[((z-1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				v4 = verts[(  (z)*width + (x-1))].position - verts[((z)*width + (x))].position;
				n1 = glm::cross(v1,v2); n2 = glm::cross(v2,v3); n3 = glm::cross(v3,v4);
				n = (n1+n2+n3)/3.0f;
			}
			// front right corner - 1 tri 2 vertices
			else if(z == (height-1) && x == (width-1))
			{
				v1 = verts[((z-1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				v2 = verts[(  (z)*width + (x-1))].position - verts[((z)*width + (x))].position;
				n = glm::cross(v1,v2);
			}
			// right edge - 3 tri 4 vertices
			else if((z > 0 && z < (height-1)) && x == (width-1))
			{
				v1 = verts[((z-1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				v2 = verts[(  (z)*width + (x-1))].position - verts[((z)*width + (x))].position;
				v3 = verts[((z+1)*width + (x-1))].position - verts[((z)*width + (x))].position;
				v4 = verts[((z+1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				n1 = glm::cross(v1,v2); n2 = glm::cross(v2,v3); n3 = glm::cross(v3,v4);
				n = (n1+n2+n3)/3.0f;
			}
			// back right corner - 2 tri 3 vertices
			else if(z == 0 && x == (width-1))
			{
				v1 = verts[(  (z)*width + (x-1))].position - verts[((z)*width + (x))].position;
				v2 = verts[((z+1)*width + (x-1))].position - verts[((z)*width + (x))].position;
				v3 = verts[((z+1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				n1 = glm::cross(v1,v2); n2 = glm::cross(v2,v3);
				n = (n1+n2)/2.0f;
			}
			// back edge - 3 tri 4 vertices
			else if(z == 0 && (x > 0 && x < (width-1)))
			{
				v1 = verts[(  (z)*width + (x-1))].position - verts[((z)*width + (x))].position;
				v2 = verts[((z+1)*width + (x-1))].position - verts[((z)*width + (x))].position;
				v3 = verts[((z+1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				v4 = verts[(  (z)*width + (x+1))].position - verts[((z)*width + (x))].position;
				n1 = glm::cross(v1,v2); n2 = glm::cross(v2,v3); n3 = glm::cross(v3,v4);
				n = (n1+n2+n3)/3.0f;
			}
			// internal - 6 tri 6 vertices
			else
			{
				v1 = verts[(  (z)*width + (x+1))].position - verts[((z)*width + (x))].position;
				v2 = verts[((z-1)*width + (x+1))].position - verts[((z)*width + (x))].position;
				v3 = verts[((z-1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				v4 = verts[(  (z)*width + (x-1))].position - verts[((z)*width + (x))].position;
				v5 = verts[((z+1)*width + (x-1))].position - verts[((z)*width + (x))].position;
				v6 = verts[((z+1)*width + (x)  )].position - verts[((z)*width + (x))].position;
				n1 = glm::cross(v1,v2); n2 = glm::cross(v2,v3); n3 = glm::cross(v3,v4);
				n4 = glm::cross(v4,v5); n5 = glm::cross(v5,v6); n6 = glm::cross(v6,v1);
				n = (n1+n2+n3+n4+n5+n6)/6.0f;
			}

			verts[z*width + x].normal = glm::normalize(n);
		}
	}
}

void create_triangle_mesh(const MainPtr &app, unsigned int width, unsigned int height, bool use_tesselation)
{
	if (app->get_mesh_manager()->contains("terrain"))
	{
		return;
	}

	// Assumes a texture exists by the name heightmap
	GM::Framework::RawImagePtr img = app->get_texture_manager()->get_or_create_image("terrain/heightmap/heightmap.png");

	if (img == nullptr || img->get_height() == 0 || img->get_width() == 0)
	{
		throw clan::Exception("Unable to fetch required image for heightmap");
	}

	Core::VaoLayout vao_layout;
	Core::RenderCommand render_command;

	std::vector<unsigned int> indices;
	std::vector<TerrainVertex> vertices;

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			if (use_tesselation)
				build_terrain_indices_triangles(indices, x, y, width, height);
			else
				build_terrain_indices_triangle_strip(indices, x, y, width, height);
				
			build_terrain_vertices(vertices, img, x, y, width, height);
		}
	}
	build_terrain_normals(vertices, width, height);

	auto vertex_allocation = app->get_buffer_manager()->allocate_and_upload(vertices, GL_DYNAMIC_DRAW);
	
	vao_layout
		.for_buffer(vertex_allocation)
			.use_as(GL_ARRAY_BUFFER)
				.bind_interleaved(
					Core::VaoArg<glm::vec3>(Core::ShaderConstants::Position),
					Core::VaoArg<glm::vec3>(Core::ShaderConstants::Normal));
		;

	render_command.set_vertices(vertex_allocation, vertices);

	auto index_allocation = app->get_buffer_manager()->allocate_and_upload(indices);
	vao_layout
		.for_buffer(index_allocation)
			.use_as(GL_ELEMENT_ARRAY_BUFFER);
	render_command.set_indices(index_allocation, indices);

	if (use_tesselation)
		render_command.set_draw_mode(GL_PATCHES);
	else 
		render_command.set_draw_mode(GL_TRIANGLE_STRIP);

	auto mesh = std::make_shared<Framework::Mesh>(render_command, vao_layout, app->get_vao_manager(), "terrain");
	app->get_mesh_manager()->add(mesh);
}

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 4, 1);

	auto entity_manager = app->get_entity_manager();
	auto samples_component_serializer = std::make_shared<Samples::SamplesComponentSerializer>(app);

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/terrain");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/terrain");
	auto texture_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/textures/samples");

	// Set up resource data
	entity_manager->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);
	app->get_texture_manager()->add_templates(json_path + "/texture_templates.json");
	app->get_texture_manager()->add_format_templates(json_path + "/texture_format_templates.json");
	app->get_texture_manager()->set_texture_path(texture_path);

	// Setup resources
	create_triangle_mesh(app, 1024, 1024, false); // True when tesselation is active, false when not... (Need to make sure the tesselation material is active before this is set to true though).

	// Create our entities
	auto camera_entity = entity_manager->create_entity("camera");
	auto skybox = entity_manager->create_entity("skybox");
	auto terrain = entity_manager->create_entity("terrain");
	auto light = entity_manager->create_entity("light");

	// Apply an entity template, as defined in entity_templates.json
	entity_manager->apply("fps_camera", camera_entity);
	entity_manager->apply("skybox", skybox);
	entity_manager->apply("terrain", terrain);
	entity_manager->apply("light", light);
	
	// Set up the projection for the camera
	if (camera_entity->has_component<Framework::Camera>()) {
		auto camera = camera_entity->get_component<Framework::Camera>();
		camera->set_projection(app->get_resolution());
	}

	Framework::TransformPtr camera_transform;
	if (camera_entity->has_component<Framework::Transform>()) {
		camera_transform = camera_entity->get_component<Framework::Transform>();
	}

	Framework::RenderablePtr terrain_renderable;
	if (terrain->has_component<Framework::Renderable>()) {
		terrain_renderable = terrain->get_component<Framework::Renderable>();
		terrain_renderable->get_material()->get<int>(GM_PROPERTY_PATCH_VERTICES) = 4;
	}
	
	// Hide the cursor since we have an FPS Camera
	app->hide_cursor();

	// Set some run time limits
	float max_run_time = -1;
	float run_time = 0.f;

	auto update_slot = app->on_update().connect([&](float dt) mutable {
		run_time += dt;
		if ((max_run_time > 0 && run_time > max_run_time) || app->is_key_down(GLFW_KEY_ESCAPE))
		{
			app->stop_running();
			return;
		}

		if (terrain_renderable->get_material()->has_property("camera_position")) {
			terrain_renderable->get_material()->get<glm::vec3>("camera_position") = camera_transform->get_position();
		}
	});


	// Start rendering
	app->run();

	return true;
}

int main() {
	
	bool successful = mainTest();

#ifdef WIN32
	system("pause");
#endif

	if (successful) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}
