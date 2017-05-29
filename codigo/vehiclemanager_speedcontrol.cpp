bool traci_api::VehicleManager::speedControlOverride(VEHICLE* vhc, float& speed)
{
    BaseSpeedController* controller;
    try
    {
        controller = speed_controllers.at(vhc);
        speed = controller->nextTimeStep();

        if (!controller->repeat())
        {
            speed_controllers.erase(vhc);
            delete controller;
        }

        return true;
    }
    catch (std::out_of_range& e)
    {
        return false;
    }
}