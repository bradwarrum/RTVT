/*
 * ByteQueue.h
 *
 * Created: 10/21/2014 3:22:29 PM
 *  Author: ee63pc3-user
 */ 

#ifndef BYTEQUEUE_H_
#define BYTEQUEUE_H_
#include <stdlib.h>
#include "ByteArray.h"

class ByteQueue {

public:
	ByteQueue(int bufferSize);
	void enqueue(char element);
	char dequeue();
	bool isFull();
	bool isEmpty();
	int getCount();
private:
	int max_size;
	volatile int numitems, wptr, rptr;
	ByteArray queue;
};



#endif /* BYTEQUEUE_H_ */