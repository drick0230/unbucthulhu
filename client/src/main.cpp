#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System/Mutex.hpp>
#include <unbucthulhu_protocol.pb.h>
#include <list>
#include <mutex>
#include <vector>
#include <map>

/////////////// TODO ADD PROPERTY TO PROTOCOL /////////////////////
#define WIDTH 30
#define HEIGHT 30

class NetworkTask
{
public:
    NetworkTask() : thread(&NetworkTask::run, this) {}
    ~NetworkTask()
    {
        this->CancelWait();
    }

    /**
     * Start the task's thread.
     */
    void Launch()
    {
        this->thread.launch();
    }

    /**
     * Cancel the task, then wait until it finished properly.
     */
    void CancelWait()
    {
        isRunning = false;
        socket.disconnect();
        this->thread.wait();
    }

    std::vector<unbucthulhu_protocol::Packet::GameInfo::Entity> entities;
    std::mutex entitiesMutex;

private:
    bool isConnected;
    bool isRunning;
    sf::Thread thread;

    sf::TcpSocket socket;

    void treat_packet(sf::Packet &sfmlPacket)
    {
        unbucthulhu_protocol::Packet packet;
        std::string packetAsString;
        sfmlPacket >> packetAsString;
        if (!packet.ParseFromString(packetAsString))
        {
            std::cerr << "Invalid packet received" << std::endl;
            return;
        }

        if (packet.has_gameinfo())
        {
            unbucthulhu_protocol::Packet::GameInfo gameInfo = packet.gameinfo();
            google::protobuf::RepeatedPtrField<unbucthulhu_protocol::Packet_GameInfo_Entity> entities = gameInfo.entities();
            this->entitiesMutex.lock();
            for (const auto entity : entities)
            {
                this->entities.push_back(entity);
            }
            this->entitiesMutex.unlock();
        }
        else if (packet.has_userinputs())
        {
        }
    }

    void run()
    {
        isConnected = false;
        const std::string serverIp = "127.0.0.1";
        const unsigned short serverPort = 55003;
        while (this->socket.connect(serverIp, serverPort, sf::seconds(5)) != sf::Socket::Status::Done)
        {
            std::cout << "Connection to " << serverIp << ":" << serverPort << " failed. Retrying in 5 seconds..." << std::endl;
            sf::sleep(sf::seconds(5));
        }

        isConnected = true;
        std::cout << "Connection to " << serverIp << ":" << serverPort << " successful with local port " << this->socket.getLocalPort() << std::endl;

        while (isConnected)
        {
            sf::Packet rcvPacket;
            if (socket.receive(rcvPacket) != sf::Socket::Status::Done)
            {
                isConnected = false;
                std::cout << "Disconnected!" << std::endl;
            }

            std::cout << "Packet of " << rcvPacket.getDataSize() << " bytes received." << std::endl;

            this->treat_packet(rcvPacket);
        }
        socket.disconnect();
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    std::map<std::string, sf::Texture> textureLibrary; // This is a library of drawable images. Todo load from files at start.
    std::vector<sf::Sprite> entitiesSprites;
    std::vector<sf::RectangleShape> rectanglesToDraw;

    NetworkTask networkTask;
    networkTask.Launch();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        entitiesSprites.clear();
        rectanglesToDraw.clear();
        networkTask.entitiesMutex.lock();
        for (auto entity : networkTask.entities)
        {
            try
            {
                entitiesSprites.push_back(sf::Sprite(sf::Texture(textureLibrary.at(entity.imgpath())), sf::IntRect(entity.posx(), entity.posy(), WIDTH, HEIGHT)));
            }
            catch (std::out_of_range)
            {
                rectanglesToDraw.push_back(sf::RectangleShape(sf::Vector2f(WIDTH, HEIGHT)));
            }
        }
        networkTask.entitiesMutex.unlock();

        window.clear();
        for (auto rectangle : rectanglesToDraw)
        {
            window.draw(rectangle);
        }
        for (auto sprite : entitiesSprites)
        {
            window.draw(sprite);
        }
        window.display();
    }

    networkTask.CancelWait();

    return 0;
}
