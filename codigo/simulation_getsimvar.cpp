bool traci_api::Simulation::packSimulationVariable(uint8_t varID, tcpip::Storage& result_store)
{
    debugPrint("Fetching SIMVAR " + std::to_string(varID));

    result_store.writeUnsignedByte(RES_GETSIMVAR);
    result_store.writeUnsignedByte(varID);
    result_store.writeString("");
    try
    {
        getSimulationVariable(varID, result_store);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

void traci_api::Simulation::getSimulationVariable(uint8_t varID, tcpip::Storage& result)
{
    VehicleManager* vhcman = traci_api::VehicleManager::getInstance();

    switch (varID)
    {
    case VAR_SIMTIME:
        result.writeUnsignedByte(VTYPE_INT);
        result.writeInt(this->getCurrentTimeMilliseconds());
        break;
    case VAR_DEPARTEDVHC_CNT:
        result.writeUnsignedByte(VTYPE_INT);
        result.writeInt(vhcman->getDepartedVehicleCount());
        break;
    case VAR_DEPARTEDVHC_LST:
        result.writeUnsignedByte(VTYPE_STRLST);
        result.writeStringList(vhcman->getDepartedVehicles());
        break;
    case VAR_ARRIVEDVHC_CNT:
        result.writeUnsignedByte(VTYPE_INT);
        result.writeInt(vhcman->getArrivedVehicleCount());
        break;
    case VAR_ARRIVEDVHC_LST:
        result.writeUnsignedByte(VTYPE_STRLST);
        result.writeStringList(vhcman->getArrivedVehicles());
        break;
    case VAR_TIMESTEPSZ:
        result.writeUnsignedByte(VTYPE_INT);
        result.writeInt(getTimeStepSizeMilliseconds());
        break;
    case VAR_NETWORKBNDS:
        result.writeUnsignedByte(VTYPE_BOUNDBOX);
        {
            double llx, lly, urx, ury;
            this->getRealNetworkBounds(llx, lly, urx, ury);

            result.writeDouble(llx);
            result.writeDouble(lly);
            result.writeDouble(urx);
            result.writeDouble(ury);
        }
        break;
        // we don't have teleporting vehicles in Paramics, nor parking (temporarily at least)
    case VAR_VHCENDTELEPORT_CNT:
    case VAR_VHCSTARTTELEPORT_CNT:
    case VAR_VHCSTARTPARK_CNT:
    case VAR_VHCENDPARK_CNT:
        result.writeUnsignedByte(VTYPE_INT);
        result.writeInt(0);
        break;

    case VAR_VHCENDTELEPORT_LST:
    case VAR_VHCSTARTTELEPORT_LST:
    case VAR_VHCSTARTPARK_LST:
    case VAR_VHCENDPARK_LST:
        result.writeUnsignedByte(VTYPE_STRLST);
        result.writeStringList(std::vector<std::string>());
        break;
    default:
        throw std::runtime_error("Unimplemented variable " + std::to_string(varID));
    }
}