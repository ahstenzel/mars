#ifndef MARS_STD_UTILITIES_H
#define MARS_STD_UTILITIES_H
/**
 * utilities.h
 * Useful global utility functions.
*/
#include "mars/std/platform.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

//----------------------------------------------------------------------------------
// Memory allocations
//----------------------------------------------------------------------------------

#ifndef MARS_MALLOC
#define MARS_MALLOC malloc
#endif

#ifndef MARS_CALLOC
#define MARS_CALLOC calloc
#endif

#ifndef MARS_REALLOC
#define MARS_REALLOC realloc
#endif

#ifndef MARS_FREE
#define MARS_FREE free
#endif

//----------------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------------

#ifndef PI
    #define PI 3.14159265358979323846f
#endif

#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif

#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif


//----------------------------------------------------------------------------------
// Functional macros
//----------------------------------------------------------------------------------

/// @brief Get the number of elements in the buffer.
#define MARS_BUFF_LEN(x) (sizeof(x)/sizeof(x[0]))

/// @brief Get the next power of 2 >= x.
#define MARS_NEXT_POW2(x) (1ULL << (unsigned long long)(log2((double)x-1.)+1.))


//----------------------------------------------------------------------------------
// Unique ID generation
//----------------------------------------------------------------------------------

// Unique ID generation
#define _MARS_ID_TYPE unsigned int	// Base type for all IDs.
#define ID_NULL 0				    // Signifier for an invalid ID.

/// @brief Generate a unique ID vaule.
/// @return ID
_MARS_ID_TYPE _mars_id_generate();


//----------------------------------------------------------------------------------
// BAMS functions
//----------------------------------------------------------------------------------

// Typedefs
typedef uint16_t Angle;	// Angle units stored as BAM (Binary angular measurement) integers.

#define ANGLE_MIN		0x0000	// No turns
#define ANGLE_MAX		0xFFFF	// Full turn
#define ANGLE_HALF		0x7FFF	// Half turn

/// @brief Convert the angles value (BAM integer) to degrees (float).
#define ANGLE2DEG(a) ((float)(a) * 360.f / (float)(ANGLE_MAX))

/// @brief Convert the degrees value (float) to angles (BAM integer).
#define DEG2ANGLE(a) ((Angle)(a * (float)(ANGLE_MAX) / 360.f))

/// @brief Convert the angles value (BAM integer) to radians (float).
#define ANGLE2RAD(a) ((float)(a) * (2.f * PI) / (float)(ANGLE_MAX))

/// @brief Convert the radians value (float) to angles (BAM integer).
#define RAD2ANGLE(a) ((Angle)(a * (float)(ANGLE_MAX) / (2.f * PI)))


//----------------------------------------------------------------------------------
// Math functions
//----------------------------------------------------------------------------------

MARS_API inline int32_t imin(int32_t x, int32_t y) { return (x < y) ? x : y; }

MARS_API inline int32_t imax(int32_t x, int32_t y) { return (x > y) ? x : y; }

MARS_API inline size_t umin(size_t x, size_t y) { return (x < y) ? x : y; }

MARS_API inline size_t umax(size_t x, size_t y) { return (x > y) ? x : y; }


//----------------------------------------------------------------------------------
// Floating point comparison
//----------------------------------------------------------------------------------

#ifndef MARS_FEQUAL_MAX_DIFF
#define MARS_FEQUAL_MAX_DIFF FLT_EPSILON
#endif

#ifndef MARS_FEQUAL_USE_RELATIVE_EPSILON
	#ifndef MARS_FEQUAL_MAX_ULPS
	#define MARS_FEQUAL_MAX_ULPS 2
	#endif
	typedef union {
		float f;
		int32_t i;
	} _Float_Cast;
#else 
	#ifndef MARS_FEQUAL_MAX_RELDIFF
	#define MARS_FEQUAL_MAX_RELDIFF FLT_EPSILON
	#endif
#endif // MARS_FEQUAL_USE_RELATIVE_EPSILON

/// @brief Check if the two floating point numbers are nearly equal.
MARS_API bool fequal(float a, float b);

/// @brief Check if a floating point number is nearly equal to zero.
#define fzequal(a) fequal(a, 0.f)


//----------------------------------------------------------------------------------
// Float vectors
//----------------------------------------------------------------------------------

/// @brief 2D vector of floats.
typedef struct {
	float x;
	float y;
} Vector2;

/// @brief 3D vector of floats.
typedef struct {
	float x;
	float y;
	float z;
} Vector3;

/// @brief 4D vector of floats.
typedef struct {
	float x;
	float y;
	float z;
	float w;
} Vector4;

typedef struct {
	float v[2];
} float2;

typedef struct {
	float v[3];
} float3;

typedef struct {
	float v[4];
} float4;


//----------------------------------------------------------------------------------
// Integer vectors
//----------------------------------------------------------------------------------

/// @brief 2D vector of integers.
typedef struct {
	int32_t x;
	int32_t y;
} Vector2i;

/// @brief 3D vector of integers.
typedef struct {
	int32_t x;
	int32_t y;
	int32_t z;
} Vector3i;

/// @brief 4D vector of integers.
typedef struct {
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t w;
} Vector4i;

typedef struct {
	int32_t v[2];
} int2;

typedef struct {
	int32_t v[3];
} int3;

typedef struct {
	int32_t v[4];
} int4;

//----------------------------------------------------------------------------------
// Quaternions
//----------------------------------------------------------------------------------

/// @brief Rotation quaternion for 3D space.
typedef struct {
	float q_0;
	float q_1;
	float q_2;
	float q_3;
} Quaternion;


//----------------------------------------------------------------------------------
// Matrices
//----------------------------------------------------------------------------------

/// @brief 2x2 matrix (elements stored as row_column).
typedef struct {
	float m_1_1;
	float m_1_2;
	float m_2_1;
	float m_2_2;
} Matrix2;

/// @brief 3x3 matrix (elements stored as row_column).
typedef struct {
	float m_1_1;
	float m_1_2;
	float m_1_3;
	float m_2_1;
	float m_2_2;
	float m_2_3;
	float m_3_1;
	float m_3_2;
	float m_3_3;
} Matrix3;

/// @brief 4x4 matrix (elements stored as row_column).
typedef struct {
	float m_1_1;
	float m_1_2;
	float m_1_3;
	float m_1_4;
	float m_2_1;
	float m_2_2;
	float m_2_3;
	float m_2_4;
	float m_3_1;
	float m_3_2;
	float m_3_3;
	float m_3_4;
	float m_4_1;
	float m_4_2;
	float m_4_3;
	float m_4_4;
} Matrix4;

typedef struct {
	float v[9];
} float9;

typedef struct {
	float v[16];
} float16;

//----------------------------------------------------------------------------------
// Vector2 math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
MARS_API inline Vector2 Vector2Zero(void) {
	Vector2 result = { 0.f, 0.f };
	return result;
}

/// @brief Vector with components value 1.
MARS_API inline Vector2 Vector2One(void) {
	Vector2 result = { 1.f, 1.f };
	return result;
}

/// @brief Add two vectors (v1 + v2).
MARS_API inline Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
	Vector2 result = { v1.x + v2.x, v1.y + v2.y };
	return result;
}

/// @brief Add vector and value.
MARS_API inline Vector2 Vector2AddValue(Vector2 v, float value) {
	Vector2 result = { v.x + value, v.y + value };
	return result;
}

/// @brief Subtract two vectors (v1 - v2).
MARS_API inline Vector2 Vector2Subtract(Vector2 v1, Vector2 v2) {
	Vector2 result = { v1.x - v2.x, v1.y - v2.y };
	return result;
}

/// @brief Subtract vector by value.
MARS_API inline Vector2 Vector2SubtractValue(Vector2 v, float value) {
	Vector2 result = { v.x - value, v.y - value };
	return result;
}

/// @brief Calculate vector length.
MARS_API inline float Vector2Length(Vector2 v) {
	float result = sqrtf((v.x*v.x) + (v.y*v.y));
	return result;
}

/// @brief Calculate vector square length.
MARS_API inline float Vector2LengthSqr(Vector2 v) {
	float result = (v.x*v.x) + (v.y*v.y);
	return result;
}

