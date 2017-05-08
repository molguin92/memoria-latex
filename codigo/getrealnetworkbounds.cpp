void traci_api::Simulation::getRealNetworkBounds(double& llx, double& lly, double& urx, double& ury)
{
    /*
     * Paramics qpg_POS_network() function, which should return the network bounds, does not make sense.
     * It returns coordinates which leave basically the whole network outside of its own bounds. 
     * 
     * Thus, we'll have to "bruteforce" the positional data for the network bounds.
     */

    // get all relevant elements in the network, and all their coordinates

    std::vector<float> x;
    std::vector<float> y;

    int node_count = qpg_NET_nodes();
    int link_count = qpg_NET_links();
    int zone_count = qpg_NET_zones();

    float tempX, tempY, tempZ;

    for (int i = 1; i <= node_count; i++)
    {
        NODE* node = qpg_NET_nodeByIndex(i);
        qpg_POS_node(node, &tempX, &tempY, &tempZ);

        x.push_back(tempX);
        y.push_back(tempY);
    }

    for (int i = 1; i <= zone_count; i++)
    {
        ZONE* zone = qpg_NET_zone(i);
        int vertices = qpg_ZNE_vertices(zone);
        for (int j = 1; j <= vertices; j++)
        {
            qpg_POS_zoneVertex(zone, j, &tempX, &tempY, &tempZ);

            x.push_back(tempX);
            y.push_back(tempY);
        }
    }

    for (int i = 1; i <= link_count; i++)
    {
        // links are always connected to zones or nodes, so we only need
        // to get position data from those that are curved

        LINK* lnk = qpg_NET_linkByIndex(i);
        if (!qpg_LNK_arc(lnk) && !qpg_LNK_arcLeft(lnk))
            continue;

        // arc are perfect sections of circles, thus we only need the start, end and middle point (for all lanes)
        float len = qpg_LNK_length(lnk);
        int lanes = qpg_LNK_lanes(lnk);

        float g, b;

        for (int j = 1; j <= lanes; j++)
        {
            // start points
            qpg_POS_link(lnk, j, 0, &tempX, &tempY, &tempZ, &b, &g);

            x.push_back(tempX);
            y.push_back(tempY);

            // middle points
            qpg_POS_link(lnk, j, len / 2.0, &tempX, &tempY, &tempZ, &b, &g);

            x.push_back(tempX);
            y.push_back(tempY);

            // end points
            qpg_POS_link(lnk, j, len, &tempX, &tempY, &tempZ, &b, &g);

            x.push_back(tempX);
            y.push_back(tempY);
        }
    }


    // we have all the coordinates, now get maximums and minimums
    // add some wiggle room as well, just in case
    urx = *std::max_element(x.begin(), x.end()) + 100;
    llx = *std::min_element(x.begin(), x.end()) - 100;
    ury = *std::max_element(y.begin(), y.end()) + 100;
    lly = *std::min_element(y.begin(), y.end()) - 100;
}