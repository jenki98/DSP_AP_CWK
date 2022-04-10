#include "CircularBuffer.h"

CCircularBuffer::CCircularBuffer(long buffSize) {
	bufferLength = buffSize;
	buffer = new float[buffSize];
	tail = 0;
}

CCircularBuffer::~CCircularBuffer()
{
	delete buffer;
}

// return the array value given an arbitrary position, using the
	// modulus "%" division command
float CCircularBuffer::AtPosition(int index) {
	return buffer[index % bufferLength];
}

// add a new value at position [tail % length]
void CCircularBuffer::Put(float value) {
	buffer[tail % bufferLength] = value;
	tail++;
}

// return how many floats have been written
int CCircularBuffer::PutCount() {
	return tail;
}

// return length of the buffer
int CCircularBuffer::Size() {
	return bufferLength;
}