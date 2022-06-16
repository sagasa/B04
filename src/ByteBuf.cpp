#include "ByteBuf.h"

ByteBuf::ByteBuf(uint32_t size) {
	buf.reserve(size);
	clear();
}

ByteBuf::ByteBuf(uint8_t* arr, uint32_t size) {
	if (arr == NULL) {
		buf.reserve(size);
		clear();
	}
	else {
		buf.reserve(size);
		clear();
		putBytes(arr, size);
	}
}

void ByteBuf::clear() {
	rpos = 0;
	wpos = 0;
	buf.clear();
}

std::unique_ptr<ByteBuf> ByteBuf::clone() {
	std::unique_ptr<ByteBuf> ret = std::make_unique<ByteBuf>(buf.size());

	// Copy data
	for (uint32_t i = 0; i < buf.size(); i++) {
		ret->put((uint8_t)get(i));
	}

	// Reset positions
	ret->setReadPos(0);
	ret->setWritePos(0);

	return ret;
}


/**
 * リサイズ 読み書きのポインタをリセット
 */
void ByteBuf::reset() {
	rpos = 0;
	wpos = 0;
}

uint32_t ByteBuf::size() {
	return buf.size();
}


// Read Functions

uint8_t ByteBuf::get() const {
	return read<uint8_t>();
}

uint8_t ByteBuf::get(uint32_t index) const {
	return read<uint8_t>(index);
}

void ByteBuf::getBytes(uint8_t* buf, uint32_t len) const {
	for (uint32_t i = 0; i < len; i++) {
		buf[i] = read<uint8_t>();
	}
}

char ByteBuf::getChar() const {
	return read<char>();
}

char ByteBuf::getChar(uint32_t index) const {
	return read<char>(index);
}

double ByteBuf::getDouble() const {
	return read<double>();
}

double ByteBuf::getDouble(uint32_t index) const {
	return read<double>(index);
}

float ByteBuf::getFloat() const {
	return read<float>();
}

float ByteBuf::getFloat(uint32_t index) const {
	return read<float>(index);
}

uint32_t ByteBuf::getInt() const {
	return read<uint32_t>();
}

uint32_t ByteBuf::getInt(uint32_t index) const {
	return read<uint32_t>(index);
}

uint64_t ByteBuf::getLong() const {
	return read<uint64_t>();
}

uint64_t ByteBuf::getLong(uint32_t index) const {
	return read<uint64_t>(index);
}

uint16_t ByteBuf::getShort() const {
	return read<uint16_t>();
}

uint16_t ByteBuf::getShort(uint32_t index) const {
	return read<uint16_t>(index);
}

// Write Functions
void ByteBuf::put(uint8_t b) {
	append<uint8_t>(b);
}

void ByteBuf::put(uint8_t b, uint32_t index) {
	insert<uint8_t>(b, index);
}

void ByteBuf::putBytes(uint8_t* b, uint32_t len) {
	// Insert the data one byte at a time into the internal buffer at position i+starting index
	for (uint32_t i = 0; i < len; i++)
		append<uint8_t>(b[i]);
}

void ByteBuf::putBytes(uint8_t* b, uint32_t len, uint32_t index) {
	wpos = index;

	// Insert the data one byte at a time into the internal buffer at position i+starting index
	for (uint32_t i = 0; i < len; i++)
		append<uint8_t>(b[i]);
}

void ByteBuf::putChar(char value) {
	append<char>(value);
}

void ByteBuf::putChar(char value, uint32_t index) {
	insert<char>(value, index);
}

void ByteBuf::putDouble(double value) {
	append<double>(value);
}

void ByteBuf::putDouble(double value, uint32_t index) {
	insert<double>(value, index);
}
void ByteBuf::putFloat(float value) {
	append<float>(value);
}

void ByteBuf::putFloat(float value, uint32_t index) {
	insert<float>(value, index);
}

void ByteBuf::putInt(uint32_t value) {
	append<uint32_t>(value);
}

void ByteBuf::putInt(uint32_t value, uint32_t index) {
	insert<uint32_t>(value, index);
}

void ByteBuf::putLong(uint64_t value) {
	append<uint64_t>(value);
}

void ByteBuf::putLong(uint64_t value, uint32_t index) {
	insert<uint64_t>(value, index);
}

void ByteBuf::putShort(uint16_t value) {
	append<uint16_t>(value);
}

void ByteBuf::putShort(uint16_t value, uint32_t index) {
	insert<uint16_t>(value, index);
}

void* ByteBuf::data()
{
	return buf.data();
}
