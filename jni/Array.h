#ifndef _CntrArray__h
#define _CntrArray__h

#include "PreInclude.h"
#include "ffDefs.h"
#include <vector>
// Array.h : header file
//

//#define  LOG_TAG    "hellojni"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


//#ifdef __cplusplus
//extern "C" {
//#endif

template<class TYPE, class ARG_TYPE>
class CContrObArray
{
public:
	CContrObArray();
	CContrObArray(TCHAR* mess) : Name(mess) {}
	CContrObArray(int Sz, int Incr)
	{
		SetSize( Sz, Incr );
	}
	~CContrObArray();
// Attributes
public:
	LPCSTR Name;
	std::vector<TYPE> Array;
	TYPE typeDef;

// Operations
public:
	// Common Functon
	void OutErr(TCHAR * mess);
	void RemoveAt(int Ind, int Count);
	void RemoveAll();
	void SetSize(int Sz, int Incr = -1);
	int GetSize();
	void Add(ARG_TYPE  Obj);
	void Set(int Ind, ARG_TYPE Obj);
	TYPE Get(int Ind);
	void SetAt(int Ind, ARG_TYPE Obj);
	TYPE GetAt(int Ind);
	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);
	void Copy(const CContrObArray& src);
	int Append(const CContrObArray& src);
};


template<class TYPE, class ARG_TYPE>
CContrObArray<TYPE, ARG_TYPE>::CContrObArray ()
{
	//Name = NULL_STRING;
	Name = "";
}

template<class TYPE, class ARG_TYPE>
CContrObArray<TYPE, ARG_TYPE>::~CContrObArray ()
{
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::OutErr(TCHAR * mess)
{
	//	_OutErr(Name+":"+mess);
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::Copy(const CContrObArray& src)
{
	//Array.Copy(src.Array);
	Array = src.Array;
}

template<class TYPE, class ARG_TYPE>
int CContrObArray<TYPE, ARG_TYPE>::Append(const CContrObArray& src)
{
	int index = Array.size();
	Array.insert(Array.end(), src.Array.begin(), src.Array.end());
	return index;
}

template<class TYPE, class ARG_TYPE>
TYPE  CContrObArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
{
	//return Array.GetAt(nIndex);
	if (nIndex < Array.size() && nIndex >= 0)
	{
		return Array.at(nIndex);
	}
	else
	{
		return 0;
	}
}

template<class TYPE, class ARG_TYPE>
TYPE& CContrObArray<TYPE, ARG_TYPE>::operator[](int nIndex)
{
	//return Array.ElementAt(nIndex);
	if (nIndex < Array.size() && nIndex >= 0)
	{
		return Array.at(nIndex);
	}
	else
	{
		OutErr("operator[]!");
		return typeDef;
	}
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::SetSize(int Sz, int Incr)
{
	if (Incr > 0)
		Array.reserve(Incr);
	Array.resize(Sz);
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::RemoveAll()
{
	//Array.clear(0);
	Array.clear();
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::RemoveAt(int Ind, int Count)
{
	if (Ind < Array.size() && Ind >= 0)
	{
		Array.erase(Array.begin() + Ind, Array.begin() + Ind + Count);
	}
	else
	{
		OutErr("Remove!");
	}
}

template<class TYPE, class ARG_TYPE>
int CContrObArray<TYPE, ARG_TYPE>::GetSize()
{
	return Array.size();
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::Set(int Ind, ARG_TYPE Obj)
{
	if (Ind < Array.size() && Ind >= 0)
	{
		Array[Ind] = Obj;
	}
	else
	{
		OutErr("Set!");
	}
}

template<class TYPE, class ARG_TYPE>
TYPE CContrObArray<TYPE, ARG_TYPE>::Get(int Ind)
{
	if (Ind < Array.size() && Ind >= 0)
	{
		return Array[Ind];
	}
	else
	{
		OutErr("Get!");
		if (Array.size()) return Array[0];
		return NULL;
	}
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::SetAt(int Ind, ARG_TYPE Obj)
{
	Set(Ind, Obj);
}

template<class TYPE, class ARG_TYPE>
TYPE CContrObArray<TYPE, ARG_TYPE>::GetAt(int Ind)
{
	return Get(Ind);
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::Add(ARG_TYPE  Obj)
{
	//Array.Add(Obj);
	Array.push_back(Obj);
}

//#ifdef __cplusplus
//}
//#endif

/////////////////////////////////////////////////////////////////////////////
#endif
