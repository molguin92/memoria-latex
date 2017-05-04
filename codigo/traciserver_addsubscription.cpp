void traci_api::TraCIServer::addSubscription(uint8_t sub_type, std::string object_id, int start_time, int end_time, std::vector<uint8_t> variables)
{
    std::string errors;
    tcpip::Storage temp;

    // first check if this corresponds to an update for an existing subscription
    for (auto it = subs.begin(); it != subs.end(); ++it)
    {
        uint8_t result = (*it)->updateSubscription(sub_type, object_id, start_time, end_time, variables, temp, errors);

        switch (result)
        {
        case VariableSubscription::STATUS_OK:
            // update ok, return now
            debugPrint("Updated subscription");
            writeStatusResponse(sub_type, STATUS_OK, "");
            writeToOutputWithSize(temp, true);
            return;
        case VariableSubscription::STATUS_UNSUB:
            // unsubscribe command, remove the subscription
            debugPrint("Unsubscribing...");
            delete *it;
            it = subs.erase(it);
            // we don't care about the deleted iterator, since we return from the loop here
            writeStatusResponse(sub_type, STATUS_OK, "");
            return;
        case VariableSubscription::STATUS_ERROR:
            // error when updating
            debugPrint("Error updating subscription.");
            writeStatusResponse(sub_type, STATUS_ERROR, errors);
            break;
        case VariableSubscription::STATUS_NOUPD:
            // no update, try next subscription
            continue;
        default:
            throw std::runtime_error("Received unexpected result " + std::to_string(result) + " when trying to update subscription.");
        }
    }

    // if we reach here, it means we need to add a new subscription.
    // note: it could also mean it's an unsubscribe command for a car that reached its
    // destination. Check number of variables and do nothing if it's 0.

    if(variables.size() == 0)
    {
        // unsub command that didn't match any of the currently running subscriptions, so just
        // tell the client it's ok, everything's alright

        debugPrint("Unsub from subscription already removed.");
        writeStatusResponse(sub_type, STATUS_OK, "");
        return;
    }


    debugPrint("No update. Adding new subscription.");
    VariableSubscription* sub;
    switch (sub_type)
    {
    case CMD_SUB_VHCVAR:
        debugPrint("Adding VHC subscription.");
        sub = new VehicleVariableSubscription(object_id, start_time, end_time, variables);
        break;
    case CMD_SUB_SIMVAR:
        debugPrint("Adding SIM subscription.");
        sub = new SimulationVariableSubscription(object_id, start_time, end_time, variables);
        break;
    default:
        writeStatusResponse(sub_type, STATUS_NIMPL, "Subscription type not implemented: " + std::to_string(sub_type));
        return;
    }

    uint8_t result = sub->handleSubscription(temp, true, errors); // validate

    if (result == VariableSubscription::STATUS_EXPIRED)
    {
        debugPrint("Expired subscription.");

        writeStatusResponse(sub_type, STATUS_ERROR, "Expired subscription.");
        return;
    }
    else if (result != VariableSubscription::STATUS_OK)
    {
        debugPrint("Error adding subscription.");

        writeStatusResponse(sub_type, STATUS_ERROR, errors);
        return;
    }

    writeStatusResponse(sub_type, STATUS_OK, "");
    writeToOutputWithSize(temp, true);
    subs.push_back(sub);
}