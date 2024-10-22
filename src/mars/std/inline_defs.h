/**
 * inline_defs.h
 * With this header, we can switch whether functions should be flagged as inline at build time.
 * When MARS_USE_INLINE is set, function definitions will look like this:
 *   header:
 *     inline void foo(x) {...}
 *   source:
 *     extern void foo(x);
 * Otherwise, they look like this:
 *   header:
 *     void foo(x);
 *   source:
 *     void foo(x) {...}
*/
// Clear previous definitions
#if defined(_inline_defs_prefix)
#undef _inline_defs_prefix
#endif

#if defined(_inline_defs_suffix)
#undef _inline_defs_suffix
#endif

// Define function prefix (whether to use inline or extern keyword)
#if defined(_inline_defs_header)
	#if defined(MARS_USE_INLINE)
		#define _inline_defs_prefix _inline_defs_prefix
	#else 
		#define _inline_defs_prefix MARS_API
	#endif
#else
	#if defined(MARS_USE_INLINE)
		#define _inline_defs_prefix extern
	#else
		#define _inline_defs_prefix
	#endif
#endif

// Define function suffix (whether to include function body)
#if ((defined(_inline_defs_header) && defined(MARS_USE_INLINE)) || (!defined(_inline_defs_header) && !defined(MARS_USE_INLINE)))
	#define _inline_defs_suffix(...) {__VA_ARGS__}
#else
	#define _inline_defs_suffix(...) ;
#endif

//----------------------------------------------------------------------------------
// Vector2 math
//----------------------------------------------------------------------------------

_inline_defs_prefix Vector2 Vector2Zero(void)_inline_defs_suffix(
	Vector2 result = { 0.f, 0.f };
	return result;
)

/// @brief Vector with components value 1.
_inline_defs_prefix Vector2 Vector2One(void)_inline_defs_suffix(
	Vector2 result = { 1.f, 1.f };
	return result;
)

/// @brief Add two vectors (v1 + v2).
_inline_defs_prefix Vector2 Vector2Add(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	Vector2 result = { v1.x + v2.x, v1.y + v2.y };
	return result;
)

/// @brief Add vector and value.
_inline_defs_prefix Vector2 Vector2AddValue(Vector2 v, float value)_inline_defs_suffix(
	Vector2 result = { v.x + value, v.y + value };
	return result;
)

/// @brief Subtract two vectors (v1 - v2).
_inline_defs_prefix Vector2 Vector2Subtract(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	Vector2 result = { v1.x - v2.x, v1.y - v2.y };
	return result;
)

/// @brief Subtract vector by value.
_inline_defs_prefix Vector2 Vector2SubtractValue(Vector2 v, float value)_inline_defs_suffix(
	Vector2 result = { v.x - value, v.y - value };
	return result;
)

/// @brief Calculate vector length.
_inline_defs_prefix float Vector2Length(Vector2 v)_inline_defs_suffix(
	float result = sqrtf((v.x*v.x) + (v.y*v.y));
	return result;
)

/// @brief Calculate vector square length.
_inline_defs_prefix float Vector2LengthSqr(Vector2 v)_inline_defs_suffix(
	float result = (v.x*v.x) + (v.y*v.y);
	return result;
)

/// @brief Calculate two vectors dot product.
_inline_defs_prefix float Vector2DotProduct(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	float result = (v1.x*v2.x + v1.y*v2.y);
	return result;
)

/// @brief Calculate distance between two vectors.
_inline_defs_prefix float Vector2Distance(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	float result = sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
	return result;
)

/// @brief Calculate square distance between two vectors.
_inline_defs_prefix float Vector2DistanceSqr(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	float result = ((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
	return result;
)

/// @brief Calculate angle between two vectors (BAM integers).
/// @brief NOTE: Angle is calculated from origin point (0, 0).
_inline_defs_prefix Angle Vector2Angle(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	float result = atan2f((v2.y - v1.y), (v2.x - v1.x));
	return RAD2ANGLE(result);
)

/// @brief Multiply vector by vector.
_inline_defs_prefix Vector2 Vector2Multiply(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	Vector2 result = { v1.x*v2.x, v1.y*v2.y };
	return result;
)

/// @brief Multiply each component by value.
_inline_defs_prefix Vector2 Vector2MultiplyValue(Vector2 v, float value)_inline_defs_suffix(
	Vector2 result = { v.x*value, v.y*value };
	return result;
)

/// @brief Negate vector.
_inline_defs_prefix Vector2 Vector2Negate(Vector2 v)_inline_defs_suffix(
	Vector2 result = { -v.x, -v.y };
	return result;
)

/// @brief Divide vector by vector.
_inline_defs_prefix Vector2 Vector2Divide(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	Vector2 result = { v1.x/v2.x, v1.y/v2.y };
	return result;
)

/// @brief Divide each component by value.
_inline_defs_prefix Vector2 Vector2DivideValue(Vector2 v, float value)_inline_defs_suffix(
	Vector2 result = { v.x/value, v.y/value };
	return result;
)

/// @brief Calculate reflected vector to normal.
_inline_defs_prefix Vector2 Vector2Reflect(Vector2 v, Vector2 normal)_inline_defs_suffix(
	Vector2 result = { 0 };
	float dotProduct = (v.x*normal.x + v.y*normal.y);
	result.x = v.x - (2*normal.x)*dotProduct;
	result.y = v.y - (2*normal.y)*dotProduct;
	return result;
)

/// @brief Normalize vector.
_inline_defs_prefix Vector2 Vector2Normalize(Vector2 v)_inline_defs_suffix(
	float len = Vector2Length(v);
	Vector2 result = { v.x / len, v.y / len };
	return result;
)

/// @brief Get min value for each pair of components.
_inline_defs_prefix Vector2 Vector2Min(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	Vector2 result = { 0 };

	result.x = fminf(v1.x, v2.x);
	result.y = fminf(v1.y, v2.y);

	return result;
)

/// @brief Get max value for each pair of components.
_inline_defs_prefix Vector2 Vector2Max(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	Vector2 result = { 0 };

	result.x = fmaxf(v1.x, v2.x);
	result.y = fmaxf(v1.y, v2.y);

	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix float Vector2MinComponent(Vector2 v)_inline_defs_suffix(
	return fminf(v.x, v.y);
)

/// @brief Get the maximum value in the vector.
_inline_defs_prefix float Vector2MaxComponent(Vector2 v)_inline_defs_suffix(
	return fmaxf(v.x, v.y);
)

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
_inline_defs_prefix Vector2 Vector2Clamp(Vector2 v, Vector2 minv, Vector2 maxv)_inline_defs_suffix(
	Vector2 result = { 0 };
	result.x = fminf(maxv.x, fmaxf(minv.x, v.x));
	result.y = fminf(maxv.y, fmaxf(minv.y, v.y));
	return result;
)

/// @brief Check whether two given vectors are almost equal.
_inline_defs_prefix bool Vector2Equals(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	return (fequal(v1.x, v2.x) && fequal(v1.y, v2.y));
)

/// @brief Check whether the vector is all zeros.
_inline_defs_prefix bool Vector2IsZero(Vector2 v)_inline_defs_suffix(
	return (fzequal(v.x) && fzequal(v.y));
)

/// @brief Check whether any element in the vector is zero.
_inline_defs_prefix bool Vector2AnyZero(Vector2 v)_inline_defs_suffix(
	return (fzequal(v.x) || fzequal(v.y));
)

/// @brief Cast a float vector to an integer vector.
_inline_defs_prefix Vector2i Vector2CastInt(Vector2 v)_inline_defs_suffix(
	Vector2i result = { (int32_t)(v.x), (int32_t)(v.y) };
	return result;
)

/// @brief Rotate the point v1 around the point v2 by the given angle.
_inline_defs_prefix Vector2 Vector2RotatePoint(Vector2 v1, Vector2 v2, Angle angle)_inline_defs_suffix(
	Vector2 result = { 0 };
	float sinres = sinf(ANGLE2RAD(angle));
	float cosres = cosf(ANGLE2RAD(angle));

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	float xr = result.x * cosres - result.y * sinres;
	float yr = result.x * sinres + result.y * cosres;
	result.x = xr + v2.x;
	result.y = yr + v2.y;

	return result;
)

/// @brief Reflect the vector v1 across v2, along the line between them.
_inline_defs_prefix Vector2 Vector2ReflectPoint(Vector2 v1, Vector2 v2)_inline_defs_suffix(
	Vector2 result = { 0 };

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.x = -result.x;
	result.y = -result.y;
	result.x += v2.x;
	result.y += v2.y;

	return result;
)

/// @brief Convert the vector to an array of floats.
_inline_defs_prefix float2 Vector2Array(Vector2 v)_inline_defs_suffix(
	float2 arr = {{v.x, v.y}};
	return arr;
)


//----------------------------------------------------------------------------------
// Vector3 math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
_inline_defs_prefix Vector3 Vector3Zero(void)_inline_defs_suffix(
	Vector3 result = { 0.f, 0.f, 0.f };
	return result;
)

/// @brief Vector with components value 1.
_inline_defs_prefix Vector3 Vector3One(void)_inline_defs_suffix(
	Vector3 result = { 1.f, 1.f, 1.f };
	return result;
)

/// @brief Add two vectors.
_inline_defs_prefix Vector3 Vector3Add(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	Vector3 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return result;
)

/// @brief Add vector and value.
_inline_defs_prefix Vector3 Vector3AddValue(Vector3 v, float value)_inline_defs_suffix(
	Vector3 result = { v.x + value, v.y + value, v.z + value };
	return result;
)

/// @brief Subtract two vectors.
_inline_defs_prefix Vector3 Vector3Subtract(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	Vector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return result;
)

/// @brief Subtract vector by value.
_inline_defs_prefix Vector3 Vector3SubtractValue(Vector3 v, float value)_inline_defs_suffix(
	Vector3 result = { v.x - value, v.y - value, v.z - value };
	return result;
)

/// @brief Multiply vector by vector.
_inline_defs_prefix Vector3 Vector3Multiply(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	Vector3 result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z };
	return result;
)

/// @brief Multiply each component by value.
_inline_defs_prefix Vector3 Vector3MultiplyValue(Vector3 v, float value)_inline_defs_suffix(
	Vector3 result = { v.x*value, v.y*value, v.z*value };
	return result;
)

/// @brief Calculate two vectors cross product.
_inline_defs_prefix Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	Vector3 result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
	return result;
)

/// @brief Calculate vectors perpendicular vector.
_inline_defs_prefix Vector3 Vector3Perpendicular(Vector3 v)_inline_defs_suffix(
	Vector3 result = { 0 };

	float minv = fabsf(v.x);
	Vector3 cardinalAxis = { 1.f, 0.f, 0.f };

	if (fabsf(v.y) < minv) {
		minv = fabsf(v.y);
		Vector3 tmp = { 0.f, 1.f, 0.f };
		cardinalAxis = tmp;
	}

	if (fabsf(v.z) < minv) {
		Vector3 tmp = { 0.f, 0.f, 1.f };
		cardinalAxis = tmp;
	}

	// Cross product between vectors
	result.x = v.y*cardinalAxis.z - v.z*cardinalAxis.y;
	result.y = v.z*cardinalAxis.x - v.x*cardinalAxis.z;
	result.z = v.x*cardinalAxis.y - v.y*cardinalAxis.x;

	return result;
)

/// @brief Calculate vector length.
_inline_defs_prefix float Vector3Length(const Vector3 v)_inline_defs_suffix(
	float result = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	return result;
)

/// @brief Calculate vector square length.
_inline_defs_prefix float Vector3LengthSqr(const Vector3 v)_inline_defs_suffix(
	float result = v.x*v.x + v.y*v.y + v.z*v.z;
	return result;
)

/// @brief Calculate two vectors dot product.
_inline_defs_prefix float Vector3DotProduct(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	float result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	return result;
)

/// @brief Calculate distance between two vectors.
_inline_defs_prefix float Vector3Distance(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	float result = 0.0f;

	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	float dz = v2.z - v1.z;
	result = sqrtf(dx*dx + dy*dy + dz*dz);

	return result;
)

/// @brief Calculate square distance between two vectors.
_inline_defs_prefix float Vector3DistanceSqr(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	float result = 0;

	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	float dz = v2.z - v1.z;
	result = dx*dx + dy*dy + dz*dz;

	return result;
)

/// @brief Calculate angle between two vectors.
_inline_defs_prefix Angle Vector3Angle(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	float result = 0.0f;

	Vector3 cross = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
	float len = sqrtf(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z);
	float dot = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	result = atan2f(len, dot);

	return RAD2ANGLE(result);
)

/// @brief Negate vector.
_inline_defs_prefix Vector3 Vector3Negate(Vector3 v)_inline_defs_suffix(
	Vector3 result = { -v.x, -v.y, -v.z };
	return result;
)

/// @brief Divide vector by vector.
_inline_defs_prefix Vector3 Vector3Divide(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	Vector3 result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z };
	return result;
)

