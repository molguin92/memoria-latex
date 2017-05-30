void traci_api::VehicleManager::handleDelayedTriggers()
{
    // handle lane set triggers
    debugPrint("Handling vehicle triggers: lane set triggers");
    for (auto kv = lane_set_triggers.begin(); kv != lane_set_triggers.end();)
    {
        kv->second->handleTrigger();

        /* check if need repeating */
        if (!kv->second->repeat())
        {
            delete kv->second;
            kv = lane_set_triggers.erase(kv);
        }
        else
            ++kv;
    }
    debugPrint("Handling vehicle triggers: done");
}