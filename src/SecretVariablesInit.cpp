#include  "SecretVariablesInit.h"

using namespace drogon;
using namespace lastfmapi;

void SecretVariablesInit::initAndStart(const Json::Value& config)
{
	apiKey = config["api_key_lastfm"].asString();
}

void SecretVariablesInit::shutdown()
{
	
}

