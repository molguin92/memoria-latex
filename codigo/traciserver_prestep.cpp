void traci_api::TraCIServer::preStep()
{
    std::lock_guard<std::mutex> lock(socket_lock);
    if (multiple_timestep && Simulation::getInstance()->getCurrentTimeMilliseconds() < target_time)
    {
        VehicleManager::getInstance()->reset();
        return;
    }

    multiple_timestep = false;
    target_time = 0;
    
    tcpip::Storage cmdStore; // individual commands in the message

    debugPrint("Waiting for incoming commands from the TraCI client...");

    // receive and parse messages until we get a simulation step command
    while (running && ssocket.receiveExact(incoming))
    {
        incoming_size = incoming.size();

        debugPrint("Got message of length " + std::to_string(incoming_size));
        //debugPrint("Incoming: " + incoming.hexDump());


        /* Multiple commands may arrive at once in one message,
        * divide them into multiple storages for easy handling */
        while (incoming_size > 0 && incoming.valid_pos())
        {
            uint8_t cmdlen = incoming.readUnsignedByte();
            cmdStore.writeUnsignedByte(cmdlen);

            debugPrint("Got command of length " + std::to_string(cmdlen));


            for (uint8_t i = 0; i < cmdlen - 1; i++)
                cmdStore.writeUnsignedByte(incoming.readUnsignedByte());

            bool simstep = this->parseCommand(cmdStore);
            cmdStore.reset();

            // if the received command was a simulation step command, return so that 
            // Paramics can do its thing.
            if (simstep)
            {
                VehicleManager::getInstance()->reset();
                return;
            }
        }

        this->sendResponse();
        incoming.reset();
        outgoing.reset();
    }
}