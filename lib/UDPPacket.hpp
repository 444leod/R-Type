/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** UDPPacket
*/

#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

/**
 * @brief Overload of the << operator for std::byte.
 *
 * @param os The output stream.
 * @param byte The byte to output.
 * @return std::ostream& The output stream.
 */
inline std::ostream& operator<<(std::ostream& os, const std::byte& byte) {
    os << static_cast<int>(byte);
    return os;
}

/**
 * @brief Convert a byte array to a string representation.
 *
 * @param bytes The byte array.
 * @param size The size of the byte array.
 * @return std::string The string representation of the byte array.
 */
inline std::string print_bytes(const std::byte* bytes, const std::size_t size) {
    std::string str;
    for (std::size_t i = 0; i < size; i++) {
        str += std::to_string(static_cast<int>(bytes[i])) + " ";
    }
    return str;
}

/**
 * @brief Overload of the << operator for std::vector<std::byte>.
 *
 * @param os The output stream.
 * @param bytes The byte vector to output.
 * @return std::ostream& The output stream.
 */
inline std::ostream& operator<<(std::ostream& os, const std::vector<std::byte>& bytes) {
    if (bytes.empty())
        return os;
    os << bytes[0];
    for (std::size_t i = 1; i < bytes.size(); i++) {
        os << " " << bytes[i];
    }
    return os;
}

/**
 * @brief Structure representing a UDP packet.
 */
struct UDPPacket {
    uint32_t sequence_number = 0;   ///< Sequence number of the packet.
    uint32_t ack_number = 0;        ///< Acknowledgment number of the packet.
    uint16_t payload_length = 0;    ///< Length of the payload.
    uint16_t checksum = 0;          ///< Checksum of the packet.
    std::vector<std::byte> payload; ///< Payload of the packet.

    /**
     * @brief Exception class for UDPPacket errors.
     */
    class Exception final : public std::exception {
      public:
        /**
         * @brief Construct a new Exception object.
         *
         * @param message The error message.
         * @param function The function where the error occurred.
         * @param line The line number where the error occurred.
         */
        explicit Exception(const std::string& message, const char* function = __builtin_FUNCTION(), int line = __builtin_LINE()) : _message(message + " in function: " + function + " at line: " + std::to_string(line)) {}
        /**
         * @brief Get the error message.
         *
         * @return const char* The error message.
         */
        [[nodiscard]] const char* what() const noexcept override { return this->_message.c_str(); }

      private:
        std::string _message; ///< The error message.
    };

    /**
     * @brief Default constructor.
     */
    explicit UDPPacket() = default;

    /**
     * @brief Construct a new UDPPacket object from raw data.
     *
     * @param data The raw data.
     * @param length The length of the raw data.
     */
    UDPPacket(const char* data, const size_t& length) { _deserialize(data, length); }

    /**
     * @brief Serialize the packet to a byte vector.
     *
     * @return std::vector<uint8_t> The serialized packet.
     */
    [[nodiscard]] std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer;
        buffer.resize(sizeof(sequence_number) + sizeof(ack_number) + sizeof(payload_length) + sizeof(checksum) + payload.size());

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

    /**
     * @brief Calculate the checksum of the packet.
     *
     * @return uint16_t The checksum.
     */
    [[nodiscard]] uint16_t calculateChecksum() const {
        uint16_t sum = 0;

        sum += (sequence_number >> 16) & 0xFFFF;
        sum += sequence_number & 0xFFFF;

        sum += (ack_number >> 16) & 0xFFFF;
        sum += ack_number & 0xFFFF;

        sum += payload_length;

        for (const auto& byte : payload) {
            sum += static_cast<unsigned short>(byte);
        }
        return sum;
    }

    /**
     * @brief Append a string to the packet.
     *
     * @param str The string to append.
     *
     * @return UDPPacket& Reference to the packet.
     */
    UDPPacket& operator<<(const std::string& str) {
        this->raw_append(static_cast<unsigned short>(str.size()));
        this->raw_append(static_cast<unsigned char>(sizeof(char)));
        this->raw_append(str.data(), str.size());

        return *this;
    }

    /**
     * @brief Append a C-string to the packet. (for packet << "Hello World")
     *
     * @param str The C-string to append.
     *
     * @return UDPPacket& Reference to the packet.
     */
    UDPPacket& operator<<(const char* str) {
        *this << std::string(str);
        return *this;
    }

    /**
     * @brief Append a vector to the packet.
     *
     * @tparam T The type of the vector elements.
     *
     * @param vector The vector to append.
     *
     * @return UDPPacket& Reference to the packet.
     */
    template <typename T> UDPPacket& operator<<(const std::vector<T&>& vector) {
        const std::uint16_t len = vector.size();
        const std::uint16_t size = sizeof(T);

        this->raw_append(len);
        this->raw_append(size);
        for (auto item : vector) {
            this->raw_append(item);
        }

        return *this;
    }

    /**
     * @brief Append data to the packet.
     *
     * @tparam T The type of the data.
     *
     * @param data The data to append.
     *
     * @return UDPPacket& Reference to the packet.
     */
    template <typename T> UDPPacket& operator<<(T const& data) noexcept {
        const std::uint8_t size = sizeof(T);

        this->append(&data, size);

        return *this;
    }

