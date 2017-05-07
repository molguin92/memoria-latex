int traci_api::Simulation::runSimulation(uint32_t target_timems)
{
    auto current_simtime = this->getCurrentTimeSeconds();
    auto target_simtime = target_timems / 1000.0;
    int steps_performed = 0;

    traci_api::VehicleManager::getInstance()->reset();

    if (target_timems == 0)
    {
        debugPrint("Running one simulation step...");

        qps_GUI_runSimulation();
        traci_api::VehicleManager::getInstance() ->handleDelayedTriggers();
        steps_performed = 1;
    }
    else if (target_simtime > current_simtime)
    {
        debugPrint("Running simulation up to target time: " + std::to_string(target_simtime));
        debugPrint("Current time: " + std::to_string(current_simtime));

        while (target_simtime > current_simtime)
        {
            qps_GUI_runSimulation();
            steps_performed++;
            traci_api::VehicleManager::getInstance() ->handleDelayedTriggers();

            current_simtime = this->getCurrentTimeSeconds();

            debugPrint("Current time: " + std::to_string(current_simtime));
        }
    }
    else
    {
        debugPrint("Invalid target simulation time: " + std::to_string(target_timems));
        debugPrint("Current simulation time: " + std::to_string(current_simtime));
        debugPrint("Doing nothing");
    }

    stepcnt += steps_performed;
    return steps_performed;
}