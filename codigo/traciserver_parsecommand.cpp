void traci_api::TraCIServer::parseCommand(tcpip::Storage& storage)
{
    /* ... */
    uint8_t cmdLen = storage.readUnsignedByte();
    uint8_t cmdId = storage.readUnsignedByte();
    tcpip::Storage state;
    /* ... */

    if (cmdId >= CMD_SUB_INDVAR && cmdId <= CMD_SUB_SIMVAR)
    {
        // subscription
        // | begin Time | end Time | Object ID | Variable Number | The list of variables to return
		/* read subscription params */
		/* ... */
        addSubscription(cmdId, oID, btime, etime, vars);
    }
    else
    {
        switch (cmdId)
        {
        case CMD_GETVERSION:
		/* ... */
		/* ... */
        default:
            debugPrint("Command not implemented!");
            writeStatusResponse(cmdId, STATUS_NIMPL, "Method not implemented.");
        }
    }
}