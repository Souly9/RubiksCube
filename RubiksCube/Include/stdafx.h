#define GLFW_INCLUDE_NONE

/********** Constants **********/
#define COMBUSTION_GL_VERSION_MAJOR 4
#define COMBUSTION_GL_VERSION_MINOR 3

#define COMBUSTION_SCENE_BUFFER_INDEX 10
#define COMBUSTION_MODEL_BUFFER_INDEX 11
#define COMBUSTION_GENERIC_LIGHT_BUFFER_INDEX 30
#define COMBUSTION_AMBIENT_STRENGTH 0.01f
#define COMBUSTION_MAX_FORWARD_LIGHTS 10
#define COMBUSTION_TEXTURE_LIMIT 30
#define COMBUSTION_FLOAT_TOLERANCE 1e-6f

#define MAYBE_UNUSED(e) static_cast<void>(e)

/********** Standard Libraries **********/
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cstdio>

/********** Application Header **********/
#include "ObjReader.h"
#include "VertexStructs.h"
#include "Mesh3D.h"
#include "HighResClock.h"
#include "WindowManager.h"
#include "GeometryBatch.h"
#include "VertexAttribPointer.h"
#include "RenderBatch.h"
#include "Material.h"
#include "MaterialUtilities.h"
#include "GeometryManager.h"
#include "Scene.h"
#include "Camera.h"
#include "DataHolder.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "InputSignals.h"
#include "ApplicationEvents.h"
#include "Color.h"
#include "Object.h"
#include "RubixCubeEnums.h"
#include "InputManager.h"
#include "RayStructs.h"

/********** Math Library **********/
#include <glm/matrix.hpp>
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

/********** OpenGL **********/
#include "glad/glad.h"
#include <glfw3.h>
