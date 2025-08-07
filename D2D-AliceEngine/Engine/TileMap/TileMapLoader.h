// ������������������������������������������������������������������������������������������
// TileParser.h
// ������������������������������������������������������������������������������������������
#pragma once
#include <Extension/json.hpp>
#include <unordered_map>
#include <vector>
#include <string>
using json = nlohmann::json;

// �� ���� ������
struct CollData {
	int index = -1;
	int collisionChannel = -1;
	int enemyType = -1;
	bool isUsable = true;
};

// blocks.tsj ���Ͽ� �����ϴ� ����ü
struct TileSet {
	int columns;                 // Ÿ�ϼ��� �� ��
	std::string image;           // �̹��� ���� ���
	int imageheight;             // �̹��� ����
	int imagewidth;              // �̹��� �ʺ�
	int margin;                  // �̹��� �����ڸ� ����
	std::string name;            // Ÿ�ϼ� �̸�
	int spacing;                 // Ÿ�� �� ����
	int tilecount;               // ��ü Ÿ�� ����
	std::string tiledversion;    // Tiled ����
	int tileheight;              // ���� Ÿ�� ����
	int tilewidth;               // ���� Ÿ�� �ʺ�
	std::string type;            // Ÿ�� ("tileset")
	std::string version;         // ���� ���� ����
};

/* ���������� Ÿ�ϸ�(Map) ���������� */
struct TileMapLayer {
	std::string name;
	int width{}, height{};
	int id;
	std::vector<int> data;            // GID ����Ʈ (row-major)
	bool visible{ true };
	float opacity{ 1.f };
	std::string type;
	int x{ 0 }, y{ 0 };
};

struct TileMap {
	std::string mapName;              // ���ϸ� ��
	int compressionlevel{ 0 };       // ���� ���� (0 = ����)
	bool infinite{ false };           // ���� �� ����
	int nextlayerid{ 0 };            // ���� ���̾� ID
	int nextobjectid{ 0 }; 		 // ���� ������Ʈ ID
	std::string version;         // Tiled ����
	int width{}, height{};
	int tileWidth{}, tileHeight{};
	std::string orientation;          // "orthogonal"��
	std::string renderorder;          // "right-down" ��
	std::string type;        // "map" ����
	std::vector<TileMapLayer> layers;
	std::vector<int> usedGIDs;        // �ߺ� ���ŵ� GID ����
};

struct TileMapColiderInfo 
{
	int index;						// 1������ ����
	int collisionChannel;			// �ݸ��� ���̾�
	int enemyType;					// �� Ÿ��
};

/* ���������� �δ�(���� ��ƿ) ���������� */
class TileMapLoader {
public:
	static void LoadTileSet(const std::wstring& filePath, TileSet& tileSet);
	static void LoadTileMap(const std::wstring& filePath, TileMap& tileMap); // TileMap�� Load�Լ��� Parse�Լ� �߰�
	static std::unordered_map<int, CollData> LoadTileMapColliderInfo(const std::wstring& filePath); // TileMap�� Load�Լ��� Parse�Լ� �߰�
};

void from_json(const nlohmann::json& j, TileSet& ts);
void to_json(nlohmann::json& j, const TileSet& ts);
void from_json(const json& j, TileMapLayer& l);
void to_json(json& j, const TileMapLayer& l);
void from_json(const json& j, TileMap& m); 
void to_json(json& j, const TileMap& m);
void from_json(const json& j, TileMapColiderInfo& m);
void to_json(json& j, const TileMapColiderInfo& m);
