#ifndef DX11_UTILS_H
#define DX11_UTILS_H

//#include "Core_pch.h"





//=================================================================================


DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
{
	// Inverse-transpose is just applied to normals.  So zero out 
	// translation row so that it doesn't get into our inverse-transpose
	// calculation--we don't want the inverse-transpose of the translation.
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));
}


//=================================================================================



#define DEBUGLOG( msg )								\
do{													\
   std::ostringstream os_;							\
   os_ << msg;										\
   OutputDebugString( os_.str().c_str() );			\
}while(0)

#pragma comment (lib, "d3d11.lib")

namespace Memory
{
	template<typename T>
	void SafaDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
	template<typename T>
	void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}
	template<typename T>
	void SafeRelese(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
}

#endif //DX11_UTILS_H