/// @brief Calculate two vectors dot product.
MARS_API inline float Vector2DotProduct(Vector2 v1, Vector2 v2) {
	float result = (v1.x*v2.x + v1.y*v2.y);
	return result;
}

/// @brief Calculate distance between two vectors.
MARS_API inline float Vector2Distance(Vector2 v1, Vector2 v2) {
	float result = sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
	return result;
}

/// @brief Calculate square distance between two vectors.
MARS_API inline float Vector2DistanceSqr(Vector2 v1, Vector2 v2) {
	float result = ((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
	return result;
}

/// @brief Calculate angle between two vectors (BAM integers).
/// @brief NOTE: Angle is calculated from origin point (0, 0).
MARS_API inline Angle Vector2Angle(Vector2 v1, Vector2 v2) {
	float result = atan2f((v2.y - v1.y), (v2.x - v1.x));
	return RAD2ANGLE(result);
}

/// @brief Multiply vector by vector.
MARS_API inline Vector2 Vector2Multiply(Vector2 v1, Vector2 v2) {
	Vector2 result = { v1.x*v2.x, v1.y*v2.y };
	return result;
}

/// @brief Multiply each component by value.
MARS_API inline Vector2 Vector2MultiplyValue(Vector2 v, float value) {
	Vector2 result = { v.x*value, v.y*value };
	return result;
}

/// @brief Negate vector.
MARS_API inline Vector2 Vector2Negate(Vector2 v) {
	Vector2 result = { -v.x, -v.y };
	return result;
}

/// @brief Divide vector by vector.
MARS_API inline Vector2 Vector2Divide(Vector2 v1, Vector2 v2) {
	Vector2 result = { v1.x/v2.x, v1.y/v2.y };
	return result;
}

/// @brief Divide each component by value.
MARS_API inline Vector2 Vector2DivideValue(Vector2 v, float value) {
	Vector2 result = { v.x/value, v.y/value };
	return result;
}

/// @brief Calculate reflected vector to normal.
MARS_API inline Vector2 Vector2Reflect(Vector2 v, Vector2 normal) {
	Vector2 result = { 0 };
	float dotProduct = (v.x*normal.x + v.y*normal.y);
	result.x = v.x - (2*normal.x)*dotProduct;
	result.y = v.y - (2*normal.y)*dotProduct;
	return result;
}

/// @brief Normalize vector.
MARS_API inline Vector2 Vector2Normalize(Vector2 v) {
	float len = Vector2Length(v);
	Vector2 result = { v.x / len, v.y / len };
	return result;
}

/// @brief Get min value for each pair of components.
MARS_API inline Vector2 Vector2Min(Vector2 v1, Vector2 v2) {
	Vector2 result = { 0 };

	result.x = fminf(v1.x, v2.x);
	result.y = fminf(v1.y, v2.y);

	return result;
}

/// @brief Get max value for each pair of components.
MARS_API inline Vector2 Vector2Max(Vector2 v1, Vector2 v2) {
	Vector2 result = { 0 };

	result.x = fmaxf(v1.x, v2.x);
	result.y = fmaxf(v1.y, v2.y);

	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline float Vector2MinComponent(Vector2 v) {
	return fminf(v.x, v.y);
}

/// @brief Get the maximum value in the vector.
MARS_API inline float Vector2MaxComponent(Vector2 v) {
	return fmaxf(v.x, v.y);
}

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
MARS_API inline Vector2 Vector2Clamp(Vector2 v, Vector2 min, Vector2 max) {
	Vector2 result = { 0 };
	result.x = fminf(max.x, fmaxf(min.x, v.x));
	result.y = fminf(max.y, fmaxf(min.y, v.y));
	return result;
}

/// @brief Check whether two given vectors are almost equal.
MARS_API inline bool Vector2Equals(Vector2 v1, Vector2 v2) {
	return (fequal(v1.x, v2.x) && fequal(v1.y, v2.y));
}

/// @brief Check whether the vector is all zeros.
MARS_API inline bool Vector2IsZero(Vector2 v) {
	return (fzequal(v.x) && fzequal(v.y));
}

/// @brief Check whether any element in the vector is zero.
MARS_API inline bool Vector2AnyZero(Vector2 v) {
	return (fzequal(v.x) || fzequal(v.y));
}

/// @brief Cast a float vector to an integer vector.
MARS_API inline Vector2i Vector2CastInt(Vector2 v) {
	Vector2i result = { (int32_t)(v.x), (int32_t)(v.y) };
	return result;
}

/// @brief Rotate the point v1 around the point v2 by the given angle.
MARS_API inline Vector2 Vector2RotatePoint(Vector2 v1, Vector2 v2, Angle angle) {
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
}

/// @brief Reflect the vector v1 across v2, along the line between them.
MARS_API inline Vector2 Vector2ReflectPoint(Vector2 v1, Vector2 v2) {
	Vector2 result = { 0 };

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.x = -result.x;
	result.y = -result.y;
	result.x += v2.x;
	result.y += v2.y;

	return result;
}

/// @brief Convert the vector to an array of floats.
MARS_API inline float2 Vector2Array(Vector2 v) {
	float2 arr = {{v.x, v.y}};
	return arr;
}


//----------------------------------------------------------------------------------
// Vector3 math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
MARS_API inline Vector3 Vector3Zero(void) {
	Vector3 result = { 0.f, 0.f, 0.f };
	return result;
}

/// @brief Vector with components value 1.
MARS_API inline Vector3 Vector3One(void) {
	Vector3 result = { 1.f, 1.f, 1.f };
	return result;
}

/// @brief Add two vectors.
MARS_API inline Vector3 Vector3Add(Vector3 v1, Vector3 v2) {
	Vector3 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return result;
}

/// @brief Add vector and value.
MARS_API inline Vector3 Vector3AddValue(Vector3 v, float value) {
	Vector3 result = { v.x + value, v.y + value, v.z + value };
	return result;
}

/// @brief Subtract two vectors.
MARS_API inline Vector3 Vector3Subtract(Vector3 v1, Vector3 v2) {
	Vector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return result;
}

/// @brief Subtract vector by value.
MARS_API inline Vector3 Vector3SubtractValue(Vector3 v, float value) {
	Vector3 result = { v.x - value, v.y - value, v.z - value };
	return result;
}

/// @brief Multiply vector by vector.
MARS_API inline Vector3 Vector3Multiply(Vector3 v1, Vector3 v2) {
	Vector3 result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z };
	return result;
}

/// @brief Multiply each component by value.
MARS_API inline Vector3 Vector3MultiplyValue(Vector3 v, float value) {
	Vector3 result = { v.x*value, v.y*value, v.z*value };
	return result;
}

/// @brief Calculate two vectors cross product.
MARS_API inline Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2) {
	Vector3 result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
	return result;
}

/// @brief Calculate vectors perpendicular vector.
MARS_API inline Vector3 Vector3Perpendicular(Vector3 v) {
	Vector3 result = { 0 };

	float min = fabsf(v.x);
	Vector3 cardinalAxis = { 1.f, 0.f, 0.f };

	if (fabsf(v.y) < min)
	{
		min = fabsf(v.y);
		Vector3 tmp = { 0.f, 1.f, 0.f };
		cardinalAxis = tmp;
	}

	if (fabsf(v.z) < min)
	{
		Vector3 tmp = { 0.f, 0.f, 1.f };
		cardinalAxis = tmp;
	}

	// Cross product between vectors
	result.x = v.y*cardinalAxis.z - v.z*cardinalAxis.y;
	result.y = v.z*cardinalAxis.x - v.x*cardinalAxis.z;
	result.z = v.x*cardinalAxis.y - v.y*cardinalAxis.x;

	return result;
}

/// @brief Calculate vector length.
MARS_API inline float Vector3Length(const Vector3 v) {
	float result = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	return result;
}

/// @brief Calculate vector square length.
MARS_API inline float Vector3LengthSqr(const Vector3 v) {
	float result = v.x*v.x + v.y*v.y + v.z*v.z;
	return result;
}

/// @brief Calculate two vectors dot product.
MARS_API inline float Vector3DotProduct(Vector3 v1, Vector3 v2) {
	float result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	return result;
}

/// @brief Calculate distance between two vectors.
MARS_API inline float Vector3Distance(Vector3 v1, Vector3 v2) {
	float result = 0.0f;

	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	float dz = v2.z - v1.z;
	result = sqrtf(dx*dx + dy*dy + dz*dz);

	return result;
}

/// @brief Calculate square distance between two vectors.
MARS_API inline float Vector3DistanceSqr(Vector3 v1, Vector3 v2) {
	float result = 0;

	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	float dz = v2.z - v1.z;
	result = dx*dx + dy*dy + dz*dz;

	return result;
}

/// @brief Calculate angle between two vectors.
MARS_API inline Angle Vector3Angle(Vector3 v1, Vector3 v2) {
	float result = 0.0f;

	Vector3 cross = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
	float len = sqrtf(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z);
	float dot = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	result = atan2f(len, dot);

	return RAD2ANGLE(result);
}

/// @brief Negate vector.
MARS_API inline Vector3 Vector3Negate(Vector3 v) {
	Vector3 result = { -v.x, -v.y, -v.z };
	return result;
}

/// @brief Divide vector by vector.
MARS_API inline Vector3 Vector3Divide(Vector3 v1, Vector3 v2) {
	Vector3 result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z };
	return result;
}

