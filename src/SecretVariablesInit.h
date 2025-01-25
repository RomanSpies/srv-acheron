#pragma once

#include <drogon/plugins/Plugin.h>
#include <drogon/drogon.h>
#include "CacheAccess.h"

namespace lastfmapi
{

    class SecretVariablesInit : public drogon::Plugin<SecretVariablesInit>
    {
    public:
        SecretVariablesInit() = default;

        void initAndStart(const Json::Value& config) override;

        void shutdown() override;

        

    
    };

}
