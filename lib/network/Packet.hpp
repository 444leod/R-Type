/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Packet.hpp
*/

#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <utility>
#include <vector>
#include <cstddef>
#include <cstring>
#include <iostream>

enum class PACKET_TYPE {
    NONE = 0,
    CONNECT,
    DISCONNECT
};

 inline std::ostream& operator<<(std::ostream& os, const std::byte &byte) {
    os << static_cast<int>(byte);
    return os;
}

inline std::string print_bytes(const std::byte *bytes, const std::size_t size)
{
    std::string str;
    for (std::size_t i = 0; i < size; i++) {
        str += std::to_string(static_cast<int>(bytes[i])) + " ";
    }
    return str;
}

inline std::ostream& operator<<(std::ostream& os, const std::vector<std::byte> &bytes)
{
    os << bytes[0];
    for (std::size_t i = 1; i < bytes.size(); i++) {
        os << " " << bytes[i];
    }
    return os;
}

class Packet {
    class Exception final : public std::exception {
        public:
            explicit Exception(std::string  message) : _message(std::move(message)) {}
            [[nodiscard]] const char *what() const noexcept override { return this->_message.c_str(); }
        private:
            std::string _message;
    };

public:
    Packet() = default;
    Packet(const std::vector<std::byte>& bytes) noexcept
    {
        this->_data = bytes;
        this->_size = bytes.size();
        this->_end = this->_data.end();
    }
    ~Packet() = default;

    Packet& operator<<(const std::string& str)
    {
        std::cout << "string!!!!" << std::endl;
        this->raw_append(str.size());
        std::cout << "azezae" << std::endl;
        this->raw_append(sizeof(char));
        std::cout << "azezae" << std::endl;
        this->raw_append(str);
        return *this;
    }

     Packet& operator<<(const char *str)
    {
        *this << std::string(str);
        return *this;
    }

     template<typename T>
     Packet& operator<<(const std::vector<T&>& vector)
     {
        const std::uint8_t len = vector.size();
        const std::uint8_t size = sizeof(T);


         return *this;
     }

    template<typename T>
    Packet& operator<<(T const& data) noexcept
    {
        const std::uint8_t size = sizeof(T);
        std::cout << "size: " << static_cast<unsigned int>(size) << std::endl;

        this->append(&data, size);

//        const auto *raw_len = reinterpret_cast<const std::byte *>(&size);
//        const auto *raw_data = reinterpret_cast<const std::byte *>(data);
//
//        this->_data.insert(_end, raw_len, raw_len + sizeof(std::size_t));
//        this->_data.insert(_end, raw_data, raw_data + sizeof(data));
//        this->_size += sizeof(data) + sizeof(std::size_t);
//        this->_end = this->_data.end();

        return *this;
    }

     Packet& operator>>(std::string& str)
    {
        if (_pos == _data.size())
            throw Exception("No remaining data to unpack.");


        return *this;
    }

     template<typename T>
     Packet& operator>>(std::vector<T&>& vector)
    {
        return *this;
    }

    template<typename T>
    Packet& operator>>(T& data)
     {
        if (_pos == _data.size())
            throw Exception("No remaining data to unpack.");
        const std::uint8_t size = sizeof(T);
        const auto len = this->get<std::uint8_t>();

        std::cout << "len: " << static_cast<unsigned int>(len) << ", size: " << static_cast<unsigned int>(size) << std::endl;

        if (len != size)
            throw Exception("Invalid data size.");

        _pos += sizeof(std::uint8_t); //only in case the len is correct!

        this->raw_copy(&data, size);
        return *this;
     }

    void append(const void *data, const std::uint8_t size) noexcept
    {
        std::cout << data << std::endl;
        const auto *raw_len = reinterpret_cast<const std::byte *>(&size);
        const auto *raw_data = static_cast<const std::byte *>(data);

        std::cout << "raw_len: " << print_bytes(raw_len, sizeof(unsigned char)) << std::endl;
        std::cout << "raw_data: " << print_bytes(raw_data, size) << std::endl;

        this->raw_append(raw_len, sizeof(std::uint8_t));
        this->raw_append(raw_data, size);
    }

    [[nodiscard]] std::vector<std::byte> raw() const noexcept
    {
        return this->_data;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return this->_size;
    }

    [[nodiscard]] std::size_t remains() const noexcept
    {
        return this->_size - this->_pos;
    }

private:
    void raw_append(const std::byte *data, const std::uint8_t& size)
    {
        std::cout << "raw appending: " << print_bytes(data, size) << std::endl;
        this->_data.insert(_end, data, data + size);
        this->_size += size;
        this->_end = this->_data.end();
    }

     void raw_append(const void *data, const std::uint8_t& size)
    {
        const auto *raw = reinterpret_cast<const std::byte *>(&data);
        this->raw_append(raw, size);
    }

     template<typename T>
     void raw_append(const T* data)
    {
        this->raw_append(data, sizeof(T));
    }

     template<typename T>
     void raw_append(const T& data)
    {
        this->raw_append(&data);
    }

     template<typename T>
     T raw_copy()
    {
        T var;
        this->raw_copy(&var);
        return var;
    }

    template<typename T>
    void raw_copy(T *dest)
    {
        this->raw_copy(dest, sizeof(T));
    }

    void raw_copy(void *dest, const std::uint8_t& size)
    {
        std::memcpy(dest, &(this->_data[this->_pos]), size);
        _pos += size;
    }

    void get(void *dest, const std::uint8_t size) const
    {
        std::memcpy(dest, &(this->_data[this->_pos]), size);
    }

     template<typename T>
     void get(T *data) const
    {
        this->get(data, sizeof(T));
    }

     template<typename T>
     T get() const
    {
        T var;
        this->get(&var);
        return var;
    }

    PACKET_TYPE _type = PACKET_TYPE::NONE;
    std::vector<std::byte> _data = {};
    std::vector<std::byte>::iterator _end = _data.begin();
    std::vector<std::byte>::iterator _begin = _data.begin();
    std::size_t _pos = 0;
    std::size_t _size = 0;
};



#endif //PACKET_HPP
