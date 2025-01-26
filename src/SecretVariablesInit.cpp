#include  "SecretVariablesInit.h"

using namespace drogon;
using namespace lastfmapi;

void SecretVariablesInit::initAndStart(const Json::Value& config)
{
	apiKey = config["api_key_lastfm"].asString();
	matrixIdForPresenceUpdates = config["matrixIdForPresenceUpdates"].asString();
	navidromeUserForScrobble = config["navidromeUserForScrobble"].asString();

	matrixPresenceMutex.lock();
	Json::Value& currentPresenceRef = *currentPresenceState;
	currentPresenceRef["user"] = matrixIdForPresenceUpdates;
	currentPresenceRef["presence"] = "offline";
	matrixPresenceMutex.unlock();
}

void SecretVariablesInit::shutdown()
{
	
}

