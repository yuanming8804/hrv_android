/**
** Queue.h
**
** Queue.h defines, constants and structures for Queue.c.
**
** Revisions:
**
** 2002/06/26:fle:Original
**
*/
#ifndef __Queue_h__
#define __Queue_h__

/**
** ¶ÓÁÐÔªËØ
*/
typedef union _UQueueElement // not UNICODE
{
   unsigned long   mul;
     signed long   ml;
/**/
   unsigned int    mui;
     signed int    mi;
/**/
   unsigned short  musi;
     signed short  msi;
/**/
   unsigned char   muc;
     signed char   mc;
/**/
   float           mf;
/**/
/**/

} UQueueElement;


/**
**
*/
class CQueue
{
private:
	unsigned long   mulCapacity;
	unsigned long   mulNumElements;
	unsigned long   mulHead;
	unsigned long   mulTail;
	UQueueElement * mpQueueElement;

	void reinitQueueElements();

public:
	CQueue( unsigned long capacity = 10 );

	virtual ~CQueue();

	void reinitialize();

	unsigned long queueCapacityGet()
	{
		return this->mulCapacity - 1;
	}

	void * queueArrayPtrGet( )
	{
		return this->mpQueueElement;
	}

	unsigned long queueHeadGet( )
	{
		return this->mulHead;
	}

	unsigned long queueTailGet( )
	{
		return this->mulTail;
	}

	float dequeueFloat();

	bool enqueueFloat( float value );

	unsigned long nextTailNonDestructive();

	unsigned long nextHeadNonDestructive();

	unsigned long numElementsGet()
	{
		return this->mulNumElements;
	}

	bool queueIsEmpty()
	{
		return ( this->mulHead == this->mulTail );
	}

	bool queueIsFull()
	{
		return ( this->mulHead == this->nextTailNonDestructive( ));
	}

};

/**
** End of Queue.h
*/

#endif   /* if !defined ( __Queue_h__ ) */

