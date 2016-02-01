

#include "Tile.h"

Tile::Tile(int blockId, const Material* material)
{
	replaceable = false;
	this->blockId = blockId;
	thickness = 0.0F;
	explosionResistance = 0.0F;
	canBuildOver = false;
	soundType = &SoundType::NORMAL;
	renderType = 0;
	type = TileType::Unspecified;
	tileEntityType = TileEntityType::TILEENTITY_NONE;
	slippery = false;
	ticks = false;
	this->material = material;
	textureIsotropic = 0;
	friction = 0.6F;
	particleGravity = 1.0F;
	renderPass = RenderLayer::RENDERLAYER_OPAQUE;
	name = "";
	if(tiles[blockId])
	{
		printf("Slot %d is already occupied by %p when adding %p", blockId, tiles[blockId], this);
	}

	translucency[blockId] = -1.0F;
	solid[blockId] = true;
	lightBlock[blockId] = 0xFF;
}
Tile::Tile(int blockId, TextureUVCoordinateSet texture, const Material* material) : Tile(blockId, material), texture(texture){}
Tile::Tile(int blockId, std::string texture, const Material* material) : Tile(blockId, getTextureUVCoordinateSet(texture, 0), material){}

void Tile::setTextureAtlas(std::shared_ptr<TextureAtlas> _atlas)
{
	_terrainTextureAtlas = _atlas;
}

TileID Tile::transformToValidBlockId(TileID oldID)
{
	transformToValidBlockId(oldID, 0, 0, 0);
}

/* Prevents a crash when opening a world with a NULL tile in the world
	by replacing it with an update block, thanks Shoghi
*/
TileID Tile::transformToValidBlockId(TileID oldID, int x, int y, int z)
{
	if(oldID == 0 || tiles[oldID] != NULL)
	{
		return oldID;
	}
	else
	{
		return (((x + y + z) << 31) < 0)? info_updateGame1->blockId : info_updateGame2->blockId;
	}
}

int Tile::getIDByName(const std::string& name, bool par1bool)
{
	/* TODO */
	for(Tile* tile : tiles)
	{
		if(Util::toLower(tile->getDescriptionId()) == Util::toLower(name))
		{
			return tile->blockId;
		}
	}

	return 0xFFFFFFFF; // I don't know why it returns this
}

bool Tile::isTileType(const FullTile& tile, TileType checkType)
{
	return tiles[tile.blockId] && tiles[tile.blockId]->tileType == checkType;
}

/* Checks if the Player can see this block's face */
bool Tile::isFaceVisible(TileSource* region, int x, int y, int z, FacingID face)
{
	switch(face)
	{
	case 0:
		return !region->isSolidRenderTile(x, y - 1, z);
	case 1:
		return !region->isSolidRenderTile(x, y + 1, z);
	case 2:
		return !region->isSolidRenderTile(x, y, z - 1);
	case 3:
		return !region->isSolidRenderTile(x, y, z + 1);
	case 4:
		return !region->isSolidRenderTile(x - 1, y, z);
	case 5:
		return !region->isSolidRenderTile(x + 1, y, z);
	default:
		return !region->isSolidRenderTile(x, y, z);
	}
}

Tile* Tile::getTile(int blockId)
{
	return tiles[blockId];
}

const Material* Tile::getTileMaterial(int blockId)
{
	Tile* tile = getTile(blockId)
	return (tile)? tile->material : Material::air;
}

int Tile::getLightEmission(const TileID blockId)
{
	return lightEmission[blockId];
}

TileType Tile::getTileType() const
{
	return tileType;
}

TextureUVCoordinateSet Tile::getTextureUVCoordinateSet(const std::string& textureName, int textureIndex)
{
	return _terrainTextureAtlas->getTextureItem(textureName)[textureIndex];
}

const TextureAtlasTextureItem& Tile::getTextureItem(const std::string& textureName)
{
	return _terrainTextureAtlas->getTextureItem(textureName);
}

int Tile::getRenderShape() const
{
	return renderType;
}

float Tile::getShadeBrightness() const
{
	if(solid[blockId] || tileType == TileType::LeafTile)
	{
		return 0.2F;
	}
	return 1.0F;
}

