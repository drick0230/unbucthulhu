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

    /**
     * Get a copy of the latest entities received.
     */
    std::vector<unbucthulhu_protocol::Packet::GameInfo::Entity> &get_entities()
    {
        std::vector<unbucthulhu_protocol::Packet::GameInfo::Entity> entities;

        this->entitiesMutex.lock();
        for (const auto entity : this->entities)
        {
            entities.push_back(entity);
        }
        this->entitiesMutex.unlock();

        return entities;
    }

private:
    bool isConnected;
    bool isRunning;
    sf::Thread thread;

    std::vector<unbucthulhu_protocol::Packet::GameInfo::Entity> entities;
    std::mutex entitiesMutex;

    sf::TcpSocket socket;

    void treat_packet(sf::Packet &sfmlPacket)
    {
        unbucthulhu_protocol::Packet packet;
        packet.ParseFromArray(sfmlPacket.getData(), sfmlPacket.getDataSize());

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
            std::cout << "Connection to " << serverIp << ":" << serverPort << " failed. Retrying in 5 seconds...";
            sf::sleep(sf::seconds(5));
        }

        isConnected = true;

        while (isConnected)
        {

            char buffer[128];
            std::size_t received = 0;
            if (socket.receive(buffer, sizeof(buffer), received) != sf::Socket::Status::Done)
            {
                isConnected = false;
            }

            std::cout << "Received: " << buffer << std::endl;
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
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

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
        /*
                entitiesSprites.clear();
                auto entities = networkTask.get_entities();
                for (auto entity : entities)
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
         */
        window.clear();
        /*   for (auto rectangle : rectanglesToDraw)
          {
              window.draw(rectangle);
          }
          for (auto sprite : entitiesSprites)
          {
              window.draw(sprite);
          } */
        window.draw(shape);
        window.display();
    }

    networkTask.CancelWait();

    return 0;
}
