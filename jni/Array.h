#ifndef _CntrArray__h
#define _CntrArray__h

#include <vector>
#include <PreInclude.h>

// Array.h : header file
//
//using namespace std;

template<class TYPE, class ARG_TYPE>
class CContrObArray
{
public:
	CContrObArray();
	CContrObArray( int Sz, int Incr )
	{
		SetSize( Sz, Incr );
	}
	~CContrObArray();
// Attributes
public:
	LPCSTR Name;
	std::vector<TYPE> Array;

// Operations
public:
	// Common Functon
	void OutErr(TCHAR * mess);
	void RemoveAt(int Ind, int Count);
	void RemoveAll();
	void SetSize(int Sz, int Incr = -1);
	int GetSize();
	void Add(ARG_TYPE Obj);
	void Set(int Ind, ARG_TYPE Obj);
	TYPE Get(int Ind);
	void SetAt(int Ind, ARG_TYPE Obj);
	TYPE GetAt(int Ind);
	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);
	void Copy( const CContrObArray& src );
	int Append( const CContrObArray& src );
};

//#include "Array.cpp"
/////////////////////////////////////////////////////////////////////////////
#endif
