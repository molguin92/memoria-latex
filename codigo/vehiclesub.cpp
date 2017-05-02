void traci_api::VehicleVariableSubscription::getObjectVariable(uint8_t var_id, tcpip::Storage& result)
{
    VehicleManager::getInstance()->getVehicleVariable(objID, var_id, result);
}

uint8_t traci_api::VehicleVariableSubscription::getResponseCode() const
{
    return RES_SUB_VHCVAR;
}