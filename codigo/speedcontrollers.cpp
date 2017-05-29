// Triggers.h
namespace traci_api
{
    class BaseSpeedController
    {
        public:
        virtual ~BaseSpeedController()
        {
        }
        virtual float nextTimeStep() = 0;
        virtual bool repeat() = 0;
    };
    
    class HoldSpeedController : public BaseSpeedController
    {
        private:
        VEHICLE* vhc;
        float target_speed;
        
        public:
        HoldSpeedController(VEHICLE* vhc, float target_speed) : vhc(vhc), target_speed(target_speed){}
        ~HoldSpeedController() override {}
        
        float nextTimeStep() override;
        bool repeat() override { return true; }
    };
    
    class LinearSpeedChangeController : public BaseSpeedController
    {
        private:
        VEHICLE* vhc;
        int duration;
        bool done;
        
        float acceleration;
        
        public:
        LinearSpeedChangeController(VEHICLE* vhc, float target_speed, int duration);
        ~LinearSpeedChangeController() override {};
        
        float nextTimeStep() override;
        bool repeat() override { return !done; }
    };
}

// Triggers.cpp
float traci_api::HoldSpeedController::nextTimeStep()
{
    float current_speed = qpg_VHC_speed(vhc);
    float diff = target_speed - current_speed;
    if (abs(diff) < NUMERICAL_EPS)
    {
        if (target_speed < NUMERICAL_EPS && !qpg_VHC_stopped(vhc))
            qps_VHC_stopped(vhc, PTRUE);
        return current_speed;
    }

    /* find acceleration/deceleration needed to reach speed asap */
    float accel = 0;
    if (diff < 0)
    {
        /* decelerate */
        accel = max(qpg_VTP_deceleration(qpg_VHC_type(vhc)), diff);
    }
    else
    {
        /* accelerate */
        accel = min(qpg_VTP_acceleration(qpg_VHC_type(vhc)), diff);
    }

    return current_speed + (qpg_CFG_timeStep()*accel);
}

traci_api::LinearSpeedChangeController::LinearSpeedChangeController(VEHICLE* vhc, float target_speed, int duration) : vhc(vhc), duration(0), done(false)
{
    /* 
     * calculate acceleration needed for each timestep. if duration is too short, i.e.
     * it causes the needed acceleration to be greater than the maximum allowed, we'll use
     * the maximum for the duration, but we'll never reach the desired speed.
     */

    float current_speed = qpg_VHC_speed(vhc);
    float diff = target_speed - current_speed;
    // first, check if we actually need to change the speed
    // this will do nothing if we don't
    if (abs(diff) < NUMERICAL_EPS)
    {
        done = true;
        acceleration = 0;
        return;
    }

    float timestep_sz = qpg_CFG_timeStep();
    float duration_s = duration / 1000.0f;
    int d_factor = round(duration_s / timestep_sz);
    this->duration = d_factor * (timestep_sz * 1000);

    acceleration = diff / (duration / 1000.0f); // acceleration (m/s2)
    if (diff < 0)
    {
        /* decelerate */
        acceleration = max(qpg_VTP_deceleration(qpg_VHC_type(vhc)), acceleration);
    }
    else
    {
        /* accelerate */
        acceleration = min(qpg_VTP_acceleration(qpg_VHC_type(vhc)), acceleration);
    }
}

float traci_api::LinearSpeedChangeController::nextTimeStep()
{
    float timestep_sz = qpg_CFG_timeStep();
    duration -= timestep_sz * 1000;
    if (duration <= 0)
        done = true;
    
    return qpg_VHC_speed(vhc) + (timestep_sz * acceleration);
}