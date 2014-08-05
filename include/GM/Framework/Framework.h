#pragma once

//
// An include all header file for Framework
//

// The immediate files

#include "DefinitionsComponentNames.h"
#include "DefinitionsPropertyNames.h"

#include "Entity.h"
#include "EntityManager.h"

// Components
#include "Components/Camera.h"

#include "Components/Light.h"

#include "Components/IRenderable.h"

#include "Components/Renderable.h"

#include "Components/Transform.h"

// IO
#include "IO/AssimpMeshIO.h"

#include "IO/IMeshIO.h"
#include "IO/IImageIO.h"

#include "IO/SoilImageIO.h"

#include "IO/TextIO.h"

// Managers
#include "Managers/BufferManager.h"

#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"

#include "Managers/ShaderManager.h"

#include "Managers/TextureManager.h"

#include "Managers/VaoManager.h"

// Systems
#include "Systems/RenderSystem.h"

#include "Systems/SceneSystem.h"

// Templates
#include "Templates/EntityTemplateManager.h"
#include "Templates/EntityTemplateParser.h"

#include "Templates/MaterialTemplateManager.h"
#include "Templates/MaterialTemplateParser.h"

#include "Templates/ShaderTemplateManager.h"
#include "Templates/ShaderTemplateParser.h"

#include "Templates/TextureFormatTemplateManager.h"
#include "Templates/TextureFormatTemplateParser.h"
#include "Templates/TextureTemplateManager.h"
#include "Templates/TextureTemplateParser.h"

// Totem
#include "Totem/Totem.h"

// Utilities
#include "Utilities/Color.h"
#include "Utilities/ComponentSerializer.h"

#include "Utilities/Material.h"
#include "Utilities/Mesh.h"

#include "Utilities/PropertySerializer.h"

#include "Utilities/RawImage.h"

#include "Utilities/Tools.h"
