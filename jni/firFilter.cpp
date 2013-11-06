
/**
** Required include files.
*/

//#include "StdAfx.h"
#include "firFilter.h"


/**
**
*/
double firCoeficientsHighPass[ defFirNumCoeficients ] =
{
	// constants are in same order as the samples for multiplying
	-0.0910069,          //  9 or 10
	-0.0910069,          //  8     9
	-0.0910069,          //  7     8
	-0.0910069,          //  6     7
	-0.0910069,          //  5     6
	-0.0910069,          //  4     5
	-0.0910069,          //  3     4
	-0.0910069,          //  2     3
	-0.0910069,          //  1     2
	0.9101598            //  0 or  1
};


/**
**
*/
double firCoeficientsLowPass[ defFirNumCoeficients ] =
{
	0.1662567,
	0.0712959,
	0.0813788,
	0.0886387,
	0.0924299,
	0.0924299,
	0.0886387,
	0.0813788,
	0.0712959,
	0.1662567
};


/**
**
*/

/**
**
*/
CFirFilter::CFirFilter( )
{
	this->reinitialize( );
}


/**
**
*/
void CFirFilter::reinitialize( )
{
   for ( int i = 0; i < defFirNumSamples; i++ )
   {
      this->mFirSamples[ i ] = 0;
   }
}


/**
**
*/
CFirFilter::~CFirFilter()
{
}


/**
**
*/

/**
** FirFilter
**
** FirFilter implements a high pass or a low pass filter, depending
** on the pFirCoefficients.  The High Pass filter eliminates the
** noise due to breathing.  The low pass filter eliminates the noise
** due to the rebound pressure wave (the dichrotic (sp?) notch).
** Both of these are necessary to reduce the incidence of artifacts.
**
** We collect ten samples where each sample is offset by ten,
** then we apply the constants to the samples to generate the
** next filtered value.
**
** The new sample always goes at the end of the set of samples.
** After we multiply, we shift each sample down one to be ready
** for the next sample.
*/

int CFirFilter::filterNewValue( double * pFirCoefficients,
                                int      value             )
{

   this->mFirSamples[ defFirNumSamples - 1 ] = value;

   double dFirValue = pFirCoefficients[ 0 ] * ( double )this->mFirSamples[ 0 ];

   for ( int idx = 1; idx < defFirNumSamples; idx++ )
   {
      int firSamp = this->mFirSamples[ idx ];

      dFirValue += pFirCoefficients[ idx ] * ( double )firSamp;

                                    // slide value down one for next time
      this->mFirSamples[ idx - 1 ] = firSamp;
   }


   int iFirValue = ( int )( dFirValue + 0.5 );

   return iFirValue;
}


/**
**
*/

/**
** End of firFilter.c
*/


