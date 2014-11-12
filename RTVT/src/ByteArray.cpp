/*
 * ByteArray.cpp
 *
 * Created: 10/22/2014 3:58:33 PM
 *  Author: ee63pc3-user
 */ 

 #include "ByteArray.h"

 ByteArray::ByteArray(int size) {
	this->_size = size;
	array = (char *) malloc(sizeof(char) * size);
 }

 ByteArray::~ByteArray() {
	free(array);
 }

char ByteArray::at(int i) {
	return array[i];
 }
 void ByteArray::set(int i, char c) {
	array[i] = c;
 }
 int ByteArray::size() {return _size;}