/// @brief Divide each component by value.
_inline_defs_prefix Vector3 Vector3DivideValue(Vector3 v, float value)_inline_defs_suffix(
	Vector3 result = { v.x/value, v.y/value, v.z/value };
	return result;
)

/// @brief Calculate reflected vector to normal.
_inline_defs_prefix Vector3 Vector3Reflect(Vector3 v, Vector3 normal)_inline_defs_suffix(
	Vector3 result = { 0 };

	// I is the original vector
	// N is the normal of the incident plane
	// R = I - (2*N*(DotProduct[I, N]))

	float dotProduct = (v.x*normal.x + v.y*normal.y + v.z*normal.z);

	result.x = v.x - (2*normal.x)*dotProduct;
	result.y = v.y - (2*normal.y)*dotProduct;
	result.z = v.z - (2*normal.z)*dotProduct;

	return result;
)

/// @brief Normalize vector.
_inline_defs_prefix Vector3 Vector3Normalize(Vector3 v)_inline_defs_suffix(
	float len = Vector3Length(v);
	Vector3 result = { v.x / len, v.y / len, v.z / len };
	return result;
)

/// @brief Get min value for each pair of components.
_inline_defs_prefix Vector3 Vector3Min(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	Vector3 result = { 0 };

	result.x = fminf(v1.x, v2.x);
	result.y = fminf(v1.y, v2.y);
	result.z = fminf(v1.z, v2.z);

	return result;
)