/// @brief Divide each component by value.
MARS_API inline Vector3 Vector3DivideValue(Vector3 v, float value) {
	Vector3 result = { v.x/value, v.y/value, v.z/value };
	return result;
}

/// @brief Calculate reflected vector to normal.
MARS_API inline Vector3 Vector3Reflect(Vector3 v, Vector3 normal) {
	Vector3 result = { 0 };

	// I is the original vector
	// N is the normal of the incident plane
	// R = I - (2*N*(DotProduct[I, N]))

	float dotProduct = (v.x*normal.x + v.y*normal.y + v.z*normal.z);

	result.x = v.x - (2*normal.x)*dotProduct;
	result.y = v.y - (2*normal.y)*dotProduct;
	result.z = v.z - (2*normal.z)*dotProduct;

	return result;
}

/// @brief Normalize vector.
MARS_API inline Vector3 Vector3Normalize(Vector3 v) {
	float len = Vector3Length(v);
	Vector3 result = { v.x / len, v.y / len, v.z / len };
	return result;
}

/// @brief Get min value for each pair of components.
MARS_API inline Vector3 Vector3Min(Vector3 v1, Vector3 v2) {
	Vector3 result = { 0 };

	result.x = fminf(v1.x, v2.x);
	result.y = fminf(v1.y, v2.y);
	result.z = fminf(v1.z, v2.z);

	return result;
}

/// @brief Get max value for each pair of components.
MARS_API inline Vector3 Vector3Max(Vector3 v1, Vector3 v2) {
	Vector3 result = { 0 };

	result.x = fmaxf(v1.x, v2.x);
	result.y = fmaxf(v1.y, v2.y);
	result.z = fmaxf(v1.z, v2.z);

	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline float Vector3MinComponent(Vector3 v) {
	float result = v.x;
	result = fminf(result, v.y);
	result = fminf(result, v.z);
	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline float Vector3MaxComponent(Vector3 v) {
	float result = v.x;
	result = fmaxf(result, v.y);
	result = fmaxf(result, v.z);
	return result;
}

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
MARS_API inline Vector3 Vector3Clamp(Vector3 v, Vector3 min, Vector3 max) {
	Vector3 result = { 0 };

	result.x = fminf(max.x, fmaxf(min.x, v.x));
	result.y = fminf(max.y, fmaxf(min.y, v.y));
	result.z = fminf(max.z, fmaxf(min.z, v.z));

	return result;
}

/// @brief Check whether two given vectors are almost equal.
MARS_API inline bool Vector3Equals(Vector3 v1, Vector3 v2) {
	return (fequal(v1.x, v2.x) && fequal(v1.y, v2.y) && fequal(v1.z, v2.z));
}

/// @brief Check whether the vector is all zeros.
MARS_API inline bool Vector3IsZero(Vector3 v) {
	return (fzequal(v.x) && fzequal(v.y) && fzequal(v.z));
}

/// @brief Check whether any element in the vector is zero.
MARS_API inline bool Vector3AnyZero(Vector3 v) {
	return (fzequal(v.x) || fzequal(v.y) || fzequal(v.z));
}

/// @brief Cast a float vector to an integer vector.
MARS_API inline Vector3i Vector3CastInt(Vector3 v) {
	Vector3i result = { (int32_t)(v.x), (int32_t)(v.y), (int32_t)(v.z) };
	return result;
}

/// @brief Convert the vector to an array of floats.
MARS_API inline float3 Vector3Array(Vector3 v) {
	float3 arr = {{v.x, v.y, v.z}};
	return arr;
}


//----------------------------------------------------------------------------------
// Vector4 math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
MARS_API inline Vector4 Vector4Zero(void) {
	Vector4 result = { 0.f, 0.f, 0.f, 0.f };
	return result;
}

/// @brief Vector with components value 1.
MARS_API inline Vector4 Vector4One(void) {
	Vector4 result = { 1.f, 1.f, 1.f, 1.f };
	return result;
}

/// @brief Add two vectors.
MARS_API inline Vector4 Vector4Add(Vector4 v1, Vector4 v2) {
	Vector4 result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
	return result;
}

/// @brief Add Vector4 and value.
MARS_API inline Vector4 Vector4AddValue(Vector4 v, float value) {
	Vector4 result = {v.x + value, v.y + value, v.z + value, v.w + value};
	return result;
}

/// @brief Subtract two vectors.
MARS_API inline Vector4 Vector4Subtract(Vector4 v1, Vector4 v2) {
	Vector4 result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
	return result;
}

/// @brief Subtract vector and value.
MARS_API inline Vector4 Vector4SubtractValue(Vector4 v, float value) {
	Vector4 result = {v.x - value, v.y - value, v.z - value, v.w - value};
	return result;
}

/// @brief Computes the length of a vector.
MARS_API inline float Vector4Length(Vector4 v) {
	float result = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
	return result;
}

/// @brief Calculate two vector multiplication.
MARS_API inline Vector4 Vector4Multiply(Vector4 v1, Vector4 v2) {
	Vector4 result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w };
	return result;
}

/// @brief Multiply each component by value.
MARS_API inline Vector4 Vector4MultiplyValue(Vector4 v, float value) {
	Vector4 result = { v.x*value, v.y*value, v.z*value, v.w*value };
	return result;
}

/// @brief Divide two vectors.
MARS_API inline Vector4 Vector4Divide(Vector4 v1, Vector4 v2) {
	Vector4 result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w };
	return result;
}

/// @brief Divide each component by value.
MARS_API inline Vector4 Vector4DivideValue(Vector4 v, float value) {
	Vector4 result = { v.x/value, v.y/value, v.z/value, v.w/value };
	return result;
}

/// @brief Negate vector.
MARS_API inline Vector4 Vector4Negate(Vector4 v) {
	Vector4 result = { -v.x, -v.y, -v.z, -v.w };
	return result;
}

/// @brief Normalize vector.
MARS_API inline Vector4 Vector4Normalize(Vector4 v) {
	float len = Vector4Length(v);
	Vector4 result = { v.x / len, v.y / len, v.z / len, v.w / len };
	return result;
}

/// @brief Get min value for each pair of components.
MARS_API inline Vector4 Vector4Min(Vector4 v1, Vector4 v2) {
	Vector4 result = { 0 };

	result.x = fminf(v1.x, v2.x);
	result.y = fminf(v1.y, v2.y);
	result.z = fminf(v1.z, v2.z);
	result.w = fminf(v1.w, v2.w);

	return result;
}

