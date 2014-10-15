#pragma once

// Transform
#define GM_PROPERTY_POSITION			"Position"			// glm::vec3
#define GM_PROPERTY_SCALE 				"Scale"				// glm::vec3
#define GM_PROPERTY_ORIENTATION			"Orientation"		// glm::quat
#define GM_PROPERTY_OBJECT_MATRIX		"ObjectMatrix"		// glm::mat4
#define GM_PROPERTY_WORLD_MATRIX		"WorldMatrix"		// glm::mat4
#define GM_PROPERTY_OBJECT_MATRIX_NO_SCALE	"ObjectMatrixNoScale"	// glm::mat4
#define GM_PROPERTY_WORLD_MATRIX_NO_SCALE	"WorldMatrixNoScale"	// glm::mat4

// Culler and render system
#define GM_PROPERTY_CULLED				"Culled"			// bool
#define GM_PROPERTY_VISIBLE				"Visible"			// bool

// Camera
#define GM_PROPERTY_PROJECTION_MATRIX	"ProjectionMatrix"	// glm::mat4
#define GM_PROPERTY_VIEW_MATRIX			"ViewMatrix"		// glm::mat4
#define GM_PROPERTY_NORMAL_MATRIX		"NormalMatrix"		// glm::mat3
#define GM_PROPERTY_FOV					"FOV"				// float
#define GM_PROPERTY_NEAR_CLIPPING		"NearClipping"		// float
#define GM_PROPERTY_FAR_CLIPPING		"FarClipping"		// float
#define GM_PROPERTY_MAX_VERTICAL_ANGLE	"MaxVerticalAngle"	// float

// Material
#define GM_PROPERTY_MATERIAL_NAME		"MaterialName"		// std::string
#define GM_PROPERTY_PATCH_VERTICES		"PatchVertices"		// int

// Mesh
#define GM_PROPERTY_MESH_NAME					"MeshName"				// std::string
#define GM_PROPERTY_MESH_PIVOT_POINT_OFFSET		"MeshPivotPointOffset"	// glm::vec3

// Light
#define GM_PROPERTY_RADIUS						"Radius"				// float
#define GM_PROPERTY_MATERIAL_COLOR_DIFFUSE		"MaterialColorDiffuse"	// glm::vec3
#define GM_PROPERTY_MATERIAL_COLOR_SPECULAR		"MaterialColorSpecular" // glm::vec3
#define GM_PROPERTY_MATERIAL_COLOR_AMBIENT		"MaterialColorAmbient"	// glm::vec3
#define GM_PROPERTY_ACTIVATED                   "Activated"             // bool