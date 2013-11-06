/**
** Queue.cpp
**
** Queue.cpp implementation.
**
** Revisions:
**
** 2002/06/26:fle:Original
**
*/

/**
** Required include files.
*/

#include <PreInclude.h>
#include "Queue.h"

/**
**
*/
CQueue::CQueue( unsigned long capacity )
      :  mulNumElements(    0 ),
         mulHead       (    0 ),
         mulTail       (    0 ),
         mulCapacity   (    0 ),
         mpQueueElement( NULL )

{
	this->mpQueueElement = new UQueueElement [ capacity + 1 ];

	if ( this->mpQueueElement != NULL )
	{
		capacity++;		// to hold n elements, must allocate room for n + 1

		this->mulCapacity = capacity;

		reinitQueueElements();
	}
}

/**
**
*/
void CQueue::reinitialize( )
{
   this->mulNumElements = 0;
   this->mulHead        = 0;
   this->mulTail        = 0;

   reinitQueueElements();
}


/**
**
*/
void CQueue::reinitQueueElements()
{
	for ( unsigned long i = 0; i < this->mulCapacity; i++ )
	{
		this->mpQueueElement[ i ].mul = 0;
	}
}

/**
**
*/
CQueue::~CQueue()
{
	if ( this->mpQueueElement != NULL )
	{
		delete [] this->mpQueueElement;
		this->mpQueueElement = NULL;
	}
}

/**
**
*/
float CQueue::dequeueFloat()
{
   if ( this->queueIsEmpty( ))
      return 0;


   float val = this->mpQueueElement[ mulHead ].mf;

   this->mulHead = this->nextHeadNonDestructive();

   this->mulNumElements--;

   return val;
}

/**
**
*/
bool CQueue::enqueueFloat( float value )
{
   if ( this->queueIsFull( ))
   {
      return false;
   }

   this->mpQueueElement[ this->mulTail ].mf = value;

   this->mulTail = this->nextTailNonDestructive( );

   this->mulNumElements++;

   return true;
}

/**
**
*/
unsigned long CQueue::nextHeadNonDestructive()
{
   unsigned long tmpHead = this->mulHead + 1;

   if ( tmpHead >= this->mulCapacity )
      tmpHead = 0;

   return tmpHead;
}

/**
**
*/
unsigned long CQueue::nextTailNonDestructive()
{
   unsigned long tmpTail = this->mulTail + 1;

   if ( tmpTail >= this->mulCapacity )
   {
      tmpTail = 0;
   }

   return tmpTail;
}

/**
** End of Queue.c
*/


