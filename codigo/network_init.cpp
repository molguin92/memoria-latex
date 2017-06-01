traci_api::Network::Network()
{
    int routes = qpg_NET_busroutes();
    for (int i = 1; i <= routes; i++)
    {
        BUSROUTE* route = qpg_NET_busrouteByIndex(i);
        std::string name = qpg_BSR_name(route);

        route_name_map[name] = route;

        int link_n = qpg_BSR_links(route);
        std::vector<std::string> link_names;

        LINK* current_link = qpg_BSR_firstLink(route);
        link_names.push_back(qpg_LNK_name(current_link));

        for (int link_i = 0; link_i < link_n - 1; link_i++)
        {
            current_link = qpg_BSR_nextLink(route, current_link);
            link_names.push_back(qpg_LNK_name(current_link));
        }

        route_links_map[route] = link_names;
    }
}