void traci_api::TraCIServer::processSubscriptions(tcpip::Storage& sub_store)
{
	/* ... */
    for (auto i = subs.begin(); i != subs.end();)
    {
		/* polymorphic evaluation of subscriptions; (*i) may be Vehicle or Sim subscription */
        sub_res = (*i)->handleSubscription(temp, false, errors); 
        if (sub_res == VariableSubscription::STATUS_EXPIRED 
			|| sub_res == VariableSubscription::STATUS_OBJNOTFOUND)
        {
            delete *i;
            i = subs.erase(i);
        }
        else
        {
            writeToStorageWithSize(temp, sub_results, true);
            count++;
            ++i; // increment
        }
        temp.reset();
    }
	/* ... */
    sub_store.writeInt(count);
    sub_store.writeStorage(sub_results);
}