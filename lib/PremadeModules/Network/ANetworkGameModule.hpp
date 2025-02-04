#ifndef A_NETWORK_GAME_MODULE_HPP
#define A_NETWORK_GAME_MODULE_HPP

#include <Engine/AScene.hpp>
#include <Engine/Modules/AGameModule.hpp>

#include <Network/NetworkAgent.hpp>
#include "APacketHandlerSceneModule.hpp"

#include <queue>
#include <SFML/Graphics.hpp>
#include <sstream>

class ANetworkGameModule : public engine::AGameModule, public ntw::NetworkAgent
{
  public:
    explicit ANetworkGameModule(const std::uint32_t& port = 0)
        : AGameModule(), NetworkAgent(port)
        , _sfmlWindow(sf::VideoMode(1200, 600), "Network Monitor") {}

    ~ANetworkGameModule() override = default;

    void start(engine::AScene& scene) override
    {
        const auto packetHandlerSceneModule = scene.getModule<APacketHandlerSceneModule>();

        if (packetHandlerSceneModule == nullptr)
        {
            _packetHandler = &ANetworkGameModule::_doNothing;
        }
        else
        {
            _packetHandler = &ANetworkGameModule::_handlePacket;
        }
        _packetHandlerSceneModule = packetHandlerSceneModule;

        for (auto& [endpoint, packet] : _storedPackets)
        {
            (this->*_packetHandler)(endpoint, packet);
        }
        _storedPackets.clear();
    }

    void stop() override { this->_packetHandler = &ANetworkGameModule::_doNothing; }

    void refresh(engine::AScene& scene) override
    {
        const auto packetHandlerSceneModule = scene.getModule<APacketHandlerSceneModule>();

        if (packetHandlerSceneModule == nullptr)
        {
            _packetHandler = &ANetworkGameModule::_doNothing;
        }
        else
        {
            _packetHandler = &ANetworkGameModule::_handlePacket;
        }
        _packetHandlerSceneModule = packetHandlerSceneModule;
    }

    void update() override
    {
        while (!_packetQueue.empty())
        {
            const auto& [endpoint, packet] = _packetQueue.front();
            this->_send(endpoint, packet);
            _logPacket("Sent", endpoint, packet);
            _packetQueue.pop();
        }

        _updateSFMLWindow();

        for (auto& [src, packet] : _packets)
            (this->*_packetHandler)(src, packet);

        _packets.clear();
    }

    void addClient(const ntw::ClientInformation& endpoint) { this->_clients.push_back(endpoint); }

    void removeClient(const std::uint32_t& id)
    {
        const auto it = std::ranges::find_if(this->_clients, [id](const ntw::ClientInformation& client) { return client.id == id; });

        if (it == this->_clients.end())
            return;
        this->_clients.erase(it);
    }

    void removeClient(const asio::ip::udp::endpoint& endpoint)
    {
        const auto it = std::ranges::find_if(this->_clients, [endpoint](const ntw::ClientInformation& client) { return client.endpoint == endpoint; });

        if (it == this->_clients.end())
            return;
        this->_clients.erase(it);
    }

    void removeClient(const std::string& name)
    {
        const auto it = std::ranges::find_if(this->_clients, [name](const ntw::ClientInformation& client) { return client.name == name; });

        if (it == this->_clients.end())
            return;
        this->_clients.erase(it);
    }

    void sendPacket(const std::uint32_t& clientId, ntw::UDPPacket& packet)
    {
        const auto it = std::ranges::find_if(this->_clients, [clientId](const ntw::ClientInformation& client) { return client.id == clientId; });

        if (it == this->_clients.end())
            return;
        _lastPacket = std::pair(it->endpoint, packet);
        this->sendPacket(it->endpoint, packet);
    }

    void sendPacket(const std::string& name, const ntw::UDPPacket& packet)
    {
        const auto it = std::ranges::find_if(this->_clients, [name](const ntw::ClientInformation& client) { return client.name == name; });

        if (it == this->_clients.end())
            return;
        _lastPacket = std::pair(it->endpoint, packet);
        this->sendPacket(it->endpoint, packet);
    }

    void sendPacket(const asio::ip::udp::endpoint& endpoint, const ntw::UDPPacket& packet)
    {
        _packetQueue.emplace(endpoint, packet);
    }

    void sendPacket(const ntw::UDPPacket& packet)
    {
        for (const auto& client : this->_clients)
        {
            _packetQueue.emplace(client.endpoint, packet);
        }
    }

    void resend()
    {
        if (!_lastPacket.has_value())
            return;

        if (std::holds_alternative<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>>(_lastPacket.value()))
        {
            const auto& [endpoint, packet] = std::get<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>>(_lastPacket.value());
            this->sendPacket(endpoint, packet);
        }
        else
        {
            const auto& packet = std::get<ntw::UDPPacket>(_lastPacket.value());
            this->sendPacket(packet);
        }
    }

    [[nodiscard]] std::vector<ntw::ClientInformation> clients() const { return this->_clients; }

  protected:
    void _onPacketReceived(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) override
    {
        // std::cout << "Packet received from " << src << std::endl;
        _logPacket("Received", src, packet);
        _packets.emplace_back(src, packet);
    }

