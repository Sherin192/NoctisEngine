#ifndef NOCTIS_VECTOR_H
#define NOCTIS_VECTOR_H
#define GLM_FORCE_INLINE
#include "glm/glm.hpp"

namespace noctis::math
{

//------------------------------------------------------------------------------------
//		NvecBase: Vector base class which defined a common functions and operators.
//------------------------------------------------------------------------------------
	template <typename T>
	class NvecBase
	{
	protected:
		auto& GetImpl() const { return static_cast<T const&>(*this); }
		auto& GetImpl() { return static_cast<T&>(*this); }

		//C.35: A base class destructor should be either public and virtual, or protected and nonvirtual
		~NvecBase() = default;
	public:

		auto& operator+=(NvecBase& rhs)
		{
			GetImpl().data += rhs.GetImpl().data;
			return static_cast<T>(*this);
		}

		auto& operator-=(NvecBase& rhs)
		{
			GetImpl().data -= rhs.GetImpl().data;
			return static_cast<T>(*this);
		}

		auto& operator*=(NvecBase& rhs)
		{
			GetImpl().data *= rhs.GetImpl().data;
			return static_cast<T>(*this);
		}

		auto& operator/=(NvecBase& rhs)
		{
			GetImpl().data /= rhs.GetImpl().data;
			return static_cast<T>(*this);
		}

		friend auto operator+(const NvecBase& lhs, const NvecBase& rhs)
		{
			return T(lhs.GetImpl().data + rhs.GetImpl().data);
		}

		friend auto operator-(const NvecBase& lhs, const NvecBase& rhs)
		{
			return T(lhs.GetImpl().data - rhs.GetImpl().data);
		}

		friend auto operator*(const NvecBase& lhs, const NvecBase& rhs)
		{
			return T(lhs.GetImpl().data * rhs.GetImpl().data);
		}

		friend auto operator*(const NvecBase& lhs, const float s)
		{
			return T(lhs.GetImpl().data * s);
		}
			
		friend auto operator*(const float s, const NvecBase& lhs)
		{
			return T(s * lhs.GetImpl().data);
		}
		
		friend auto operator/(const NvecBase &lhs , const NvecBase& rhs)
		{
			return T(lhs.GetImpl().data / rhs.GetImpl().data);
		}

		friend auto reflect(const NvecBase& i, const NvecBase& n)
		{
			return T(glm::reflect(i.GetImpl().data, n.GetImpl().data));
		}

		friend auto dot(const NvecBase& lhs, const NvecBase& rhs)
		{
			return glm::dot(lhs.data, rhs.data);
		}

		friend auto distance(const NvecBase& p0, const NvecBase& p1)
		{
			return glm::distance(p0.data, p1.data);
		}

		friend auto length(const NvecBase& vec)
		{
			return glm::length(vec.data);
		}

		friend void normalize(NvecBase<T>& v)
		{
			glm::normalize(static_cast<T&>(v).data);
		}
	};

//====================================================================================





//------------------------------------------------------------------------------------
//		Nvec4: Vector base class which defined a common functions and operators.
//------------------------------------------------------------------------------------
	class Nvec4 : public NvecBase<Nvec4>
	{
	public:

		union
		{
			struct {
				float x, y, z, w;
			};
			float elem[4];
			glm::vec4 data;
		};

		Nvec4() = default;

		Nvec4(float s) : x(s), y(s), z(s), w(s) {}

		Nvec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

		Nvec4(const glm::vec4& _data) : data(_data) {}

		auto& operator+=(Nvec4& rhs)
		{
			data += rhs.data;
			return *this;
		}

		auto& operator-=(Nvec4& rhs)
		{
			data -= rhs.data;
			return *this;
		}

		auto& operator*=(Nvec4& rhs)
		{
			data *= rhs.data;
			return *this;
		}

		auto& operator/=(Nvec4& rhs)
		{
			data /= rhs.data;
			return *this;
		}
	};

//====================================================================================





//------------------------------------------------------------------------------------
//		Nvec3: Vector base class which defined a common functions and operators.
//------------------------------------------------------------------------------------
	class Nvec3 : public NvecBase<Nvec3>
	{
	public:

		union
		{
			struct {
				float x, y, z;
			};
			float elem[3];
			glm::vec3 data;
		};

		Nvec3() = default;

		Nvec3(float s) : x(s), y(s), z(s) {}

		Nvec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		static Nvec3 Zero() { return Nvec3(0.0f); }
		static Nvec3 One() { return Nvec3(1.0f); }
		static Nvec3 Forward() { return Nvec3(1.0f, 0.0f, 0.0f); }
		static Nvec3 Up() { return Nvec3(0.0f, 1.0f, 0.0f); }

		Nvec3(const glm::vec3& _data) : data(_data) {}

		auto& operator+=(const Nvec3& rhs)
		{
			data += rhs.data;
			return *this;
		}

		auto& operator-=(const Nvec3& rhs)
		{
			data -= rhs.data;
			return *this;
		}

		auto& operator*=(const Nvec3& rhs)
		{
			data *= rhs.data;
			return *this;
		}

		auto& operator/=(const Nvec3& rhs)
		{
			data /= rhs.data;
			return *this;
		}

	};

//====================================================================================





//====================================================================================
// Free functions for Nvec3.
//====================================================================================
	inline Nvec3 cross(const Nvec3& lhs, const Nvec3& rhs)
	{
		return Nvec3(glm::cross(lhs.data, rhs.data));
	}

//====================================================================================






//------------------------------------------------------------------------------------
//		Nvec2: Vector base class which defined a common functions and operators.
//------------------------------------------------------------------------------------
	class Nvec2 : public NvecBase<Nvec2>
	{
	public:

		union
		{
			struct {
				float x, y;
			};
			float elem[2];
			glm::vec2 data;
		};

		Nvec2() = default;

		Nvec2(float s) : x(s), y(s) {}

		Nvec2(float _x, float _y) : x(_x), y(_y) {}

		Nvec2(const glm::vec2& _data) : data(_data) {}

		auto operator+=(Nvec2& rhs)
		{
			data += rhs.data;
			return *this;
		}

		auto operator-=(Nvec2& rhs)
		{
			data -= rhs.data;
			return *this;
		}

		auto operator*=(Nvec2& rhs)
		{
			data *= rhs.data;
			return *this;
		}
	};

//====================================================================================
}

#endif //NOCTIS_VECTOR_H
