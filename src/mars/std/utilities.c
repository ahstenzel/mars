#include "mars/std/utilities.h"

// Define custom allocators for inih

void* ini_malloc(size_t size) {
    return MARS_MALLOC(size);
}

void ini_free(void* ptr) {
    MARS_FREE(ptr);
}

void* ini_realloc(void* ptr, size_t size) {
    return MARS_REALLOC(ptr, size);
}

extern int32_t imin(int32_t x, int32_t y);

extern int32_t imax(int32_t x, int32_t y);

extern size_t umin(size_t x, size_t y);

extern size_t umax(size_t x, size_t y);

bool fequal(float a, float b) {
	// Pure equality shortcut
	if (a == b) {
		return true;
	}
	
	// Use a fixed epsilon to catch values close to zero
	float fDiff = fabsf(a - b);
	if (fDiff < MARS_FEQUAL_MAX_DIFF) {
		return true;
	}

	// Only do comparisons on values with the same sign
	if (signbit(a) != signbit(b)) {
		return false;
	}

#ifndef MARS_FEQUAL_USE_RELATIVE_EPSILON
	// Determine difference between values using ULPs
	_Float_Cast fA = {a};
	_Float_Cast fB = {b};
	return (abs(fA.i - fB.i) < MARS_FEQUAL_MAX_ULPS);
#else
	// Determine difference between values using a relative epsilon
	float fA = fabsf(a);
	float fB = fabsf(b);
	return (fDiff <= fmaxf(fA, fB) * MARS_FEQUAL_MAX_RELDIFF);
#endif // MARS_FEQUAL_USE_RELATIVE_EPSILON
}

#undef _inline_defs_header
#include "inline_defs.h"

Vector3 QuaternionRotate(Vector3 p, Quaternion q) {
	Quaternion qp = { 0.f, p.x, p.y, p.x };
	Quaternion qi = QuaternionInvert(q);
	Quaternion r = QuaternionMultiply(qi, qp);
	r = QuaternionMultiply(r, q);
	Vector3 result = { r.q_1, r.q_2, r.q_3 };
	return result;
}

Quaternion QuaternionFromAxisAngle(Angle a, Vector3 v) {
	Quaternion result = { 0 };
	result.q_0 = cosf(ANGLE2RAD(a) / 2.f);
	float s = sinf(ANGLE2RAD(a) / 2.f);
	result.q_1 = v.x * s;
	result.q_2 = v.y * s;
	result.q_3 = v.z * s;
	return result;
}

Quaternion QuaternionFromRotationMatrix(Matrix3 m) {
	Quaternion result = { 0 };

	float r0 = sqrtf((1.f + m.m_1_1 + m.m_2_2 + m.m_3_3) / 4.f);
	float r1 = sqrtf((1.f + m.m_1_1 - m.m_2_2 - m.m_3_3) / 4.f);
	float r2 = sqrtf((1.f - m.m_1_1 + m.m_2_2 - m.m_3_3) / 4.f);
	float r3 = sqrtf((1.f - m.m_1_1 - m.m_2_2 + m.m_3_3) / 4.f);
	float max = r0;
	max = fmaxf(max, r1);
	max = fmaxf(max, r2);
	max = fmaxf(max, r3);
	
	if (fequal(max, r0)) {
		result.q_0 = r0;
		result.q_1 = (m.m_3_2 - m.m_2_3) / (4.f * r0);
		result.q_2 = (m.m_1_3 - m.m_3_1) / (4.f * r0);
		result.q_3 = (m.m_2_1 - m.m_1_2) / (4.f * r0);
	}
	else if (fequal(max, r1)) {
		result.q_0 = (m.m_3_2 - m.m_2_3) / (4.f * r1);
		result.q_1 = r1;
		result.q_2 = (m.m_1_2 + m.m_2_1) / (4.f * r1);
		result.q_3 = (m.m_1_3 + m.m_3_1) / (4.f * r1);
	}
	else if (fequal(max, r2)) {
		result.q_0 = (m.m_1_3 - m.m_3_1) / (4.f * r2);
		result.q_1 = (m.m_1_2 + m.m_2_1) / (4.f * r2);
		result.q_2 = r2;
		result.q_3 = (m.m_2_3 + m.m_3_2) / (4.f * r2);
	}
	else {
		result.q_0 = (m.m_2_1 - m.m_1_2) / (4.f * r3);
		result.q_1 = (m.m_1_3 + m.m_3_1) / (4.f * r3);
		result.q_2 = (m.m_2_3 + m.m_3_2) / (4.f * r3);
		result.q_3 = r3;
	}
	
	return result;
}

