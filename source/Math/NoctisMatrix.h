#ifndef NOCTIS_MATRIX_H
#define NOCTIS_MATRIX_H

#include "NoctisVector.h"
#include "glm/gtx/transform.hpp"

namespace noctis::math
{

	template <typename T>
	class NmatBase
	{
	protected:
		auto& GetImpl() const { return static_cast<T const&>(*this); }
		auto& GetImpl() { return static_cast<T&>(*this); }

		//C.35: A base class destructor should be either public and virtual, or protected and nonvirtual
		~NmatBase() = default;

	public:

		friend auto operator+(const NmatBase& lhs, const NmatBase& rhs)
		{
			return T(lhs.GetImpl().data + rhs.GetImpl().data);
		}

		friend auto operator-(const NmatBase& lhs, const NmatBase& rhs)
		{
			return T(lhs.GetImpl().data - rhs.GetImpl().data);
		}

		friend auto operator*(const NmatBase& lhs, const NmatBase& rhs)
		{
			return T(lhs.GetImpl().data * rhs.GetImpl().data);
		}
	};


	class Nmat4 : public NmatBase<Nmat4>
	{
	public:

		union
		{
			struct {
				float _11, _21, _31, _41;
				float _12, _22, _32, _42;
				float _13, _23, _33, _43;
				float _14, _24, _34, _44;
			};
			float elem[16];
			glm::mat4 data;
		};

		Nmat4() : data() {};

		Nmat4(float s) : data(s)
		{
		}

		explicit Nmat4(glm::mat4& _data) : data(_data) {}
	};

	Nmat4 inverse(const Nmat4& m);
	Nmat4 transpose(const Nmat4& m);

	Nmat4 inverseTranspose(const Nmat4& m);
	Nmat4 translate(const Nmat4& m, const Nvec3& t);
	Nmat4 rotate(const Nmat4& m, const Nvec3& t);
	Nmat4 scale(const Nmat4& m, const Nvec3& s);
	Nmat4 identity();
	Nmat4 translation(const Nvec3& t);
	Nmat4 rotation(const Nvec3& r);
	Nmat4 scaling(const Nvec3& s);


	//------------------------------------------------------------------------------------
	//		perspectiveLH01: Creates a left-handed perspective matrix.
	//		The nearand far clip planes correspond to z normalized device coordinates of
	//		0 and +1 respectively. (Direct3D clip volume definition).
	//------------------------------------------------------------------------------------
	Nmat4 perspectiveLH01(float fov, float aspectRatio, float nearPlane, float farPlane);


	//------------------------------------------------------------------------------------
	//		perspectiveLH_11: Creates a left-handed perspective matrix.
	//		The near and far clip planes correspond to z normalized device coordinates of
	//		 -1 and +1 respectively. (OpenGL clip volume definition).
	//------------------------------------------------------------------------------------
	Nmat4 perspectiveLH_11(float fov, float aspectRatio, float nearPlane, float farPlane);




	//------------------------------------------------------------------------------------
	//		perspectiveRH01: Creates a right-handed perspective matrix.
	//		The nearand far clip planes correspond to z normalized device coordinates of
	//		0 and +1 respectively. (Direct3D clip volume definition).
	//------------------------------------------------------------------------------------
	Nmat4 perspectiveRH01(float fov, float aspectRatio, float nearPlane, float farPlane);





	//------------------------------------------------------------------------------------
	//		perspectiveRH_11: Creates a right-handed perspective matrix.
	//		The nearand far clip planes correspond to z normalized device coordinates of
	//		 -1 and +1 respectively. (OpenGL clip volume definition).
	//------------------------------------------------------------------------------------
	Nmat4 perspectiveRH_11(float fov, float aspectRatio, float nearPlane, float farPlane);





	//------------------------------------------------------------------------------------
	//		lookAtLH: Build a left handed look at view matrix.
	//------------------------------------------------------------------------------------
	Nmat4 lookAtLH(const Nvec3& pos, const Nvec3& target, const Nvec3& up);





	//------------------------------------------------------------------------------------
	//		lookAtLH: Build a right handed look at view matrix.
	//------------------------------------------------------------------------------------
	Nmat4 lookAtRH(const Nvec3& pos, const Nvec3& target, const Nvec3& up);
}
#endif //NOCTIS_MATRIX_H