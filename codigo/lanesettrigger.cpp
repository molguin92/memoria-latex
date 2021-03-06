void traci_api::LaneSetTrigger::handleTrigger()
{
    int t_lane = target_lane;
    // make sure we stay within maximum number of lanes
    int maxlanes = qpg_LNK_lanes(qpg_VHC_link(vehicle));
    if (t_lane > maxlanes)
        t_lane = maxlanes;
    else if (t_lane < 1)
        t_lane = 1;

    int current_lane = qpg_VHC_lane(vehicle);
    if (current_lane > t_lane) // move outwards
        qps_VHC_laneChange(vehicle, -1);
    else if (current_lane < t_lane)
        qps_VHC_laneChange(vehicle, +1); // move inwards
    else
        qps_VHC_laneChange(vehicle, 0); // stay in this lane
}