/// @brief Get max value for each pair of components.
_inline_defs_prefix Vector3 Vector3Max(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	Vector3 result = { 0 };

	result.x = fmaxf(v1.x, v2.x);
	result.y = fmaxf(v1.y, v2.y);
	result.z = fmaxf(v1.z, v2.z);

	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix float Vector3MinComponent(Vector3 v)_inline_defs_suffix(
	float result = v.x;
	result = fminf(result, v.y);
	result = fminf(result, v.z);
	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix float Vector3MaxComponent(Vector3 v)_inline_defs_suffix(
	float result = v.x;
	result = fmaxf(result, v.y);
	result = fmaxf(result, v.z);
	return result;
)

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
_inline_defs_prefix Vector3 Vector3Clamp(Vector3 v, Vector3 minv, Vector3 maxv)_inline_defs_suffix(
	Vector3 result = { 0 };

	result.x = fminf(maxv.x, fmaxf(minv.x, v.x));
	result.y = fminf(maxv.y, fmaxf(minv.y, v.y));
	result.z = fminf(maxv.z, fmaxf(minv.z, v.z));

	return result;
)

/// @brief Check whether two given vectors are almost equal.
_inline_defs_prefix bool Vector3Equals(Vector3 v1, Vector3 v2)_inline_defs_suffix(
	return (fequal(v1.x, v2.x) && fequal(v1.y, v2.y) && fequal(v1.z, v2.z));
)

/// @brief Check whether the vector is all zeros.
_inline_defs_prefix bool Vector3IsZero(Vector3 v)_inline_defs_suffix(
	return (fzequal(v.x) && fzequal(v.y) && fzequal(v.z));
)

/// @brief Check whether any element in the vector is zero.
_inline_defs_prefix bool Vector3AnyZero(Vector3 v)_inline_defs_suffix(
	return (fzequal(v.x) || fzequal(v.y) || fzequal(v.z));
)

/// @brief Cast a float vector to an integer vector.
_inline_defs_prefix Vector3i Vector3CastInt(Vector3 v)_inline_defs_suffix(
	Vector3i result = { (int32_t)(v.x), (int32_t)(v.y), (int32_t)(v.z) };
	return result;
)

/// @brief Convert the vector to an array of floats.
_inline_defs_prefix float3 Vector3Array(Vector3 v)_inline_defs_suffix(
	float3 arr = {{v.x, v.y, v.z}};
	return arr;
)


//----------------------------------------------------------------------------------
// Vector4 math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
_inline_defs_prefix Vector4 Vector4Zero(void)_inline_defs_suffix(
	Vector4 result = { 0.f, 0.f, 0.f, 0.f };
	return result;
)

/// @brief Vector with components value 1.
_inline_defs_prefix Vector4 Vector4One(void)_inline_defs_suffix(
	Vector4 result = { 1.f, 1.f, 1.f, 1.f };
	return result;
)

/// @brief Add two vectors.
_inline_defs_prefix Vector4 Vector4Add(Vector4 v1, Vector4 v2)_inline_defs_suffix(
	Vector4 result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
	return result;
)

/// @brief Add Vector4 and value.
_inline_defs_prefix Vector4 Vector4AddValue(Vector4 v, float value)_inline_defs_suffix(
	Vector4 result = {v.x + value, v.y + value, v.z + value, v.w + value};
	return result;
)

/// @brief Subtract two vectors.
_inline_defs_prefix Vector4 Vector4Subtract(Vector4 v1, Vector4 v2)_inline_defs_suffix(
	Vector4 result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
	return result;
)

/// @brief Subtract vector and value.
_inline_defs_prefix Vector4 Vector4SubtractValue(Vector4 v, float value)_inline_defs_suffix(
	Vector4 result = {v.x - value, v.y - value, v.z - value, v.w - value};
	return result;
)

/// @brief Computes the length of a vector.
_inline_defs_prefix float Vector4Length(Vector4 v)_inline_defs_suffix(
	float result = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
	return result;
)

/// @brief Calculate two vector multiplication.
_inline_defs_prefix Vector4 Vector4Multiply(Vector4 v1, Vector4 v2)_inline_defs_suffix(
	Vector4 result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w };
	return result;
)

/// @brief Multiply each component by value.
_inline_defs_prefix Vector4 Vector4MultiplyValue(Vector4 v, float value)_inline_defs_suffix(
	Vector4 result = { v.x*value, v.y*value, v.z*value, v.w*value };
	return result;
)

/// @brief Divide two vectors.
_inline_defs_prefix Vector4 Vector4Divide(Vector4 v1, Vector4 v2)_inline_defs_suffix(
	Vector4 result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w };
	return result;
)

/// @brief Divide each component by value.
_inline_defs_prefix Vector4 Vector4DivideValue(Vector4 v, float value)_inline_defs_suffix(
	Vector4 result = { v.x/value, v.y/value, v.z/value, v.w/value };
	return result;
)

/// @brief Negate vector.
_inline_defs_prefix Vector4 Vector4Negate(Vector4 v)_inline_defs_suffix(
	Vector4 result = { -v.x, -v.y, -v.z, -v.w };
	return result;
)

/// @brief Normalize vector.
_inline_defs_prefix Vector4 Vector4Normalize(Vector4 v)_inline_defs_suffix(
	float len = Vector4Length(v);
	Vector4 result = { v.x / len, v.y / len, v.z / len, v.w / len };
	return result;
)

/// @brief Get min value for each pair of components.
_inline_defs_prefix Vector4 Vector4Min(Vector4 v1, Vector4 v2)_inline_defs_suffix(
	Vector4 result = { 0 };

	result.x = fminf(v1.x, v2.x);
	result.y = fminf(v1.y, v2.y);
	result.z = fminf(v1.z, v2.z);
	result.w = fminf(v1.w, v2.w);

	return result;
)

