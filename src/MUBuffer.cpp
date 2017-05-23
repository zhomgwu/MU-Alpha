MUBuffer::MUBuffer()
: _size(0)
, _length(0)
, _data(NULL) {
}

MUBuffer::~MUBuffer() {
	if (_data != NULL) {
		delete _data;
		_data = NULL;
	}
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
	return (char*)_data + _shift;
}

int MUBuffer::length() {
	return _length - shift;
}

void MUBuffer::shift(int sflen) {
	_shift += sflen;
}

void *MUBuffer::originBuffer() {
	return _data;
}

int MUBuffer::originLength() {
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

void MUBuffer::reset() {
	if (_shift != 0) {	
		_length = 0;
		_shift = 0;
		memset(_data, 0, _size);	
	}
}

MUBuffer& MUBuffer::operator = (const MUBuffer& buffer) {
	resize(buffer._size)
	_length = buffer._length;
	_shift = buffer._shift;
	memcpy(_data, buffer._data, buffer._length);
}
