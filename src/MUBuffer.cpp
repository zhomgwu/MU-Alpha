MUBuffer::MUBuffer()
: _size(0)
, _length(0)
, _data(NULL) {
}

MUBuffer::~MUBuffer() {
}

bool MUBuffer::resize(int size) {
	int msize = 32;
	while (msize < size) {
		msize << 1;
	}

	if (_size == 0) {
		_data = malloc(msize);
		_size = msize;
	}
	else if (_size < msize) {
		void *tmp = _data;
		_data = malloc(msize);
		_size = msize;
		memcpy(_data, tmp, _length);
		free(tmp);
	}
	return true;
}

void MUBuffer::write(void *data, int len) {
	if (_length+len > _size) {
		resize(_length+len);
	}
	
	memcpy(_data+length, data, len);
	_length += len;
}

void *MUBuffer::buffer() {
	return _data;
}

void *MUBuffer::shiftBuffer() {
	return _data + _shift;
}

int MUBuffer::length() {
	return _length;
}

template<T>
bool MUBuffer::read(T &data) {
	if (sizeof(data) > (_length - _shift)) {
		return false;
	}
	memcpy(data, _data+_shift, sizeof(data));
	_shift += sizeof(data);
	return true;
}

void MUBuffer::clearup() {
	if (_length > 0) {
		free(_data);
		_length = 0;
		_size = 0;
	}
}
