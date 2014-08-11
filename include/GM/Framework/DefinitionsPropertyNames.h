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
#define PROPERTY_NORMAL_MATRIX		"NormalMatrix"		// glm::mat3
#define PROPERTY_FOV				"FOV"				// float
#define PROPERTY_NEAR_CLIPPING		"NearClipping"		// float
#define PROPERTY_FAR_CLIPPING		"FarClipping"		// float
#define PROPERTY_MAX_VERTICAL_ANGLE "MaxVerticalAngle"	// float

// Material
#define PROPERTY_MATERIAL_NAME		"MaterialName"		// std::string

// Mesh
#define PROPERTY_MESH_NAME			"MeshName"			// std::string

// Light
#define PROPERTY_RADIUS				"Radius"			// float
#define PROPERTY_MATERIAL_COLOR_DIFFUSE		"MaterialColorDiffuse" // glm::vec3
#define PROPERTY_MATERIAL_COLOR_SPECULAR	"MaterialColorSpecular" // glm::vec3
#define PROPERTY_MATERIAL_COLOR_AMBIENT		"MaterialColorAmbient" // glm::vec3
