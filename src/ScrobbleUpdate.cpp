#include "ScrobbleUpdate.h"



ScrobbleUpdate::ScrobbleUpdate()
{
	

	timer_ = folly::AsyncTimeout::make(eventBase_, [this]() noexcept
	{
		jsonMutex.lock_shared();
		std::shared_ptr<Json::Value> corrected = std::make_shared<Json::Value>(*currentJsonResponse);
		jsonMutex.unlock_shared();
		Json::Value& refer_corrected = *corrected;
		refer_corrected["recent_tracks"][0]["now_playing"] = false;
		websocketEventQueue.write(corrected);
		LOG_INFO << "Switched to idle playback state...";
	});

	eventLoopThread_ = std::thread([this]()
		{
			eventBase_.loopForever();
		});
}

ScrobbleUpdate::~ScrobbleUpdate()
{
	eventBase_.runInEventBaseThread([this]()
		{
			if (timer_)
			{
				timer_->cancelTimeout();
				timer_.reset();
			}
			eventBase_.terminateLoopSoon();
		});

	if (eventLoopThread_.joinable())
	{
		eventLoopThread_.join();
	}
}





Task<void> ScrobbleUpdate::receiveUpdate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback)
{
	const std::shared_ptr<Json::Value>& scrobbleRequest = req->getJsonObject();
	auto resp = HttpResponse::newHttpResponse();

	if (!scrobbleRequest || !scrobbleRequest->isMember("song"))
	{
		LOG_ERROR << "scrobble request from navidrome failed to process";
		resp->setStatusCode(k400BadRequest);
		callback(resp);
		co_return;
	}


	const Json::Value& scrobbleValue = *scrobbleRequest;
	std::string username = scrobbleValue["user"].asString();
	if (username != navidromeUserForScrobble)
	{
		LOG_WARN << "Scrobble request was made, but does not match designated user. Skipping....";
		resp->setStatusCode(k204NoContent);
		callback(resp);
		co_return;
	}

	std::string songname = scrobbleValue["song"].asString();
	std::string albumname = scrobbleValue["album"].asString();
	std::string artistname = scrobbleValue["artist"].asString();
	int timeoutduration = static_cast<int>(std::round(1.1 * scrobbleValue["tracklength"].asInt() + 5));
	std::string album_url;
	std::string album_cover_url;

	folly::Hash hashfunc;
	size_t searchKey = hashfunc(albumname + artistname);

	auto iter = lastfmRequestCache.find(searchKey);

	if (iter != lastfmRequestCache.end())
	{
		LOG_INFO << "Song " << songname << " found in cache - skipping LastFM Query. Current size of Cache: " << lastfmRequestCache.size() << " entries";
		album_url = iter->second.albumUrl;
		album_cover_url = iter->second.albumCoverUrl;
	}
	else
	{
		LOG_INFO << "New song - requesting info for " << songname << " from lastFM API";
		auto client = HttpClient::newHttpClient("https://ws.audioscrobbler.com");

		auto httpreq = HttpRequest::newHttpRequest();

		httpreq->setMethod(Get);
		httpreq->setPath("/2.0/?method=track.getInfo");
		httpreq->setParameter("track", songname);
		httpreq->setParameter("artist", artistname);
		httpreq->setParameter("api_key", apiKey);
		httpreq->setParameter("format", "json");
		httpreq->setParameter("autocorrect", "1");

		auto lastfmresponse = co_await client->sendRequestCoro(httpreq, 10.0);

		std::shared_ptr<Json::Value> lastfmjptr = lastfmresponse ? lastfmresponse->getJsonObject() : nullptr;

		if (!lastfmjptr)
		{
			LOG_ERROR << "LastFm sent malformed JSON";
			lastfmjptr = std::make_shared<Json::Value>();
		}

		const Json::Value& lastfmjson = *lastfmjptr;

		std::string temp_url = lastfmjson["track"]["album"]["url"].asString();
		album_url = !temp_url.empty() ? temp_url : "void";
		std::string temp_cover = lastfmjson["track"]["album"]["image"][2]["#text"].asString();
		album_cover_url = !temp_cover.empty() ? temp_cover : "https://rospies.dev/notfound.png";

		LastFMCache newCache(album_url, album_cover_url);

		lastfmRequestCache.insert(searchKey, std::move(newCache));
		LOG_INFO << "Caching new entry. Current size of Cache: " << lastfmRequestCache.size() << " entries";

	}
	

	std::shared_ptr<Json::Value> commitValPtr = std::make_shared<Json::Value>();
	Json::Value& commitVal = *commitValPtr;

	Json::Value recentTracks(Json::arrayValue);
	Json::Value lastTrack;
	commitVal["user"] = "Marneus__Calgar";

	lastTrack["artist"] = scrobbleValue["artist"];
	lastTrack["title"] = scrobbleValue["song"];
	lastTrack["url"] = album_url;
	lastTrack["album_cover"] = album_cover_url;
	lastTrack["now_playing"] = true;

	recentTracks.append(lastTrack);

	commitVal["recent_tracks"] = recentTracks;

	websocketEventQueue.write(commitValPtr);
	LOG_INFO << "New song placed into event Queue";

	eventBase_.runInEventBaseThread([this, timeoutduration]()
		{
			if (timer_)
			{
				timer_->cancelTimeout();
				timer_->scheduleTimeout(std::chrono::seconds(timeoutduration));
			}
		});

	auto dbAccessor = app().getFastDbClient("postgres");

	dbAccessor->execSqlAsync("INSERT INTO scrobbled (username, song, artist, album) VALUES ($1, $2, $3, $4)",
		[](const orm::Result& r)
		{
			LOG_INFO << "Listed song in DB!";
		},
		[](const orm::DrogonDbException& e)
		{
			LOG_ERROR << "Failed to list song in DB!";
		},
		"Marneus__Calgar", songname, artistname , albumname

	);

	
	resp->setStatusCode(k200OK);
	callback(resp);
	co_return;
	

}
