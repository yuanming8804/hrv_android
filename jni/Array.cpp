//#include "stdafx.h"
#include "ffDefs.h"
//#include <vector>
#include "Array.h"



//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static TCHAR THIS_FILE[] = __FILE__;
//#endif

/////////////////////////////////////////////////////////////////////////////
// CArray

void _OutErr(LPCSTR add)
{
}


template<class TYPE, class ARG_TYPE>
CContrObArray<TYPE, ARG_TYPE>::CContrObArray ()
{
	Name = NULL_STRING;
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
void CContrObArray<TYPE, ARG_TYPE>::Copy( const CContrObArray& src )
{
	Array.Copy(src.Array);
}

template<class TYPE, class ARG_TYPE>
int CContrObArray<TYPE, ARG_TYPE>::Append( const CContrObArray& src )
{
	int index = Array.size();
	Array.insert(Array.end(),src.Array.begin(),src.Array.end());
	return index;
}

template<class TYPE, class ARG_TYPE>
TYPE  CContrObArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
{
	return Array.GetAt(nIndex);
}

template<class TYPE, class ARG_TYPE>
TYPE& CContrObArray<TYPE, ARG_TYPE>::operator[](int nIndex)
{
	return Array.ElementAt(nIndex);
}


template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::SetSize(int Sz,int Incr)
{
	if(Incr > 0)
		Array.reserve(Incr);
	Array.resize(Sz);
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::RemoveAll()
{
	Array.clear(0);
}

template<class TYPE, class ARG_TYPE>
int CContrObArray<TYPE, ARG_TYPE>::GetSize()
{
	return Array.size();
}

template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::Set(int Ind,ARG_TYPE Obj)
{
	if (Ind<Array.size() && Ind>=0)
	{
		Array[Ind]=Obj;
	}
	else
	{
		OutErr("Set!");
	}

}

template<class TYPE, class ARG_TYPE>
TYPE CContrObArray<TYPE, ARG_TYPE>::Get(int Ind)
{
	if (Ind<Array.size() && Ind>=0)
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
void CContrObArray<TYPE, ARG_TYPE>::SetAt(int Ind,ARG_TYPE Obj)
{
	Set(Ind,Obj);
}

template<class TYPE, class ARG_TYPE>
TYPE CContrObArray<TYPE, ARG_TYPE>::GetAt(int Ind)
{
	return Get(Ind);
}


template<class TYPE, class ARG_TYPE>
void CContrObArray<TYPE, ARG_TYPE>::Add(ARG_TYPE  Obj)
{
	Array.Add(Obj);
}


//CContrObArray<int, int>::CContrObArray ()
template<class TYPE, class ARG_TYPE>
CContrObArray<int, int>::CContrObArray ()
{
	Name = "int ";
}


CContrObArray<int, int>::~CContrObArray ()
{
}



CContrObArray<DWORD, DWORD>::CContrObArray ()
{
	Name = "dword ";
}


CContrObArray<DWORD, DWORD>::~CContrObArray ()
{
}



void CContrObArray<DWORD, DWORD>::SetSize(int Sz, int Incr)
{
	if(Incr > 0)
		Array.reserve(Incr);
	Array.resize(Sz);
}

int CContrObArray<DWORD, DWORD>::GetSize()
{
	return Array.size();
}


void CContrObArray<DWORD, DWORD>::Set(int Ind,DWORD Obj)
{
	if (Ind < Array.size() && Ind>=0)
	{
		Array[Ind]=Obj;
	}
	else
	{
		OutErr("Set!");
	}

}

DWORD CContrObArray<DWORD,DWORD>::Get(int Ind)
{
	if (Ind<Array.size() && Ind>=0)
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

void CContrObArray<DWORD, DWORD>::RemoveAt(int Ind, int Count)
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

void CContrObArray<DWORD, DWORD>::Add(DWORD Obj)
{
	Array.push_back(Obj);
}

CContrObArray<BYTE, BYTE>::CContrObArray ()
{
	Name = "byte ";
}


CContrObArray<BYTE, BYTE>::~CContrObArray ()
{
}

void CContrObArray<BYTE, BYTE>::SetSize(int Sz, int Incr)
{
	if(Incr > 0)
		Array.reserve(Incr);
	Array.resize(Sz);
}

void CContrObArray<BYTE, BYTE>::RemoveAll()
{
	Array.clear();
}

int CContrObArray<BYTE, BYTE>::GetSize()
{
	return Array.size();
}


void CContrObArray<BYTE, BYTE>::Set(int Ind, BYTE Obj)
{
	if (Ind<Array.size() && Ind>=0)
	{
		Array[Ind] = Obj;
	}
	else
	{
		OutErr(" Set!");
	}

}

BYTE CContrObArray<BYTE, BYTE>::Get(int Ind)
{
	if (Ind<Array.size() && Ind >= 0)
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

void CContrObArray<BYTE,BYTE>::Add(BYTE Obj)
{
	Array.push_back(Obj);
}

void CContrObArray<BYTE,BYTE>::RemoveAt(int Ind, int Count)
{
	if (Ind<Array.size() && Ind>= 0)
	{
		Array.erase(Array.begin() + Ind, Array.begin() + Ind + Count);
	}
	else
	{
		OutErr("Remove!");
	}
}

BYTE CContrObArray<BYTE,BYTE>::operator[](int nIndex) const
{
	if (nIndex<Array.size() && nIndex>=0)
	{
		return Array.at(nIndex);
	}
	else
	{
		return 0;
	}
}

BYTE bDef;

BYTE& CContrObArray<BYTE, BYTE>::operator[](int nIndex)
{
	if (nIndex<Array.size() && nIndex>=0)
	{
		return Array.at(nIndex);
	}
	else
	{
		OutErr("operator[]!");
		return bDef;
	}
}

CContrObArray<float,float>::CContrObArray ()
{
	Name = "float ";
}


CContrObArray<float,float>::~CContrObArray ()
{
}

void CContrObArray<float,float>::OutErr(TCHAR * mess)
{
//	_OutErr(Name+":"+mess);
}

void CContrObArray<float,float>::SetSize(int Sz,int Incr)
{
	if(Incr > 0)
		Array.reserve(Incr);
	Array.resize(Sz);
}

void CContrObArray<float,float>::RemoveAll()
{
	Array.clear();
}

int CContrObArray<float,float>::GetSize()
{
	return Array.size();
}

// void CContrObArray<float,float>::Set(int Ind,ARG_TYPE Obj)
void CContrObArray<float,float>::Set(int Ind, float Obj)
{
	if (Ind<Array.size() && Ind>=0)
	{
		Array[Ind] = Obj;
	}
	else
	{
		OutErr("Set!");
	}

}

float CContrObArray<float,float>::Get(int Ind)
{
	if (Ind<Array.size() && Ind>=0)
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


// void CContrObArray<float,float>::Add(ARG_TYPE  Obj)
void CContrObArray<float,float>::Add(float Obj)
{
	Array.push_back(Obj);
}

float  CContrObArray<float,float>::operator[](int nIndex) const
{
	if (nIndex<Array.size() && nIndex>=0)
	{
		return Array.at(nIndex);
	}
	else
	{
		return 0.0f;
	}
}

float Def;

float& CContrObArray<float,float>::operator[](int nIndex)
{
	if (nIndex<Array.size() && nIndex>=0)
	{
		return Array.at(nIndex);
	}
	else
	{
		OutErr("operator[]!");
		return Def;
	}
}

void CContrObArray<float,float>::Copy( const CContrObArray& src )
{
	Array = src.Array;
}

int CContrObArray<float,float>::Append( const CContrObArray& src )
{
	int index = Array.size();
	Array.insert(Array.end(),src.Array.begin(),src.Array.end());
	return index;
}

void CContrObArray<float,float>::SetAt(int Ind,float Obj)
{
	Set(Ind,Obj);
}

float CContrObArray<float,float>::GetAt(int Ind)
{
	return Get(Ind);
}







//------------


void CContrObArray<int, int>::SetSize(int Sz, int Incr)
{
	if(Incr > 0)
		Array.reserve(Incr);
	Array.resize(Sz);
}

void CContrObArray<int, int>::RemoveAll()
{
	Array.clear();
}

int CContrObArray<int, int>::GetSize()
{
	return Array.size();
}

// void CContrObArray<int,int>::Set(int Ind,ARG_TYPE Obj)
void CContrObArray<int, int>::Set(int Ind, int Obj)
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

int CContrObArray<int, int>::Get(int Ind)
{
	if (Ind<Array.size() && Ind>=0)
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


// void CContrObArray<int,int>::Add(ARG_TYPE  Obj)
void CContrObArray<int, int>::Add(int Obj)
{
	Array.push_back(Obj);
}

void CContrObArray<int,int>::RemoveAt(int Ind,int Count)
{
	if (Ind<Array.size() && Ind>=0)
	{
		Array.erase(Array.begin()+Ind,Array.begin()+Ind+Count);
	}
	else
	{
		OutErr("Remove!");
	}
}

int  CContrObArray<int,int>::operator[](int nIndex) const
{
	if (nIndex<Array.size() && nIndex>=0)
	{
		return Array.at(nIndex);
	}
	else
	{
		return 0;
	}
}

int iDef;

int& CContrObArray<int,int>::operator[](int nIndex)
{
	if (nIndex<Array.size() && nIndex>=0)
	{
		return Array.at(nIndex);
	}
	else
	{
		OutErr("operator[]!");
		return iDef;
	}
}

void CContrObArray<int,int>::Copy( const CContrObArray& src )
{
	Array = src.Array;
}

int CContrObArray<int,int>::Append( const CContrObArray& src )
{
	int index = Array.size();
	Array.insert(Array.end(),src.Array.begin(),src.Array.end());
	return index;
}

void CContrObArray<int,int>::SetAt(int Ind,int Obj)
{
	Set(Ind,Obj);
}

int CContrObArray<int,int>::GetAt(int Ind)
{
	return Get(Ind);
}

// ------------------ String
/*
CContrObArray<CString,CString>::CContrObArray ()
{
	Name = "string ";
}


CContrObArray<CString,CString>::~CContrObArray ()
{
}

void CContrObArray<CString,CString>::SetSize(int Sz,int Incr)
{
	if(Incr > 0)
		Array.reserve(Incr);
	Array.resize(Sz);
}

void CContrObArray<CString,CString>::RemoveAll()
{
	Array.clear();
}

int CContrObArray<CString,CString>::GetSize()
{
	return Array.size();
}


void CContrObArray<CString,CString>::Set(int Ind,CString Obj)
{
	if (Ind<Array.size() && Ind>=0)
	{
		Array[Ind]=Obj;
	}
	else
	{
		OutErr(" Set!");
	}

}
CString csDef=NULL_STRING;

CString CContrObArray<CString,CString>::Get(int Ind)
{
	if (Ind<Array.size() && Ind>=0)
	{
		return Array[Ind];
	}
	else
	{
		OutErr(" Get!");
		if (Array.size()) return Array[0];
		return csDef;
	}
}


void CContrObArray<CString,CString>::Add(CString Obj)
{
	Array.push_back(Obj);
}

CString  CContrObArray<CString,CString>::operator[](int nIndex) const
{
	if (nIndex<Array.size() && nIndex>=0)
	{
		return Array.at(nIndex);
	}
	else
	{
		_OutErr(Name+"operator[]");
		return csDef;
	}
}



CString& CContrObArray<CString,CString>::operator[](int nIndex)
{
	if (nIndex<Array.size() && nIndex>=0)
	{
		return Array.at(nIndex);
	}
	else
	{
		OutErr("operator[]!");
		return csDef;
	}
}

void CContrObArray<CString,CString>::Copy( const CContrObArray& src )
{
	Array.Copy(src.Array);
}

int CContrObArray<CString,CString>::Append( const CContrObArray& src )
{
	int index = Array.size();
	Array.insert(Array.end(),src.Array.begin(),src.Array.end());
	return index;
}

void CContrObArray<CString,CString>::SetAt(int Ind,CString Obj)
{
	Set(Ind,Obj);
}

CString CContrObArray<CString,CString>::GetAt(int Ind)
{
	return Get(Ind);
}

*/
