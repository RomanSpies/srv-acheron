#include "LastFMSocket.h"

LastFMSocket::LastFMSocket() : stopFlag_(false), eventThread_(&LastFMSocket::waitForEventsAndBroadcast, this) {}

LastFMSocket::~LastFMSocket()
{
	stopFlag_ = true;
	websocketEventQueue.write(nullptr);
	if (eventThread_.joinable())
	{
		eventThread_.join();
    } 
	
}

void LastFMSocket::handleNewConnection(const HttpRequestPtr& req, const WebSocketConnectionPtr& conn)
{
	
	auto id = lastFmUpdates_.subscribe("updates", [conn](const std::string& topic, const Json::Value& message)
		{
			conn->sendJson(message);
		});

	conn->setContext(std::make_shared<SubscriberID>(id));

	jsonMutex.lock_shared();
	Json::Value val = *currentJsonResponse;
	jsonMutex.unlock_shared();

	conn->sendJson(val);
	LOG_INFO << "WebSocket client connected! ID: " << id;

	const std::string& userIP = req->getHeader("X-Forwarded-For");

	auto dbAccessor = app().getFastDbClient("postgres");

	dbAccessor->execSqlAsync("INSERT INTO accessed (ip) VALUES ($1)",
		[](const orm::Result& r)
		{
			LOG_INFO << "User access logged!";
		},
		[](const orm::DrogonDbException& e)
		{
			LOG_ERROR << "User access failed to log!";
		},
		userIP);

}

void LastFMSocket::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
	auto id = conn->getContextRef<SubscriberID>();
	lastFmUpdates_.unsubscribe("updates", id);
	LOG_INFO << "WebSocket client disconnected! ID: " << id;
	

}

void LastFMSocket::handleNewMessage(const WebSocketConnectionPtr& conn, std::string&& message, const WebSocketMessageType& type)
{
	if (type == WebSocketMessageType::Text)
	{
		auto id = conn->getContextRef<SubscriberID>();
		LOG_INFO << "WebSocket Client with ID " << id  << " said: " << message;
	}
}

void LastFMSocket::waitForEventsAndBroadcast()
{

	std::shared_ptr<Json::Value> valuePointer;
	while (!stopFlag_)
	{
		  
		websocketEventQueue.blockingRead(valuePointer);

		if (valuePointer)
        {
        	jsonMutex.lock();

        	currentJsonResponse.reset();

        	currentJsonResponse = valuePointer;

        	jsonMutex.unlock();


        	const Json::Value& sendVal = *valuePointer;

        	lastFmUpdates_.publish("updates", sendVal);
		}

	}
}



