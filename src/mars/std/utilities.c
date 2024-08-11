#include "mars/std/utilities.h"

extern int32_t imin(int32_t x, int32_t y);

extern int32_t imax(int32_t x, int32_t y);

bool fequal(float a, float b) {
	// Pure equality shortcut
	if (a == b) {
		return true;
	}
	
	// Use a fixed epsilon to catch values close to zero
	float fDiff = fabs(a - b);
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

extern Vector2 Vector2Zero(void);

extern Vector2 Vector2One(void);

extern Vector2 Vector2Add(Vector2 v1, Vector2 v2);

extern Vector2 Vector2AddValue(Vector2 v, float value);

extern Vector2 Vector2Subtract(Vector2 v1, Vector2 v2);

extern Vector2 Vector2SubtractValue(Vector2 v, float value);

extern float Vector2Length(Vector2 v);

extern float Vector2LengthSqr(Vector2 v);

extern float Vector2DotProduct(Vector2 v1, Vector2 v2);

extern float Vector2Distance(Vector2 v1, Vector2 v2);

extern float Vector2DistanceSqr(Vector2 v1, Vector2 v2);

extern Angle Vector2Angle(Vector2 v1, Vector2 v2);

extern Vector2 Vector2Multiply(Vector2 v1, Vector2 v2);

extern Vector2 Vector2MultiplyValue(Vector2 v, float value);

extern Vector2 Vector2Negate(Vector2 v);

extern Vector2 Vector2Divide(Vector2 v1, Vector2 v2);

extern Vector2 Vector2DivideValue(Vector2 v, float value);

extern Vector2 Vector2Reflect(Vector2 v, Vector2 normal);

extern Vector2 Vector2Normalize(Vector2 v);

extern Vector2 Vector2Min(Vector2 v1, Vector2 v2);

extern Vector2 Vector2Min(Vector2 v1, Vector2 v2);

extern float Vector2MinComponent(Vector2 v);

extern float Vector2MaxComponent(Vector2 v);

extern Vector2 Vector2Clamp(Vector2 v, Vector2 min, Vector2 max);

extern bool Vector2Equals(Vector2 v1, Vector2 v2);

extern bool Vector2IsZero(Vector2 v);

extern bool Vector2AnyZero(Vector2 v);

extern Vector2i Vector2CastInt(Vector2 v);

extern Vector2 Vector2RotatePoint(Vector2 v1, Vector2 v2, Angle angle);

extern Vector2 Vector2ReflectPoint(Vector2 v1, Vector2 v2);

extern float2 Vector2Array(Vector2 v);

extern Vector3 Vector3Zero(void);

extern Vector3 Vector3One(void);

extern Vector3 Vector3Add(Vector3 v1, Vector3 v2);

extern Vector3 Vector3AddValue(Vector3 v, float value);

extern Vector3 Vector3Subtract(Vector3 v1, Vector3 v2);

extern Vector3 Vector3SubtractValue(Vector3 v, float value);

extern Vector3 Vector3Multiply(Vector3 v1, Vector3 v2);

extern Vector3 Vector3MultiplyValue(Vector3 v, float value);

extern Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2);

extern Vector3 Vector3Perpendicular(Vector3 v);

extern float Vector3Length(const Vector3 v);

extern float Vector3LengthSqr(const Vector3 v);

extern float Vector3DotProduct(Vector3 v1, Vector3 v2);

extern float Vector3Distance(Vector3 v1, Vector3 v2);

extern float Vector3DistanceSqr(Vector3 v1, Vector3 v2);

extern Angle Vector3Angle(Vector3 v1, Vector3 v2);

extern Vector3 Vector3Negate(Vector3 v);

extern Vector3 Vector3Divide(Vector3 v1, Vector3 v2);

extern Vector3 Vector3DivideValue(Vector3 v, float value);

extern Vector3 Vector3Reflect(Vector3 v, Vector3 normal);

extern Vector3 Vector3Normalize(Vector3 v);

extern Vector3 Vector3Min(Vector3 v1, Vector3 v2);

extern Vector3 Vector3Max(Vector3 v1, Vector3 v2);

extern float Vector3MinComponent(Vector3 v);

extern float Vector3MaxComponent(Vector3 v);

extern Vector3 Vector3Clamp(Vector3 v, Vector3 min, Vector3 max);

extern bool Vector3Equals(Vector3 v1, Vector3 v2);

extern bool Vector3IsZero(Vector3 v);

extern bool Vector3AnyZero(Vector3 v);

extern Vector3i Vector3CastInt(Vector3 v);

extern float3 Vector3Array(Vector3 v);

extern Vector4 Vector4Zero(void);

extern Vector4 Vector4One(void);

extern Vector4 Vector4Add(Vector4 v1, Vector4 v2);

extern Vector4 Vector4AddValue(Vector4 v, float value);

extern Vector4 Vector4Subtract(Vector4 v1, Vector4 v2);

