/*
 * ByteArray.h
 *
 * Created: 10/22/2014 3:55:47 PM
 *  Author: ee63pc3-user
 */ 


#ifndef BYTEARRAY_H_
#define BYTEARRAY_H_
#include <stdlib.h>

class ByteArray {
public:
	ByteArray(int size);
	~ByteArray();
	char at(int i);
	void set(int i, char c);
	int size();
private:
	int _size;
	char * array;
};




#endif /* BYTEARRAY_H_ */