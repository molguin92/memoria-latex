void traci_api::TraCIServer::waitForCommands()
{
    tcpip::Storage incoming; // the whole incoming message
    tcpip::Storage cmdStore; // individual commands in the message

    debugPrint("Waiting for incoming commands from TraCI client...");

    /* While the connection is open, receive commands from the client */
    while (running && ssocket.receiveExact(incoming))
    {
        auto msize = incoming.size();
        debugPrint("Got message of length " + std::to_string(msize));

        /* Multiple commands may arrive at once in one message, 
         * divide them into multiple storages for easy handling */
        while (msize > 0 && incoming.valid_pos())
        {
            uint8_t cmdlen = incoming.readUnsignedByte();
            cmdStore.writeUnsignedByte(cmdlen);
            debugPrint("Got command of length " + std::to_string(cmdlen));

            for (uint8_t i = 0; i < cmdlen - 1; i++)
                cmdStore.writeUnsignedByte(incoming.readUnsignedByte());

            this->parseCommand(cmdStore);
            cmdStore.reset();
        }
        this->sendResponse();
        incoming.reset();
        outgoing.reset();
        debugPrint("------ waiting for commands ------");
    }
}