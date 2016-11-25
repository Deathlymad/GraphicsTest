#include "World.h"

World::World(ThreadManager* manager, Camera* ref, unsigned x, unsigned z) : EngineObject(), player(ref),
chunkX(x), chunkZ(z), allocator(16), initialized(false), ground(string("assets/textures/Test_tex2.bmp")), _heightmap( chunkX * 4, chunkZ * 4)
{
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
	{
		lock_guard<mutex> lock(safeguard);
		setPos(toWorldPos(player->getPos()));
	}
	for (Terrain& terr : allocator)
		terr.update(mgr);
}

void World::render(Shader& s, RenderingEngine::RenderState state)
{
	s.bind();
	ground.bind( s);
	for (Terrain& terr : allocator)
	{
		if (terr.isInit())
			terr.Draw();
		else
			terr.init();
	}
}

World::~World()
{
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

	if (!allocator[pos].isPos(xOff, zOff)) //checks if cache is correct
	{
		allocator[pos].setPos(chunkX, chunkZ, xOff, zOff, _heightmap.get((((pos & 12) >> 2) * chunkZ), ((pos & 3) * chunkX)));
	}
}

void World::setPos(vec2 pos)
{
	for (unsigned char x = 0; x < 4; x++)
		for (unsigned char z = 0; z < 4; z++)
			TerrainForPos(pos, x, z);
}



World::Heightmap::Heightmap(unsigned x, unsigned z) : _x(x), _z(z)
{
	_mem.reset(new float[x * z]);
	_pos0 = _mem.get();
}

World::Heightmap::Heightmap(const Heightmap &other) : _x(other._x), _z(other._z)
{
	_mem.reset(new float[_x * _z]);
	_pos0 = _mem.get();
	memcpy(_pos0, other._pos0, sizeof(float) * _x * _z);
}

World::Heightmap::Heightmap(Heightmap &&other) : _x(other._x), _z(other._z)
{
	_mem.reset(other._mem.release());
	_pos0 = _mem.get();
}

World::Heightmap & World::Heightmap::operator=(const Heightmap &other)
{
	_x = other._x;
	_z = other._z;
	_mem.reset(new float[_x * _z]);
	_pos0 = _mem.get();
	memcpy(_pos0, other._pos0, sizeof(float) * _x * _z);
	return *this;
}

World::Heightmap & World::Heightmap::operator=(Heightmap &&other)
{
	_x = other._x;
	_z = other._z;
	_mem.reset(other._mem.release());
	_pos0 = _mem.get();
	return *this;
}

float * World::Heightmap::get()
{
	return _pos0;
}

float * World::Heightmap::get(unsigned x, unsigned z)
{
	return &_pos0[(x * _z) + z];
}

float * World::Heightmap::operator[](unsigned x)
{
	return &_pos0[x * _z];
}
