#ifndef _BUILTIN_INL_
#define _BUILTIN_INL_

inline void ftrv(float matrix[][4], float vec1[], float vec2[]){
/*
	vec2[0] = matrix[0][0]*vec1[0] + matrix[0][1]*vec1[1] + matrix[0][2]*vec1[2] + matrix[0][3]*vec1[3];
	vec2[1] = matrix[1][0]*vec1[0] + matrix[1][1]*vec1[1] + matrix[1][2]*vec1[2] + matrix[1][3]*vec1[3];
	vec2[2] = matrix[2][0]*vec1[0] + matrix[2][1]*vec1[1] + matrix[2][2]*vec1[2] + matrix[2][3]*vec1[3];
	vec2[3] = matrix[3][0]*vec1[0] + matrix[3][1]*vec1[1] + matrix[3][2]*vec1[2] + matrix[3][3]*vec1[3];
*/
	vec2[0] = matrix[0][0]*vec1[0] + matrix[1][0]*vec1[1] + matrix[2][0]*vec1[2] + matrix[3][0]*vec1[3];
	vec2[1] = matrix[0][1]*vec1[0] + matrix[1][1]*vec1[1] + matrix[2][1]*vec1[2] + matrix[3][1]*vec1[3];
	vec2[2] = matrix[0][2]*vec1[0] + matrix[1][2]*vec1[1] + matrix[2][2]*vec1[2] + matrix[3][2]*vec1[3];
	vec2[3] = matrix[0][3]*vec1[0] + matrix[1][3]*vec1[1] + matrix[2][3]*vec1[2] + matrix[3][3]*vec1[3];
}

inline float fipr(float vec1[], float vec2[]){
	return vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2] + vec1[3]*vec2[3];
}

inline void add4(float vec1[], float vec2[], float vec3[]){
	vec3[0] = vec1[0] + vec2[0];
	vec3[1] = vec1[1] + vec2[1];
	vec3[2] = vec1[2] + vec2[2];
	vec3[3] = vec1[3] + vec2[3];
}

inline void sub4(float vec1[], float vec2[], float vec3[]){
	vec3[0] = vec1[0] - vec2[0];
	vec3[1] = vec1[1] - vec2[1];
	vec3[2] = vec1[2] - vec2[2];
	vec3[3] = vec1[3] - vec2[3];
}

#endif //_BUILTIN_INL_

