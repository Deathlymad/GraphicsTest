#pragma once
#include <vector>

#include "Terrain.h"
#include "ModelRenderer.h"


class World : public EngineObject
{
public:
	World(Camera*, unsigned x, unsigned z);

	virtual void load(RessourceHandler* loader);
	virtual void init(KeyMap* map);

	virtual void update();
	virtual void render(Shader*, RenderingEngine::RenderState);

	~World();
private:
	unsigned getMemPosForTerrain(int x, int z, bool invX, bool invZ);
	void TerrainForPos(vec2 pos, int xOff, int zOff);
	void setPos(vec2 pos);

	vec2 toWorldPos(vec3 pos) { return vec2(pos.x, pos.z); }

	NoiseGraph generator;
	unsigned chunkX, chunkZ;

	vector<Terrain> allocator;
	Camera* player;

	Texture ground;
	bool initialized;
};