extern Vector4 Vector4SubtractValue(Vector4 v, float value);

extern float Vector4Length(Vector4 v);

extern Vector4 Vector4Multiply(Vector4 v1, Vector4 v2);

extern Vector4 Vector4MultiplyValue(Vector4 v, float value);

extern Vector4 Vector4Divide(Vector4 v1, Vector4 v2);

extern Vector4 Vector4DivideValue(Vector4 v, float value);

extern Vector4 Vector4Negate(Vector4 v);

extern Vector4 Vector4Normalize(Vector4 v);

extern Vector4 Vector4Min(Vector4 v1, Vector4 v2);

extern Vector4 Vector4Max(Vector4 v1, Vector4 v2);

extern float Vector4MinComponent(Vector4 v);

extern float Vector4MaxComponent(Vector4 v);

extern Vector4 Vector4Clamp(Vector4 v, Vector4 min, Vector4 max);

extern bool Vector4Equals(Vector4 v1, Vector4 v2);

extern bool Vector4IsZero(Vector4 v);

extern bool Vector4AnyZero(Vector4 v);

extern Vector4i Vector4CastInt(Vector4 v);

extern float4 Vector4Array(Vector4 v);

extern Vector2i Vector2iZero(void);

extern Vector2i Vector2iOne(void);

extern Vector2i Vector2iAdd(Vector2i v1, Vector2i v2);

extern Vector2i Vector2iAddValue(Vector2i v, int32_t value);

extern Vector2i Vector2iSubtract(Vector2i v1, Vector2i v2);

extern Vector2i Vector2iSubtractValue(Vector2i v, int32_t value);

extern float Vector2iLength(Vector2i v);

extern int32_t Vector2iLengthSqr(Vector2i v);

extern int32_t Vector2iDotProduct(Vector2i v1, Vector2i v2);

extern float Vector2iDistance(Vector2i v1, Vector2i v2);

extern int32_t Vector2iDistanceSqr(Vector2i v1, Vector2i v2);

extern Angle Vector2iAngle(Vector2i v1, Vector2i v2);

extern Vector2i Vector2iMultiply(Vector2i v1, Vector2i v2);

extern Vector2i Vector2iMultiplyValue(Vector2i v, int32_t value);

extern Vector2i Vector2iNegate(Vector2i v);

extern Vector2i Vector2iDivide(Vector2i v1, Vector2i v2);

extern Vector2i Vector2iDivideValue(Vector2i v, int32_t value);

extern Vector2i Vector2iReflect(Vector2i v, Vector2i normal);

extern Vector2i Vector2iMin(Vector2i v1, Vector2i v2);

extern Vector2i Vector2iMax(Vector2i v1, Vector2i v2);

extern int32_t Vector2iMinComponent(Vector2i v);

extern int32_t Vector2iMaxComponent(Vector2i v);

extern Vector2i Vector2iClamp(Vector2i v, Vector2i min, Vector2i max);

extern bool Vector2iEquals(Vector2i v1, Vector2i v2);

extern bool Vector2iIsZero(Vector2i v);

extern bool Vector2iAnyZero(Vector2i v);

extern Vector2 Vector2iCastFloat(Vector2i v);

extern int2 Vector2iArray(Vector2i v);

extern Vector3i Vector3iZero(void);

extern Vector3i Vector3iOne(void);

extern Vector3i Vector3iAdd(Vector3i v1, Vector3i v2);

extern Vector3i Vector3iAddValue(Vector3i v, int32_t value);

extern Vector3i Vector3iSubtract(Vector3i v1, Vector3i v2);

extern Vector3i Vector3iSubtractValue(Vector3i v, int32_t value);

extern Vector3i Vector3iMultiply(Vector3i v1, Vector3i v2);

extern Vector3i Vector3iMultiplyValue(Vector3i v, int32_t value);

extern Vector3i Vector3iCrossProduct(Vector3i v1, Vector3i v2);

extern Vector3i Vector3iPerpendicular(Vector3i v);

extern float Vector3iLength(const Vector3i v);

extern int32_t Vector3iLengthSqr(const Vector3i v);

extern int32_t Vector3iDotProduct(Vector3i v1, Vector3i v2);

extern float Vector3iDistance(Vector3i v1, Vector3i v2);

extern int32_t Vector3iDistanceSqr(Vector3i v1, Vector3i v2);

extern Angle Vector3iAngle(Vector3i v1, Vector3i v2);

extern Vector3i Vector3iNegate(Vector3i v);

extern Vector3i Vector3iDivide(Vector3i v1, Vector3i v2);

extern Vector3i Vector3iDivideValue(Vector3i v, int32_t value);

extern Vector3i Vector3iReflect(Vector3i v, Vector3i normal);

extern Vector3i Vector3iMin(Vector3i v1, Vector3i v2);

