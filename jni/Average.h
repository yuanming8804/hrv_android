/**
** Average.h
**
** Average.h defines, constants and structures for Average.c.
**
** Revisions:
**
** 2002/06/27:fle:Original
**
*/
#if !defined ( __Average_h__ )

#define __Average_h__

/**
** Required include files.
*/
#include "Queue.h"

/**
**
*/
class CRunningAverageFloat
{
private:
	CQueue   mQueue;
	float    mfCurrentSum;
	float    mfCurrentRunningAverage;

public:
	CRunningAverageFloat( unsigned int numSamples = 10 );

	virtual ~CRunningAverageFloat();

	void reinitialize();

	void averageNewSample( float value );

	float currentRunningAverageGet( )
	{
		return this->mfCurrentRunningAverage;
	}

};

#endif   /* if !defined ( __Average_h__ ) */




