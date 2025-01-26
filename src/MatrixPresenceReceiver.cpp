#include "MatrixPresenceReceiver.h"

Task<void> MatrixPresenceReceiver::deliveryEndpoint(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback)
{
	const std::shared_ptr<Json::Value>& presenceUpdate = req->getJsonObject();
	auto resp = HttpResponse::newHttpResponse();
	if (!presenceUpdate)
	{
		resp->setStatusCode(k400BadRequest);
		callback(resp);
		co_return;
	}
	if (!presenceUpdate->isMember(matrixIdForPresenceUpdates))
	{
		resp->setStatusCode(k202Accepted);
		LOG_INFO << "Presence update was received, but state of designated user has not changed - skipping...";
		callback(resp);
		co_return;
	}

	const Json::Value& presenceUpdateData = *presenceUpdate;
	const Json::Value& userPresenceUpdateData = presenceUpdateData[matrixIdForPresenceUpdates];

	auto eventPtr = std::make_shared<Json::Value>();
	Json::Value& eventRef = *eventPtr;

	eventRef["user"] = userPresenceUpdateData["user_id"];
	eventRef["presence"] = userPresenceUpdateData["presence"];
	eventRef["last_active"] = userPresenceUpdateData["last_active"];
	eventRef["status_msg"] = userPresenceUpdateData["last_active"];

	synapsePresenceQueue.write(eventPtr);

	resp->setStatusCode(k200OK);
	callback(resp);
	co_return;


}
