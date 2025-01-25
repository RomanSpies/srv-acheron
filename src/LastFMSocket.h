#pragma once
#include <drogon/WebSocketController.h>
#include <folly/SharedMutex.h>
#include "CacheAccess.h"
#include <drogon/PubSubService.h>

using namespace drogon;
class LastFMSocket : public WebSocketController<LastFMSocket>
{
public:
	WS_PATH_LIST_BEGIN
	WS_PATH_ADD("/lastFMSocket");
	WS_PATH_LIST_END

	void handleConnectionClosed(const WebSocketConnectionPtr& conn) override;

	void handleNewConnection(const HttpRequestPtr& req, const WebSocketConnectionPtr& conn) override;

	void handleNewMessage(const WebSocketConnectionPtr& conn, std::string&& message, const WebSocketMessageType& type) override;

public:
	LastFMSocket();
	~LastFMSocket() override;

private:
	std::atomic<bool> stopFlag_{ false };
	std::thread eventThread_;
	PubSubService<Json::Value> lastFmUpdates_;

	void waitForEventsAndBroadcast();
	


};
