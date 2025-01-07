#include "mars/std/utilities.h"
#include <string.h>

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
#include "utilities.h"

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

uint32_t CRC32Calculate(const void* _data, size_t _length, uint32_t _previousCRC) {
	uint32_t crc = _previousCRC;
	const uint8_t* current = (const uint8_t*)_data;
	while(_length-- != 0) {
		crc = (crc >> 8) ^ crc32Lookup[(crc & 0xFF) ^ *current++];
	}
	return crc;
}

const uint32_t crc32Lookup[256] = {
	//const uint32_t Polynomial = 0xEDB88320;
	//for (int i = 0; i <= 0xFF; i++) {
	//  uint32_t crc = i;
	//  for (int j = 0; j < 8; j++)
	//    crc = (crc >> 1) ^ ((crc & 1) * Polynomial);
	//  crc32Lookup[i] = crc;
	//}
	0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,
	0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,
	0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,
	0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
	0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,
	0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,
	0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
	0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,
	0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,
	0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,
	0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,
	0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,
	0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,
	0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
	0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,
	0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,
	0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,
	0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
	0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,
	0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,
	0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
	0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,
	0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,
	0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,
	0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,
	0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
	0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,
	0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
	0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,
	0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,
	0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,
	0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D,
};

char* _mars_strdup(const char* _str) {
	if (!_str) { return NULL; }
	size_t len = strlen(_str);
	char* ret = MARS_MALLOC(len + 1);
	if (ret) { 
		memcpy(ret, _str, len); 
		ret[len] = '\0';
	}
	return ret;
}

char* _mars_strndup(const char* _str, size_t _size) {
	if (!_str) { return NULL; }
	size_t len = umin(strlen(_str), _size);
	char* ret = MARS_MALLOC(len + 1);
	if (ret) { 
		memcpy(ret, _str, len);
		ret[len] = '\0';
	}
	return ret;
}
