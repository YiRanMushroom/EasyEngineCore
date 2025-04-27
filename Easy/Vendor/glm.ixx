module;

#include <cmath>
#include <climits>
#include <cfloat>
#include <limits>
#include <cassert>
#include "glm/fwd.hpp"
#include <functional>
#include <exception>

import yrm.msvc.std.compat;

export module Easy.Vendor.glm;

export {
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat2x2.hpp"
#include "glm/mat2x3.hpp"
#include "glm/mat2x4.hpp"
#include "glm/mat3x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat3x4.hpp"
#include "glm/mat4x2.hpp"
#include "glm/mat4x3.hpp"
#include "glm/mat4x4.hpp"

#include "glm/trigonometric.hpp"
#include "glm/exponential.hpp"
#include "glm/common.hpp"
#include "glm/packing.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"
#include "glm/vector_relational.hpp"
#include "glm/integer.hpp"
}