#include "MatrixPresenceReceiver.h"

Task<void> MatrixPresenceReceiver::deliveryEndpoint(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback)
{
	const std::shared_ptr<Json::Value>& presenceUpdate = req->getJsonObject();
	auto resp = HttpResponse::newHttpResponse();
	if (!(presenceUpdate && presenceUpdate->isMember("presence")))
	{
		resp->setStatusCode(k400BadRequest);
		callback(resp);
		co_return;
	}
	const Json::Value& presenceUpdateData = *presenceUpdate;

	auto eventPtr = std::make_shared<Json::Value>();
	Json::Value& eventRef = *eventPtr;

	eventRef["user"] = presenceUpdateData["user_id"];
	eventRef["presence"] = presenceUpdateData["presence"];

	synapsePresenceQueue.write(eventPtr);

	resp->setStatusCode(k200OK);
	callback(resp);
	co_return;


}
