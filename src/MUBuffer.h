/*
Use for assign memory of binary data
*/

#ifndef __MUBUFFER_H__
#define __MUBUFFER_H__

class MUBuffer {
public:
	MUBuffer();
	~MUBuffer();

public:

	bool resize(int size);

	void write(void *data, int len);

	void shift(int sflen);

	void *buffer();

	int length();

	void *originBuffer();

	int originLength();

	template<T>
	bool read(T &data);

	void clearup();

	void reset();

	MUBuffer & operator = (const MUBuffer& buffer);

private:
	int _size;
	int _length;
	int _shift;
	void *_data;
};

#endif //__MUBUFFER_H__
