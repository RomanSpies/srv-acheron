#pragma once
#include <drogon/HttpController.h>
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>

#include <sodium.h>
#include <mimalloc-2.1/mimalloc.h>
#include <chrono>
#include <re2/re2.h>
#include <jwt-cpp/traits/open-source-parsers-jsoncpp/traits.h>
#include <folly/Hash.h>
#include "CacheAccess.h"





using namespace drogon;

class LastFMApi : public HttpController<LastFMApi>
{
public:

	METHOD_LIST_BEGIN
	ADD_METHOD_TO(LastFMApi::getBestOfSongs, "/bestOfSongs", Get);
	METHOD_LIST_END

	Task<void> getBestOfSongs(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
	




public:
	LastFMApi() = default;

};