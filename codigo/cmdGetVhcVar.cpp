void traci_api::TraCIServer::cmdGetVhcVar(tcpip::Storage& input)
{
    tcpip::Storage result;
    try
    {
        VehicleManager::getInstance()->packVehicleVariable(input, result);
        this->writeStatusResponse(CMD_GETVHCVAR, STATUS_OK, "");
        this->writeToOutputWithSize(result, false);
    }
    catch (NotImplementedError& e)
    {
        debugPrint("Variable not implemented");
        debugPrint(e.what());
        this->writeStatusResponse(CMD_GETVHCVAR, STATUS_NIMPL, e.what());
    }
    catch (std::exception& e)
    {
        debugPrint("Fatal error???");
        debugPrint(e.what());
        this->writeStatusResponse(CMD_GETVHCVAR, STATUS_ERROR, e.what());
        throw;
    }
}