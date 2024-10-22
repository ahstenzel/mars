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
// Linear algebra
//----------------------------------------------------------------------------------

/*
	Inline definitions are kept in a seperate file.
	Through macros we can enable/disable the necessary keywords
	to signal to the compiler these functions should be inlined.
	Functions that should never be inlined are defined below.
*/
#define _inline_defs_header
#include "inline_defs.h"
#undef _inline_defs_header

/// @brief Rotate the quaternion.
MARS_API Vector3 QuaternionRotate(Vector3 p, Quaternion q);

/// @brief Create a quaternion from the axis & angle of rotation about that axis.
MARS_API Quaternion QuaternionFromAxisAngle(Angle a, Vector3 v);

/// @brief Create a quaternion from the rotation matrix.
MARS_API Quaternion QuaternionFromRotationMatrix(Matrix3 m);

/// @brief Create a quaternion from yaw, pitch, and roll.
MARS_API Quaternion QuaternionFromEulerAngles(Angle roll, Angle pitch, Angle yaw);

/// @brief Construct a submatrix by eliminating row i and column j.
MARS_API Matrix2 Matrix3Sub(Matrix3 m, unsigned int i, unsigned int j);

/// @brief Compute the determinant of the matrix.
MARS_API float Matrix3Determinant(Matrix3 m);

/// @brief Construct a submatrix by eliminating row i and column j.
MARS_API Matrix3 Matrix4Sub(Matrix4 m, unsigned int i, unsigned int j);

/// @brief Compute the determinant of the matrix.
MARS_API float Matrix4Determinant(Matrix4 m);

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