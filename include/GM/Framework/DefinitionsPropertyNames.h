#pragma once

// Transform
#define PROPERTY_POSITION			"Position"			// glm::vec3
#define PROPERTY_SCALE 				"Scale"				// glm::vec3
#define PROPERTY_ORIENTATION		"Orientation"		// glm::quat
#define PROPERTY_OBJECT_MATRIX		"OjectMatrix"		// glm::mat4
#define PROPERTY_WORLD_MATRIX		"WorldMatrix"		// glm::mat4

// Culler and render system
#define PROPERTY_CULLED				"Culled"			// bool
#define PROPERTY_VISIBLE			"Visible"			// bool

// Camera
#define PROPERTY_PROJECTION_MATRIX	"ProjectionMatrix"	// glm::mat4
#define PROPERTY_VIEW_MATRIX		"ViewMatrix"		// glm::mat4

// Material
#define PROPERTY_MATERIAL_NAME		"MaterialName"		// std::string

// Mesh
#define PROPERTY_MESH_NAME			"MeshName"			// std::string
