#include "GM/Framework/Components/Tessellate.h"

#include "GM/Framework/Systems/RenderSystem.h"

#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"

#include <glm/ext.hpp>
#include <GL/gl3w.h>

using namespace GM::Framework;

Tessellate::Tessellate(const EntityPtr &owner, const RenderSystemPtr &render_system, const std::string &name)
: Component(owner, name)
{
    tess_level_inner = owner->add(GM_PROPERTY_TESS_LEVEL_INNER, 1);
    tess_level_outer = owner->add(GM_PROPERTY_TESS_LEVEL_OUTER, 1);
    patch_vertices = owner->add(GM_PROPERTY_PATCH_VERTICES, 1);
}



Tessellate::~Tessellate()
{
    
}



std::string
Tessellate::get_type() const
{
    return get_static_type();
}



int
Tessellate::get_tess_level_inner() const
{
    return tess_level_inner;
}



int
Tessellate::get_tess_level_outer() const
{
    return tess_level_outer;
}



int
Tessellate::get_patch_vertices() const
{
    return patch_vertices;
}