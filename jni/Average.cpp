/**
** Average.cpp
**
** Average.cpp implementation.
**
** Revisions:
**
** 2002/06/27:fle:Original
**
*/

/**
** Required include files.
*/
#include "Queue.h"
#include "Average.h"

/**
**
*/
CRunningAverageFloat::CRunningAverageFloat( unsigned int numSamples )
      :  mQueue                 ( numSamples ),
         mfCurrentRunningAverage(          0 ),
         mfCurrentSum           (          0 )
{
}

/**
**
*/
void CRunningAverageFloat::reinitialize( )
{
   this->mQueue.reinitialize( );

   this->mfCurrentRunningAverage = 0;
   this->mfCurrentSum            = 0;
}

/**
**
*/
CRunningAverageFloat::~CRunningAverageFloat( )
{
}

/**
**
*/
void CRunningAverageFloat::averageNewSample( float newValue )
{
	float dropValue;

	if ( this->mQueue.queueIsFull( ))
	{
		dropValue = ( float )this->mQueue.dequeueFloat();
	}
	else
	{
		dropValue = 0;
	}

	this->mQueue.enqueueFloat( newValue );

	this->mfCurrentSum = this->mfCurrentSum + newValue - dropValue;

	this->mfCurrentRunningAverage =
		 this->mfCurrentSum / ( float )this->mQueue.numElementsGet();
}
