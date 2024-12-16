/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** UDPPacket
*/

#pragma once

#include <vector>
#include <string>
#include <cstring>

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
    if (bytes.empty())
        return os;
    os << bytes[0];
    for (std::size_t i = 1; i < bytes.size(); i++) {
        os << " " << bytes[i];
    }
    return os;
}

struct UDPPacket {
    uint32_t sequence_number = 0;
    uint32_t ack_number = 0;
    uint16_t payload_length = 0;
    uint16_t checksum = 0;
    std::vector<std::byte> payload;

    class Exception final : public std::exception {
    public:
        explicit Exception(const std::string  &message,
                    const char* function = __builtin_FUNCTION(),
                    int line = __builtin_LINE()) : _message(message + " in function: " + function + " at line: " + std::to_string(line)) {}
        [[nodiscard]] const char *what() const noexcept override { return this->_message.c_str(); }
    private:
        std::string _message;
    };

    // String constructor (sending)
    explicit UDPPacket() = default;

    // Raw data constructor (receiving data and deserializing)
    UDPPacket(const char* data, const size_t& length)
    {
        _deserialize(data, length);
    }

    [[nodiscard]] std::vector<uint8_t> serialize() const
    {
        std::vector<uint8_t> buffer;
        buffer.resize(sizeof(sequence_number) + sizeof(ack_number) +
                     sizeof(payload_length) + sizeof(checksum) +
                     payload.size());

        size_t offset = 0;
        memcpy(buffer.data() + offset, &sequence_number, sizeof(sequence_number));
        offset += sizeof(sequence_number);
        memcpy(buffer.data() + offset, &ack_number, sizeof(ack_number));
        offset += sizeof(ack_number);
        memcpy(buffer.data() + offset, &payload_length, sizeof(payload_length));
        offset += sizeof(payload_length);
        memcpy(buffer.data() + offset, &checksum, sizeof(checksum));
        offset += sizeof(checksum);
        memcpy(buffer.data() + offset, payload.data(), payload.size());

        return buffer;
    }

    [[nodiscard]] uint16_t calculateChecksum() const {
        uint16_t sum = 0;

        // little explaination: sequence and ack are 32 bits, but checksum is 16 bits (for optimization purposes)
        // in order to checkum the whole 32 bits, we need to split them into 16 bits chunks
        // for exemple: 0x12345678 -> 0x1234 + 0x5678
        sum += (sequence_number >> 16) & 0xFFFF; // 0x00001234 => 0x1234
        sum += sequence_number & 0xFFFF; // 0x5678

        sum += (ack_number >> 16) & 0xFFFF;
        sum += ack_number & 0xFFFF;

        sum += payload_length;

        for (const auto& byte : payload) {
            sum += static_cast<unsigned short>(byte);
        }
        return sum;
    }

    UDPPacket& operator<<(const std::string& str)
    {
        this->raw_append(static_cast<unsigned short>(str.size()));
        this->raw_append(static_cast<unsigned char>(sizeof(char)));
        this->raw_append(str.data(), str.size());

        return *this;
    }

     UDPPacket& operator<<(const char *str)
    {
        *this << std::string(str);
        return *this;
    }

    /**
     * @WIP - Not working yet
    */
     template<typename T>
     UDPPacket& operator<<(const std::vector<T&>& vector)
     {
        const std::uint16_t len = vector.size();
        const std::uint16_t size = sizeof(T);

        this->raw_append(len);
        this->raw_append(size);
        for (auto item : vector ) {
            this->raw_append(item);
        }

         return *this;
     }

    template<typename T>
    UDPPacket& operator<<(T const& data) noexcept
    {
        const std::uint8_t size = sizeof(T);

        this->append(&data, size);

        return *this;
    }

    UDPPacket& operator>>(std::string& str)
    {
        if (_pos == payload.size())
            throw Exception("No remaining data to unpack.");

        std::uint16_t len;
        this->raw_copy(&len);

        std::uint8_t size;
        this->raw_copy(&size);

        if (size != sizeof(char))
            throw Exception("Invalid data size.");

        str.resize(len);
        this->raw_copy(str.data(), len);

        return *this;
    }