bool Tile::isObstructingChests(TileSource* region, const TilePos& pos)
{
	return isObstructingChests(region, pos.x, pos.y, pos.z);
}

/* Determines if a texture on this block is identical regardless of orientation
	Used when determining what blocks can have randomly rotated textures (ie. sand)
*/
bool Tile::isTextureIsotropic(FacingID face) const
{
	/* TODO, just guessing what this does since the function is inline */
	return ((textureIsotropic >> face) & 0xF) > 0;
}

/* Allow all textures on this block to rotate randomly (ie. sand) */
Tile* Tile::setAllFacesIsotropic()
{
	textureIsotropic = 0xFF;
	return this;
}

/* pushes the given AABB to the AABB pool */
void Tile::addAABB(const AABB& aabb, const AABB* par2AABB, std::vector<AABB>& pool)
{
	if(aabb->isEmpty() || !par2AABB->intersects(aabb))
	{
		return;
	}
	pool.emplace_back(aabb);
}

bool Tile::isSolidRender() const
{
	/* TODO */
	return renderType == 0;
}

void Tile::setSolid(bool solid)
{
	Tile::solid[blockId] = solid;
}

bool Tile::isUnbreakable() const
{
	return destroyTime == -1.0;
}

bool Tile::isLiquidTile() const
{
	return material == Material::water || material == Material::lava;
}

TileEntityType Tile::getTileEntityType() const
{
	return tileEntityType;
}

bool Tile::hasTileEntity()
{
	return tileEntityType != TileEntityType::TILEENTITY_NONE; // 0
}

Tile::RenderLayer Tile::getRenderLayer() const
{
	return renderPass;
}

float Tile::getThickness() const
{
	return thickness;
}

/* Checks if this block has transparent textures */
bool Tile::isAlphaTested()
{
	return renderLayer == RenderLayer::RENDERLAYER_OPTIONAL_ALPHATEST ||
			renderLayer == RenderLayer::RENDERLAYER_ALPHATEST ||
			renderLayer == RenderLayer::RENDERLAYER_ALPHATEST_SINGLE_SIDE;
}

/* Drop an ItemInstance at this Block's position */
void Tile::popResource(TileSource* region, int x, int y, int z, const ItemInstance& itemStack)
{
	Level* level = region->getLevel();
	if(level->_clientSide || level->getLevelData().getGameType() == 1)
	{
		return;
	}
	/* TODO: random number magic */
	ItemEntity* droppedItem = new ItemEntity(*region, (float) x, (float) y, (float) z, itemStack);
	droppedItem->throwTime = 10;
	level->addEntity(droppedItem);
}

bool Tile::canBeBuiltOver() const
{
	return canBuildOver;
}

bool Tile::isReplaceableByPlacing() const
{
	return replaceable;
}

void Tile::destroyEffect(TileSource& region, const TilePos& pos, const Vec3& par2Vec3)
{
	region.getLevel()->destroyEffect(region, pos.x, pos.y, pos.z, par2Vec3);
}

Tile* Tile::setCategory(int category)
{
	creativeCategory = category;
	return this;
}

MobSpawnerData* Tile::_getTypeToSpawn(TileSource& region, int par1int, const TilePos& pos) const
{
	Level* level = region.getLevel();
	Biome::MobList& _mobs = region.getBiome(pos)->getMobs(par1int);
	if(_mobs.empty())
	{
		return;
	}

	int count = _mobs.size();

	if(count != 0)
	{
		unsigned int var0int = level.rng.genrand_int32();
		/* TODO */
	}

	return &_mobs[0];
}

/* Called when the block is fertilized with bone meal
	Used by crops
*/
bool Tile::onFertilized(TileSource*, int, int, int)
{
	return false;
}

const AABB& Tile::getShape(TileSource* region, int x, int y, int z, AABB& par4AABB, bool par5bool)
{
	return getShape(region->getData(x, y, z), par4AABB, par5bool);
}

const AABB& Tile::getShape(DataID, AABB&, bool)
{
	return visualShape;
}

bool Tile::isObstructingChests(TileSource* region, int x, int y, int z)
{
	return solid[blockId];
}

