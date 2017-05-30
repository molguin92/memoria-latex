int traci_api::VehicleManager::rerouteVehicle(VEHICLE* vhc, LINK* lnk)
{
    if (0 == qpg_VHC_uniqueID(vhc)) // dummy vhc
        return 0;

    // check if the vehicle has a special route
    std::unordered_map<LINK*, int>* exit_map;
    try
    {
        exit_map = vhc_routes.at(vhc);
    }
    catch (std::out_of_range& e)
    {
        // no special route, return default
        return 0;
    }

    int next_exit = 0;
    try
    {
        next_exit = exit_map->at(lnk);
    }
    catch (std::out_of_range& e)
    {
        // outside route, clear 
        exit_map->clear();
        delete exit_map;
        vhc_routes.erase(vhc);
    }

    return next_exit;
}