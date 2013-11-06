
/**
** DataCollector.h
**
** DataCollector.h defines, constants and structures for DataCollector.c.
**
** Revisions:
**
** 2002/05/04:fle:Original
**
*/


#if !defined ( __DataCollector_h__ )

#define __DataCollector_h__


/**
** Required include files.
*/

#include "Average.h"
#include "FirFilter.h"
#include "FFDefs.h"


/**
**
*/
///////CALLBACKS////////////////////////
typedef int (*pNewIBI)(int iIBI);

/**
**
*/

#define defNumAverageSensorSamples      (      8 )

#define defPpgMultiplier                (   2.0f )
#define defPpgMultiplierStr             (  "2.0" )

#define defIbiMultiplier                (   5.113f )
#define defIbiMultiplierStr             (  "5.113" )

#define defFirThreshold                 (     51 )
#define defFirHysteresis                (      4 )

#define defFirNumFilters                (     10 )

/**
**
*/

class CDataCollector
{
private:
   CRunningAverageFloat   mAverageSensorSamples;
   CFirFilterHighPass     mFirFilterHighPass[ defFirNumFilters ];
   CFirFilterLowPass      mFirFilterLowPass [ defFirNumFilters ];
   int                    miAverageSensorSampleCount;
   bool                   mbReportAverageSensorSample;
   bool                   mbReportIbi;
   int                    miIbiCurrentCount;
   unsigned int           muiIbiSampleCount;
   unsigned int           muiPpgSampleCount;
   int                    miThreshold;
   int                    miHysteresis;
   bool                   mbInPeak;
   int                    miIbiLatestInterval;

public:
   CDataCollector( int numAverageSensorSamples = defNumAverageSensorSamples,
                   int firThreshold            = defFirThreshold,
                   int firHysteresis           = defFirHysteresis );

   virtual ~CDataCollector();


   void reinitialize();

   float averageSensorSampleGet()
   {
		this->mbReportAverageSensorSample = false;
		return this->mAverageSensorSamples.currentRunningAverageGet();
   }

   void collectNewSamplePpg( unsigned int value );

   void collectNewSampleIbi( unsigned int value );


   int latestIbiGet()
   {
		this->mbReportIbi = false;
		return this->miIbiLatestInterval;
   }

   bool reportAverageSensorSampleGet()
   {
		return this->mbReportAverageSensorSample;
   }

   bool reportIbiGet()
   {
		return this->mbReportIbi;
   }

   void risingEdgeIbi( int value );
////////////CALLBACK/////////////////
   void reportNewIBI( int iIBI);
};


/**
**
*/

/**
** End of DataCollector.h
*/

#endif   /* if !defined ( __DataCollector_h__ ) */