    /**
        * @WIP - Not working yet
    */
     template<typename T>
     UDPPacket& operator>>(std::vector<T&>& vector)
    {
        // if (_pos == payload.size())
        //     throw Exception("No remaining data to unpack.");
        //
        // if (this->remains() < sizeof(std::uint16_t) + sizeof(std::uint8_t))
        //     throw Exception("Invalid data size.");
        //
        // const auto len = this->get<std::uint16_t>();
        // const auto size = this->get_offset<std::uint8_t>(sizeof(std::uint16_t));
        //
        // if (size != sizeof(T))
        //     throw Exception("Invalid data size.");
        //
        // if (this->remains() < (len * sizeof(T)) + sizeof(std::uint16_t) + sizeof(std::uint8_t))
        //     throw Exception("Invalid data size (not enough data).");
        //
        // _pos += sizeof(std::uint16_t) + sizeof(std::uint8_t);
        // vector.resize(len);
        // this->raw_copy(vector.data(), len);

        return *this;
    }

    template<typename T>
    UDPPacket& operator>>(T& data)
     {
        if (_pos == payload.size())
            throw Exception("No remaining data to unpack.");
        const std::uint8_t size = sizeof(T);

        const auto len = this->get<std::uint8_t>();

        if (len != size)
            throw Exception("Invalid data size.");

        _pos += sizeof(std::uint8_t); //only in case the len is correct!

        this->raw_copy(&data, size);
        return *this;
     }

    void append(const void *data, const std::uint8_t size) noexcept
    {
        const auto *raw_len = reinterpret_cast<const std::byte *>(&size);
        const auto *raw_data = static_cast<const std::byte *>(data);

        this->raw_append(raw_len, sizeof(std::uint8_t));
        this->raw_append(raw_data, size);
    }

    [[nodiscard]] std::vector<std::byte> raw() const noexcept
    {
        return this->payload;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return this->payload_length;
    }

    [[nodiscard]] std::size_t remains() const noexcept
    {
        return this->payload_length - this->_pos;
    }

private:
    void _deserialize(const char* data, size_t length)
    {
        size_t offset = 0;

        memcpy(&sequence_number, data + offset, sizeof(sequence_number));
        offset += sizeof(sequence_number);
        memcpy(&ack_number, data + offset, sizeof(ack_number));
        offset += sizeof(ack_number);
        memcpy(&payload_length, data + offset, sizeof(payload_length));
        offset += sizeof(payload_length);
        memcpy(&checksum, data + offset, sizeof(checksum));
        offset += sizeof(checksum);

        payload.resize(payload_length);
        memcpy(payload.data(), data + offset, payload_length);
    }

    void raw_append(const std::byte *data, const std::uint8_t& size)
    {
        this->payload.insert(_end, data, data + size);
        this->payload_length += size;
        this->checksum = calculateChecksum();
        this->_end = this->payload.end();
    }

    void raw_append(const void *data, const std::uint8_t& size)
    {
        const auto *raw = static_cast<const std::byte *>(data);
        this->raw_append(raw, size);
    }

    template<typename T>
    void raw_append(const T& data)
    {
        this->raw_append(&data, sizeof(T));
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
        std::memcpy(dest, &(this->payload[this->_pos]), size);
        _pos += size;
    }

    void get(void *dest, const std::uint8_t size) const
    {
        std::memcpy(dest, &(this->payload[this->_pos]), size);
    }

    template<typename T>
    void get(T *data) const
    {
        this->get(data, sizeof(T));
    }

    template<typename T>
    [[nodiscard]] T get() const
    {
        T var;
        this->get(&var);
        return var;
    }

    template<typename T>
    [[nodiscard]] T get_offset(const std::uint8_t offset)
    {
        T var;
        _pos += offset;
        this->get(&var);
        _pos -= offset;
        return var;
    }

    std::vector<std::byte>::iterator _end = payload.begin();
    std::vector<std::byte>::iterator _begin = payload.begin();
    std::size_t _pos = 0;
};