/* Checks if the given face should be rendered
	Always returns false with invisible bedrock
*/
bool Tile::shouldRenderFace(TileSource* region, int x, int y, int z, FacingID face, const AABB& aabb) const
{
	if(face == 0 && aabb.min.y > 0.0F ||
		face == 1 && aabb.max.y < 1.0F ||
		face == 2 && aabb.min.z > 0.0F ||
		face == 3 && aabb.max.z < 1.0F ||
		face == 4 && aabb.min.x > 0.0F ||
		face == 5 && aabb.max.z < 1.0F)
	{
		return true;
	}

	Tile* tile = tiles[region->getTileAndData({x, y, z}).blockId];
	if(!tile)
	{
		return true;
	}

	if((tile == invisible_bedrock) || ((tile == topSnow || tile == woolCarpet) && face == 1))
	{
		return false;
	}

	if(tile == leaves || tile == leaves2)
	{
		return !LeafTile::isDeepLeafTile(*region, {x, y, z});
	}

	return false;
}

const TextureUVCoordinateSet& Tile::getTexture(FacingID side)
{
	return texture;
}

/* Gets texture in the GUI */
const TextureUVCoordinateSet& Tile::getTexture(FacingID side, int data)
{
	return getTexture(side);
}

/* Gets texture in the world */
const TextureUVCoordinateSet& Tile::getTexture(TileSource* region, int x, int y, int z, FacingID side)
{
	return getTexture(side, region->getData(x, y, z));
}

std::vector<std::pair<TextureUVCoordinateSet, Rect2D>> Tile::getTessellatedUVs()
{
	return std::vector<std::pair<TextureUVCoordinateSet, Rect2D>>();
}

const TextureUVCoordinateSet& Tile::getCarriedTexture(FacingID side, int data)
{
	return getTexture(side, data);
}

const AABB& Tile::getAABB(TileSource* region, int x, int y, int z, AABB& aabb, int data, bool par6bool, int par7int)
{
	AABB retval = getShape(data, aabb, false);
	aabb.set(retval);
	return retval.move(x, y, z);
}

/* Add collision boxes */
void Tile::addAABBs(TileSource* region, int x, int y, int z, const AABB* par4AABB, std::vector<AABB>& pool)
{
	AABB var0aabb();
	addAABB(getAABB(region, x, y, z, var0aabb, 0, false, 0), par4AABB, pool);
}

const AABB& Tile::getTileAABB(TileSource* region, int x, int y, int z, AABB& aabb)
{
	return getAABB(region, x, y, z, aabb, 0, false, 0);
}

/* Checks if mobs can walk towards this block when pathfinding */
bool Tile::isPathfindable(TileSource*, int, int, int)
{
	return !material->blocksMotion;
}

/* Can this block be highlighted with the pointer */
bool Tile::mayPick()
{
	return true;
}

bool Tile::mayPick(int, bool)
{
	return mayPick();
}

bool Tile::mayPlace(TileSource* region, int x, int y, int z, FacingID face)
{
	return mayPlace(region, x, y, z);
}

bool Tile::mayPlace(TileSource* region, int x, int y, int z)
{
	int blockId = region->getTile(x, y, z).blockId;
	return blockId == 0 || tiles[blockId]->material->replaceable;
}

int Tile::getTickDelay()
{
	return 10;
}

/* Called every world tick to execute Block logic, etc */
void Tile::tick(TileSource*, int, int, int, Random*){}
/* Called every world tick to spawn particles, etc on this block */
void Tile::animateTick(TileSource*, int, int, int, Random*){}
/* Called when this block is destroyed by the player */
void Tile::destroy(TileSource*, int, int, int, int){}
/* Called when the player is destroying the block */
void Tile::playerWillDestroy(Player*, int, int, int, int){}
/* Called when a block next to this block is changed and a block update is scheduled */
void Tile::neighborChanged(TileSource*, int, int, int, int, int, int){}
/* Called when this block is added to the world (placed by the player or otherwise) */
void Tile::onPlace(TileSource*, int, int, int){}
/* Called when this block is removed (broken by the player or otherwise) */
void Tile::onRemove(TileSource*, int, int, int){}

/* Does this Block take up 2 block spaces
	Used by Chests and Beds
*/
bool Tile::getSecondPart(TileSource&, const TilePos&, TilePos&)
{
	return false;
}

