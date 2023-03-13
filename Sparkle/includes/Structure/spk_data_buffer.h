#pragma once

#include <vector>
#include <stdexcept>

namespace spk
{
	class DataBuffer
	{
	private:
		unsigned int _id;
		size_t _size;
		size_t _readed;

		std::vector<uint8_t> _content;

	public:
		DataBuffer()
		{
			clear();
		}

		bool empty() const
		{
			return (_readed >= _size);
		}

		size_t getSize() const
		{
			return _size;
		}

		size_t getReaded() const
		{
			return _readed;
		}

		void clear()
		{
			_id = 0;
			_size = 0;
			_readed = 0;
			_content.clear();
		}

		template<typename DataType>
		DataBuffer& operator << (const DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			size_t old_size = _content.size();
			size_t data_size = sizeof(DataType);

			if (_content.max_size() - _content.size() < data_size) {
				throw std::length_error("DataBuffer: adding this data will exceed maximum size of the vector.");
			}

			_content.resize(_content.size() + sizeof(DataType));

			std::memcpy(_content.data() + old_size, &data, sizeof(DataType));

			_size = _content.size();

			return (*this);
		}

		template<typename DataType>
		DataBuffer& operator >> (DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

			size_t next_size = _readed;

			std::memcpy(&data, _content.data() + next_size, sizeof(DataType));

			_readed += sizeof(DataType);

			return (*this);
		}
	};
}