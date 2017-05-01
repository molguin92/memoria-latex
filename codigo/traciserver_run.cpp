/**
 * \brief Starts this instance, binding it to a port and awaiting connections. This method is blocking, and as such should be run in a separate thread.
 */
void traci_api::TraCIServer::run()
{
    running = true;
    std::string version_str = "Paramics TraCI plugin v" + std::string(PLUGIN_VERSION) + " on Paramics v" + std::to_string(qpg_UTL_parentProductVersion());
    infoPrint(version_str);
    debugPrint("Simulation start time: " + std::to_string(qpg_CFG_simulationTime()));
    infoPrint("Awaiting connections on port " + std::to_string(port));
    ssocket.accept();
    infoPrint("Accepted connection");
    this->waitForCommands();
}