Quaternion QuaternionFromEulerAngles(Angle roll, Angle pitch, Angle yaw) {
	Quaternion result = { 0 };

	float u_s = sinf(ANGLE2RAD(roll) / 2.f);
	float u_c = cosf(ANGLE2RAD(roll) / 2.f);
	float v_s = sinf(ANGLE2RAD(pitch) / 2.f);
	float v_c = cosf(ANGLE2RAD(pitch) / 2.f);
	float w_s = sinf(ANGLE2RAD(yaw) / 2.f);
	float w_c = cosf(ANGLE2RAD(yaw) / 2.f);

	result.q_0 = (u_c * v_c * w_c) + (u_s * v_s + w_s);
	result.q_1 = (u_s * v_c * w_c) - (u_c * v_s + w_s);
	result.q_2 = (u_c * v_s * w_c) + (u_s * v_c + w_s);
	result.q_3 = (u_c * v_c * w_s) - (u_s * v_s + w_c);

	return result;
}

Matrix2 Matrix3Sub(Matrix3 m, unsigned int i, unsigned int j) {
	float tmp[4] = { 0 };
	unsigned int idx = 0;
	float9 m_arr = Matrix3Array(m);
	for(unsigned int row = 0; row < 3; ++row) {
		if (row == i) { continue; }
		for(unsigned int col = 0; col < 3; ++col) {
			if (col == j) { continue; }
			tmp[idx++] = m_arr.v[(row * 3) + col];
		}
	}
	Matrix2 result = {
		tmp[0], tmp[1],
		tmp[2], tmp[3]
	};
	return result;
}

float Matrix3Determinant(Matrix3 m) {
	Matrix2 s_1 = Matrix3Sub(m, 1, 1);
	Matrix2 s_2 = Matrix3Sub(m, 1, 2);
	Matrix2 s_3 = Matrix3Sub(m, 1, 3);
	float d_1 = Matrix2Determinant(s_1);
	float d_2 = Matrix2Determinant(s_2);
	float d_3 = Matrix2Determinant(s_3);
	return (m.m_1_1*d_1) - (m.m_1_2*d_2) + (m.m_1_3*d_3);
}

Matrix3 Matrix4Sub(Matrix4 m, unsigned int i, unsigned int j)  {
	float tmp[9] = { 0 };
	unsigned int idx = 0;
	float16 m_arr = Matrix4Array(m);
	for(unsigned int row = 0; row < 4; ++row) {
		if (row == i) { continue; }
		for(unsigned int col = 0; col < 4; ++col) {
			if (col == j) { continue; }
			tmp[idx++] = m_arr.v[(row * 4) + col];
		}
	}
	Matrix3 result = {
		tmp[0], tmp[1], tmp[2],
		tmp[3], tmp[4], tmp[5],
		tmp[6], tmp[7], tmp[8]
	};
	return result;
}

float Matrix4Determinant(Matrix4 m)  {
	Matrix3 s_1 = Matrix4Sub(m, 1, 1);
	Matrix3 s_2 = Matrix4Sub(m, 1, 2);
	Matrix3 s_3 = Matrix4Sub(m, 1, 3);
	Matrix3 s_4 = Matrix4Sub(m, 1, 4);
	float d_1 = Matrix3Determinant(s_1);
	float d_2 = Matrix3Determinant(s_2);
	float d_3 = Matrix3Determinant(s_3);
	float d_4 = Matrix3Determinant(s_4);
	return (m.m_1_1*d_1) - (m.m_1_2*d_2) + (m.m_1_3*d_3) - (m.m_1_4*d_4);
}

