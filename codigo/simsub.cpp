void traci_api::SimulationVariableSubscription::getObjectVariable(uint8_t var_id, tcpip::Storage& result)
{
    Simulation::getInstance()->getSimulationVariable(var_id, result);
}

uint8_t traci_api::SimulationVariableSubscription::getResponseCode() const
{
    return RES_SUB_SIMVAR;
}