#include <iostream>
#include <drogon/drogon.h>
#include <sodium.h>
#include <trantor/utils/Logger.h>
#include <cstdlib>
#include "CacheAccess.h"


using namespace drogon;


folly::SharedMutex jsonMutex;
folly::SharedMutex clientsMutex;
folly::SharedMutex matrixPresenceMutex;

std::shared_ptr<Json::Value> currentJsonResponse;
std::shared_ptr<Json::Value> currentPresenceState;


std::string apiKey;

folly::AtomicHashMap<size_t, LastFMCache> lastfmRequestCache(5000);

folly::MPMCQueue<std::shared_ptr<Json::Value>> websocketEventQueue(1024);
folly::MPMCQueue<std::shared_ptr<Json::Value>> synapsePresenceQueue(1024);


int main()
{
	setenv("SSL_CERT_DIR", "./certs", 1);
	currentPresenceState = std::make_shared<Json::Value>();
	currentJsonResponse = std::make_shared<Json::Value>();
	Json::Value& currentPresenceRef = *currentPresenceState;
	currentPresenceRef["user"] = "@roman:comms.rospies.dev";
	currentPresenceRef["presence"] = "offline";
	sodium_init();
	std::cout << "Waiting 120 seconds..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(120));
	std::cout << "Launching app..." << std::endl;

	

	app().loadConfigFile("config.json").run();
	
}