/// @brief Get max value for each pair of components.
MARS_API inline Vector4 Vector4Max(Vector4 v1, Vector4 v2) {
	Vector4 result = { 0 };

	result.x = fmaxf(v1.x, v2.x);
	result.y = fmaxf(v1.y, v2.y);
	result.z = fmaxf(v1.z, v2.z);
	result.w = fmaxf(v1.w, v2.w);

	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline float Vector4MinComponent(Vector4 v) {
	float result = v.x;
	result = fminf(result, v.y);
	result = fminf(result, v.z);
	result = fminf(result, v.w);
	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline float Vector4MaxComponent(Vector4 v) {
	float result = v.x;
	result = fmaxf(result, v.y);
	result = fmaxf(result, v.z);
	result = fmaxf(result, v.w);
	return result;
}

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
MARS_API inline Vector4 Vector4Clamp(Vector4 v, Vector4 min, Vector4 max) {
	Vector4 result = { 0 };

	result.x = fminf(max.x, fmaxf(min.x, v.x));
	result.y = fminf(max.y, fmaxf(min.y, v.y));
	result.z = fminf(max.z, fmaxf(min.z, v.z));
	result.w = fminf(max.w, fmaxf(min.w, v.w));

	return result;
}

/// @brief Check whether two given vectors are almost equal.
MARS_API inline bool Vector4Equals(Vector4 v1, Vector4 v2) {
	return (fequal(v1.x, v2.x) && fequal(v1.y, v2.y) && fequal(v1.z, v2.z) && fequal(v1.w, v2.w));
}

/// @brief Check whether the vector is all zeros.
MARS_API inline bool Vector4IsZero(Vector4 v) {
	return (fzequal(v.x) && fzequal(v.y) && fzequal(v.z) && fzequal(v.w));
}

/// @brief Check whether any element in the vector is zero.
MARS_API inline bool Vector4AnyZero(Vector4 v) {
	return (fzequal(v.x) || fzequal(v.y) || fzequal(v.z) || fzequal(v.w));
}

/// @brief Cast a float vector to an integer vector.
MARS_API inline Vector4i Vector4CastInt(Vector4 v) {
	Vector4i result = { (int32_t)(v.x), (int32_t)(v.y), (int32_t)(v.z), (int32_t)(v.w) };
	return result;
}

/// @brief Convert the vector to an array of floats.
MARS_API inline float4 Vector4Array(Vector4 v) {
	float4 arr = {{v.x, v.y, v.z, v.w}};
	return arr;
}


//----------------------------------------------------------------------------------
// Vector2i math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
MARS_API inline Vector2i Vector2iZero(void) {
	Vector2i result = { 0, 0 };
	return result;
}

/// @brief Vector with components value 1.
MARS_API inline Vector2i Vector2iOne(void) {
	Vector2i result = { 1, 1 };
	return result;
}

/// @brief Add two vectors (v1 + v2).
MARS_API inline Vector2i Vector2iAdd(Vector2i v1, Vector2i v2) {
	Vector2i result = { v1.x + v2.x, v1.y + v2.y };
	return result;
}

/// @brief Add vector and value.
MARS_API inline Vector2i Vector2iAddValue(Vector2i v, int32_t value) {
	Vector2i result = { v.x + value, v.y + value };
	return result;
}

/// @brief Subtract two vectors (v1 - v2)
MARS_API inline Vector2i Vector2iSubtract(Vector2i v1, Vector2i v2) {
	Vector2i result = { v1.x - v2.x, v1.y - v2.y };
	return result;
}

/// @brief Subtract vector by value.
MARS_API inline Vector2i Vector2iSubtractValue(Vector2i v, int32_t value) {
	Vector2i result = { v.x - value, v.y - value };
	return result;
}

/// @brief Calculate vector length.
MARS_API inline float Vector2iLength(Vector2i v) {
	float result = sqrtf((float)((v.x*v.x) + (v.y*v.y)));
	return result;
}

/// @brief Calculate vector square length.
MARS_API inline int32_t Vector2iLengthSqr(Vector2i v) {
	int32_t result = (v.x*v.x) + (v.y*v.y);
	return result;
}

/// @brief Calculate two vectors dot product.
MARS_API inline int32_t Vector2iDotProduct(Vector2i v1, Vector2i v2) {
	int32_t result = (v1.x*v2.x + v1.y*v2.y);
	return result;
}

/// @brief Calculate distance between two vectors.
MARS_API inline float Vector2iDistance(Vector2i v1, Vector2i v2) {
	float result = sqrtf((float)((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y)));
	return result;
}

/// @brief Calculate square distance between two vectors.
MARS_API inline int32_t Vector2iDistanceSqr(Vector2i v1, Vector2i v2) {
	int32_t result = ((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
	return result;
}

/// @brief Calculate angle between two vectors.
/// @brief NOTE: Angle is calculated from origin point (0, 0).
MARS_API inline Angle Vector2iAngle(Vector2i v1, Vector2i v2) {
	float result = atan2f((float)(v2.y - v1.y), (float)(v2.x - v1.x));
	return RAD2ANGLE(result);
}

/// @brief Multiply vector by vector.
MARS_API inline Vector2i Vector2iMultiply(Vector2i v1, Vector2i v2) {
	Vector2i result = { v1.x*v2.x, v1.y*v2.y };
	return result;
}

/// @brief Multiply each component by value.
MARS_API inline Vector2i Vector2iMultiplyValue(Vector2i v, int32_t value) {
	Vector2i result = { v.x*value, v.y*value };
	return result;
}

/// @brief Negate vector.
MARS_API inline Vector2i Vector2iNegate(Vector2i v) {
	Vector2i result = { -v.x, -v.y };
	return result;
}

/// @brief Divide vector by vector.
MARS_API inline Vector2i Vector2iDivide(Vector2i v1, Vector2i v2) {
	Vector2i result = { v1.x/v2.x, v1.y/v2.y };
	return result;
}

/// @brief Divide each component by value.
MARS_API inline Vector2i Vector2iDivideValue(Vector2i v, int32_t value) {
	Vector2i result = { v.x/value, v.y/value };
	return result;
}

/// @brief Calculate reflected vector to normal.
MARS_API inline Vector2i Vector2iReflect(Vector2i v, Vector2i normal) {
	Vector2i result = { 0 };
	uint32_t dotProduct = (v.x*normal.x + v.y*normal.y);
	result.x = v.x - (2*normal.x)*dotProduct;
	result.y = v.y - (2*normal.y)*dotProduct;
	return result;
}

/// @brief Get min value for each pair of components.
MARS_API inline Vector2i Vector2iMin(Vector2i v1, Vector2i v2) {
	Vector2i result = { 0 };

	result.x = imin(v1.x, v2.x);
	result.y = imin(v1.y, v2.y);

	return result;
}

/// @brief Get max value for each pair of components.
MARS_API inline Vector2i Vector2iMax(Vector2i v1, Vector2i v2) {
	Vector2i result = { 0 };

	result.x = imax(v1.x, v2.x);
	result.y = imax(v1.y, v2.y);

	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline int32_t Vector2iMinComponent(Vector2i v) {
	return imin(v.x, v.y);
}

/// @brief Get the maximum value in the vector.
MARS_API inline int32_t Vector2iMaxComponent(Vector2i v) {
	return imax(v.x, v.y);
}

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
MARS_API inline Vector2i Vector2iClamp(Vector2i v, Vector2i min, Vector2i max) {
	Vector2i result = { 0 };
	result.x = imin(max.x, imax(min.x, v.x));
	result.y = imin(max.y, imax(min.y, v.y));
	return result;
}

/// @brief Check whether two given vectors are equal.
MARS_API inline bool Vector2iEquals(Vector2i v1, Vector2i v2) {
	return (v1.x == v2.x) && (v1.y == v2.y);
}

/// @brief Check whether the vector is all zeros.
MARS_API inline bool Vector2iIsZero(Vector2i v) {
	return (v.x == 0) && (v.y == 0);
}

/// @brief Check whether any element in the vector is zero.
MARS_API inline bool Vector2iAnyZero(Vector2i v) {
	return (v.x == 0) || (v.y == 0);
}

/// @brief Cast an integer vector to a float vector.
MARS_API inline Vector2 Vector2iCastFloat(Vector2i v) {
	Vector2 result = { (float)(v.x), (float)(v.y) };
	return result;
}

/// @brief Convert the vector to an array of integers.
MARS_API inline int2 Vector2iArray(Vector2i v) {
	int2 arr = {{v.x, v.y}};
	return arr;
}


//----------------------------------------------------------------------------------
// Vector3i math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
MARS_API inline Vector3i Vector3iZero(void) {
	Vector3i result = { 0, 0, 0 };
	return result;
}

/// @brief Vector with components value 1.
MARS_API inline Vector3i Vector3iOne(void) {
	Vector3i result = { 1, 1, 1 };
	return result;
}

/// @brief Add two vectors.
MARS_API inline Vector3i Vector3iAdd(Vector3i v1, Vector3i v2) {
	Vector3i result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return result;
}

/// @brief Add vector and value.
MARS_API inline Vector3i Vector3iAddValue(Vector3i v, int32_t value) {
	Vector3i result = { v.x + value, v.y + value, v.z + value };
	return result;
}

/// @brief Subtract two vectors.
MARS_API inline Vector3i Vector3iSubtract(Vector3i v1, Vector3i v2) {
	Vector3i result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return result;
}

/// @brief Subtract vector by value.
MARS_API inline Vector3i Vector3iSubtractValue(Vector3i v, int32_t value) {
	Vector3i result = { v.x - value, v.y - value, v.z - value };
	return result;
}

/// @brief Multiply vector by vector.
MARS_API inline Vector3i Vector3iMultiply(Vector3i v1, Vector3i v2) {
	Vector3i result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z };
	return result;
}

/// @brief Multiply each component by value.
MARS_API inline Vector3i Vector3iMultiplyValue(Vector3i v, int32_t value) {
	Vector3i result = { v.x*value, v.y*value, v.z*value };
	return result;
}

/// @brief Calculate two vectors cross product.
MARS_API inline Vector3i Vector3iCrossProduct(Vector3i v1, Vector3i v2) {
	Vector3i result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
	return result;
}

/// @brief Calculate vectors perpendicular vector.
MARS_API inline Vector3i Vector3iPerpendicular(Vector3i v) {
	Vector3i result = { 0 };

	int32_t min = abs(v.x);
	Vector3i cardinalAxis = { 1, 0, 0 };

	if (abs(v.y) < min) {
		min = abs(v.y);
		Vector3i tmp = { 0, 1, 0 };
		cardinalAxis = tmp;
	}

	if (abs(v.z) < min) {
		Vector3i tmp = { 0, 0, 1 };
		cardinalAxis = tmp;
	}

	// Cross product between vectors
	result.x = v.y*cardinalAxis.z - v.z*cardinalAxis.y;
	result.y = v.z*cardinalAxis.x - v.x*cardinalAxis.z;
	result.z = v.x*cardinalAxis.y - v.y*cardinalAxis.x;

	return result;
}

/// @brief Calculate vector length.
MARS_API inline float Vector3iLength(const Vector3i v) {
	float result = sqrtf((float)(v.x*v.x + v.y*v.y + v.z*v.z));
	return result;
}

/// @brief Calculate vector square length.
MARS_API inline int32_t Vector3iLengthSqr(const Vector3i v) {
	int32_t result = v.x*v.x + v.y*v.y + v.z*v.z;
	return result;
}

/// @brief Calculate two vectors dot product.
MARS_API inline int32_t Vector3iDotProduct(Vector3i v1, Vector3i v2) {
	int32_t result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	return result;
}

/// @brief Calculate distance between two vectors.
MARS_API inline float Vector3iDistance(Vector3i v1, Vector3i v2) {
	float result = 0.0f;

	int32_t dx = v2.x - v1.x;
	int32_t dy = v2.y - v1.y;
	int32_t dz = v2.z - v1.z;
	result = sqrtf((float)(dx*dx + dy*dy + dz*dz));

	return result;
}

/// @brief Calculate square distance between two vectors.
MARS_API inline int32_t Vector3iDistanceSqr(Vector3i v1, Vector3i v2) {
	int32_t result = 0;

	int32_t dx = v2.x - v1.x;
	int32_t dy = v2.y - v1.y;
	int32_t dz = v2.z - v1.z;
	result = dx*dx + dy*dy + dz*dz;

	return result;
}

/// @brief Calculate angle between two vectors.
MARS_API inline Angle Vector3iAngle(Vector3i v1, Vector3i v2) {
	float result = 0.0f;

	Vector3i cross = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
	float len = sqrtf((float)(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z));
	float dot = (float)(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	result = atan2f(len, dot);

	return RAD2ANGLE(result);
}

/// @brief Negate vector.
MARS_API inline Vector3i Vector3iNegate(Vector3i v) {
	Vector3i result = { -v.x, -v.y, -v.z };
	return result;
}

/// @brief Divide vector by vector.
MARS_API inline Vector3i Vector3iDivide(Vector3i v1, Vector3i v2) {
	Vector3i result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z };
	return result;
}

/// @brief Divide each component by value.
MARS_API inline Vector3i Vector3iDivideValue(Vector3i v, int32_t value) {
	Vector3i result = { v.x/value, v.y/value, v.z/value };
	return result;
}

/// @brief Calculate reflected vector to normal.
MARS_API inline Vector3i Vector3iReflect(Vector3i v, Vector3i normal) {
	Vector3i result = { 0 };

	// I is the original vector
	// N is the normal of the incident plane
	// R = I - (2*N*(DotProduct[I, N]))

	int32_t dotProduct = (v.x*normal.x + v.y*normal.y + v.z*normal.z);

	result.x = v.x - (2*normal.x)*dotProduct;
	result.y = v.y - (2*normal.y)*dotProduct;
	result.z = v.z - (2*normal.z)*dotProduct;

	return result;
}

/// @brief Get min value for each pair of components.
MARS_API inline Vector3i Vector3iMin(Vector3i v1, Vector3i v2) {
	Vector3i result = { 0 };

	result.x = imin(v1.x, v2.x);
	result.y = imin(v1.y, v2.y);
	result.z = imin(v1.z, v2.z);

	return result;
}

/// @brief Get max value for each pair of components.
MARS_API inline Vector3i Vector3iMax(Vector3i v1, Vector3i v2) {
	Vector3i result = { 0 };

	result.x = imax(v1.x, v2.x);
	result.y = imax(v1.y, v2.y);
	result.z = imax(v1.z, v2.z);

	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline int32_t Vector3iMinComponent(Vector3i v) {
	int32_t result = v.x;
	result = imin(result, v.y);
	result = imin(result, v.z);
	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline int32_t Vector3iMaxComponent(Vector3i v) {
	int32_t result = v.x;
	result = imax(result, v.y);
	result = imax(result, v.z);
	return result;
}

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
MARS_API inline Vector3i Vector3iClamp(Vector3i v, Vector3i min, Vector3i max) {
	Vector3i result = { 0 };

	result.x = imin(max.x, imax(min.x, v.x));
	result.y = imin(max.y, imax(min.y, v.y));
	result.z = imin(max.z, imax(min.z, v.z));

	return result;
}

/// @brief Check whether two given vectors are equal.
MARS_API inline bool Vector3iEquals(Vector3i v, Vector3i v2) {
	return (v.x == v2.x) && (v.y == v2.y) && (v.z == v2.z);
}

/// @brief Check whether the vector is all zeros.
MARS_API inline bool Vector3iIsZero(Vector3i v) {
	return (v.x == 0) && (v.y == 0) && (v.z == 0);
}

/// @brief Check whether any element in the vector is zero.
MARS_API inline bool Vector3iAnyZero(Vector3i v) {
	return (v.x == 0) || (v.y == 0) || (v.z == 0);
}

/// @brief Cast an integer vector to a float vector.
MARS_API inline Vector3 Vector3iCastFloat(Vector3i v) {
	Vector3 result = { (float)(v.x), (float)(v.y), (float)(v.z) };
	return result;
}

/// @brief Convert the vector to an array of integers.
MARS_API inline int3 Vector3iArray(Vector3i v) {
	int3 arr = {{v.x, v.y, v.z}};
	return arr;
}


//----------------------------------------------------------------------------------
// Vector4i math
//----------------------------------------------------------------------------------

/// @brief Vector with components value 0.
MARS_API inline Vector4i Vector4iZero(void) {
	Vector4i result = { 0, 0, 0, 0 };
	return result;
}

/// @brief Vector with components value 1.
MARS_API inline Vector4i Vector4iOne(void) {
	Vector4i result = { 1, 1, 1, 1 };
	return result;
}

/// @brief Add two vectors (v1 + v2).
MARS_API inline Vector4i Vector4iAdd(Vector4i v1, Vector4i v2) {
	Vector4i result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
	return result;
}

/// @brief Add vector and value.
MARS_API inline Vector4i Vector4iAddValue(Vector4i v, int32_t value) {
	Vector4i result = {v.x + value, v.y + value, v.z + value, v.w + value};
	return result;
}

/// @brief Subtract two Vectors (v1 - v2)
MARS_API inline Vector4i Vector4iSubtract(Vector4i v1, Vector4i v2) {
	Vector4i result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
	return result;
}

/// @brief Subtract vector and value.
MARS_API inline Vector4i Vector4iSubtractValue(Vector4i v, int32_t value) {
	Vector4i result = {v.x - value, v.y - value, v.z - value, v.w - value};
	return result;
}

/// @brief Get identity vector.
MARS_API inline Vector4i Vector4iIdentity(void) {
	Vector4i result = { 0, 0, 0, 1 };
	return result;
}

/// @brief Computes the length of a vector.
MARS_API inline float Vector4iLength(Vector4i v) {
	float result = sqrtf((float)(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w));
	return result;
}

/// @brief Calculate two vectors multiplication.
MARS_API inline Vector4i Vector4iMultiply(Vector4i v1, Vector4i v2) {
	Vector4i result = { 0 };

	int32_t qax = v1.x, qay = v1.y, qaz = v1.z, qaw = v1.w;
	int32_t qbx = v2.x, qby = v2.y, qbz = v2.z, qbw = v2.w;

	result.x = qax*qbw + qaw*qbx + qay*qbz - qaz*qby;
	result.y = qay*qbw + qaw*qby + qaz*qbx - qax*qbz;
	result.z = qaz*qbw + qaw*qbz + qax*qby - qay*qbx;
	result.w = qaw*qbw - qax*qbx - qay*qby - qaz*qbz;

	return result;
}

/// @brief Multiply each component by value.
MARS_API inline Vector4i Vector4iMultiplyValue(Vector4i v, int32_t value) {
	Vector4i result = { v.x*value, v.y*value, v.z*value, v.w*value };
	return result;
}

/// @brief Divide two vectors.
MARS_API inline Vector4i Vector4iDivide(Vector4i v1, Vector4i v2) {
	Vector4i result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w };
	return result;
}

/// @brief Divide each component by value.
MARS_API inline Vector4i Vector4iDivideValue(Vector4i v, int32_t value) {
	Vector4i result = { v.x/value, v.y/value, v.z/value, v.w/value };
	return result;
}

/// @brief Negate vector.
MARS_API inline Vector4i Vector4iNegate(Vector4i v) {
	Vector4i result = { -v.x, -v.y, -v.z, -v.w };
	return result;
}

/// @brief Get min value for each pair of components.
MARS_API inline Vector4i Vector4iMin(Vector4i v1, Vector4i v2) {
	Vector4i result = { 0 };

	result.x = imin(v1.x, v2.x);
	result.y = imin(v1.y, v2.y);
	result.z = imin(v1.z, v2.z);
	result.w = imin(v1.w, v2.w);

	return result;
}

/// @brief Get max value for each pair of components.
MARS_API inline Vector4i Vector4iMax(Vector4i v1, Vector4i v2) {
	Vector4i result = { 0 };

	result.x = imax(v1.x, v2.x);
	result.y = imax(v1.y, v2.y);
	result.z = imax(v1.z, v2.z);
	result.w = imax(v1.w, v2.w);

	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline int32_t Vector4iMinComponent(Vector4i v) {
	int32_t result = v.x;
	result = imin(result, v.y);
	result = imin(result, v.z);
	result = imin(result, v.w);
	return result;
}

/// @brief Get the minimum value in the vector.
MARS_API inline int32_t Vector4iMaxComponent(Vector4i v) {
	int32_t result = v.x;
	result = imax(result, v.y);
	result = imax(result, v.z);
	result = imax(result, v.w);
	return result;
}

/// @brief Clamp the components of the vector between min and max values specified by the given vectors.
MARS_API inline Vector4i Vector4iClamp(Vector4i v, Vector4i min, Vector4i max) {
	Vector4i result = { 0 };

	result.x = imin(max.x, imax(min.x, v.x));
	result.y = imin(max.y, imax(min.y, v.y));
	result.z = imin(max.z, imax(min.z, v.z));
	result.w = imin(max.w, imax(min.w, v.w));

	return result;
}

/// @brief Check whether two given vectors are equal.
MARS_API inline bool Vector4iEquals(Vector4i v1, Vector4i v2) {
	return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w);
}

/// @brief Check whether the vector is all zeros.
MARS_API inline bool Vector4iIsZero(Vector4i v) {
	return (v.x == 0) && (v.y == 0) && (v.z == 0) && (v.w == 0);
}

/// @brief Check whether any element in the vector is zero.
MARS_API inline bool Vector4iAnyZero(Vector4i v) {
	return (v.x == 0) || (v.y == 0) || (v.z == 0) || (v.w == 0);
}

/// @brief Cast an integer vector to a float vector.
MARS_API inline Vector4 Vector4iCastFloat(Vector4i v) {
	Vector4 result = { (float)(v.x), (float)(v.y), (float)(v.z), (float)(v.w) };
	return result;
}

/// @brief Convert the vector to an array of integers.
MARS_API inline int4 Vector4iArray(Vector4i v) {
	int4 arr = {{v.x, v.y, v.z, v.w}};
	return arr;
}


//----------------------------------------------------------------------------------
// Quaternion math
//----------------------------------------------------------------------------------

MARS_API inline Quaternion QuaternionIdentity() {
	Quaternion result = { 1.f, 0.f, 0.f, 0.f };
	return result;
}

MARS_API inline bool QuaternionIsIdentity(Quaternion q) {
	return (fequal(q.q_0, 1.f) && fzequal(q.q_1) && fzequal(q.q_2) && fzequal(q.q_3));
}

MARS_API inline Quaternion QuaternionInvert(Quaternion q) {
	Quaternion result = { q.q_0, -q.q_1, -q.q_2, -q.q_3 };
	return result;
}

MARS_API inline Quaternion QuaternionMultiply(Quaternion q1, Quaternion q2) {
	Quaternion result = { 0 };

	result.q_0 = (q1.q_0 * q2.q_0) - (q1.q_1 * q2.q_1) - (q1.q_2 * q2.q_2) - (q1.q_3 * q2.q_3);
	result.q_1 = (q1.q_0 * q2.q_1) + (q1.q_1 * q2.q_0) - (q1.q_2 * q2.q_3) + (q1.q_3 * q2.q_2);
	result.q_2 = (q1.q_0 * q2.q_2) + (q1.q_1 * q2.q_3) + (q1.q_2 * q2.q_0) - (q1.q_3 * q2.q_1);
	result.q_3 = (q1.q_0 * q2.q_3) - (q1.q_1 * q2.q_2) + (q1.q_2 * q2.q_1) + (q1.q_3 * q2.q_0);

	return result;
}

MARS_API Vector3 QuaternionRotate(Vector3 p, Quaternion q);

MARS_API Quaternion QuaternionFromAxisAngle(Angle a, Vector3 v);

MARS_API Quaternion QuaternionFromRotationMatrix(Matrix3 m);

MARS_API Quaternion QuaternionFromEulerAngles(Angle roll, Angle pitch, Angle yaw);

/// @brief Convert the quaternion to an array of floats.
MARS_API inline float4 QuaternionArray(Quaternion q) {
	float4 arr = {{q.q_0, q.q_1, q.q_2, q.q_3}};
	return arr;
}


//----------------------------------------------------------------------------------
// Matrix2 math
//----------------------------------------------------------------------------------

/// @brief Create a 2x2 identity matrix.
MARS_API inline Matrix2 Matrix2Identity() {
	Matrix2 m = { 1.f, 0.f, 0.f, 1.f };
	return m;
}

/// @brief Check if the matrix is the identity matrix.
MARS_API inline bool Matrix2IsIdentity(Matrix2 m) {
	return (
		fequal(m.m_1_1, 1.f) && fzequal(m.m_1_2) && 
		fzequal(m.m_2_1)     && fequal(m.m_2_2, 1.f)
	);
}

/// @brief Check if the two matrices are equal.
MARS_API inline bool Matrix2Equals(Matrix2 m1, Matrix2 m2) {
	return (
		fequal(m1.m_1_1, m2.m_1_1) && fequal(m1.m_1_2, m2.m_1_2) &&
		fequal(m1.m_2_1, m2.m_2_1) && fequal(m1.m_2_2, m2.m_2_2)
	);
}

/// @brief Add the two matrices component-wise.
MARS_API inline Matrix2 Matrix2Add(Matrix2 m1, Matrix2 m2) {
	Matrix2 result = {
		m1.m_1_1 + m2.m_1_1, m1.m_1_2 + m2.m_1_2,
		m1.m_2_1 + m2.m_2_1, m1.m_2_2 + m2.m_2_2
	};
	return result;
}

/// @brief Subtract the two matrices component-wise.
MARS_API inline Matrix2 Matrix2Subtract(Matrix2 m1, Matrix2 m2) {
	Matrix2 result = {
		m1.m_1_1 - m2.m_1_1, m1.m_1_2 - m2.m_1_2,
		m1.m_2_1 - m2.m_2_1, m1.m_2_2 - m2.m_2_2
	};
	return result;
}

/// @brief Scale every component of the matrix.
MARS_API inline Matrix2 Matrix2Scale(Matrix2 m, float s) {
	Matrix2 result = {
		m.m_1_1 * s, m.m_1_2 * s,
		m.m_2_1 * s, m.m_2_2 * s
	};
	return result;
}

/// @brief Multiply the two matrices.
MARS_API inline Matrix2 Matrix2Multiply(Matrix2 m1, Matrix2 m2) {
	Matrix2 result = {
		(m1.m_1_1*m2.m_1_1)+(m1.m_1_2*m2.m_2_1), 
		(m1.m_1_1*m2.m_1_2)+(m1.m_1_2*m2.m_2_2),
		(m1.m_2_1*m2.m_1_1)+(m1.m_2_2*m2.m_2_1), 
		(m1.m_2_1*m2.m_1_2)+(m1.m_2_2*m2.m_2_2)
	};
	return result;
}

/// @brief Multiply the matrix by a vector.
MARS_API inline Vector2 Matrix2VecMultiply(Matrix2 m, Vector2 v) {
	Vector2 result = {
		(m.m_1_1*v.x)+(m.m_1_2*v.y),
		(m.m_2_1*v.x)+(m.m_2_2*v.y)
	};
	return result;
}

/// @brief Convert the matrix to an array of floats.
MARS_API inline float4 Matrix2Array(Matrix2 m) {
	float4 result = {
		m.m_1_1, m.m_1_2,
		m.m_2_1, m.m_2_2
	};
	return result;
}

/// @brief Construct the transposition of the matrix.
MARS_API inline Matrix2 Matrix2Transpose(Matrix2 m) {
	Matrix2 result = {
		m.m_1_1, m.m_2_1,
		m.m_1_2, m.m_2_2
	};
	return result;
}

/// @brief Compute the determinant of the matrix.
MARS_API inline float Matrix2Determinant(Matrix2 m) {
	return (m.m_1_1 * m.m_2_2) - (m.m_1_2 * m.m_2_1);
}


//----------------------------------------------------------------------------------
// Matrix3 math
//----------------------------------------------------------------------------------

/// @brief Create a 3x3 identity matrix.
MARS_API inline Matrix3 Matrix3Identity() {
	Matrix3 m = { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
	return m;
}

/// @brief Check if the matrix is the identity matrix.
MARS_API inline bool Matrix3IsIdentity(Matrix3 m) {
	return (
		fequal(m.m_1_1, 1.f) && fzequal(m.m_1_2)     && fzequal(m.m_1_3) && 
		fzequal(m.m_2_1)     && fequal(m.m_2_2, 1.f) && fzequal(m.m_2_3) &&
		fzequal(m.m_3_1)     && fzequal(m.m_3_2)     && fequal(m.m_3_3, 1.f)
	);
}

/// @brief Check if the two matrices are equal.
MARS_API inline bool Matrix3Equals(Matrix3 m1, Matrix3 m2) {
	return (
		fequal(m1.m_1_1, m2.m_1_1) && fequal(m1.m_1_2, m2.m_1_2) && fequal(m1.m_1_3, m2.m_1_3) &&
		fequal(m1.m_2_1, m2.m_2_1) && fequal(m1.m_2_2, m2.m_2_2) && fequal(m1.m_2_3, m2.m_2_3) &&
		fequal(m1.m_3_1, m2.m_3_1) && fequal(m1.m_3_2, m2.m_3_2) && fequal(m1.m_3_3, m2.m_3_3)
	);
}

/// @brief Add the two matrices component-wise.
MARS_API inline Matrix3 Matrix3Add(Matrix3 m1, Matrix3 m2) {
	Matrix3 result = {
		m1.m_1_1 + m2.m_1_1, m1.m_1_2 + m2.m_1_2, m1.m_1_3 + m2.m_1_3,
		m1.m_2_1 + m2.m_2_1, m1.m_2_2 + m2.m_2_2, m1.m_2_3 + m2.m_2_3,
		m1.m_3_1 + m2.m_3_1, m1.m_3_2 + m2.m_3_2, m1.m_3_3 + m2.m_3_3
	};
	return result;
}

/// @brief Subtract the two matrices component-wise.
MARS_API inline Matrix3 Matrix3Subtract(Matrix3 m1, Matrix3 m2) {
	Matrix3 result = {
		m1.m_1_1 - m2.m_1_1, m1.m_1_2 - m2.m_1_2, m1.m_1_3 - m2.m_1_3,
		m1.m_2_1 - m2.m_2_1, m1.m_2_2 - m2.m_2_2, m1.m_2_3 - m2.m_2_3,
		m1.m_3_1 - m2.m_3_1, m1.m_3_2 - m2.m_3_2, m1.m_3_3 - m2.m_3_3
	};
	return result;
}

/// @brief Scale every component of the matrix.
MARS_API inline Matrix3 Matrix3Scale(Matrix3 m, float s) {
	Matrix3 result = {
		m.m_1_1 * s, m.m_1_2 * s, m.m_1_3 * s,
		m.m_2_1 * s, m.m_2_2 * s, m.m_2_3 * s,
		m.m_3_1 * s, m.m_3_2 * s, m.m_3_3 * s
	};
	return result;
}

/// @brief Multiply the two matrices.
MARS_API inline Matrix3 Matrix3Multiply(Matrix3 m1, Matrix3 m2) {
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
}

/// @brief Multiply the matrix by a vector.
MARS_API inline Vector3 Matrix3VecMultiply(Matrix3 m, Vector3 v) {
	Vector3 result = {
		(m.m_1_1*v.x)+(m.m_1_2*v.y)+(m.m_1_3*v.z),
		(m.m_2_1*v.x)+(m.m_2_2*v.y)+(m.m_2_3*v.z),
		(m.m_3_1*v.x)+(m.m_3_2*v.y)+(m.m_3_3*v.z)
	};
	return result;
}

/// @brief Construct the transposition of the matrix.
MARS_API inline Matrix3 Matrix3Transpose(Matrix3 m) {
	Matrix3 result = {
		m.m_1_1, m.m_2_1, m.m_3_1,
		m.m_1_2, m.m_2_2, m.m_3_2,
		m.m_1_3, m.m_2_3, m.m_3_3
	};
	return result;
}

/// @brief Convert the matrix to an array of floats.
MARS_API inline float9 Matrix3Array(Matrix3 m) {
	float9 result = {
		m.m_1_1, m.m_1_2, m.m_1_3, 
		m.m_2_1, m.m_2_2, m.m_2_3, 
		m.m_3_1, m.m_3_2, m.m_3_3
	};
	return result;
}

/// @brief Construct a submatrix by eliminating row i and column j.
MARS_API Matrix2 Matrix3Sub(Matrix3 m, unsigned int i, unsigned int j);

/// @brief Compute the determinant of the matrix.
MARS_API float Matrix3Determinant(Matrix3 m);


//----------------------------------------------------------------------------------
// Matrix4 math
//----------------------------------------------------------------------------------

/// @brief Create a 4x4 identity matrix.
MARS_API inline Matrix4 Matrix4Identity() {
	Matrix4 m = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
	return m;
}

/// @brief Check if the matrix is the identity matrix.
MARS_API inline bool Matrix4IsIdentity(Matrix4 m) {
	return (
		fequal(m.m_1_1, 1.f) && fzequal(m.m_1_2)     && fzequal(m.m_1_3)     && fzequal(m.m_1_4) && 
		fzequal(m.m_2_1)     && fequal(m.m_2_2, 1.f) && fzequal(m.m_2_3)     && fzequal(m.m_2_4) && 
		fzequal(m.m_3_1)     && fzequal(m.m_3_2)     && fequal(m.m_3_3, 1.f) && fzequal(m.m_3_4) && 
		fzequal(m.m_4_1)     && fzequal(m.m_4_2)     && fzequal(m.m_4_3)     && fequal(m.m_4_4, 1.f)
	);
}

/// @brief Check if the two matrices are equal.
MARS_API inline bool Matrix4Equals(Matrix4 m1, Matrix4 m2) {
	return (
		fequal(m1.m_1_1, m2.m_1_1) && fequal(m1.m_1_2, m2.m_1_2) && fequal(m1.m_1_3, m2.m_1_3) && fequal(m1.m_1_4, m2.m_1_4) &&
		fequal(m1.m_2_1, m2.m_2_1) && fequal(m1.m_2_2, m2.m_2_2) && fequal(m1.m_2_3, m2.m_2_3) && fequal(m1.m_2_4, m2.m_2_4) &&
		fequal(m1.m_3_1, m2.m_3_1) && fequal(m1.m_3_2, m2.m_3_2) && fequal(m1.m_3_3, m2.m_3_3) && fequal(m1.m_3_4, m2.m_3_4) &&
		fequal(m1.m_4_1, m2.m_4_1) && fequal(m1.m_4_2, m2.m_4_2) && fequal(m1.m_4_3, m2.m_4_3) && fequal(m1.m_4_4, m2.m_4_4)
	);
}

/// @brief Add the two matrices component-wise.
MARS_API inline Matrix4 Matrix4Add(Matrix4 m1, Matrix4 m2) {
	Matrix4 result = {
		m1.m_1_1 + m2.m_1_1, m1.m_1_2 + m2.m_1_2, m1.m_1_3 + m2.m_1_3, m1.m_1_4 + m2.m_1_4,
		m1.m_2_1 + m2.m_2_1, m1.m_2_2 + m2.m_2_2, m1.m_2_3 + m2.m_2_3, m1.m_2_4 + m2.m_2_4,
		m1.m_3_1 + m2.m_3_1, m1.m_3_2 + m2.m_3_2, m1.m_3_3 + m2.m_3_3, m1.m_3_4 + m2.m_3_4,
		m1.m_4_1 + m2.m_4_1, m1.m_4_2 + m2.m_4_2, m1.m_4_3 + m2.m_4_3, m1.m_4_4 + m2.m_4_4
	};
	return result;
}

/// @brief Subtract the two matrices component-wise.
MARS_API inline Matrix4 Matrix4Subtract(Matrix4 m1, Matrix4 m2) {
	Matrix4 result = {
		m1.m_1_1 - m2.m_1_1, m1.m_1_2 - m2.m_1_2, m1.m_1_3 - m2.m_1_3, m1.m_1_4 - m2.m_1_4,
		m1.m_2_1 - m2.m_2_1, m1.m_2_2 - m2.m_2_2, m1.m_2_3 - m2.m_2_3, m1.m_2_4 - m2.m_2_4,
		m1.m_3_1 - m2.m_3_1, m1.m_3_2 - m2.m_3_2, m1.m_3_3 - m2.m_3_3, m1.m_3_4 - m2.m_3_4,
		m1.m_4_1 - m2.m_4_1, m1.m_4_2 - m2.m_4_2, m1.m_4_3 - m2.m_4_3, m1.m_4_4 - m2.m_4_4
	};
	return result;
}

/// @brief Scale every component of the matrix.
MARS_API inline Matrix4 Matrix4Scale(Matrix4 m, float s) {
	Matrix4 result = {
		m.m_1_1 * s, m.m_1_2 * s, m.m_1_3 * s, m.m_1_4 * s,
		m.m_2_1 * s, m.m_2_2 * s, m.m_2_3 * s, m.m_2_4 * s,
		m.m_3_1 * s, m.m_3_2 * s, m.m_3_3 * s, m.m_3_4 * s,
		m.m_4_1 * s, m.m_4_2 * s, m.m_4_3 * s, m.m_4_4 * s
	};
	return result;
}

/// @brief Multiply the two matrices.
MARS_API inline Matrix4 Matrix4Multiply(Matrix4 m1, Matrix4 m2) {
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
}

/// @brief Multiply the matrix by a vector.
MARS_API inline Vector4 Matrix4VecMultiply(Matrix4 m, Vector4 v) {
	Vector4 result = {
		(m.m_1_1*v.x)+(m.m_1_2*v.y)+(m.m_1_3*v.z)+(m.m_1_4*v.w),
		(m.m_2_1*v.x)+(m.m_2_2*v.y)+(m.m_2_3*v.z)+(m.m_2_4*v.w),
		(m.m_3_1*v.x)+(m.m_3_2*v.y)+(m.m_3_3*v.z)+(m.m_3_4*v.w),
		(m.m_4_1*v.x)+(m.m_4_2*v.y)+(m.m_4_3*v.z)+(m.m_4_4*v.w)
	};
	return result;
}

/// @brief Construct the transposition of the matrix.
MARS_API inline Matrix4 Matrix4Transpose(Matrix4 m) {
	Matrix4 result = {
		m.m_1_1, m.m_2_1, m.m_3_1, m.m_4_1,
		m.m_1_2, m.m_2_2, m.m_3_2, m.m_4_2,
		m.m_1_3, m.m_2_3, m.m_3_3, m.m_4_3,
		m.m_1_4, m.m_2_4, m.m_3_4, m.m_4_4
	};
	return result;
}

/// @brief Convert the matrix to an array of floats.
MARS_API inline float16 Matrix4Array(Matrix4 m) {
	float16 result = {
		m.m_1_1, m.m_1_2, m.m_1_3, m.m_1_4,
		m.m_2_1, m.m_2_2, m.m_2_3, m.m_2_4,
		m.m_3_1, m.m_3_2, m.m_3_3, m.m_3_4,
		m.m_4_1, m.m_4_2, m.m_4_3, m.m_4_4
	};
	return result;
}

/// @brief Construct a submatrix by eliminating row i and column j.
MARS_API Matrix3 Matrix4Sub(Matrix4 m, unsigned int i, unsigned int j);

/// @brief Compute the determinant of the matrix.
MARS_API float Matrix4Determinant(Matrix4 m);


//----------------------------------------------------------------------------------
// Matrix transformations
//----------------------------------------------------------------------------------

/// @brief Construct a translation matrix.
MARS_API Matrix4 MatrixTranslate(Vector3 v);

/// @brief Construct a rotation matrix.
MARS_API Matrix4 MatrixRotate(Angle a, Vector3 v);

/// @brief Construct a scaling matrix.
MARS_API Matrix4 MatrixScale(Vector3 v);

/// @brief Construct a look-at matrix.
MARS_API Matrix4 MatrixLookat(Vector3 pos, Vector3 at, Vector3 up);

/// @brief Construct an orthographic projection matrix.
MARS_API Matrix4 MatrixProjectionOrtho(float left, float right, float near, float far, float top, float bottom);

/// @brief Construct a perspective projection matrix.
MARS_API Matrix4 MatrixProjectionPerspective(float fov, float aspect, float near, float far);

#endif // MARS_STD_UTILITIES_H