    void _storePacket(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) { _storedPackets.emplace_back(src, packet); }

    void _doNothing(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
    {
        (void)src;
        (void)packet;
        std::cout << "No packet handler" << std::endl;
    }

    void _handlePacket(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) { _packetHandlerSceneModule->handlePacket(src, packet); }

    void _logPacket(const std::string& type, const asio::ip::udp::endpoint& endpoint, const ntw::UDPPacket& packet)
    {
        std::ostringstream oss;

        auto packetCopy = packet;
        PACKET_TYPE packetType;
        packetCopy >> packetType;

        const auto packetTypeStr = packetTypeToString.find(packetType);
        std::string finalPacketType = packetTypeStr != packetTypeToString.end() ? packetTypeStr->second : "Unknown";
        oss << type << " | " << endpoint << " | "
            << "Seq: " << packet.sequence_number << ", "
            << "Ack: " << packet.ack_number << ", "
            << finalPacketType << ", "
            << "Size: " << packet.size() << " bytes: " << packet.payload;

        _logs.push_back(oss.str());
        if (_logs.size() > 20) // Limit log entries
            _logs.pop_front();
    }

    void _updateSFMLWindow()
{
    static float verticalOffset = 0.0f; // Offset for vertical scrolling
    static float horizontalOffset = 0.0f; // Offset for horizontal scrolling

    _sfmlWindow.clear(sf::Color::Black);

    sf::Event ev{};
    while (_sfmlWindow.pollEvent(ev))
    {
        if (ev.type == sf::Event::Closed)
            _sfmlWindow.close();

        // Handle vertical scrolling with arrow keys or mouse wheel
        if (ev.type == sf::Event::KeyPressed)
        {
            if (ev.key.code == sf::Keyboard::Up)
                verticalOffset = std::max(0.0f, verticalOffset - 30.0f);
            if (ev.key.code == sf::Keyboard::Down)
                verticalOffset += 30.0f;
            if (ev.key.code == sf::Keyboard::Left)
                horizontalOffset = std::max(0.0f, horizontalOffset - 30.0f);
            if (ev.key.code == sf::Keyboard::Right)
                horizontalOffset += 30.0f;
        }
        else if (ev.type == sf::Event::MouseWheelScrolled)
        {
            if (ev.mouseWheelScroll.delta > 0)
                verticalOffset = std::max(0.0f, verticalOffset - 30.0f);
            else
                verticalOffset += 30.0f;
        }
    }

        if (!_font.has_value())
        {
            _font = sf::Font();
            if (!_font->loadFromFile("./assets/arial.ttf"))
            {
                engine::RestrictedGame::instance().stop();
                return;
            }
        }

    float y = 10.0f - verticalOffset; // Apply vertical offset

    for (const auto& log : _logs)
    {
        std::istringstream iss(log);
        std::string type, endpoint, packetInfo;

        // Parse the log structure
        if (std::getline(iss, type, '|') &&
            std::getline(iss, endpoint, '|') &&
            std::getline(iss, packetInfo, '|'))
        {
            // Display the date (prepend to logs)
            auto now = std::time(nullptr);
            char timeBuffer[20];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
            std::string dateStr(timeBuffer);

            // Display the date
            sf::Text dateText(dateStr, _font.value(), 14);
            dateText.setPosition(10.0f - horizontalOffset, y);
            dateText.setFillColor(sf::Color::Yellow);
            _sfmlWindow.draw(dateText);

            // Display the type (e.g., "Sent" or "Received")
            sf::Text typeText(type, _font.value(), 18);
            typeText.setPosition(150.0f - horizontalOffset, y);
            typeText.setFillColor(type == "Sent" ? sf::Color::Green : sf::Color::Red);
            _sfmlWindow.draw(typeText);

            // Display the endpoint (IP and port)
            sf::Text endpointText(endpoint, _font.value(), 16);
            endpointText.setPosition(300.0f - horizontalOffset, y);
            endpointText.setFillColor(sf::Color::Cyan);
            _sfmlWindow.draw(endpointText);

            // Display the packet info (sequence, ack, etc.)
            sf::Text packetText(packetInfo, _font.value(), 16);
            packetText.setPosition(500.0f - horizontalOffset, y);
            packetText.setFillColor(sf::Color::White);
            _sfmlWindow.draw(packetText);
        }

        y += 30.0f; // Move down for the next log entry
    }

    _sfmlWindow.display();
}



  protected:
    void (ANetworkGameModule::*_packetHandler)(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet) = &ANetworkGameModule::_storePacket;

    std::vector<ntw::ClientInformation> _clients;

    std::vector<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>> _storedPackets;

    std::shared_ptr<APacketHandlerSceneModule> _packetHandlerSceneModule;

    std::optional<std::variant<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>, ntw::UDPPacket>> _lastPacket;

    std::queue<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>> _packetQueue;

    sf::RenderWindow _sfmlWindow;
    std::deque<std::string> _logs;

    //list of packet received
    std::vector<std::pair<asio::ip::udp::endpoint, ntw::UDPPacket>> _packets;

    std::optional<sf::Font> _font = std::nullopt;
};

#endif // A_NETWORK_GAME_MODULE_HPP
