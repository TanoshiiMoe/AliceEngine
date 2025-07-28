#include "pch.h"
#include "TileMapLoader.h"
#include <fstream>
#include <stdexcept>
#include <Helpers/StringHelper.h>

// TileSet ����ü�� ���� JSON ���� �Լ�
void from_json(const nlohmann::json& j, TileSet& ts)
{
	j.at("columns").get_to(ts.columns);
	j.at("image").get_to(ts.image);
	j.at("imageheight").get_to(ts.imageheight);
	j.at("imagewidth").get_to(ts.imagewidth);
	j.at("margin").get_to(ts.margin);
	j.at("name").get_to(ts.name);
	j.at("spacing").get_to(ts.spacing);
	j.at("tilecount").get_to(ts.tilecount);
	j.at("tiledversion").get_to(ts.tiledversion);
	j.at("tileheight").get_to(ts.tileheight);
	j.at("tilewidth").get_to(ts.tilewidth);
	j.at("type").get_to(ts.type);
	j.at("version").get_to(ts.version);
}

void to_json(nlohmann::json& j, const TileSet& ts)
{
	j["columns"] = ts.columns;
	j["image"] = ts.image;
	j["imageheight"] = ts.imageheight;
	j["imagewidth"] = ts.imagewidth;
	j["margin"] = ts.margin;
	j["name"] = ts.name;
	j["spacing"] = ts.spacing;
	j["tilecount"] = ts.tilecount;
	j["tiledversion"] = ts.tiledversion;
	j["tileheight"] = ts.tileheight;
	j["tilewidth"] = ts.tilewidth;
	j["type"] = ts.type;
	j["version"] = ts.version;
}

// ���Ͽ��� TileSet �ε��ϴ� �Լ�
void TileMapLoader::LoadTileSet(const std::wstring& filePath, TileSet& tileSet)
{
	std::ifstream inFile(filePath);
	if (inFile.is_open()) {
		json j;
		inFile >> j;
		inFile.close();

		// JSON���� TileSet ����ü�� �ڵ� ��ȯ
		tileSet = j.get<TileSet>();
	}
	else {
		throw std::runtime_error("Failed to open tileset file: " + StringHelper::wstring_to_string(filePath));
	}
}

void TileMapLoader::LoadTileMap(const std::wstring& filePath, TileMap& tileMap)
{
	std::ifstream inFile(filePath);
	if (inFile.is_open()) {
		json j;
		inFile >> j;
		inFile.close();

		tileMap = j.get<TileMap>();
	}
	else {
		throw std::runtime_error("Failed to open tilemap file: " + StringHelper::wstring_to_string(filePath));
	}
}

// TileLayer JSON ����
void from_json(const json& j, TileMapLayer& tl) {
	j.at("data").get_to(tl.data);
	j.at("height").get_to(tl.height);
	j.at("id").get_to(tl.id);
	j.at("name").get_to(tl.name);
	j.at("opacity").get_to(tl.opacity);
	j.at("type").get_to(tl.type);
	j.at("visible").get_to(tl.visible);
	j.at("width").get_to(tl.width);
	j.at("x").get_to(tl.x);
	j.at("y").get_to(tl.y);
}

void to_json(json& j, const TileMapLayer& tl) {
	j = json{
		{"data", tl.data},
		{"height", tl.height},
		{"id", tl.id},
		{"name", tl.name},
		{"opacity", tl.opacity},
		{"type", tl.type},
		{"visible", tl.visible},
		{"width", tl.width},
		{"x", tl.x},
		{"y", tl.y}
	};
}
// TileMap JSON ����
void from_json(const json& j, TileMap& tm) {
	j.at("compressionlevel").get_to(tm.compressionlevel);
	j.at("height").get_to(tm.height);
	j.at("infinite").get_to(tm.infinite);
	j.at("layers").get_to(tm.layers);
	j.at("nextlayerid").get_to(tm.nextlayerid);
	j.at("nextobjectid").get_to(tm.nextobjectid);
	j.at("orientation").get_to(tm.orientation);
	j.at("renderorder").get_to(tm.renderorder);
	j.at("tiledversion").get_to(tm.version);
	j.at("tileheight").get_to(tm.tileHeight);
	j.at("tilewidth").get_to(tm.tileWidth);
	j.at("type").get_to(tm.type);
	j.at("version").get_to(tm.version);
	j.at("width").get_to(tm.width);
}

void to_json(json& j, const TileMap& tm) {
	j = json{
		{"compressionlevel", tm.compressionlevel},
		{"height", tm.height},
		{"infinite", tm.infinite},
		{"layers", tm.layers},
		{"nextlayerid", tm.nextlayerid},
		{"nextobjectid", tm.nextobjectid},
		{"orientation", tm.orientation},
		{"renderorder", tm.renderorder},
		{"tiledversion", tm.version},
		{"tileheight", tm.tileHeight},
		{"tilewidth", tm.tileWidth},
		{"type", tm.type},
		{"version", tm.version},
		{"width", tm.width}
	};
}