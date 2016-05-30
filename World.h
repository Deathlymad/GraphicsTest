#pragma once
#include <vector>

#include "Terrain.h"
#include "ModelRenderer.h"


class World : public EngineObject
{
public:
	World(Camera*, unsigned x, unsigned z);

	virtual void init(KeyMap* map);

	virtual void update();
	virtual void render(Shader*, RenderingEngine::RenderState);

	~World();
private:
	unsigned getMemPosForTerrain(int x, int z, bool invX, bool invZ);
	void TerrainForPos(int x, int z);
	void setPos(vec2 pos);

	vec2 toWorldPos(vec3 pos) { return vec2(pos.x, pos.z); }

	NoiseGraph generator;
	unsigned chunkX, chunkZ;

	vector<Terrain> allocator;
	Camera* player;

	bool initialized;
};