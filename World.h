#pragma once
#include <vector>

#include "Terrain.h"
#include "ModelRenderer.h"


class World : public EngineObject
{
public:
	World(ThreadManager*, Camera*, unsigned x, unsigned z);

	virtual void load(RessourceHandler* loader);
	virtual void init(KeyMap* map);

	virtual void update(ThreadManager*);
	virtual void render(Shader*, RenderingEngine::RenderState);

	static unsigned getMemPosForTerrain(int x, int z, bool invX, bool invZ);

	~World();
private:
	void TerrainForPos(vec2 pos, int xOff, int zOff);
	void setPos(vec2 pos);

	vec2 toWorldPos(vec3 pos) { return vec2(pos.x, pos.z); }

	unsigned chunkX, chunkZ;

	vector<Terrain*> allocator;
	float** _heightmap;
	Camera* player;

	mutex safeguard;

	Texture ground; //needs to become atlas
	bool initialized;
};