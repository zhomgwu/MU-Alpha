#include "MUConfig.h"

MUConfig::MUConfig() {

}

MUConfig::~MUConfig() {

}

bool MUConfig::loadConfig(const char *file) {
	return true;
}

int MUConfig::totalLines() {
	return 0;
}

bool MUConfig::nextLine() {
	return true;
}

template<T>
bool MUConfig::getLineKey(T& key) {
	return true;
}

template<T>
bool MUConfig::getLineValue(T& value) {
	return true;
}
