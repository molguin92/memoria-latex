/**
 * \brief Starts this instance, binding it to a port and awaiting connections.
 */
void traci_api::TraCIServer::waitForConnection()
{
    running = true;
    std::string version_str = "Paramics TraCI plugin v" + std::string(PLUGIN_VERSION) + " on Paramics v" + std::to_string(qpg_UTL_parentProductVersion());
    infoPrint(version_str);
    infoPrint("Timestep size: " + std::to_string(static_cast<int>(qpg_CFG_timeStep() * 1000.0f)) + "ms");
    infoPrint("Simulation start time: " + std::to_string(Simulation::getInstance()
		->getCurrentTimeMilliseconds()) + "ms");
    infoPrint("Awaiting connections on port " + std::to_string(port));

    {
        std::lock_guard<std::mutex> lock(socket_lock);
        ssocket.accept();
    }

    infoPrint("Accepted connection");
}