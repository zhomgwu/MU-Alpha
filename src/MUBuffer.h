/*
Use for assign memory
*/

#ifndef __MU_BUFFER_H__
#define __MU_BUFFER_H__

class MUBuffer {
public:
	MUBuffer();
	~MUBuffer();

public:

	bool resize(int size);

	void write(void *data, int len);

	void *buffer();

	int length();

	template<T>
	bool read(T &data);

	void clearup();

private:
	int _size;
	int _length;
	int _shift;
	void *_data;
};

#endif //__MU_BUFFER_H__
