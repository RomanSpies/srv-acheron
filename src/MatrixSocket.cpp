#include "MatrixSocket.h"

MatrixSocket::MatrixSocket() : stopFlag_(false), eventThread_(&MatrixSocket::waitForEventsAndBroadcast, this) {}

MatrixSocket::~MatrixSocket()
{
	stopFlag_ = true;
	synapsePresenceQueue.write(nullptr);
	if (eventThread_.joinable())
	{
		eventThread_.join();
	}

}

void MatrixSocket::handleNewConnection(const HttpRequestPtr& req, const WebSocketConnectionPtr& conn)
{

	auto id = presenceUpdates_.subscribe("presence", [conn](const std::string& topic, const Json::Value& message)
		{
			conn->sendJson(message);
		});

	conn->setContext(std::make_shared<SubscriberID>(id));

	matrixPresenceMutex.lock_shared();
	Json::Value val = *currentPresenceState;
	matrixPresenceMutex.unlock_shared();

	conn->sendJson(val);
	LOG_INFO << "WebSocket client connected! ID: " << id;

	

}

void MatrixSocket::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
	auto id = conn->getContextRef<SubscriberID>();
	presenceUpdates_.unsubscribe("presence", id);
	LOG_INFO << "WebSocket client disconnected! ID: " << id;


}

void MatrixSocket::handleNewMessage(const WebSocketConnectionPtr& conn, std::string&& message, const WebSocketMessageType& type)
{
	if (type == WebSocketMessageType::Text)
	{
		auto id = conn->getContextRef<SubscriberID>();
		LOG_INFO << "WebSocket Client with ID " << id << " said: " << message;
	}
}

void MatrixSocket::waitForEventsAndBroadcast()
{

	std::shared_ptr<Json::Value> valuePointer;
	while (!stopFlag_)
	{

		synapsePresenceQueue.blockingRead(valuePointer);

		if (valuePointer)
		{
			matrixPresenceMutex.lock();

			currentPresenceState.reset();

			currentPresenceState = valuePointer;

			matrixPresenceMutex.unlock();


			const Json::Value& sendVal = *valuePointer;

			presenceUpdates_.publish("presence", sendVal);
		}

	}
}


