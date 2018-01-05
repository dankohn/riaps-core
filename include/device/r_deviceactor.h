//
// Created by istvan on 5/8/17.
//

#ifndef RIAPS_CORE_R_DEVICE_H
#define RIAPS_CORE_R_DEVICE_H

#include <componentmodel/r_actor.h>
#include <json.h>
#include <set>
#include <string>
#include <vector>


namespace riaps {

    class DeviceActor : public Actor {

    public:
        ~DeviceActor();

        static DeviceActor* CreateDeviceActor(nlohmann::json& configJson,
                                              const std::string& deviceName,
                                              const std::string& jsonFile  ,
                                              std::map<std::string, std::string>& actualParams);

    protected:
        DeviceActor(const std::string&     applicationname ,
                    const std::string&     actorname       ,
                    const std::string&     devicename      ,
                    const std::string&     jsonFile        ,
                    nlohmann::json jsonActorconfig         ,
                    nlohmann::json& configJson             ,
                    std::map<std::string, std::string>& commandLineParams
        );

        std::string _applicationName;
        std::string _deviceActorName;

        std::vector<component_conf> _component_configurations;
    };
}

#endif //RIAPS_CORE_R_DEVICE_H