    /**
     * @brief Extract a string from the packet payload and store it in the
     * string variable.
     *
     * @param str The string to extract.
     *
     * @return UDPPacket& Reference to the packet.
     *
     * @throw Exception If there is no remaining data to unpack.
     * @throw Exception If the data size is invalid.
     */
    UDPPacket& operator>>(std::string& str) {
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
     * @brief Extract a vector from the packet payload and store it in the
     * vector variable.
     *
     * @tparam T The type of the vector elements.
     *
     * @param vector The vector to extract.
     *
     * @return UDPPacket& Reference to the packet.
     */
    template <typename T> UDPPacket& operator>>(std::vector<T&>& vector) { return *this; }

    /**
     * @brief Extract data from the packet payload and store it in the data
     * variable.
     *
     * @tparam T The type of the data.
     *
     * @param data The data to extract.
     *
     * @return UDPPacket& Reference to the packet.
     *
     * @throw Exception If there is no remaining data to unpack.
     * @throw Exception If the data size is invalid.
     */
    template <typename T> UDPPacket& operator>>(T& data) {
        if (_pos == payload.size())
            throw Exception("No remaining data to unpack.");
        const std::uint8_t size = sizeof(T);

        const auto len = this->get<std::uint8_t>();

        if (len != size)
            throw Exception("Invalid data size.");

        _pos += sizeof(std::uint8_t);

        this->raw_copy(&data, size);
        return *this;
    }

    /**
     * @brief Append data to the packet.
     *
     * @param data The data to append.
     * @param size The size of the data.
     */
    void append(const void* data, const std::uint8_t size) noexcept {
        const auto* raw_len = reinterpret_cast<const std::byte*>(&size);
        const auto* raw_data = static_cast<const std::byte*>(data);

        this->raw_append(raw_len, sizeof(std::uint8_t));
        this->raw_append(raw_data, size);
    }

    /**
     * @brief Get the raw payload of the packet.
     *
     * @return std::vector<std::byte> The raw payload.
     */
    [[nodiscard]] std::vector<std::byte> raw() const noexcept { return this->payload; }

    /**
     * @brief Get the size of the payload.
     *
     * @return std::size_t The size of the payload.
     */
    [[nodiscard]] std::size_t size() const noexcept { return this->payload_length; }

    /**
     * @brief Get the remaining size of the payload.
     *
     * @return std::size_t The remaining size of the payload.
     */
    [[nodiscard]] std::size_t remains() const noexcept { return this->payload_length - this->_pos; }

  private:
    /**
     * @brief Deserialize raw data into the packet.
     *
     * @param data The raw data.
     * @param length The length of the raw data.
     */
    void _deserialize(const char* data, size_t length) {
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

    /**
     * @brief Append raw data to the payload.
     *
     * @param data The raw data.
     * @param size The size of the raw data.
     */
    void raw_append(const std::byte* data, const std::uint8_t& size) {
        this->payload.insert(_end, data, data + size);
        this->payload_length += size;
        this->checksum = calculateChecksum();
        this->_end = this->payload.end();
    }

    /**
     * @brief Append raw data to the payload.
     *
     * @param data The raw data.
     * @param size The size of the raw data.
     */
    void raw_append(const void* data, const std::uint8_t& size) {
        const auto* raw = static_cast<const std::byte*>(data);
        this->raw_append(raw, size);
    }

    /**
     * @brief Append data to the payload.
     *
     * @tparam T The type of the data.
     *
     * @param data The data to append.
     */
    template <typename T> void raw_append(const T& data) { this->raw_append(&data, sizeof(T)); }

    /**
     * @brief Copy raw data from the payload, will shift the position.
     *
     * @tparam T The type of the data.
     *
     * @return T The copied data.
     */
    template <typename T> T raw_copy() {
        T var;
        this->raw_copy(&var);
        return var;
    }

    /**
     * @brief Copy raw data from the payload, will shift the position.
     *
     * @tparam T The type of the data.
     * @param dest The destination to copy the data to.
     */
    template <typename T> void raw_copy(T* dest) { this->raw_copy(dest, sizeof(T)); }

    /**
     * @brief Copy raw data from the payload, will shift the position.
     *
     * @param dest The destination to copy the data to.
     * @param size The size of the data to copy.
     */
    void raw_copy(void* dest, const std::uint8_t& size) {
        std::memcpy(dest, &(this->payload[this->_pos]), size);
        _pos += size;
    }

    /**
     * @brief Get raw data from the payload without shifting the position.
     *
     * @param dest The destination to get the data to.
     * @param size The size of the data to get.
     */
    void get(void* dest, const std::uint8_t size) const { std::memcpy(dest, &(this->payload[this->_pos]), size); }

    /**
     * @brief Get raw data from the payload without shifting the position.
     *
     * @tparam T The type of the data.
     *
     * @param data The destination to get the data to.
     */
    template <typename T> void get(T* data) const { this->get(data, sizeof(T)); }

    /**
     * @brief Get raw data from the payload without shifting the position.
     *
     * @tparam T The type of the data.
     *
     * @return T The raw data.
     */
    template <typename T> [[nodiscard]] T get() const {
        T var;
        this->get(&var);
        return var;
    }

    /**
     * @brief Get raw data from the payload without shifting the position,
     *  with an offset.
     *
     * @tparam T The type of the data.
     *
     * @param offset The offset to get the data from.
     *
     * @return T The raw data.
     */
    template <typename T> [[nodiscard]] T get_offset(const std::uint8_t offset) {
        T var;
        _pos += offset;
        this->get(&var);
        _pos -= offset;
        return var;
    }

    std::vector<std::byte>::iterator _end = payload.begin();   ///< End iterator of the payload.
    std::vector<std::byte>::iterator _begin = payload.begin(); ///< Begin iterator of the payload.
    std::size_t _pos = 0;                                      ///< Current position in the payload.
};
