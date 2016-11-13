#include "World.h"

World::World(ThreadManager* manager, Camera* ref, unsigned x, unsigned z) : EngineObject(), player(ref),
chunkX(x), chunkZ(z), allocator(16, nullptr), initialized(false), ground(string("assets/textures/Test_tex2.bmp"))
{
	_heightmap = (float**)malloc(sizeof(float) * (( 4 * chunkX) + 1));
	for (unsigned i = 0; i < ((4 * chunkX) + 1); i++)
		_heightmap[i] = (float*)malloc(sizeof(float) * ((4 * chunkZ) + 1));
}

void World::load(RessourceHandler& loader)
{
	ground.load(loader);
}

void World::init(KeyMap& map)
{
	if (initialized)
		return;
	initialized = true;
	ground.glDownload();
}

void World::update(ThreadManager& mgr)
{
	lock_guard<mutex> lock(safeguard);
	setPos(toWorldPos(player->getPos()));
	for (Terrain* terr : allocator)
		terr->update(mgr);
}

void World::render(Shader& s, RenderingEngine::RenderState state)
{
	s.bind();
	ground.bind();
	for (Terrain* terr : allocator)
	{
		if (terr)
		{
			if (terr->isInit())
				terr->Draw();
			else
				terr->init();
		}
	}
}

World::~World()
{
	for (Terrain* terr : allocator)
	{
		delete terr;
	}
	for (unsigned i = 0; i < 4 * chunkX; i++)
		delete[] _heightmap[i];
	delete[] _heightmap;
}

unsigned World::getMemPosForTerrain(int x, int z, bool invX, bool invZ)
{
	int bitX, bitZ;
	bitX = x % 4;
	bitZ = z % 4;

	if (invX)
		bitX = 4 - bitX;
	if (invZ)
		bitZ = 4 - bitZ;

	return (bitX | (bitZ << 2)) % 16;
}

void World::TerrainForPos(vec2 p, int xO, int zO)
{
	int xOff = (xO - 2) * chunkX
	  , zOff = (zO - 2) * chunkZ;
	unsigned pos = getMemPosForTerrain((int)(abs(xOff) / chunkX), (int)(abs(zOff) / chunkZ), xOff < 0, zOff < 0);

	if (!allocator[pos] || !(allocator[pos]->isPos(xOff, zOff))) //checks if cache is correct
	{
		float** heightmap = (float**)malloc(sizeof(float*) * (chunkX + 1));
		unsigned it = 0;
		for (; it <= chunkX; it++)
			heightmap[it] = &_heightmap[((pos & 3) * chunkX) + it][(pos & 12) * chunkZ];
		allocator[pos] = move(new Terrain(100.0f, xOff, zOff, chunkX, chunkZ, heightmap));

		free(heightmap);
	}
}

void World::setPos(vec2 pos)
{
	for (unsigned char x = 0; x < 4; x++)
		for (unsigned char z = 0; z < 4; z++)
			TerrainForPos(pos, x, z);
}