void Tile::onGraphicsModeChanged(bool par0bool, bool par1bool)
{
	fancy = par0bool;
}

int Tile::getResource(int, Random*)
{
	return blockId;
}

int Tile::getResourceCount(Random*)
{
	return 1;
}

float Tile::getDestroyProgress(Player* player)
{
	if(destroyTime < 0.0F)
	{
		return 0.0F;
	}

	if(player->canDestroy(this))
	{
		return (player->getDestroySpeed(this) / destroyTime) * 0.033333;
	}

	return 0.01F / destroyTime;
}

void Tile::spawnResources(TileSource*, int, int, int, int, float)
{
	/* TODO: Do this */
}

bool Tile::spawnBurnResources(TileSource*, float, float, float)
{
	return false;
}

float Tile::getExplosionResistance(Entity*)
{
	return explosionResistance * 0.2F;
}

HitResult Tile::clip(TileSource* region, int x, int y, int z, const Vec3& par4Vec3, const Vec3& par5Vec3, bool par6bool, int par7int)
{
	/* TODO: this is not the code you are looking for */
	return HitResult(x, y, z, 0, par4Vec3);
}

/* Called when this block is blown up by TNT or a Creeper */
void Tile::wasExploded(TileSource*, int, int, int){}

/* Called when this block is right clicked (or tapped on) */
bool Tile::use(Player*, int, int, int)
{
	return false;
}

/* Called when the block is stepped on
	Doesn't actually seem to be called
*/
void Tile::stepOn(Entity*, int, int, int){}
/* Called when the block is landed on */
void Tile::fallOn(TileSource*, int, int, int, Entity*, float){}

int Tile::getPlacementDataValue(Mob* placer, int x, int y, int z, FacingID side, float par5float, float par6float, float par7float, int data)
{
	return data;
}

void Tile::prepareRender(TileSource*, int, int, int){}
/* Appears to do the same thing as ::use */
void Tile::attack(Player*, int, int, int){}
/* Used by water to push Entities with its current */
void Tile::handleEntityInside(TileSource*, int, int, int, Entity*, Vec3&){}

int Tile::getColor(int)
{
	return 0xFFFFFFFF;
}

int Tile::getColor(TileSource* region, int x, int y, int z)
{
	return 0xFFFFFFFF;
}

bool Tile::isSignalSource()
{
	return false;
}

bool Tile::getSignal(TileSource*, int, int, int)
{
	return false;
}

bool Tile::getSignal(TileSource*, int, int, int, int)
{
	return false;
}

bool Tile::getDirectSignal(TileSource*, int, int, int, int)
{
	return false;
}

/* Called when an entity is inside the hitbox of this block */
void Tile::entityInside(TileSource*, int, int, int, Entity*){}
/* Called when the player breaks this block */
void Tile::playerDestroy(Player* player, int x, int y, int z, int par4int)
{
	spawnResources(player->region, x, y, z, par4int, 1.0F);
}

bool Tile::canSurvive(TileSource*, int, int, int)
{
	return true;
}

std::string Tile::getName() const
{
	/* TODO: Only certain C++ STLs have std::string::operator+(const std::string&) AFAIK */
	return I18n::get(getDescriptionId() + ".name");
}

std::string Tile::getDescriptionId() const
{
	return name;
}

std::string Tile::getDescriptionId(const ItemInstance*) const
{
	return getDescriptionId();
}

std::string Tile::getTypeDescriptionId(int)
{
	return "";
}

Tile* Tile::setDescriptionId(const std::string& descriptionId)
{
	name = "tile." + descriptionId;
	return this;
}

/* Called when an event occurs for this block
	Used by TileEntities
*/
void Tile::triggerEvent(TileSource*, int, int, int, int, int){}

const TextureUVCoordinateSet& Tile::getTextureNum(int)
{
	return texture;
}

const MobSpawnerData* Tile::getMobToSpawn(TileSource& region, const TilePos& pos) const
{
	if(region.getBrightness(pos) > 8.0F)
	{
		return NULL;
	}

	return _getTypeToSpawn(region, 0xB00, pos);
}

const int Tile::getIconYOffset() const
{
	return 0;
}

void Tile::setShape(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	visualShape.set(minX, minY, minZ, maxX, maxY, maxZ);
}

