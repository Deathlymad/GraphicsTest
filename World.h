#pragma once
#include <vector>

#include "Terrain.h"
#include "ModelRenderer.h"


class World : public EngineObject
{
	class Heightmap
	{
	public:
		Heightmap(unsigned x, unsigned z);
		explicit Heightmap(const Heightmap&);
		explicit Heightmap(Heightmap&&);
		Heightmap& operator=(const Heightmap&);
		Heightmap& operator=(Heightmap&&);

		float* get();
		float* get( unsigned x, unsigned z);

		float* operator[] (unsigned x);

	private:
		unique_ptr<float> _mem;
		float* _pos0;
		unsigned _x, _z;
	};
public:
	World(ThreadManager*, Camera*, unsigned x, unsigned z);

	virtual void load(RessourceHandler& loader);
	virtual void init(KeyMap& map);

	virtual void update(ThreadManager&);
	virtual void render(Shader&, RenderingEngine::RenderState);

	static unsigned getMemPosForTerrain(int x, int z, bool invX, bool invZ);

	~World();
private:
	void TerrainForPos(vec2 pos, int xOff, int zOff);
	void setPos(vec2 pos);

	vec2 toWorldPos(vec3 pos) { return vec2(pos.x, pos.z); }

	unsigned chunkX, chunkZ;

	vector<Terrain> allocator;
	Heightmap _heightmap;
	Camera* player;

	mutex safeguard;

	Texture ground; //needs to become atlas
	bool initialized;
};