Matrix4 MatrixTranslate(Vector3 v) {
	Matrix4 result = Matrix4Identity();
	result.m_1_4 = v.x;
	result.m_2_4 = v.y;
	result.m_3_4 = v.z;
	return result;
}

Matrix4 MatrixRotate(Angle a, Vector3 v) {
	Matrix4 result = Matrix4Identity();
	float c = cosf(ANGLE2RAD(a));
	float s = sinf(ANGLE2RAD(a));
	result.m_1_1 = c + (v.x * v.x * (1.f - c));
	result.m_2_1 = (v.y * v.x * (1.f - c)) + (v.z * s);
	result.m_3_1 = (v.z * v.x * (1.f - c)) - (v.y * s);
	result.m_1_2 = (v.x * v.y * (1.f - c)) - (v.z * s);
	result.m_2_2 = c + (v.y * v.y * (1.f - c));
	result.m_3_2 = (v.z * v.y * (1.f - c)) + (v.x * s);
	result.m_1_3 = (v.x * v.z * (1.f - c)) + (v.y * s);
	result.m_2_3 = (v.y * v.z * (1.f - c)) - (v.x * s);
	result.m_3_3 = c + (v.z * v.z * (1.f - c));
	return result;
}

Matrix4 MatrixScale(Vector3 v) {
	Matrix4 result = Matrix4Identity();
	result.m_1_1 = v.x;
	result.m_2_2 = v.y;
	result.m_3_3 = v.z;
	return result;
}

Matrix4 MatrixLookat(Vector3 pos, Vector3 at, Vector3 up) {
	Matrix4 m1 = Matrix4Identity();
	Matrix4 m2 = MatrixTranslate(Vector3Negate(pos));
	Vector3 direction = Vector3Normalize(Vector3Subtract(pos, at));
	Vector3 right = Vector3Normalize(Vector3CrossProduct(up, direction));
	Vector3 camUp = Vector3CrossProduct(direction, right);

	m1.m_1_1 = right.x;
	m1.m_2_1 = camUp.x;
	m1.m_3_1 = direction.x;
	m1.m_1_2 = right.y;
	m1.m_2_2 = camUp.y;
	m1.m_3_2 = direction.y;
	m1.m_1_3 = right.z;
	m1.m_2_3 = camUp.z;
	m1.m_3_3 = direction.z;

	return Matrix4Multiply(m1, m2);
}

Matrix4 MatrixProjectionOrtho(float left, float right, float near, float far, float top, float bottom) {
	Matrix4 result = Matrix4Identity();
	result.m_1_1 = 2.f / (right - left);
	result.m_2_2 = 2.f / (top - bottom);
	result.m_3_3 = -2.f / (far - near);
	result.m_1_4 = (right + left) / (right - left);
	result.m_2_4 = (top + bottom) / (top - bottom);
	result.m_3_4 = (far + near) / (far - near);
	return result;
}

Matrix4 MatrixProjectionPerspective(float fov, float aspect, float near, float far) {
	Matrix4 result = Matrix4Identity();
	float top = tanf(fov / 2.f) * near;
	float bottom = -top;
	float right = top * aspect;
	float left = bottom * aspect;

	result.m_1_1 = (2.f * near) / (right - left);
	result.m_2_2 = (2.f * near) / (top - bottom);
	result.m_1_3 = (right + left) / (right - left);
	result.m_2_3 = (top + bottom) / (top - bottom);
	result.m_3_3 = -(far + near) / (far - near);
	result.m_4_3 = -1.f;
	result.m_3_4 = (-2.f * far * near) / (far - near);
	result.m_4_4 = 0.f;
	return result;
}

#define _MARS_ID_START 100000000u
#define _MARS_ID_END   999999999u
#define _MARS_ID_LIMIT (_MARS_ID_END - _MARS_ID_START)

static _MARS_ID_TYPE _mars_id_seed = ID_NULL;

_MARS_ID_TYPE _mars_id_generate() {
	return ((_mars_id_seed++) % _MARS_ID_LIMIT) + _MARS_ID_START;
}