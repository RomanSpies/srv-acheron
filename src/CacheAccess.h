#pragma once
#include <memory>
#include <string>
#include <folly/PackedSyncPtr.h>
#include <folly/SharedMutex.h>
#include <folly/MPMCQueue.h>
#include <folly/AtomicHashMap.h>

struct LastFMCache
{
	std::string albumUrl;
	std::string albumCoverUrl;

	LastFMCache(const std::string& albumurl, const std::string& albumcoverurl) : albumUrl(albumurl), albumCoverUrl(albumcoverurl) {}

	LastFMCache(LastFMCache&& other) noexcept : albumUrl(std::move(other.albumUrl)), albumCoverUrl(std::move(other.albumCoverUrl)) {}

	LastFMCache& operator=(LastFMCache&& other) noexcept
	{
		if (this != &other)
		{
			albumUrl = std::move(other.albumUrl);
			albumCoverUrl = std::move(other.albumCoverUrl);
		}
		return *this;
	}
};



extern folly::SharedMutex jsonMutex;
extern folly::SharedMutex songNameMutex;

extern folly::SharedMutex matrixPresenceMutex;

extern folly::AtomicHashMap<size_t, LastFMCache> lastfmRequestCache;

extern folly::MPMCQueue<std::shared_ptr<Json::Value>> websocketEventQueue;

extern folly::MPMCQueue<std::shared_ptr<Json::Value>> synapsePresenceQueue;

extern std::shared_ptr<Json::Value> currentJsonResponse;
extern std::shared_ptr<Json::Value> currentPresenceState;

extern std::string currentSongName;

extern std::string apiKey;

extern std::string navidromeUserForScrobble;

extern std::string matrixIdForPresenceUpdates;

