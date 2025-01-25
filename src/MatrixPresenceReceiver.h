#pragma once
#include <drogon/HttpController.h>
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>

#include <sodium.h>
#include <mimalloc-2.1/mimalloc.h>
#include "CacheAccess.h"





using namespace drogon;

class MatrixPresenceReceiver : public HttpController<MatrixPresenceReceiver>
{
public:

	METHOD_LIST_BEGIN
	ADD_METHOD_TO(MatrixPresenceReceiver::deliveryEndpoint, "/matrixPresenceUpdate", Post);
	METHOD_LIST_END

	Task<void> deliveryEndpoint(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);





public:
	MatrixPresenceReceiver() = default;

};