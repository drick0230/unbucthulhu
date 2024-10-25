#include "unbucthulhu_protocol_generated.h"
#include "flatbuffers/minireflect.h"
#include <iostream>

int main()
{
    flatbuffers::FlatBufferBuilder builder(1024);
    auto entities = {UnbucthulhuProtocol::CreateEntity(builder, 80, 16, builder.CreateString("img.png"))};
    auto gameInfo = UnbucthulhuProtocol::CreateGameInfo(builder, builder.CreateVector(entities));
    auto packet = UnbucthulhuProtocol::CreatePacket(builder, gameInfo, 0);
    builder.Finish(packet);

    std::cout << flatbuffers::FlatBufferToString(builder.GetBufferPointer(), UnbucthulhuProtocol::PacketTypeTable()) << std::endl;
}
