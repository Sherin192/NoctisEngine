#include "NoctisMatrix.h"


namespace noctis::math
{
 Nmat4 inverse(const Nmat4& m)
{
	return Nmat4(glm::inverse(m.data));
}


 Nmat4 transpose(const Nmat4& m)
{
	return Nmat4(glm::transpose(m.data));
}

 Nmat4 inverseTranspose(const Nmat4& m)
{
	return Nmat4((transpose(inverse(m))));
}

 Nmat4 translate(const Nmat4& m, const Nvec3& t)
{
	return Nmat4(glm::translate(m.data, t.data));
}

 Nmat4 rotate(const Nmat4& m, const Nvec3& t)
{
	return Nmat4(glm::rotate(glm::rotate(glm::rotate(m.data, t.elem[0], { 1.0f, 0.0f, 0.0f }), t.elem[1], { 0.0f, 1.0f, 0.0f }), t.elem[2], { 0.0f, 0.0f, 1.0f }));
}

 Nmat4 scale(const Nmat4& m, const Nvec3& s)
{
	return Nmat4(glm::scale(m.data, s.data));
}

 Nmat4 identity()
{
	return Nmat4(1);
}

 Nmat4 translation(const Nvec3& t)
{
	return translate(identity(), t);
}


 Nmat4 rotation(const Nvec3& r)
{
	return rotate(identity(), r);
}


 Nmat4 scaling(const Nvec3& s)
{
	 auto test = identity();
	return scale(identity(), s);
}





//------------------------------------------------------------------------------------
//		perspectiveLH01: Creates a left-handed perspective matrix.
//		The nearand far clip planes correspond to z normalized device coordinates of
//		0 and +1 respectively. (Direct3D clip volume definition).
//------------------------------------------------------------------------------------
 Nmat4 perspectiveLH01(float fov, float aspectRatio, float nearPlane, float farPlane)
 {
	 return Nmat4(glm::perspectiveLH_ZO(fov, aspectRatio, nearPlane, farPlane));
 }





//------------------------------------------------------------------------------------
//		perspectiveLH01: Creates a left-handed perspective matrix.
//		The nearand far clip planes correspond to z normalized device coordinates of
//		 -1 and +1 respectively. (OpenGL clip volume definition).
//------------------------------------------------------------------------------------
 Nmat4 perspectiveLH_11(float fov, float aspectRatio, float nearPlane, float farPlane)
 {
	 return Nmat4(glm::perspectiveLH_NO(fov, aspectRatio, nearPlane, farPlane));
 }





//------------------------------------------------------------------------------------
//		perspectiveRH01: Creates a right-handed perspective matrix.
//		The nearand far clip planes correspond to z normalized device coordinates of
//		0 and +1 respectively. (Direct3D clip volume definition).
//------------------------------------------------------------------------------------
 Nmat4 perspectiveRH01(float fov, float aspectRatio, float nearPlane, float farPlane)
 {
	 return Nmat4(glm::perspectiveRH_ZO(fov, aspectRatio, nearPlane, farPlane));
 }





//------------------------------------------------------------------------------------
//		perspectiveRH_11: Creates a right-handed perspective matrix.
//		The nearand far clip planes correspond to z normalized device coordinates of
//		 -1 and +1 respectively. (OpenGL clip volume definition).
//------------------------------------------------------------------------------------
 Nmat4 perspectiveRH_11(float fov, float aspectRatio, float nearPlane, float farPlane)
 {
	 return Nmat4(glm::perspectiveRH_NO(fov, aspectRatio, nearPlane, farPlane));
 }


 


//------------------------------------------------------------------------------------
//		lookAtLH: Build a left handed look at view matrix.
//------------------------------------------------------------------------------------
 Nmat4 lookAtLH(const Nvec3& pos, const Nvec3& target, const Nvec3& up)
 {
	 
	 return Nmat4(glm::lookAtLH(pos.data, target.data, up.data));
 }





//------------------------------------------------------------------------------------
//		lookAtLH: Build a right handed look at view matrix.
//------------------------------------------------------------------------------------
 Nmat4 lookAtRH(const Nvec3& pos, const Nvec3& target, const Nvec3& up)
 {

	 return Nmat4(glm::lookAtRH(
		 pos.data, target.data, up.data));
 }
}
