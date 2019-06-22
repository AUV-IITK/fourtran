#ifndef CRUC
#define CRUC

#include "base_class.h"

class Cruc : public Base_class
{
public:
    Cruc();
    virtual void loadParams () override;
    virtual void spinThreadBottom () override;
 //   bool markerAngle (master_layer::RequestMarkerAngle::Request &req,
 //                      master_layer::RequestMarkerAngle::Response &res); 

private:
    double MAJOR;
    double MINOR;
    ros::ServiceServer service;
};

#endif // CRUC
