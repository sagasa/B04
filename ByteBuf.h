#pragma once

#define DEFAULT_SIZE 4096

//#include <cstdint>
#include <vector>
#include <memory>
class ByteBuf {
public:
	ByteBuf(uint32_t size = DEFAULT_SIZE);
	ByteBuf(uint8_t* arr, uint32_t size);
	~ByteBuf() = default;

	void clear(); // 内容をすべて削除 書き込み読み取り位置をリセット
	std::unique_ptr<ByteBuf> clone(); // 完全に同じ状態のバッファを返す
	void reset();
	uint32_t size(); // Size of internal vector

	// Read
	uint8_t get() const;
	uint8_t get(uint32_t index) const;
	void getBytes(uint8_t* buf, uint32_t len) const;
	char getChar() const;
	char getChar(uint32_t index) const;
	double getDouble() const;
	double getDouble(uint32_t index) const;
	float getFloat() const;
	float getFloat(uint32_t index) const;
	uint32_t getInt() const;
	uint32_t getInt(uint32_t index) const;
	uint64_t getLong() const;
	uint64_t getLong(uint32_t index) const;
	uint16_t getShort() const;
	uint16_t getShort(uint32_t index) const;

	// Write
	void put(uint8_t b); // Relative write
	void put(uint8_t b, uint32_t index); // Absolute write at index
	void putBytes(uint8_t* b, uint32_t len); // Relative write
	void putBytes(uint8_t* b, uint32_t len, uint32_t index); // Absolute write starting at index
	void putChar(char value); // Relative
	void putChar(char value, uint32_t index); // Absolute
	void putDouble(double value);
	void putDouble(double value, uint32_t index);
	void putFloat(float value);
	void putFloat(float value, uint32_t index);
	void putInt(uint32_t value);
	void putInt(uint32_t value, uint32_t index);
	void putLong(uint64_t value);
	void putLong(uint64_t value, uint32_t index);
	void putShort(uint16_t value);
	void putShort(uint16_t value, uint32_t index);

	void* data();

	// 読み取り 書き込み
	void setReadPos(uint32_t r) {
		rpos = r;
	}

	uint32_t getReadPos() const {
		return rpos;
	}

	void setWritePos(uint32_t w) {
		wpos = w;
	}

	uint32_t getWritePos() const {
		return wpos;
	}


private:
	uint32_t wpos;
	mutable uint32_t rpos;
	std::vector<uint8_t> buf;


	template<typename T> T read() const {
		T data = read<T>(rpos);
		rpos += sizeof(T);
		return data;
	}

	template<typename T> T read(uint32_t index) const {
		if (index + sizeof(T) <= buf.size())
			return *((T*)&buf[index]);
		return 0;
	}

	template<typename T> void append(T data) {
		uint32_t s = sizeof(data);
		if (size() < (wpos + s))
			buf.resize(wpos + s);
		memcpy(&buf[wpos], (uint8_t*)&data, s);
		wpos += s;
	}

	template<typename T> void insert(T data, uint32_t index) {
		if ((index + sizeof(data)) > size())
			return;
		memcpy(&buf[index], (uint8_t*)&data, sizeof(data));
		wpos = index + sizeof(data);
	}
};