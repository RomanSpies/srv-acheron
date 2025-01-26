#pragma once
#include <drogon/HttpController.h>
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>
#include <mimalloc-2.1/mimalloc.h>
#include "CacheAccess.h"
#include <folly/io/async/EventBase.h>
#include <folly/io/async/AsyncTimeout.h>
#include <cmath>




using namespace drogon;

class ScrobbleUpdate : public HttpController<ScrobbleUpdate>
{
public:

	METHOD_LIST_BEGIN
	ADD_METHOD_TO(ScrobbleUpdate::receiveUpdate, "/scrobbleUpdate", Post);
	METHOD_LIST_END

	Task<void> receiveUpdate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);


	


public:
	ScrobbleUpdate();
	~ScrobbleUpdate() override;


private:
	folly::EventBase eventBase_;
	std::shared_ptr<folly::AsyncTimeout> timer_;
	std::thread eventLoopThread_;
};