Tile* Tile::setSoundType(const Tile::SoundType& soundType)
{
	this->soundType = soundType;
	return this;
}

Tile* Tile::setLightBlock(int _lightBlock)
{
	lightBlock[blockId] = _lightBlock;
	return this;
}

Tile* Tile::setLightEmission(float _emission)
{
	lightEmission[blockId] = _emission * 15.0F;
	return this;
}

Tile* Tile::setExplodeable(float _explodeable)
{
	explosionResistance = _explodeable * 3.0F;
	return this;
}

Tile* Tile::setDestroyTime(float _time)
{
	destroyTime = _time;
	if(explosionResistance < _time * 5.0F)
	{
		explosionResistance = _time * 5.0F;
	}

	return this;
}

void Tile::setTicking(bool _ticking)
{
	shouldTick[blockId] = _ticking;
}

int Tile::getSpawnResourcesAuxValue(int)
{
	return 0;
}

Tile* Tile::init()
{
	tiles[blockId] = this;
	if(translucency[blockId] < 0.0F)
	{
		if((renderType > 31) || (-6.0F & (1 << renderType)) == 0) // wut
		{
			translucency[blockId] = (material->translucency >= 0.8F)? material->translucency : 0.8F;
		}

		translucency[blockId] = material->translucency;
	}

	return this;
}

/* TODO: init these */
const Tile::SoundType Tile::SOUND_NORMAL;
const Tile::SoundType Tile::SOUND_WOOD;
const Tile::SoundType Tile::SOUND_GRAVEL;
const Tile::SoundType Tile::SOUND_GRASS;
const Tile::SoundType Tile::SOUND_STONE;
const Tile::SoundType Tile::SOUND_METAL;
const Tile::SoundType Tile::SOUND_GLASS;
const Tile::SoundType Tile::SOUND_CLOTH;
const Tile::SoundType Tile::SOUND_SAND;
const Tile::SoundType Tile::SOUND_SILENT;

Tile* Tile::tiles[256];
bool Tile::shouldTick[256];
bool Tile::solid[256];
int Tile::lightBlock[256];
float Tile::translucency[256];
int Tile::lightEmission[256];

std::shared_ptr<TextureAtlas> Tile::_terrainTextureAtlas;

