#include "unbucthulhu_protocol.pb.h"
#include "google/protobuf/json/json.h"
#include <cstdio>
#include <string>
#include <iostream>

int main()
{
    unbucthulhu_protocol::GameInfo gameInfo;
    unbucthulhu_protocol::GameInfo::Entity *entity = gameInfo.add_entities();
    entity->set_posx(16);
    entity->set_posy(80);
    entity->set_imgpath("img.png");

    std::string jsonString;
    absl::Status status = google::protobuf::json::MessageToJsonString(gameInfo, &jsonString);

    if (status.ok())
    {
        std::cout << jsonString;
    }
    else
    {
        std::cout << status;
    }

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
}
