
#ifndef __firFilter_h__
#define __firFilter_h__


/**
** Required include files.
*/

#define defFirNumSamples      ( 10 )
#define defFirNumCoeficients  ( 10 )


extern double firCoeficientsHighPass[ defFirNumCoeficients ];
extern double firCoeficientsLowPass [ defFirNumCoeficients ];


/**
**
*/

/**
**
*/

class CFirFilter
{
private:
   int   mFirSamples[ defFirNumSamples ];

public:
   CFirFilter( );

   virtual ~CFirFilter( );

   void reinitialize();

   int filterNewValue( double * pFirConstants,
                       int      value          );
};


/**
**
*/

/**
**
*/

class CFirFilterHighPass
   :  private CFirFilter
{
public:
   CFirFilterHighPass( )
      : CFirFilter( )
   {
   }

   virtual ~CFirFilterHighPass( )
   {
   }

   void reinitializeHp( )
   {
      this->reinitialize( );
   }

   int filterNewValueHp( int value )
   {
      return this->filterNewValue( firCoeficientsHighPass, value );
   }
};


/**
**
*/

/**
**
*/
class CFirFilterLowPass
   :  private CFirFilter
{
public:

   CFirFilterLowPass( )
      : CFirFilter( )
   {
   }

   virtual ~CFirFilterLowPass( )
   {
   }

   void reinitializeLp( )
   {
      this->reinitialize( );
   }

   int filterNewValueLp( int value )
   {
      return this->filterNewValue( firCoeficientsLowPass, value );
   }
};

/**
** End of firFilter.h
*/

#endif   /* if !defined ( __firFilter_h__ ) */