Tile* Tile::rock;
Tile* Tile::grass;
Tile* Tile::dirt;
Tile* Tile::podzol;
Tile* Tile::stoneBrick;
Tile* Tile::wood;
Tile* Tile::sapling;
Tile* Tile::unbreakable;
Tile* Tile::water;
Tile* Tile::calmWater;
Tile* Tile::lava;
Tile* Tile::calmLava;
Tile* Tile::sand;
Tile* Tile::gravel;
Tile* Tile::goldOre;
Tile* Tile::ironOre;
Tile* Tile::coalOre;
Tile* Tile::log;
Tile* Tile::leaves;
Tile* Tile::sponge;
Tile* Tile::web;
Tile* Tile::glass;
Tile* Tile::thinGlass;
Tile* Tile::lapisOre;
Tile* Tile::lapisBlock;
Tile* Tile::dispenser;
Tile* Tile::sandStone;
Tile* Tile::musicBlock;
Tile* Tile::bed;
Tile* Tile::unused_27;
Tile* Tile::unused_28;
Tile* Tile::unused_29;
Tile* Tile::unused_30;
Tile* Tile::tallgrass;
Tile* Tile::unused_32;
Tile* Tile::unused_33;
Tile* Tile::unused_34;
Tile* Tile::unused_36;
Tile* Tile::cloth;
Tile* Tile::yellowFlower;
Tile* Tile::redFlower;
Tile* Tile::brownMushroom;
Tile* Tile::redMushroom;
Tile* Tile::goldBlock;
Tile* Tile::ironBlock;
Tile* Tile::stoneSlab;
Tile* Tile::stoneSlabHalf;
Tile* Tile::redBrick;
Tile* Tile::tnt;
Tile* Tile::bookshelf;
Tile* Tile::mossStone;
Tile* Tile::obsidian;
Tile* Tile::torch;
FireTiTile::le* fire;
Tile* Tile::mobSpawner;
Tile* Tile::stairs_wood;
Tile* Tile::chest;
Tile* Tile::redStoneDust;
Tile* Tile::diamondOre;
Tile* Tile::diamondBlock;
Tile* Tile::workBench;
Tile* Tile::stonecutterBench;
Tile* Tile::crops;
Tile* Tile::farmland;
Tile* Tile::furnace;
Tile* Tile::furnace_lit;
Tile* Tile::sign;
Tile* Tile::door_wood;
Tile* Tile::ladder;
Tile* Tile::rail;
Tile* Tile::goldenRail;
Tile* Tile::stairs_stone;
Tile* Tile::wallSign;
Tile* Tile::lever;
Tile* Tile::pressurePlate_stone;
Tile* Tile::door_iron;
Tile* Tile::pressurePlate_wood;
Tile* Tile::redStoneOre;
Tile* Tile::redStoneOre_lit;
Tile* Tile::notGate_off;
Tile* Tile::notGate_on;
Tile* Tile::button;
Tile* Tile::topSnow;
Tile* Tile::ice;
Tile* Tile::snow;
Tile* Tile::cactus;
Tile* Tile::clay;
Tile* Tile::reeds;
Tile* Tile::recordPlayer;
Tile* Tile::fence;
Tile* Tile::stairs_brick;
Tile* Tile::fenceGateOak;
Tile* Tile::fenceGateSpruce;
Tile* Tile::fenceGateBirch;
Tile* Tile::fenceGateJungle;
Tile* Tile::fenceGateBigOak;
Tile* Tile::fenceGateAcacia;
Tile* Tile::pumpkin;
Tile* Tile::hellRock;
Tile* Tile::hellSand;
Tile* Tile::lightGem;
Tile* Tile::portalTile;
Tile* Tile::litPumpkin;
Tile* Tile::cake;
Tile* Tile::diode_off;
Tile* Tile::diode_on;
Tile* Tile::trapdoor;
Tile* Tile::monsterStoneEgg;
Tile* Tile::stoneBrickSmooth;
Tile* Tile::melon;
Tile* Tile::pumpkinStem;
Tile* Tile::melonStem;
Tile* Tile::netherReactor;
Tile* Tile::glowingObsidian;
Tile* Tile::woodSlab;
Tile* Tile::woodSlabHalf;
Tile* Tile::activatorRail;
Tile* Tile::cobbleWall;
Tile* Tile::carrots;
Tile* Tile::potatoes;
Tile* Tile::beetroot;
Tile* Tile::woodStairsDark;
Tile* Tile::woodStairsBirch;
Tile* Tile::woodStairsJungle;
Tile* Tile::woodStairsAcacia;
Tile* Tile::woodStairsBigOak;
Tile* Tile::hayBlock;
Tile* Tile::coalBlock;
Tile* Tile::woolCarpet;
Tile* Tile::deadBush;
Tile* Tile::leaves2;
Tile* Tile::log2;
Tile* Tile::stairs_stoneBrickSmooth;
Tile* Tile::netherBrick;
Tile* Tile::netherrack;
Tile* Tile::ironFence;
Tile* Tile::netherFence;
Tile* Tile::stairs_netherBricks;
Tile* Tile::stairs_sandStone;
Tile* Tile::quartzBlock;
Tile* Tile::stairs_quartz;
Tile* Tile::endPortalFrame;
Tile* Tile::mycelium;
Tile* Tile::brownMushroomBlock;
Tile* Tile::redMushroomBlock;
Tile* Tile::stainedClay;
Tile* Tile::hardenedClay;
Tile* Tile::vine;
Tile* Tile::packedIce;
Tile* Tile::cocoa;
Tile* Tile::doublePlant;
Tile* Tile::endStone;
Tile* Tile::waterlily;
Tile* Tile::emeraldOre;
Tile* Tile::emeraldBlock;
Tile* Tile::invisible_bedrock;
Tile* Tile::info_updateGame1;
Tile* Tile::info_updateGame2;
Tile* Tile::info_reserved6;

/* Intialize Tiles */
void Tile::initTiles()
{
	/* TODO: Do ALL this */
}

void Tile::teardownTiles()
{
	for(int i = 0; i < 256; i++)
	{
		if(!tiles[i])
		{
			continue;
		}

		delete tiles[i];
		tiles[i] = NULL;
	}
}
