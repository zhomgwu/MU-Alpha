/*
load configuration format like:
key1=value1
key2=value2
...
line as annotation if '#' is line start.
*/

#ifndef __MUCONFIG_H__
#define __MUCONFIG_H__

#include "MUBuffer.h"

class MUConfig {
public:
	MUConfig();
	~MUConfig();

public:

	bool loadConfig(const char *file);

	int totalLines();

	bool nextLine();

	template<T>
	bool getLineKey(T& key);

	template<T>
	bool getLineValue(T& value);

private:
	
	int _totalLines;
	char _filePath[256];
	MUBuffer _buffer;
};

#endif //__MUCONFIG_H__
