/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Packet.hpp
*/

#ifndef PACKET_HPP
#define PACKET_HPP

#include <utility>
#include <vector>
#include <cstddef>
#include <cstring>
#include <iostream>

enum class PACKET_TYPE {
    CONNECT = 0,
    DISCONNECT
};

class Packet {
    class Exception final : public std::exception {
        public:
            explicit Exception(std::string  message) : _message(std::move(message)) {}
            [[nodiscard]] const char *what() const noexcept override { return this->_message.c_str(); }
        private:
            std::string _message;
    };

public:
    Packet() noexcept;
    Packet(const std::vector<std::byte>& bytes) noexcept
    {
        this->_data = bytes;
        this->_size = bytes.size();
        this->_end = this->_data.end();
    }
    ~Packet() = default;

    template<typename T>
    Packet& operator<<(T const& data) noexcept
    {
        const std::size_t size = sizeof(T);

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

    template<typename T>
    Packet& operator>>(T& data) noexcept
     {
        const std::size_t size = sizeof(T);
        std::size_t len = 0;
        std::memcpy(&len, &(*_begin), sizeof(std::size_t));

        if (len != size)
            throw Exception("Invalid data size.");

        std::memcpy(&data, &(*_begin), size);
        _begin += sizeof(std::size_t) + size;
        _pos += sizeof(std::size_t) + size;
        return *this;
     }

    void append(const void *data, const std::size_t size) noexcept
    {
        const auto *raw_len = reinterpret_cast<const std::byte *>(&size);
        const auto *raw_data = static_cast<const std::byte *>(data);

        this->_data.insert(_end, raw_len, raw_len + sizeof(std::size_t));
        this->_data.insert(_end, raw_data, raw_data + sizeof(data));
        this->_size += sizeof(data) + sizeof(std::size_t);
        this->_end = this->_data.end();
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
        PACKET_TYPE _type;
        std::vector<std::byte> _data = {};
        std::vector<std::byte>::iterator _end = _data.begin();
        std::vector<std::byte>::iterator _begin = _data.begin();
        std::size_t _pos = 0;
        std::size_t _size = 0;
};



#endif //PACKET_HPP
