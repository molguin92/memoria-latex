void traci_api::TraCIServer::postStep()
{
    // after each step, have VehicleManager update its internal state
    VehicleManager::getInstance()->handleDelayedTriggers();

    if (multiple_timestep && Simulation::getInstance()->getCurrentTimeMilliseconds() < target_time)
        return;

    // after a finishing a simulation step command (completely), collect subscription results and 
    // check if there are commands remaining in the incoming storage
    this->writeStatusResponse(CMD_SIMSTEP, STATUS_OK, "");

    // handle subscriptions after simstep command
    tcpip::Storage subscriptions;
    this->processSubscriptions(subscriptions);
    outgoing.writeStorage(subscriptions);

    // finish parsing the message we got before the simstep command
    tcpip::Storage cmdStore;
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

        // weird, two simstep commands in one message?
        if (simstep)
        {
            if(!multiple_timestep)
            {
                multiple_timestep = true;
                target_time = Simulation::getInstance()->getCurrentTimeMilliseconds() + Simulation::getInstance()->getTimeStepSizeMilliseconds();
            }
            VehicleManager::getInstance()->reset();
            return;
        }
    }