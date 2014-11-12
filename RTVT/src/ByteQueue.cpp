/*
 * ByteQueue.cpp
 *
 * Created: 10/21/2014 3:23:01 PM
 *  Author: ee63pc3-user
 */ 

 #include "ByteQueue.h"

 ByteQueue::ByteQueue(int bufferSize) : queue(bufferSize){
	max_size = bufferSize;
	wptr = 0;
	rptr = 0;
	numitems = 0;
 }



 void ByteQueue::enqueue(char element){
	if (!isFull()) {
		queue.set(wptr, element);
		//Took this out because modulus is very expensive on micro
		//(Since we're incrementing by one every time, we don't have to deal with any cases where our
		// pointer is over 2*max_size - 1, this should be more efficient)
		//wptr = (wptr + 1) % (max_size-1);
		if ((++wptr) >= max_size) wptr = wptr - max_size;
		numitems++;
	}
 }

 char ByteQueue::dequeue(){
 	char element = '\0';
	if (!isEmpty()) {
	 	element = queue.at(rptr);
		//Read comment in enqueue()
	 	//rptr = (rptr + 1) % (max_size-1)
		if ((++rptr) >= max_size) rptr = rptr - max_size;
	 	numitems--;
 	}
	return element;
 }

 bool ByteQueue::isFull() {return numitems == max_size;}
 bool ByteQueue::isEmpty(){return numitems == 0;}
 int ByteQueue::getCount(){return numitems;}