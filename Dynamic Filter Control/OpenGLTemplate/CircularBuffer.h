#pragma once
class CCircularBuffer
{
public:
	CCircularBuffer(long buffSize);
	~CCircularBuffer();
	float AtPosition(int index);
	void Put(float value);
	int PutCount();
	int Size();

private:

	int bufferLength;
	float* buffer;
	int tail;

};
