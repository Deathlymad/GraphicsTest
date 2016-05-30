#include "World.h"



World::World(Camera* ref, unsigned x, unsigned z) : EngineObject(), player(ref), generator(NoiseGraph(2)),
chunkX(x), chunkZ(z), allocator(16, Terrain(generator, 0, 0, 0, 0)), initialized(false)
{

}

void World::init(KeyMap* map)
{
	if (initialized)
		return;
	setPos(vec2(0, 0));
	initialized = true;
}

void World::update()
{
}

void World::render(Shader* s, RenderingEngine::RenderState state)
{
	setPos(toWorldPos(player->getPos())); //make asynchronous (init OGL at later Point in )
	s->bind();
	for (unsigned i = 0; i < 16; i++)
	{
		allocator[i].Draw();
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

void World::TerrainForPos(int x, int z)
{
	float xOff, zOff;
	if ( x < 0)
		xOff = (-x / chunkX);
	else
		xOff = (x / chunkX);
	if (z < 0)
		zOff = (-z / chunkZ);
	else
		zOff = (z / chunkZ);

	unsigned pos = getMemPosForTerrain(xOff, zOff, x < 0, z < 0);

	if (!allocator[pos].distToPoint(vec3(x, 0, z))) //reallocates if it is the wrong Terrain
	{
		allocator.erase(allocator.begin() + pos);
		allocator.insert( allocator.begin() + pos, Terrain(generator, x, z, chunkX, chunkZ));
		allocator[pos].init();
	}
}

void World::setPos(vec2 pos)
{
	int xOff, zOff;
	xOff = roundEven(pos.x / chunkX) - 2 * chunkX;
	zOff = roundEven(pos.y / chunkZ) - 2 * chunkZ;

	for (unsigned char x = 0; x < 4; x++)
		for (unsigned char z = 0; z < 4; z++)
			TerrainForPos(xOff + (x) * chunkX, zOff + (z) * chunkZ);
}
