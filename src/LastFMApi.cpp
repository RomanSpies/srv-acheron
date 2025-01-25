#include "LastFMApi.h"


Task<void> LastFMApi::getBestOfSongs(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback)
{
	auto dbAccess = app().getFastDbClient("postgres");
	auto queryResult = co_await dbAccess->execSqlCoro("select song, album, artist, count(*) as number, to_char(max(timestamp), 'DD-MM-YYYY') as lastplay from scrobbled group by song, album, artist order by number desc limit 50");

	Json::Value columns(Json::arrayValue);
	columns.append("song");
	columns.append("album");
	columns.append("artist");
	columns.append("playcount");
	columns.append("lastplayed");

	Json::Value data(Json::arrayValue);


	for (const auto& row : queryResult)
	{
		Json::Value rowData(Json::arrayValue);
		rowData.append(row[0].as<std::string>());
		rowData.append(row[1].as<std::string>());
		rowData.append(row[2].as<std::string>());
		rowData.append(row[3].as<int>());
		rowData.append(row[4].as<std::string>());
		data.append(rowData);

	}

	Json::Value result;
	result["meta"]["total"] = queryResult.size();
	result["meta"]["limit"] = 50;
	result["columns"] = columns;
	result["data"] = data;

	auto resp = HttpResponse::newHttpJsonResponse(std::move(result));
	resp->setStatusCode(k200OK);
	resp->addHeader("Access-Control-Allow-Origin", "*");
	resp->addHeader("Access-Control-Allow-Methods", "GET");
	resp->addHeader("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Authorization");
	resp->addHeader("Access-Control-Max-Age", "86400");
	callback(resp);
	co_return;
}

