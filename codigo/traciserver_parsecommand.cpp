void traci_api::TraCIServer::parseCommand(tcpip::Storage& storage)
{
    debugPrint("Parsing command");

    uint8_t cmdLen = storage.readUnsignedByte();
    uint8_t cmdId = storage.readUnsignedByte();
    tcpip::Storage state;

    debugPrint("Command length: " + std::to_string(cmdLen));
    debugPrint("Command ID: " + std::to_string(cmdId));

    if (cmdId >= CMD_SUB_INDVAR && cmdId <= CMD_SUB_SIMVAR)
    {
        // subscription
        // | begin Time | end Time | Object ID | Variable Number | The list of variables to return

        debugPrint("Subscribing to " + std::to_string(cmdId));


        int btime = storage.readInt();
        debugPrint("Start time: " + std::to_string(btime));

        int etime = storage.readInt();
        debugPrint("End time: " + std::to_string(etime));

        std::string oID = storage.readString();
        debugPrint("Object ID: " + oID);

        int varN = storage.readUnsignedByte();
        debugPrint("N Vars: " + std::to_string(varN));

        std::vector<uint8_t> vars;
        std::string vars_s = "";

        for (int i = 0; i < varN; i++)
        {
            uint8_t vid = storage.readUnsignedByte();
            vars.push_back(vid);
            vars_s = vars_s + std::to_string(vid) + " ";
        }

        debugPrint("Vars: " + vars_s);
        addSubscription(cmdId, oID, btime, etime, vars);
    }
    else
    {
        switch (cmdId)
        {
        case CMD_GETVERSION:

            debugPrint("Got CMD_GETVERSION");
            this->writeVersion();
            break;

        case CMD_SIMSTEP:

            debugPrint("Got CMD_SIMSTEP");

            this->cmdSimStep(storage.readInt());
            break;

        case CMD_SHUTDOWN:

            debugPrint("Got CMD_SHUTDOWN");
            this->cmdShutDown();
            break;

        case CMD_GETSIMVAR:

            debugPrint("Got CMD_GETSIMVAR");
            this->cmdGetSimVar(storage.readUnsignedByte());
            break;

        case CMD_SETVHCSTATE:

            debugPrint("Got CMD_SETVHCSTATE");
            this->cmdSetVhcState(storage);
            break;

        case CMD_GETVHCVAR:

            debugPrint("Got CMD_GETVHCVAR");
            this->cmdGetVhcVar(storage);
            break;

        case CMD_GETRTEVAR:
        case CMD_GETLNKVAR:
        case CMD_GETNDEVAR:

            debugPrint("Got CMD_GETLNKVAR/CMD_GETNDEVAR/CMD_GETRTEVAR");
            this->cmdGetNetworkVar(storage, cmdId);
            break;

        case CMD_GETPOLVAR:
            debugPrint("Got CMD_GETPOLVAR");
            this->cmdGetPolygonVar(storage);
            break;

        case CMD_GETVTPVAR:
            debugPrint("Got CMD_GETVTPVAR");
            this->cmdGetVtpVar(storage);
            break;


        default:

            debugPrint("Command not implemented!");

            writeStatusResponse(cmdId, STATUS_NIMPL, "Method not implemented.");
        }
    }
}