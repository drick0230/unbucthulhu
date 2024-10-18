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
        this->CancelWait()
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
    bool isRunning;
    sf::Thread thread;

    std::vector<unbucthulhu_protocol::Packet::GameInfo::Entity> entities;
    std::mutex entitiesMutex;

    void
    treat_packet(sf::Packet &sfmlPacket)
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
        this->isRunning = true;
        // Create a socket to listen to new connections
        sf::TcpListener listener;
        listener.listen(55001);

        // Create a list to store the future clients
        std::list<sf::TcpSocket *> clients;

        // Create a selector
        sf::SocketSelector selector;

        // Add the listener to the selector
        selector.add(listener);

        // Endless loop that waits for new connections
        while (this->isRunning)
        {
            // Make the selector wait for data on any socket
            if (selector.wait())
            {
                // Test the listener
                if (selector.isReady(listener))
                {
                    // The listener is ready: there is a pending connection
                    sf::TcpSocket *client = new sf::TcpSocket;
                    if (listener.accept(*client) == sf::Socket::Done)
                    {
                        // Add the new client to the clients list
                        clients.push_back(client);

                        // Add the new client to the selector so that we will
                        // be notified when he sends something
                        selector.add(*client);
                    }
                    else
                    {
                        // Error, we won't get a new connection, delete the socket
                        delete client;
                    }
                }
                else
                {
                    // The listener socket is not ready, test all other sockets (the clients)
                    for (std::list<sf::TcpSocket *>::iterator it = clients.begin(); it != clients.end(); ++it)
                    {
                        sf::TcpSocket &client = **it;
                        if (selector.isReady(client))
                        {
                            // The client has sent some data, we can receive it
                            sf::Packet packet;
                            if (client.receive(packet) == sf::Socket::Done)
                            {
                                this->treat_packet(packet);
                            }
                        }
                    }
                }
            }
        }
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

        window.clear();
        for (auto rectangle : rectanglesToDraw)
        {
            window.draw(rectangle);
        }
        for (auto sprite : entitiesSprites)
        {
            window.draw(sprite);
        }
        window.draw(shape);
        window.display();
    }

    networkTask.CancelWait();

    return 0;
}
