#include <SFML/Network.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/Window/Keyboard.hpp>
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

    sf::TcpListener listener;
    std::list<sf::TcpSocket *> clients;
    sf::SocketSelector selector;

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
        this->listener.listen(55003);
        this->selector.add(listener);

        // Endless loop that waits for new connections
        while (this->isRunning)
        {
            // Make the selector wait for data on any socket
            if (selector.wait(sf::seconds(1)))
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
                        std::cout << "New client connected (port" << client->getLocalPort() << "):" << client->getRemoteAddress() << ":" << client->getRemotePort() << std::endl;
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

        for (auto client : this->clients)
        {
            client->disconnect();
        }
        this->listener.close();
    }
};

int main()
{
    std::string input;
    NetworkTask networkTask;
    networkTask.Launch();

    std::cout << "Press <q> to quit." << std::endl;

    while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
    {
        sf::sleep(sf::milliseconds(100));
    }

    networkTask.CancelWait();

    return 0;
}