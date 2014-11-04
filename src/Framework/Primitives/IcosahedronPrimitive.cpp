#include "GM/Framework/Primitives/IcosahedronPrimitive.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/VaoManager.h"
#include "GM/Framework/Utilities/Mesh.h"
#include "GM/Framework/DefinitionsPrimitiveNames.h"

#include "GM/Core/Utilities/VaoLayout.h"
#include "GM/Core/Utilities/ShaderConstants.h"
#include "GM/Core/Utilities/RenderCommand.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/gl3w.h>

#include <vector>
using namespace GM;
using namespace Framework;

IcosahedronPrimitive::IcosahedronPrimitive()
: IPrimitive(GM_PRIMITIVE_ICOSAHEDRON)
{
    
}



MeshPtr
IcosahedronPrimitive::create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager)
{
    struct MyVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 diffuse;
    };
    
    Core::VaoLayout vao_layout;
    Core::RenderCommand render_command;
    
    std::vector<MyVertex> vertices{
      //   position                         normal      diffuse
        { {0.000f,  0.000f,  1.000f},       {0, 0, 0},  {3.0, 2.0, 0.0}},
        { {0.894f,  0.000f,  0.447f},       {0, 0, 0},  {1.0, 0.0, 0.0}},
        { {0.276f,  0.851f,  0.447f},       {0, 0, 0},  {1.0, 1.0, 0.0}},
        { {-0.724f,  0.526f,  0.447f},      {0, 0, 0},  {0.0, 0.0, 0.5}},
        { {-0.724f, -0.526f,  0.447f},      {0, 0, 0},  {0.0, 1.0, 0.0}},
        { {0.276f, -0.851f,  0.447f},       {0, 0, 0},  {0.0, 1.0, 1.0}},
        { {0.724f,  0.526f, -0.447f},       {0, 0, 0},  {0.0, 0.0, 1.0}},
        { {-0.276f,  0.851f, -0.447f},      {0, 0, 0},  {0.5, 0.0, 0.0}},
        { {-0.894f,  0.000f, -0.447f},      {0, 0, 0},  {0.5, 0.5, 0.0}},
        { {-0.276f, -0.851f, -0.447f},      {0, 0, 0},  {0.5, 0.5, 0.5}},
        { {0.724f, -0.526f, -0.447f},       {0, 0, 0},  {0.0, 0.5, 0.0}},
        { {0.000f,  0.000f, -1.000f},       {0, 0, 0},  {0.0, 0.5, 0.5}},
    };
    
    std::vector<unsigned int> indices{
        2, 1, 0,
        3, 2, 0,
        4, 3, 0,
        5, 4, 0,
        1, 5, 0,
        11, 6, 7,
        11, 7, 8,
        11, 8, 9,
        11, 9, 10,
        11, 10, 6,
        1, 2, 6,
        2, 3, 7,
        3, 4, 8,
        4, 5, 9,
        5, 1, 10,
        2, 7, 6,
        3, 8, 7,
        4, 9, 8,
        5, 10, 9,
        1, 6, 10
    };
    
    // Create normals
    for (unsigned int i = 0; i < indices.size() / 3; ++i)
    {
        const glm::vec3 pos1 = vertices[indices[(i * 3) + 0]].position;
        const glm::vec3 pos2 = vertices[indices[(i * 3) + 1]].position;
        const glm::vec3 pos3 = vertices[indices[(i * 3) + 2]].position;
        
        const glm::vec3 normal = glm::normalize(glm::cross(pos3 - pos1, pos2 - pos1));
        
        vertices[indices[(i * 3) + 0]].normal += normal;
        vertices[indices[(i * 3) + 1]].normal += normal;
        vertices[indices[(i * 3) + 2]].normal += normal;
    }
    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
        vertices[i].normal = glm::normalize(vertices[i].normal);
    }
    
    auto vertex_allocation = buffer_manager->allocate_and_upload(vertices, GL_STATIC_DRAW);
    vao_layout
        .for_buffer(vertex_allocation)
            .use_as(GL_ARRAY_BUFFER)
                .bind_interleaved(
                    Core::VaoArg<glm::vec3>(Core::ShaderConstants::Position),
                    Core::VaoArg<glm::vec3>(Core::ShaderConstants::Normal),
                    Core::VaoArg<glm::vec3>(Core::ShaderConstants::Diffuse));
    render_command.set_vertices(vertex_allocation, vertices);
    
    auto index_allocation = buffer_manager->allocate_and_upload(indices, GL_STATIC_DRAW);
    vao_layout
        .for_buffer(index_allocation)
            .use_as(GL_ELEMENT_ARRAY_BUFFER);
    render_command.set_indices(index_allocation, indices);
    
    render_command.set_draw_mode(GL_PATCHES);
    
    return std::make_shared<Framework::Mesh>(render_command, vao_layout, vao_manager, get_name());
}