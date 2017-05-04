void traci_api::TraCIServer::writeStatusResponse(uint8_t cmdId, uint8_t cmdStatus, std::string description)
{
    debugPrint("Writing status response " + std::to_string(cmdStatus) + " for command " + std::to_string(cmdId));
    outgoing.writeUnsignedByte(1 + 1 + 1 + 4 + static_cast<int>(description.length())); // command length
    outgoing.writeUnsignedByte(cmdId); // command type
    outgoing.writeUnsignedByte(cmdStatus); // status
    outgoing.writeString(description); // description
}

void traci_api::TraCIServer::writeToOutputWithSize(tcpip::Storage& storage, bool force_extended)
{
    this->writeToStorageWithSize(storage, outgoing, force_extended);
}

void traci_api::TraCIServer::writeToStorageWithSize(tcpip::Storage& src, tcpip::Storage& dest, bool force_extended)
{
    uint32_t size = 1 + src.size();
    if (size > 255 || force_extended)
    {
		// extended-length message
        dest.writeUnsignedByte(0);
        dest.writeInt(size + 4);
    }
    else
        dest.writeUnsignedByte(size);
    dest.writeStorage(src);
}