extern Vector3i Vector3iMax(Vector3i v1, Vector3i v2);

extern int32_t Vector3iMinComponent(Vector3i v);

extern int32_t Vector3iMaxComponent(Vector3i v);

extern Vector3i Vector3iClamp(Vector3i v, Vector3i min, Vector3i max);

extern bool Vector3iEquals(Vector3i v1, Vector3i v2);

extern bool Vector3iIsZero(Vector3i v);

extern bool Vector3iAnyZero(Vector3i v);

extern Vector3 Vector3iCastFloat(Vector3i v);

extern int3 Vector3iArray(Vector3i v);

extern Vector4i Vector4iZero(void);

extern Vector4i Vector4iOne(void);

extern Vector4i Vector4iAdd(Vector4i v1, Vector4i v2);

extern Vector4i Vector4iAddValue(Vector4i v, int32_t value);

extern Vector4i Vector4iSubtract(Vector4i v1, Vector4i v2);

extern Vector4i Vector4iSubtractValue(Vector4i v, int32_t value);

extern Vector4i Vector4iIdentity(void);

extern float Vector4iLength(Vector4i v);

extern Vector4i Vector4iMultiply(Vector4i v1, Vector4i v2);

extern Vector4i Vector4iMultiplyValue(Vector4i v, int32_t value);

extern Vector4i Vector4iDivide(Vector4i v1, Vector4i v2);

extern Vector4i Vector4iDivideValue(Vector4i v, int32_t value);

extern Vector4i Vector4iNegate(Vector4i v);

extern Vector4i Vector4iMin(Vector4i v1, Vector4i v2);

extern Vector4i Vector4iMax(Vector4i v1, Vector4i v2);

extern int32_t Vector4iMinComponent(Vector4i v);

extern int32_t Vector4iMaxComponent(Vector4i v);

extern Vector4i Vector4iClamp(Vector4i v, Vector4i min, Vector4i max);

extern bool Vector4iEquals(Vector4i v1, Vector4i v2);

extern bool Vector4iIsZero(Vector4i v);

extern bool Vector4iAnyZero(Vector4i v);

extern Vector4 Vector4iCastFloat(Vector4i v);

extern int4 Vector4iArray(Vector4i v);

extern Quaternion QuaternionIdentity();

extern bool QuaternionIsIdentity(Quaternion q);

extern Quaternion QuaternionInvert(Quaternion q);

extern Quaternion QuaternionMultiply(Quaternion q1, Quaternion q2);

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

extern Quaternion QuaternionFromRotationMatrix(Matrix3 m) {
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

extern float4 QuaternionArray(Quaternion q);

extern Matrix2 Matrix2Identity();

extern bool Matrix2IsIdentity(Matrix2 m);

extern bool Matrix2Equals(Matrix2 m1, Matrix2 m2);

extern Matrix2 Matrix2Add(Matrix2 m1, Matrix2 m2);

extern Matrix2 Matrix2Subtract(Matrix2 m1, Matrix2 m2);

extern Matrix2 Matrix2Scale(Matrix2 m, float s);

extern Matrix2 Matrix2Multiply(Matrix2 m1, Matrix2 m2);

extern Vector2 Matrix2VecMultiply(Matrix2 m, Vector2 v);

extern float4 Matrix2Array(Matrix2 m);

extern Matrix2 Matrix2Transpose(Matrix2 m);

extern float Matrix2Determinant(Matrix2 m);

extern Matrix3 Matrix3Identity();

extern bool Matrix3IsIdentity(Matrix3 m);

extern bool Matrix3Equals(Matrix3 m1, Matrix3 m2);

extern Matrix3 Matrix3Add(Matrix3 m1, Matrix3 m2);

extern Matrix3 Matrix3Subtract(Matrix3 m1, Matrix3 m2);

extern Matrix3 Matrix3Scale(Matrix3 m, float s);

extern Matrix3 Matrix3Multiply(Matrix3 m1, Matrix3 m2);

extern Vector3 Matrix3VecMultiply(Matrix3 m, Vector3 v);

extern Matrix3 Matrix3Transpose(Matrix3 m);

extern float9 Matrix3Array(Matrix3 m);

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

extern Matrix4 Matrix4Identity();

extern bool Matrix4IsIdentity(Matrix4 m);

extern bool Matrix4Equals(Matrix4 m1, Matrix4 m2);

extern Matrix4 Matrix4Add(Matrix4 m1, Matrix4 m2);

extern Matrix4 Matrix4Subtract(Matrix4 m1, Matrix4 m2);

extern Matrix4 Matrix4Scale(Matrix4 m, float s);

extern Matrix4 Matrix4Multiply(Matrix4 m1, Matrix4 m2);

extern Vector4 Matrix4VecMultiply(Matrix4 m, Vector4 v);

extern Matrix4 Matrix4Transpose(Matrix4 m);

extern float16 Matrix4Array(Matrix4 m);

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