/// @brief Get max value for each pair of components.
_inline_defs_prefix Vector4 Vector4Max(Vector4 v1, Vector4 v2)_inline_defs_suffix(
	Vector4 result = { 0 };

	result.x = fmaxf(v1.x, v2.x);
	result.y = fmaxf(v1.y, v2.y);
	result.z = fmaxf(v1.z, v2.z);
	result.w = fmaxf(v1.w, v2.w);

	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix float Vector4MinComponent(Vector4 v)_inline_defs_suffix(
	float result = v.x;
	result = fminf(result, v.y);
	result = fminf(result, v.z);
	result = fminf(result, v.w);
	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix float Vector4MaxComponent(Vector4 v)_inline_defs_suffix(
	float result = v.x;
	result = fmaxf(result, v.y);
	result = fmaxf(result, v.z);
	result = fmaxf(result, v.w);
	return result;
)

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
_inline_defs_prefix Vector4 Vector4Clamp(Vector4 v, Vector4 minv, Vector4 maxv)_inline_defs_suffix(
	Vector4 result = { 0 };

	result.x = fminf(maxv.x, fmaxf(minv.x, v.x));
	result.y = fminf(maxv.y, fmaxf(minv.y, v.y));
	result.z = fminf(maxv.z, fmaxf(minv.z, v.z));
	result.w = fminf(maxv.w, fmaxf(minv.w, v.w));

	return result;
)

/// @brief Check whether two given vectors are almost equal.
_inline_defs_prefix bool Vector4Equals(Vector4 v1, Vector4 v2)_inline_defs_suffix(
	return (fequal(v1.x, v2.x) && fequal(v1.y, v2.y) && fequal(v1.z, v2.z) && fequal(v1.w, v2.w));
)

/// @brief Check whether the vector is all zeros.
_inline_defs_prefix bool Vector4IsZero(Vector4 v)_inline_defs_suffix(
	return (fzequal(v.x) && fzequal(v.y) && fzequal(v.z) && fzequal(v.w));
)

/// @brief Check whether any element in the vector is zero.
_inline_defs_prefix bool Vector4AnyZero(Vector4 v)_inline_defs_suffix(
	return (fzequal(v.x) || fzequal(v.y) || fzequal(v.z) || fzequal(v.w));
)

/// @brief Cast a float vector to an integer vector.
_inline_defs_prefix Vector4i Vector4CastInt(Vector4 v)_inline_defs_suffix(
	Vector4i result = { (int32_t)(v.x), (int32_t)(v.y), (int32_t)(v.z), (int32_t)(v.w) };
	return result;
)

/// @brief Convert the vector to an array of floats.
_inline_defs_prefix float4 Vector4Array(Vector4 v)_inline_defs_suffix(
	float4 arr = {{v.x, v.y, v.z, v.w}};
	return arr;
)


//----------------------------------------------------------------------------------
// Vector2i math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
_inline_defs_prefix Vector2i Vector2iZero(void)_inline_defs_suffix(
	Vector2i result = { 0, 0 };
	return result;
)

/// @brief Vector with components value 1.
_inline_defs_prefix Vector2i Vector2iOne(void)_inline_defs_suffix(
	Vector2i result = { 1, 1 };
	return result;
)

/// @brief Add two vectors (v1 + v2).
_inline_defs_prefix Vector2i Vector2iAdd(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	Vector2i result = { v1.x + v2.x, v1.y + v2.y };
	return result;
)

/// @brief Add vector and value.
_inline_defs_prefix Vector2i Vector2iAddValue(Vector2i v, int32_t value)_inline_defs_suffix(
	Vector2i result = { v.x + value, v.y + value };
	return result;
)

/// @brief Subtract two vectors (v1 - v2)
_inline_defs_prefix Vector2i Vector2iSubtract(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	Vector2i result = { v1.x - v2.x, v1.y - v2.y };
	return result;
)

/// @brief Subtract vector by value.
_inline_defs_prefix Vector2i Vector2iSubtractValue(Vector2i v, int32_t value)_inline_defs_suffix(
	Vector2i result = { v.x - value, v.y - value };
	return result;
)

/// @brief Calculate vector length.
_inline_defs_prefix float Vector2iLength(Vector2i v)_inline_defs_suffix(
	float result = sqrtf((float)((v.x*v.x) + (v.y*v.y)));
	return result;
)

/// @brief Calculate vector square length.
_inline_defs_prefix int32_t Vector2iLengthSqr(Vector2i v)_inline_defs_suffix(
	int32_t result = (v.x*v.x) + (v.y*v.y);
	return result;
)

/// @brief Calculate two vectors dot product.
_inline_defs_prefix int32_t Vector2iDotProduct(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	int32_t result = (v1.x*v2.x + v1.y*v2.y);
	return result;
)

/// @brief Calculate distance between two vectors.
_inline_defs_prefix float Vector2iDistance(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	float result = sqrtf((float)((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y)));
	return result;
)

/// @brief Calculate square distance between two vectors.
_inline_defs_prefix int32_t Vector2iDistanceSqr(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	int32_t result = ((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
	return result;
)

/// @brief Calculate angle between two vectors.
/// @brief NOTE: Angle is calculated from origin point (0, 0).
_inline_defs_prefix Angle Vector2iAngle(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	float result = atan2f((float)(v2.y - v1.y), (float)(v2.x - v1.x));
	return RAD2ANGLE(result);
)

/// @brief Multiply vector by vector.
_inline_defs_prefix Vector2i Vector2iMultiply(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	Vector2i result = { v1.x*v2.x, v1.y*v2.y };
	return result;
)

/// @brief Multiply each component by value.
_inline_defs_prefix Vector2i Vector2iMultiplyValue(Vector2i v, int32_t value)_inline_defs_suffix(
	Vector2i result = { v.x*value, v.y*value };
	return result;
)

/// @brief Negate vector.
_inline_defs_prefix Vector2i Vector2iNegate(Vector2i v)_inline_defs_suffix(
	Vector2i result = { -v.x, -v.y };
	return result;
)

/// @brief Divide vector by vector.
_inline_defs_prefix Vector2i Vector2iDivide(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	Vector2i result = { v1.x/v2.x, v1.y/v2.y };
	return result;
)

/// @brief Divide each component by value.
_inline_defs_prefix Vector2i Vector2iDivideValue(Vector2i v, int32_t value)_inline_defs_suffix(
	Vector2i result = { v.x/value, v.y/value };
	return result;
)

/// @brief Calculate reflected vector to normal.
_inline_defs_prefix Vector2i Vector2iReflect(Vector2i v, Vector2i normal)_inline_defs_suffix(
	Vector2i result = { 0 };
	uint32_t dotProduct = (v.x*normal.x + v.y*normal.y);
	result.x = v.x - (2*normal.x)*dotProduct;
	result.y = v.y - (2*normal.y)*dotProduct;
	return result;
)

/// @brief Get min value for each pair of components.
_inline_defs_prefix Vector2i Vector2iMin(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	Vector2i result = { 0 };

	result.x = imin(v1.x, v2.x);
	result.y = imin(v1.y, v2.y);

	return result;
)

/// @brief Get max value for each pair of components.
_inline_defs_prefix Vector2i Vector2iMax(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	Vector2i result = { 0 };

	result.x = imax(v1.x, v2.x);
	result.y = imax(v1.y, v2.y);

	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix int32_t Vector2iMinComponent(Vector2i v)_inline_defs_suffix(
	return imin(v.x, v.y);
)

/// @brief Get the maximum value in the vector.
_inline_defs_prefix int32_t Vector2iMaxComponent(Vector2i v)_inline_defs_suffix(
	return imax(v.x, v.y);
)

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
_inline_defs_prefix Vector2i Vector2iClamp(Vector2i v, Vector2i minv, Vector2i maxv)_inline_defs_suffix(
	Vector2i result = { 0 };
	result.x = imin(maxv.x, imax(minv.x, v.x));
	result.y = imin(maxv.y, imax(minv.y, v.y));
	return result;
)

/// @brief Check whether two given vectors are equal.
_inline_defs_prefix bool Vector2iEquals(Vector2i v1, Vector2i v2)_inline_defs_suffix(
	return (v1.x == v2.x) && (v1.y == v2.y);
)

/// @brief Check whether the vector is all zeros.
_inline_defs_prefix bool Vector2iIsZero(Vector2i v)_inline_defs_suffix(
	return (v.x == 0) && (v.y == 0);
)

/// @brief Check whether any element in the vector is zero.
_inline_defs_prefix bool Vector2iAnyZero(Vector2i v)_inline_defs_suffix(
	return (v.x == 0) || (v.y == 0);
)

/// @brief Cast an integer vector to a float vector.
_inline_defs_prefix Vector2 Vector2iCastFloat(Vector2i v)_inline_defs_suffix(
	Vector2 result = { (float)(v.x), (float)(v.y) };
	return result;
)

/// @brief Convert the vector to an array of integers.
_inline_defs_prefix int2 Vector2iArray(Vector2i v)_inline_defs_suffix(
	int2 arr = {{v.x, v.y}};
	return arr;
)


//----------------------------------------------------------------------------------
// Vector3i math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
_inline_defs_prefix Vector3i Vector3iZero(void)_inline_defs_suffix(
	Vector3i result = { 0, 0, 0 };
	return result;
)

/// @brief Vector with components value 1.
_inline_defs_prefix Vector3i Vector3iOne(void)_inline_defs_suffix(
	Vector3i result = { 1, 1, 1 };
	return result;
)

/// @brief Add two vectors.
_inline_defs_prefix Vector3i Vector3iAdd(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	Vector3i result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return result;
)

/// @brief Add vector and value.
_inline_defs_prefix Vector3i Vector3iAddValue(Vector3i v, int32_t value)_inline_defs_suffix(
	Vector3i result = { v.x + value, v.y + value, v.z + value };
	return result;
)

/// @brief Subtract two vectors.
_inline_defs_prefix Vector3i Vector3iSubtract(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	Vector3i result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return result;
)

/// @brief Subtract vector by value.
_inline_defs_prefix Vector3i Vector3iSubtractValue(Vector3i v, int32_t value)_inline_defs_suffix(
	Vector3i result = { v.x - value, v.y - value, v.z - value };
	return result;
)

/// @brief Multiply vector by vector.
_inline_defs_prefix Vector3i Vector3iMultiply(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	Vector3i result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z };
	return result;
)

/// @brief Multiply each component by value.
_inline_defs_prefix Vector3i Vector3iMultiplyValue(Vector3i v, int32_t value)_inline_defs_suffix(
	Vector3i result = { v.x*value, v.y*value, v.z*value };
	return result;
)

/// @brief Calculate two vectors cross product.
_inline_defs_prefix Vector3i Vector3iCrossProduct(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	Vector3i result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
	return result;
)

/// @brief Calculate vectors perpendicular vector.
_inline_defs_prefix Vector3i Vector3iPerpendicular(Vector3i v)_inline_defs_suffix(
	Vector3i result = { 0 };

	int32_t minv = abs(v.x);
	Vector3i cardinalAxis = { 1, 0, 0 };

	if (abs(v.y) < minv) {
		minv = abs(v.y);
		Vector3i tmp = { 0, 1, 0 };
		cardinalAxis = tmp;
	}

	if (abs(v.z) < minv) {
		Vector3i tmp = { 0, 0, 1 };
		cardinalAxis = tmp;
	}

	// Cross product between vectors
	result.x = v.y*cardinalAxis.z - v.z*cardinalAxis.y;
	result.y = v.z*cardinalAxis.x - v.x*cardinalAxis.z;
	result.z = v.x*cardinalAxis.y - v.y*cardinalAxis.x;

	return result;
)

/// @brief Calculate vector length.
_inline_defs_prefix float Vector3iLength(const Vector3i v)_inline_defs_suffix(
	float result = sqrtf((float)(v.x*v.x + v.y*v.y + v.z*v.z));
	return result;
)

/// @brief Calculate vector square length.
_inline_defs_prefix int32_t Vector3iLengthSqr(const Vector3i v)_inline_defs_suffix(
	int32_t result = v.x*v.x + v.y*v.y + v.z*v.z;
	return result;
)

/// @brief Calculate two vectors dot product.
_inline_defs_prefix int32_t Vector3iDotProduct(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	int32_t result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	return result;
)

/// @brief Calculate distance between two vectors.
_inline_defs_prefix float Vector3iDistance(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	float result = 0.0f;

	int32_t dx = v2.x - v1.x;
	int32_t dy = v2.y - v1.y;
	int32_t dz = v2.z - v1.z;
	result = sqrtf((float)(dx*dx + dy*dy + dz*dz));

	return result;
)

/// @brief Calculate square distance between two vectors.
_inline_defs_prefix int32_t Vector3iDistanceSqr(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	int32_t result = 0;

	int32_t dx = v2.x - v1.x;
	int32_t dy = v2.y - v1.y;
	int32_t dz = v2.z - v1.z;
	result = dx*dx + dy*dy + dz*dz;

	return result;
)

/// @brief Calculate angle between two vectors.
_inline_defs_prefix Angle Vector3iAngle(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	float result = 0.0f;

	Vector3i cross = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
	float len = sqrtf((float)(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z));
	float dot = (float)(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	result = atan2f(len, dot);

	return RAD2ANGLE(result);
)

/// @brief Negate vector.
_inline_defs_prefix Vector3i Vector3iNegate(Vector3i v)_inline_defs_suffix(
	Vector3i result = { -v.x, -v.y, -v.z };
	return result;
)

/// @brief Divide vector by vector.
_inline_defs_prefix Vector3i Vector3iDivide(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	Vector3i result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z };
	return result;
)

/// @brief Divide each component by value.
_inline_defs_prefix Vector3i Vector3iDivideValue(Vector3i v, int32_t value)_inline_defs_suffix(
	Vector3i result = { v.x/value, v.y/value, v.z/value };
	return result;
)

/// @brief Calculate reflected vector to normal.
_inline_defs_prefix Vector3i Vector3iReflect(Vector3i v, Vector3i normal)_inline_defs_suffix(
	Vector3i result = { 0 };

	// I is the original vector
	// N is the normal of the incident plane
	// R = I - (2*N*(DotProduct[I, N]))

	int32_t dotProduct = (v.x*normal.x + v.y*normal.y + v.z*normal.z);

	result.x = v.x - (2*normal.x)*dotProduct;
	result.y = v.y - (2*normal.y)*dotProduct;
	result.z = v.z - (2*normal.z)*dotProduct;

	return result;
)

/// @brief Get min value for each pair of components.
_inline_defs_prefix Vector3i Vector3iMin(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	Vector3i result = { 0 };

	result.x = imin(v1.x, v2.x);
	result.y = imin(v1.y, v2.y);
	result.z = imin(v1.z, v2.z);

	return result;
)

/// @brief Get max value for each pair of components.
_inline_defs_prefix Vector3i Vector3iMax(Vector3i v1, Vector3i v2)_inline_defs_suffix(
	Vector3i result = { 0 };

	result.x = imax(v1.x, v2.x);
	result.y = imax(v1.y, v2.y);
	result.z = imax(v1.z, v2.z);

	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix int32_t Vector3iMinComponent(Vector3i v)_inline_defs_suffix(
	int32_t result = v.x;
	result = imin(result, v.y);
	result = imin(result, v.z);
	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix int32_t Vector3iMaxComponent(Vector3i v)_inline_defs_suffix(
	int32_t result = v.x;
	result = imax(result, v.y);
	result = imax(result, v.z);
	return result;
)

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
_inline_defs_prefix Vector3i Vector3iClamp(Vector3i v, Vector3i minv, Vector3i maxv)_inline_defs_suffix(
	Vector3i result = { 0 };

	result.x = imin(maxv.x, imax(minv.x, v.x));
	result.y = imin(maxv.y, imax(minv.y, v.y));
	result.z = imin(maxv.z, imax(minv.z, v.z));

	return result;
)

/// @brief Check whether two given vectors are equal.
_inline_defs_prefix bool Vector3iEquals(Vector3i v, Vector3i v2)_inline_defs_suffix(
	return (v.x == v2.x) && (v.y == v2.y) && (v.z == v2.z);
)

/// @brief Check whether the vector is all zeros.
_inline_defs_prefix bool Vector3iIsZero(Vector3i v)_inline_defs_suffix(
	return (v.x == 0) && (v.y == 0) && (v.z == 0);
)

/// @brief Check whether any element in the vector is zero.
_inline_defs_prefix bool Vector3iAnyZero(Vector3i v)_inline_defs_suffix(
	return (v.x == 0) || (v.y == 0) || (v.z == 0);
)

/// @brief Cast an integer vector to a float vector.
_inline_defs_prefix Vector3 Vector3iCastFloat(Vector3i v)_inline_defs_suffix(
	Vector3 result = { (float)(v.x), (float)(v.y), (float)(v.z) };
	return result;
)

/// @brief Convert the vector to an array of integers.
_inline_defs_prefix int3 Vector3iArray(Vector3i v)_inline_defs_suffix(
	int3 arr = {{v.x, v.y, v.z}};
	return arr;
)


//----------------------------------------------------------------------------------
// Vector4i math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
_inline_defs_prefix Vector4i Vector4iZero(void)_inline_defs_suffix(
	Vector4i result = { 0, 0, 0, 0 };
	return result;
)

/// @brief Vector with components value 1.
_inline_defs_prefix Vector4i Vector4iOne(void)_inline_defs_suffix(
	Vector4i result = { 1, 1, 1, 1 };
	return result;
)

/// @brief Add two vectors (v1 + v2).
_inline_defs_prefix Vector4i Vector4iAdd(Vector4i v1, Vector4i v2)_inline_defs_suffix(
	Vector4i result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
	return result;
)

/// @brief Add vector and value.
_inline_defs_prefix Vector4i Vector4iAddValue(Vector4i v, int32_t value)_inline_defs_suffix(
	Vector4i result = {v.x + value, v.y + value, v.z + value, v.w + value};
	return result;
)

/// @brief Subtract two Vectors (v1 - v2)
_inline_defs_prefix Vector4i Vector4iSubtract(Vector4i v1, Vector4i v2)_inline_defs_suffix(
	Vector4i result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
	return result;
)

/// @brief Subtract vector and value.
_inline_defs_prefix Vector4i Vector4iSubtractValue(Vector4i v, int32_t value)_inline_defs_suffix(
	Vector4i result = {v.x - value, v.y - value, v.z - value, v.w - value};
	return result;
)

/// @brief Get identity vector.
_inline_defs_prefix Vector4i Vector4iIdentity(void)_inline_defs_suffix(
	Vector4i result = { 0, 0, 0, 1 };
	return result;
)

/// @brief Computes the length of a vector.
_inline_defs_prefix float Vector4iLength(Vector4i v)_inline_defs_suffix(
	float result = sqrtf((float)(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w));
	return result;
)

/// @brief Calculate two vectors multiplication.
_inline_defs_prefix Vector4i Vector4iMultiply(Vector4i v1, Vector4i v2)_inline_defs_suffix(
	Vector4i result = { 0 };

	int32_t qax = v1.x, qay = v1.y, qaz = v1.z, qaw = v1.w;
	int32_t qbx = v2.x, qby = v2.y, qbz = v2.z, qbw = v2.w;

	result.x = qax*qbw + qaw*qbx + qay*qbz - qaz*qby;
	result.y = qay*qbw + qaw*qby + qaz*qbx - qax*qbz;
	result.z = qaz*qbw + qaw*qbz + qax*qby - qay*qbx;
	result.w = qaw*qbw - qax*qbx - qay*qby - qaz*qbz;

	return result;
)

/// @brief Multiply each component by value.
_inline_defs_prefix Vector4i Vector4iMultiplyValue(Vector4i v, int32_t value)_inline_defs_suffix(
	Vector4i result = { v.x*value, v.y*value, v.z*value, v.w*value };
	return result;
)

/// @brief Divide two vectors.
_inline_defs_prefix Vector4i Vector4iDivide(Vector4i v1, Vector4i v2)_inline_defs_suffix(
	Vector4i result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w };
	return result;
)

/// @brief Divide each component by value.
_inline_defs_prefix Vector4i Vector4iDivideValue(Vector4i v, int32_t value)_inline_defs_suffix(
	Vector4i result = { v.x/value, v.y/value, v.z/value, v.w/value };
	return result;
)

/// @brief Negate vector.
_inline_defs_prefix Vector4i Vector4iNegate(Vector4i v)_inline_defs_suffix(
	Vector4i result = { -v.x, -v.y, -v.z, -v.w };
	return result;
)

/// @brief Get min value for each pair of components.
_inline_defs_prefix Vector4i Vector4iMin(Vector4i v1, Vector4i v2)_inline_defs_suffix(
	Vector4i result = { 0 };

	result.x = imin(v1.x, v2.x);
	result.y = imin(v1.y, v2.y);
	result.z = imin(v1.z, v2.z);
	result.w = imin(v1.w, v2.w);

	return result;
)

/// @brief Get max value for each pair of components.
_inline_defs_prefix Vector4i Vector4iMax(Vector4i v1, Vector4i v2)_inline_defs_suffix(
	Vector4i result = { 0 };

	result.x = imax(v1.x, v2.x);
	result.y = imax(v1.y, v2.y);
	result.z = imax(v1.z, v2.z);
	result.w = imax(v1.w, v2.w);

	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix int32_t Vector4iMinComponent(Vector4i v)_inline_defs_suffix(
	int32_t result = v.x;
	result = imin(result, v.y);
	result = imin(result, v.z);
	result = imin(result, v.w);
	return result;
)

/// @brief Get the minimum value in the vector.
_inline_defs_prefix int32_t Vector4iMaxComponent(Vector4i v)_inline_defs_suffix(
	int32_t result = v.x;
	result = imax(result, v.y);
	result = imax(result, v.z);
	result = imax(result, v.w);
	return result;
)

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
_inline_defs_prefix Vector4i Vector4iClamp(Vector4i v, Vector4i minv, Vector4i maxv)_inline_defs_suffix(
	Vector4i result = { 0 };

	result.x = imin(maxv.x, imax(minv.x, v.x));
	result.y = imin(maxv.y, imax(minv.y, v.y));
	result.z = imin(maxv.z, imax(minv.z, v.z));
	result.w = imin(maxv.w, imax(minv.w, v.w));

	return result;
)

/// @brief Check whether two given vectors are equal.
_inline_defs_prefix bool Vector4iEquals(Vector4i v1, Vector4i v2)_inline_defs_suffix(
	return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w);
)

/// @brief Check whether the vector is all zeros.
_inline_defs_prefix bool Vector4iIsZero(Vector4i v)_inline_defs_suffix(
	return (v.x == 0) && (v.y == 0) && (v.z == 0) && (v.w == 0);
)

/// @brief Check whether any element in the vector is zero.
_inline_defs_prefix bool Vector4iAnyZero(Vector4i v)_inline_defs_suffix(
	return (v.x == 0) || (v.y == 0) || (v.z == 0) || (v.w == 0);
)

/// @brief Cast an integer vector to a float vector.
_inline_defs_prefix Vector4 Vector4iCastFloat(Vector4i v)_inline_defs_suffix(
	Vector4 result = { (float)(v.x), (float)(v.y), (float)(v.z), (float)(v.w) };
	return result;
)

/// @brief Convert the vector to an array of integers.
_inline_defs_prefix int4 Vector4iArray(Vector4i v)_inline_defs_suffix(
	int4 arr = {{v.x, v.y, v.z, v.w}};
	return arr;
)


//----------------------------------------------------------------------------------
// Quaternion math
//----------------------------------------------------------------------------------

/// @brief Get the identity quaternion.
_inline_defs_prefix Quaternion QuaternionIdentity()_inline_defs_suffix(
	Quaternion result = { 1.f, 0.f, 0.f, 0.f };
	return result;
)

/// @brief Check if a quaternion is the identity quaternion.
_inline_defs_prefix bool QuaternionIsIdentity(Quaternion q)_inline_defs_suffix(
	return (fequal(q.q_0, 1.f) && fzequal(q.q_1) && fzequal(q.q_2) && fzequal(q.q_3));
)

/// @brief Invert the quaternion.
_inline_defs_prefix Quaternion QuaternionInvert(Quaternion q)_inline_defs_suffix(
	Quaternion result = { q.q_0, -q.q_1, -q.q_2, -q.q_3 };
	return result;
)

/// @brief Multiply two quaternions.
_inline_defs_prefix Quaternion QuaternionMultiply(Quaternion q1, Quaternion q2)_inline_defs_suffix(
	Quaternion result = { 0 };

	result.q_0 = (q1.q_0 * q2.q_0) - (q1.q_1 * q2.q_1) - (q1.q_2 * q2.q_2) - (q1.q_3 * q2.q_3);
	result.q_1 = (q1.q_0 * q2.q_1) + (q1.q_1 * q2.q_0) - (q1.q_2 * q2.q_3) + (q1.q_3 * q2.q_2);
	result.q_2 = (q1.q_0 * q2.q_2) + (q1.q_1 * q2.q_3) + (q1.q_2 * q2.q_0) - (q1.q_3 * q2.q_1);
	result.q_3 = (q1.q_0 * q2.q_3) - (q1.q_1 * q2.q_2) + (q1.q_2 * q2.q_1) + (q1.q_3 * q2.q_0);

	return result;
)

/// @brief Convert the quaternion to an array of floats.
_inline_defs_prefix float4 QuaternionArray(Quaternion q)_inline_defs_suffix(
	float4 arr = {{q.q_0, q.q_1, q.q_2, q.q_3}};
	return arr;
)

//----------------------------------------------------------------------------------
// Matrix2 math
//----------------------------------------------------------------------------------

/// @brief Create a 2x2 identity matrix.
_inline_defs_prefix Matrix2 Matrix2Identity()_inline_defs_suffix(
	Matrix2 m = { 1.f, 0.f, 0.f, 1.f };
	return m;
)

/// @brief Check if the matrix is the identity matrix.
_inline_defs_prefix bool Matrix2IsIdentity(Matrix2 m)_inline_defs_suffix(
	return (
		fequal(m.m_1_1, 1.f) && fzequal(m.m_1_2) && 
		fzequal(m.m_2_1)     && fequal(m.m_2_2, 1.f)
	);
)

/// @brief Check if the two matrices are equal.
_inline_defs_prefix bool Matrix2Equals(Matrix2 m1, Matrix2 m2)_inline_defs_suffix(
	return (
		fequal(m1.m_1_1, m2.m_1_1) && fequal(m1.m_1_2, m2.m_1_2) &&
		fequal(m1.m_2_1, m2.m_2_1) && fequal(m1.m_2_2, m2.m_2_2)
	);
)

/// @brief Add the two matrices component-wise.
_inline_defs_prefix Matrix2 Matrix2Add(Matrix2 m1, Matrix2 m2)_inline_defs_suffix(
	Matrix2 result = {
		m1.m_1_1 + m2.m_1_1, m1.m_1_2 + m2.m_1_2,
		m1.m_2_1 + m2.m_2_1, m1.m_2_2 + m2.m_2_2
	};
	return result;
)

/// @brief Subtract the two matrices component-wise.
_inline_defs_prefix Matrix2 Matrix2Subtract(Matrix2 m1, Matrix2 m2)_inline_defs_suffix(
	Matrix2 result = {
		m1.m_1_1 - m2.m_1_1, m1.m_1_2 - m2.m_1_2,
		m1.m_2_1 - m2.m_2_1, m1.m_2_2 - m2.m_2_2
	};
	return result;
)

/// @brief Scale every component of the matrix.
_inline_defs_prefix Matrix2 Matrix2Scale(Matrix2 m, float s)_inline_defs_suffix(
	Matrix2 result = {
		m.m_1_1 * s, m.m_1_2 * s,
		m.m_2_1 * s, m.m_2_2 * s
	};
	return result;
)

/// @brief Multiply the two matrices.
_inline_defs_prefix Matrix2 Matrix2Multiply(Matrix2 m1, Matrix2 m2)_inline_defs_suffix(
	Matrix2 result = {
		(m1.m_1_1*m2.m_1_1)+(m1.m_1_2*m2.m_2_1), 
		(m1.m_1_1*m2.m_1_2)+(m1.m_1_2*m2.m_2_2),
		(m1.m_2_1*m2.m_1_1)+(m1.m_2_2*m2.m_2_1), 
		(m1.m_2_1*m2.m_1_2)+(m1.m_2_2*m2.m_2_2)
	};
	return result;
)

/// @brief Multiply the matrix by a vector.
_inline_defs_prefix Vector2 Matrix2VecMultiply(Matrix2 m, Vector2 v)_inline_defs_suffix(
	Vector2 result = {
		(m.m_1_1*v.x)+(m.m_1_2*v.y),
		(m.m_2_1*v.x)+(m.m_2_2*v.y)
	};
	return result;
)

/// @brief Convert the matrix to an array of floats.
_inline_defs_prefix float4 Matrix2Array(Matrix2 m)_inline_defs_suffix(
	float4 result = {
		m.m_1_1, m.m_1_2,
		m.m_2_1, m.m_2_2
	};
	return result;
)

/// @brief Construct the transposition of the matrix.
_inline_defs_prefix Matrix2 Matrix2Transpose(Matrix2 m)_inline_defs_suffix(
	Matrix2 result = {
		m.m_1_1, m.m_2_1,
		m.m_1_2, m.m_2_2
	};
	return result;
)

/// @brief Compute the determinant of the matrix.
_inline_defs_prefix float Matrix2Determinant(Matrix2 m)_inline_defs_suffix(
	return (m.m_1_1 * m.m_2_2) - (m.m_1_2 * m.m_2_1);
)


//----------------------------------------------------------------------------------
// Matrix3 math
//----------------------------------------------------------------------------------

/// @brief Create a 3x3 identity matrix.
_inline_defs_prefix Matrix3 Matrix3Identity()_inline_defs_suffix(
	Matrix3 m = { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
	return m;
)

/// @brief Check if the matrix is the identity matrix.
_inline_defs_prefix bool Matrix3IsIdentity(Matrix3 m)_inline_defs_suffix(
	return (
		fequal(m.m_1_1, 1.f) && fzequal(m.m_1_2)     && fzequal(m.m_1_3) && 
		fzequal(m.m_2_1)     && fequal(m.m_2_2, 1.f) && fzequal(m.m_2_3) &&
		fzequal(m.m_3_1)     && fzequal(m.m_3_2)     && fequal(m.m_3_3, 1.f)
	);
)

/// @brief Check if the two matrices are equal.
_inline_defs_prefix bool Matrix3Equals(Matrix3 m1, Matrix3 m2)_inline_defs_suffix(
	return (
		fequal(m1.m_1_1, m2.m_1_1) && fequal(m1.m_1_2, m2.m_1_2) && fequal(m1.m_1_3, m2.m_1_3) &&
		fequal(m1.m_2_1, m2.m_2_1) && fequal(m1.m_2_2, m2.m_2_2) && fequal(m1.m_2_3, m2.m_2_3) &&
		fequal(m1.m_3_1, m2.m_3_1) && fequal(m1.m_3_2, m2.m_3_2) && fequal(m1.m_3_3, m2.m_3_3)
	);
)

/// @brief Add the two matrices component-wise.
_inline_defs_prefix Matrix3 Matrix3Add(Matrix3 m1, Matrix3 m2)_inline_defs_suffix(
	Matrix3 result = {
		m1.m_1_1 + m2.m_1_1, m1.m_1_2 + m2.m_1_2, m1.m_1_3 + m2.m_1_3,
		m1.m_2_1 + m2.m_2_1, m1.m_2_2 + m2.m_2_2, m1.m_2_3 + m2.m_2_3,
		m1.m_3_1 + m2.m_3_1, m1.m_3_2 + m2.m_3_2, m1.m_3_3 + m2.m_3_3
	};
	return result;
)

/// @brief Subtract the two matrices component-wise.
_inline_defs_prefix Matrix3 Matrix3Subtract(Matrix3 m1, Matrix3 m2)_inline_defs_suffix(
	Matrix3 result = {
		m1.m_1_1 - m2.m_1_1, m1.m_1_2 - m2.m_1_2, m1.m_1_3 - m2.m_1_3,
		m1.m_2_1 - m2.m_2_1, m1.m_2_2 - m2.m_2_2, m1.m_2_3 - m2.m_2_3,
		m1.m_3_1 - m2.m_3_1, m1.m_3_2 - m2.m_3_2, m1.m_3_3 - m2.m_3_3
	};
	return result;
)

/// @brief Scale every component of the matrix.
_inline_defs_prefix Matrix3 Matrix3Scale(Matrix3 m, float s)_inline_defs_suffix(
	Matrix3 result = {
		m.m_1_1 * s, m.m_1_2 * s, m.m_1_3 * s,
		m.m_2_1 * s, m.m_2_2 * s, m.m_2_3 * s,
		m.m_3_1 * s, m.m_3_2 * s, m.m_3_3 * s
	};
	return result;
)

/// @brief Multiply the two matrices.
_inline_defs_prefix Matrix3 Matrix3Multiply(Matrix3 m1, Matrix3 m2)_inline_defs_suffix(
	Matrix3 result = {
		(m1.m_1_1*m2.m_1_1)+(m1.m_1_2*m2.m_2_1)+(m1.m_1_3*m2.m_3_1), 
		(m1.m_1_1*m2.m_1_2)+(m1.m_1_2*m2.m_2_2)+(m1.m_1_3*m2.m_3_2), 
		(m1.m_1_1*m2.m_1_3)+(m1.m_1_2*m2.m_2_3)+(m1.m_1_3*m2.m_3_3),
		(m1.m_2_1*m2.m_1_1)+(m1.m_2_2*m2.m_2_1)+(m1.m_2_3*m2.m_3_1), 
		(m1.m_2_1*m2.m_1_2)+(m1.m_2_2*m2.m_2_2)+(m1.m_2_3*m2.m_3_2), 
		(m1.m_2_1*m2.m_1_3)+(m1.m_2_2*m2.m_2_3)+(m1.m_2_3*m2.m_3_3),
		(m1.m_3_1*m2.m_1_1)+(m1.m_3_2*m2.m_2_1)+(m1.m_3_3*m2.m_3_1), 
		(m1.m_3_1*m2.m_1_2)+(m1.m_3_2*m2.m_2_2)+(m1.m_3_3*m2.m_3_2), 
		(m1.m_3_1*m2.m_1_3)+(m1.m_3_2*m2.m_2_3)+(m1.m_3_3*m2.m_3_3)
	};
	return result;
)

/// @brief Multiply the matrix by a vector.
_inline_defs_prefix Vector3 Matrix3VecMultiply(Matrix3 m, Vector3 v)_inline_defs_suffix(
	Vector3 result = {
		(m.m_1_1*v.x)+(m.m_1_2*v.y)+(m.m_1_3*v.z),
		(m.m_2_1*v.x)+(m.m_2_2*v.y)+(m.m_2_3*v.z),
		(m.m_3_1*v.x)+(m.m_3_2*v.y)+(m.m_3_3*v.z)
	};
	return result;
)

/// @brief Construct the transposition of the matrix.
_inline_defs_prefix Matrix3 Matrix3Transpose(Matrix3 m)_inline_defs_suffix(
	Matrix3 result = {
		m.m_1_1, m.m_2_1, m.m_3_1,
		m.m_1_2, m.m_2_2, m.m_3_2,
		m.m_1_3, m.m_2_3, m.m_3_3
	};
	return result;
)

/// @brief Convert the matrix to an array of floats.
_inline_defs_prefix float9 Matrix3Array(Matrix3 m)_inline_defs_suffix(
	float9 result = {
		m.m_1_1, m.m_1_2, m.m_1_3, 
		m.m_2_1, m.m_2_2, m.m_2_3, 
		m.m_3_1, m.m_3_2, m.m_3_3
	};
	return result;
)

//----------------------------------------------------------------------------------
// Matrix4 math
//----------------------------------------------------------------------------------

/// @brief Create a 4x4 identity matrix.
_inline_defs_prefix Matrix4 Matrix4Identity()_inline_defs_suffix(
	Matrix4 m = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
	return m;
)

/// @brief Check if the matrix is the identity matrix.
_inline_defs_prefix bool Matrix4IsIdentity(Matrix4 m)_inline_defs_suffix(
	return (
		fequal(m.m_1_1, 1.f) && fzequal(m.m_1_2)     && fzequal(m.m_1_3)     && fzequal(m.m_1_4) && 
		fzequal(m.m_2_1)     && fequal(m.m_2_2, 1.f) && fzequal(m.m_2_3)     && fzequal(m.m_2_4) && 
		fzequal(m.m_3_1)     && fzequal(m.m_3_2)     && fequal(m.m_3_3, 1.f) && fzequal(m.m_3_4) && 
		fzequal(m.m_4_1)     && fzequal(m.m_4_2)     && fzequal(m.m_4_3)     && fequal(m.m_4_4, 1.f)
	);
)

/// @brief Check if the two matrices are equal.
_inline_defs_prefix bool Matrix4Equals(Matrix4 m1, Matrix4 m2)_inline_defs_suffix(
	return (
		fequal(m1.m_1_1, m2.m_1_1) && fequal(m1.m_1_2, m2.m_1_2) && fequal(m1.m_1_3, m2.m_1_3) && fequal(m1.m_1_4, m2.m_1_4) &&
		fequal(m1.m_2_1, m2.m_2_1) && fequal(m1.m_2_2, m2.m_2_2) && fequal(m1.m_2_3, m2.m_2_3) && fequal(m1.m_2_4, m2.m_2_4) &&
		fequal(m1.m_3_1, m2.m_3_1) && fequal(m1.m_3_2, m2.m_3_2) && fequal(m1.m_3_3, m2.m_3_3) && fequal(m1.m_3_4, m2.m_3_4) &&
		fequal(m1.m_4_1, m2.m_4_1) && fequal(m1.m_4_2, m2.m_4_2) && fequal(m1.m_4_3, m2.m_4_3) && fequal(m1.m_4_4, m2.m_4_4)
	);
)

/// @brief Add the two matrices component-wise.
_inline_defs_prefix Matrix4 Matrix4Add(Matrix4 m1, Matrix4 m2)_inline_defs_suffix(
	Matrix4 result = {
		m1.m_1_1 + m2.m_1_1, m1.m_1_2 + m2.m_1_2, m1.m_1_3 + m2.m_1_3, m1.m_1_4 + m2.m_1_4,
		m1.m_2_1 + m2.m_2_1, m1.m_2_2 + m2.m_2_2, m1.m_2_3 + m2.m_2_3, m1.m_2_4 + m2.m_2_4,
		m1.m_3_1 + m2.m_3_1, m1.m_3_2 + m2.m_3_2, m1.m_3_3 + m2.m_3_3, m1.m_3_4 + m2.m_3_4,
		m1.m_4_1 + m2.m_4_1, m1.m_4_2 + m2.m_4_2, m1.m_4_3 + m2.m_4_3, m1.m_4_4 + m2.m_4_4
	};
	return result;
)

/// @brief Subtract the two matrices component-wise.
_inline_defs_prefix Matrix4 Matrix4Subtract(Matrix4 m1, Matrix4 m2)_inline_defs_suffix(
	Matrix4 result = {
		m1.m_1_1 - m2.m_1_1, m1.m_1_2 - m2.m_1_2, m1.m_1_3 - m2.m_1_3, m1.m_1_4 - m2.m_1_4,
		m1.m_2_1 - m2.m_2_1, m1.m_2_2 - m2.m_2_2, m1.m_2_3 - m2.m_2_3, m1.m_2_4 - m2.m_2_4,
		m1.m_3_1 - m2.m_3_1, m1.m_3_2 - m2.m_3_2, m1.m_3_3 - m2.m_3_3, m1.m_3_4 - m2.m_3_4,
		m1.m_4_1 - m2.m_4_1, m1.m_4_2 - m2.m_4_2, m1.m_4_3 - m2.m_4_3, m1.m_4_4 - m2.m_4_4
	};
	return result;
)

/// @brief Scale every component of the matrix.
_inline_defs_prefix Matrix4 Matrix4Scale(Matrix4 m, float s)_inline_defs_suffix(
	Matrix4 result = {
		m.m_1_1 * s, m.m_1_2 * s, m.m_1_3 * s, m.m_1_4 * s,
		m.m_2_1 * s, m.m_2_2 * s, m.m_2_3 * s, m.m_2_4 * s,
		m.m_3_1 * s, m.m_3_2 * s, m.m_3_3 * s, m.m_3_4 * s,
		m.m_4_1 * s, m.m_4_2 * s, m.m_4_3 * s, m.m_4_4 * s
	};
	return result;
)

/// @brief Multiply the two matrices.
_inline_defs_prefix Matrix4 Matrix4Multiply(Matrix4 m1, Matrix4 m2)_inline_defs_suffix(
	Matrix4 result = {
		(m1.m_1_1*m2.m_1_1)+(m1.m_1_2*m2.m_2_1)+(m1.m_1_3*m2.m_3_1)+(m1.m_1_4*m2.m_4_1), 
		(m1.m_1_1*m2.m_1_2)+(m1.m_1_2*m2.m_2_2)+(m1.m_1_3*m2.m_3_2)+(m1.m_1_4*m2.m_4_2), 
		(m1.m_1_1*m2.m_1_3)+(m1.m_1_2*m2.m_2_3)+(m1.m_1_3*m2.m_3_3)+(m1.m_1_4*m2.m_4_3), 
		(m1.m_1_1*m2.m_1_4)+(m1.m_1_2*m2.m_2_4)+(m1.m_1_3*m2.m_3_4)+(m1.m_1_4*m2.m_4_4),
		(m1.m_2_1*m2.m_1_1)+(m1.m_2_2*m2.m_2_1)+(m1.m_2_3*m2.m_3_1)+(m1.m_2_4*m2.m_4_1), 
		(m1.m_2_1*m2.m_1_2)+(m1.m_2_2*m2.m_2_2)+(m1.m_2_3*m2.m_3_2)+(m1.m_2_4*m2.m_4_2), 
		(m1.m_2_1*m2.m_1_3)+(m1.m_2_2*m2.m_2_3)+(m1.m_2_3*m2.m_3_3)+(m1.m_2_4*m2.m_4_3), 
		(m1.m_2_1*m2.m_1_4)+(m1.m_2_2*m2.m_2_4)+(m1.m_2_3*m2.m_3_4)+(m1.m_2_4*m2.m_4_4),
		(m1.m_3_1*m2.m_1_1)+(m1.m_3_2*m2.m_2_1)+(m1.m_3_3*m2.m_3_1)+(m1.m_3_4*m2.m_4_1), 
		(m1.m_3_1*m2.m_1_2)+(m1.m_3_2*m2.m_2_2)+(m1.m_3_3*m2.m_3_2)+(m1.m_3_4*m2.m_4_2), 
		(m1.m_3_1*m2.m_1_3)+(m1.m_3_2*m2.m_2_3)+(m1.m_3_3*m2.m_3_3)+(m1.m_3_4*m2.m_4_3), 
		(m1.m_3_1*m2.m_1_4)+(m1.m_3_2*m2.m_2_4)+(m1.m_3_3*m2.m_3_4)+(m1.m_3_4*m2.m_4_4),
		(m1.m_4_1*m2.m_1_1)+(m1.m_4_2*m2.m_2_1)+(m1.m_4_3*m2.m_3_1)+(m1.m_4_4*m2.m_4_1), 
		(m1.m_4_1*m2.m_1_2)+(m1.m_4_2*m2.m_2_2)+(m1.m_4_3*m2.m_3_2)+(m1.m_4_4*m2.m_4_2), 
		(m1.m_4_1*m2.m_1_3)+(m1.m_4_2*m2.m_2_3)+(m1.m_4_3*m2.m_3_3)+(m1.m_4_4*m2.m_4_3), 
		(m1.m_4_1*m2.m_1_4)+(m1.m_4_2*m2.m_2_4)+(m1.m_4_3*m2.m_3_4)+(m1.m_4_4*m2.m_4_4)
	};
	return result;
)

/// @brief Multiply the matrix by a vector.
_inline_defs_prefix Vector4 Matrix4VecMultiply(Matrix4 m, Vector4 v)_inline_defs_suffix(
	Vector4 result = {
		(m.m_1_1*v.x)+(m.m_1_2*v.y)+(m.m_1_3*v.z)+(m.m_1_4*v.w),
		(m.m_2_1*v.x)+(m.m_2_2*v.y)+(m.m_2_3*v.z)+(m.m_2_4*v.w),
		(m.m_3_1*v.x)+(m.m_3_2*v.y)+(m.m_3_3*v.z)+(m.m_3_4*v.w),
		(m.m_4_1*v.x)+(m.m_4_2*v.y)+(m.m_4_3*v.z)+(m.m_4_4*v.w)
	};
	return result;
)

/// @brief Construct the transposition of the matrix.
_inline_defs_prefix Matrix4 Matrix4Transpose(Matrix4 m)_inline_defs_suffix(
	Matrix4 result = {
		m.m_1_1, m.m_2_1, m.m_3_1, m.m_4_1,
		m.m_1_2, m.m_2_2, m.m_3_2, m.m_4_2,
		m.m_1_3, m.m_2_3, m.m_3_3, m.m_4_3,
		m.m_1_4, m.m_2_4, m.m_3_4, m.m_4_4
	};
	return result;
)

/// @brief Convert the matrix to an array of floats.
_inline_defs_prefix float16 Matrix4Array(Matrix4 m)_inline_defs_suffix(
	float16 result = {
		m.m_1_1, m.m_1_2, m.m_1_3, m.m_1_4,
		m.m_2_1, m.m_2_2, m.m_2_3, m.m_2_4,
		m.m_3_1, m.m_3_2, m.m_3_3, m.m_3_4,
		m.m_4_1, m.m_4_2, m.m_4_3, m.m_4_4
	};
	return result;
)