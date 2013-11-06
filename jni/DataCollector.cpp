/**
** DataCollector.c
**
** DataCollector.c implementation.
**
** Revisions:
**
** 2002/06/27:fle:Original
**
*/

// Amplitude
// |
// |
// |
// | x -- FIR Threshold, in peak = True
// |
// |                      . .                             . .
// |    in peak == true  .   .          in peak == true  .   .
// |                    .     .                         .     .
// |                   .       .                       .       .
// |                  .         .                     .         .
// |                 .           .                   .           .
// |                .             .                 .             .
// |         -  -  x  -  -   -  -  x  -  -   -  -  x  -  -   -  -  x - -
// |              .                 .             .                 .
// |             .                   .           .                   .
// | .  in peak .                     . in peak .                     .
// |  .  false .                       . false .                       .
// |    .     .                         .     .                         .
// |      . .                             . .                             .
//  ------------------------------------------------------------------
//                          2 seconds
//



/**
** Required include files.
*/

#include "PreInclude.h"
#include "DataCollector.h"


/**
**
*/
extern pNewIBI NewIBI;
/**
**
*/

CDataCollector::CDataCollector( int numAverageSensorSamples,
                                int firThreshold,
                                int firHysteresis )
   :
      mAverageSensorSamples         ( numAverageSensorSamples ),
      miAverageSensorSampleCount    ( 0                       ),
      mbReportAverageSensorSample   ( false                   ),
      mbReportIbi                   ( false                   ),
      miIbiCurrentCount             ( 0                       ),
      muiIbiSampleCount             ( 0                       ),
      muiPpgSampleCount             ( 0                       ),
      miThreshold                   ( firThreshold            ),
      miHysteresis                  ( firHysteresis           ),
      mbInPeak                      ( false                   ),
      miIbiLatestInterval           ( 0                       )
{
}


/**
**
*/
void CDataCollector::reinitialize( )
{
	this->mAverageSensorSamples.reinitialize( );

	for ( int i = 0; i < defFirNumFilters; i++ )
	{
		this->mFirFilterHighPass[ i ].reinitializeHp( );

		this->mFirFilterLowPass[ i ].reinitializeLp( );
	}

	this->miAverageSensorSampleCount  = 0;
	this->mbReportAverageSensorSample = false;
	this->mbReportIbi                 = false;
	this->miIbiCurrentCount           = 0;
	this->muiIbiSampleCount           = 0;
	this->muiPpgSampleCount           = 0;
	this->mbInPeak                    = false;
	this->miIbiLatestInterval         = 0;
}


/**
**
*/
CDataCollector::~CDataCollector()
{
}


/**
**
** grep:report IBI frequency
**
** the report rate of the IBI controls time: the faster the
** report rate the faster time moves; we need to report IBIs
** at the rate of 65 times a second.  We are called 390
** times a second, so every 6 samples gives us the desired rate.
**
*/

#define defIbiReportRate      ( 8 )


/**
**
*/
void CDataCollector::collectNewSampleIbi( unsigned int orgValue )
{
	this->muiIbiSampleCount++;

	if (( this->muiIbiSampleCount % defIbiReportRate ) == 0 )
	{
		this->mbReportIbi = true;
	}

	int firIdx;
	int iFirValueHighPass;
	int iFirValueLowPass;

	firIdx = ( this->muiIbiSampleCount % defFirNumFilters );

	iFirValueHighPass = this->mFirFilterHighPass[ firIdx ].filterNewValueHp(( int )orgValue );

	iFirValueLowPass = this->mFirFilterLowPass[ firIdx ].filterNewValueLp( iFirValueHighPass );

	this->risingEdgeIbi( iFirValueLowPass );
}


/**
** CDataCollector::collectNewSamplePpg
**
** Called 390 times a second -- once for each sample.  FF expects
** reports at 260 times/second.
**
*/

// grep:report PPG frequency

#define defNextPpgReport   ( 4 )


void CDataCollector::collectNewSamplePpg( unsigned int orgValue )
{
	this->muiPpgSampleCount++;

	this->mAverageSensorSamples.averageNewSample(( float )orgValue );

	// reduce sample rate from 390 to 260
	if (( this->muiPpgSampleCount % 2 ) < 1 )
	{
		this->miAverageSensorSampleCount++;

		if ( this->miAverageSensorSampleCount >= defNextPpgReport )
		{
			this->miAverageSensorSampleCount = 0;
			this->mbReportAverageSensorSample = true;
		}
	}
}


/**
**
*/
void CDataCollector::risingEdgeIbi( int modValue )
{

   this->miIbiCurrentCount++;
//////////////////////////////////////////////
   if(this->miIbiCurrentCount >= 2500)
		this->miIbiLatestInterval = 1000;
//////////////////////////////////////////////

   if ( this->mbInPeak == false )
   {
      if ( modValue < this->miThreshold - this->miHysteresis )
      {
         this->mbInPeak = true;

         this->miIbiLatestInterval = this->miIbiCurrentCount;

         this->miIbiCurrentCount = 0;

         if ( this->miIbiLatestInterval > defIbiIntervalMax )
         {
            this->miIbiLatestInterval = defIbiIntervalMax;
         }
         else if ( this->miIbiLatestInterval < defIbiIntervalMin )
         {
            this->miIbiLatestInterval = defIbiIntervalMin;
         }

		 reportNewIBI(this->miIbiLatestInterval);
      }
   }
   else
   {
      if ( modValue > this->miThreshold + this->miHysteresis )
      {
         this->mbInPeak = false;
      }
   }
}

void CDataCollector::reportNewIBI( int iIBI )
{
	int ibi = ( int )(( iIBI * 2 ) + 0.5f );
	if (NewIBI != NULL)
	{
		(*NewIBI)(ibi);
	}
}

/**
** End of DataCollector.c
*/
