#pragma once

#include <sstream>
#include <iostream>
#include <mutex>
#include <string>

namespace spk
{
    class IOStream : public std::ostream
    {
    private:
        class IOBuffer : public std::stringbuf
        {
        private:
            static inline std::recursive_mutex _mutex;
            static inline size_t _maximumPrefixSize = 0;
            std::string _prefix;
            std::ostream& _outputStream;

        public:
            IOBuffer(std::ostream& p_outputStream, const std::string& p_prefix) :
                _outputStream(p_outputStream),
                _prefix("")
            {
                setPrefix(p_prefix);
            }
            ~IOBuffer()
            {
                if (pbase() != pptr()) {
                    flush();
                }
            }

            virtual int sync()
            {
                flush();
                return 0;
            }
            void flush()
            {
                _mutex.lock();
                if (_prefix.size() != 0)
                {
                    _outputStream << "[" << std::string(_maximumPrefixSize - _prefix.size(), ' ') << _prefix << "] - ";
                }
                _outputStream << str();
                str("");
                _outputStream.flush();
                _mutex.unlock();
            }
            void setPrefix(std::string p_prefix)
            {
                _prefix = p_prefix;
                _mutex.lock();
                if (_prefix.size() > _maximumPrefixSize)
                {
                    _maximumPrefixSize = _prefix.size();
                }
                _mutex.unlock();
            }
        };

    public:
        IOBuffer buffer;
    public:
        IOStream(std::ostream& p_outputStream, std::string p_prefix = "") :
            std::ostream(&buffer),
            buffer(p_outputStream, p_prefix)
        {

        }
        void setPrefix(const std::string& p_prefix)
        {
            buffer.setPrefix(p_prefix);
        }
    };

    extern thread_local spk::IOStream cout;
    extern thread_local spk::IOStream cerr;
}