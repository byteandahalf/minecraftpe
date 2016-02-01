#pragma once

enum class TileType : int {Unspecified, StairTile, HalfSlabTile, HopperTile, TopSnowTile, FenceGate, LeafTile, GlassTile, ThinFenceTile, WallTile, CarpetTile, LiquidTile, Door};

class Tile
{
public:
	enum RenderLayer { RENDERLAYER_DOUBLE_SIDED, RENDERLAYER_BLEND, RENDERLAYER_OPAQUE, RENDERLAYER_OPTIONAL_ALPHATEST, RENDERLAYER_ALPHATEST, RENDERLAYER_ALPHATEST_SINGLE_SIDE, _RENDERLAYER_COUNT };

	class SoundType
	{
	public:
		float volume;
		float pitch;
		std::string breakSound;
		std::string stepSound;
	
		SoundType(const std::string&, float, float);
		SoundType(const std::string&, const std::string&, float, float);

		float getVolume() const;
		float getPitch() const;
		const std::string& getBreakSound() const;
		const std::string& getStepSound() const;
	};

	bool replaceable; // 4
	AABB visualShape; // 8
	TextureUVCoordinateSet texture; // 36
	TileID blockId; // 68
	SoundType* soundType; // 72
	Tile::RenderLayer renderPass; // 76
	bool canBuildOver; // 80
	int renderType; // 84
	TileType type; // 88
	TileEntityType tileEntityType; // 92
	float thickness; // 96
	bool slippery; // 100
	bool ticks; // 101
	float particleGravity; // 104
	Material* material; // 108
	float friction; // 112
	float destroyTime; // 116
	float explosionResistance; // 120
	int creativeCategory; // 124
	bool fancy; // 128
	int textureIsotropic; // 132
	std::string name; // 136

	/* static functions */
	static void setTextureAtlas(std::shared_ptr<TextureAtlas>);
	static void initTiles();
	static void teardownTiles();
	static TileID transformToValidBlockId(TileID);
	static TileID transformToValidBlockId(TileID, int, int, int);
	static int getIDByName(const std::string&, bool);
	static bool isTileType(const FullTile&, TileType);
	static bool isFaceVisible(TileSource*, int, int, int, FacingID);
	static Tile* getTile(int);
	static const Material* getTileMaterial(int);
	static int getLightEmission(const TileID);

	TileType getTileType() const;
	TextureUVCoordinateSet getTextureUVCoordinateSet(const std::string&, int);
	const TextureAtlasTextureItem& getTextureItem(const std::string&);
	int getRenderShape() const;
	float getShadeBrightness() const;
	bool isObstructingChests(TileSource*, const TilePos&);
	bool isTextureIsotropic(FacingID) const;
	Tile* setAllFacesIsotropic();
	void addAABB(const AABB&, const AABB*, std::vector<AABB>&);
	bool isSolidRender() const;
	void setSolid(bool);
	bool isUnbreakable() const;
	bool isLiquidTile() const;
	TileEntityType getTileEntityType() const;
	bool hasTileEntity();
	Tile::RenderLayer getRenderLayer() const;
	float getThickness() const;
	bool isAlphaTested();
	void popResource(TileSource*, int, int, int, const ItemInstance&);
	bool canBeBuiltOver() const;
	bool isReplaceableByPlacing() const;
	void destroyEffect(TileSource&, const TilePos&, const Vec3&);
	Tile* setCategory(int);
	MobSpawnerData* _getTypeToSpawn(TileSource&, int, const TilePos&) const;

	Tile(int, const Material*);
	Tile(int, TextureUVCoordinateSet, const Material*);	Tile(int, TextureUVCoordinateSet, const Material*);
	Tile(int, std::string, const Material*);

	/* vtable */
	virtual ~Tile();
	virtual bool onFertilized(TileSource*, int, int, int);
	virtual const AABB& getShape(TileSource*, int, int, int, AABB&, bool);
	virtual const AABB& getShape(DataID, AABB&, bool);
	virtual bool isObstructingChests(TileSource*, int, int, int);
	virtual bool shouldRenderFace(TileSource*, int, int, int, FacingID, const AABB&) const;
	virtual const TextureUVCoordinateSet& getTexture(FacingID);
	virtual const TextureUVCoordinateSet& getTexture(FacingID, int);
	virtual const TextureUVCoordinateSet& getTexture(TileSource*, int, int, int, FacingID);
	virtual std::vector<std::pair<TextureUVCoordinateSet, Rect2D>> getTessellatedUVs();
	virtual const TextureUVCoordinateSet& getCarriedTexture(FacingID, int);
	virtual const AABB& getAABB(TileSource*, int, int, int, AABB&, int, bool, int);
	virtual void addAABBs(TileSource*, int, int, int, const AABB*, std::vector<AABB>&);
	virtual const AABB& getTileAABB(TileSource*, int, int, int, AABB&);
	virtual bool isPathfindable(TileSource*, int, int, int);
	virtual bool mayPick();
	virtual bool mayPick(int, bool);
	virtual bool mayPlace(TileSource*, int, int, int, FacingID);
	virtual bool mayPlace(TileSource*, int, int, int);
	virtual int getTickDelay();
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual void animateTick(TileSource*, int, int, int, Random*);
	virtual void destroy(TileSource*, int, int, int, int);
	virtual void playerWillDestroy(Player*, int, int, int, int);
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int);
	virtual void onPlace(TileSource*, int, int, int);
	virtual void onRemove(TileSource*, int, int, int);
	virtual bool getSecondPart(TileSource&, const TilePos&, TilePos&);
	virtual void onGraphicsModeChanged(bool, bool);
	virtual int getResource(int, Random*);
	virtual int getResourceCount(Random*);
	virtual float getDestroyProgress(Player*);
	virtual void spawnResources(TileSource*, int, int, int, int, float);
	virtual bool spawnBurnResources(TileSource*, float, float, float);
	virtual float getExplosionResistance(Entity*);
	virtual HitResult clip(TileSource*, int, int, int, const Vec3&, const Vec3&, bool, int);
	virtual void wasExploded(TileSource*, int, int, int);
	virtual bool use(Player*, int, int, int);
	virtual void stepOn(Entity*, int, int, int);
	virtual void fallOn(TileSource*, int, int, int, Entity*, float);
	virtual int getPlacementDataValue(Mob*, int, int, int, FacingID, float, float, float, int);
	virtual void prepareRender(TileSource*, int, int, int);
	virtual void attack(Player*, int, int, int);
	virtual void handleEntityInside(TileSource*, int, int, int, Entity*, Vec3&);
	virtual int getColor(int);
	virtual int getColor(TileSource*, int, int, int);
	virtual bool isSignalSource();
	virtual bool getSignal(TileSource*, int, int, int);
	virtual bool getSignal(TileSource*, int, int, int, int);
	virtual bool getDirectSignal(TileSource*, int, int, int, int);
	virtual void entityInside(TileSource*, int, int, int, Entity*);
	virtual void playerDestroy(Player*, int, int, int, int);
	virtual bool canSurvive(TileSource*, int, int, int);
	virtual std::string getName() const;
	virtual std::string getDescriptionId() const;
	virtual std::string getDescriptionId(const ItemInstance*) const;
	virtual std::string getTypeDescriptionId(int);
	virtual Tile* setDescriptionId(const std::string&);
	virtual void triggerEvent(TileSource*, int, int, int, int, int);
	virtual const TextureUVCoordinateSet& getTextureNum(int);
	virtual const MobSpawnerData* getMobToSpawn(TileSource&, const TilePos&) const;
	virtual const int getIconYOffset() const;
	virtual void setShape(float, float, float, float, float, float);
	virtual Tile* setSoundType(const Tile::SoundType&);
	virtual Tile* setLightBlock(int);
	virtual Tile* setLightEmission(float);
	virtual Tile* setExplodeable(float);
	virtual Tile* setDestroyTime(float);
	virtual void setTicking(bool);
	virtual int getSpawnResourcesAuxValue(int);
	virtual Tile* init();

	static const Tile::SoundType SOUND_NORMAL;
	static const Tile::SoundType SOUND_WOOD;
	static const Tile::SoundType SOUND_GRAVEL;
	static const Tile::SoundType SOUND_GRASS;
	static const Tile::SoundType SOUND_STONE;
	static const Tile::SoundType SOUND_METAL;
	static const Tile::SoundType SOUND_GLASS;
	static const Tile::SoundType SOUND_CLOTH;
	static const Tile::SoundType SOUND_SAND;
	static const Tile::SoundType SOUND_SILENT;
	
	static Tile* tiles[];
	static bool shouldTick[];
	static bool solid[];
	static int lightBlock[];
	static float translucency[];
	static int lightEmission[];

	static std::shared_ptr<TextureAtlas> _terrainTextureAtlas;

	static Tile* rock;
	static Tile* grass;
	static Tile* dirt;
	static Tile* podzol;
	static Tile* stoneBrick;
	static Tile* wood;
	static Tile* sapling;
	static Tile* unbreakable;
	static Tile* water;
	static Tile* calmWater;
	static Tile* lava;
	static Tile* calmLava;
	static Tile* sand;
	static Tile* gravel;
	static Tile* goldOre;
	static Tile* ironOre;
	static Tile* coalOre;
	static Tile* log;
	static Tile* leaves;
	static Tile* sponge;
	static Tile* web;
	static Tile* glass;
	static Tile* thinGlass;
	static Tile* lapisOre;
	static Tile* lapisBlock;
	static Tile* dispenser;
	static Tile* sandStone;
	static Tile* musicBlock;
	static Tile* bed;
	static Tile* unused_27;
	static Tile* unused_28;
	static Tile* unused_29;
	static Tile* unused_30;
	static Tile* tallgrass;
	static Tile* unused_32;
	static Tile* unused_33;
	static Tile* unused_34;
	static Tile* unused_36;
	static Tile* cloth;
	static Tile* yellowFlower;
	static Tile* redFlower;
	static Tile* brownMushroom;
	static Tile* redMushroom;
	static Tile* goldBlock;
	static Tile* ironBlock;
	static Tile* stoneSlab;
	static Tile* stoneSlabHalf;
	static Tile* redBrick;
	static Tile* tnt;
	static Tile* bookshelf;
	static Tile* mossStone;
	static Tile* obsidian;
	static Tile* torch;
	static FireTile* fire;
	static Tile* mobSpawner;
	static Tile* stairs_wood;
	static Tile* chest;
	static Tile* redStoneDust;
	static Tile* diamondOre;
	static Tile* diamondBlock;
	static Tile* workBench;
	static Tile* stonecutterBench;
	static Tile* crops;
	static Tile* farmland;
	static Tile* furnace;
	static Tile* furnace_lit;
	static Tile* sign;
	static Tile* door_wood;
	static Tile* ladder;
	static Tile* rail;
	static Tile* goldenRail;
	static Tile* stairs_stone;
	static Tile* wallSign;
	static Tile* lever;
	static Tile* pressurePlate_stone;
	static Tile* door_iron;
	static Tile* pressurePlate_wood;
	static Tile* redStoneOre;
	static Tile* redStoneOre_lit;
	static Tile* notGate_off;
	static Tile* notGate_on;
	static Tile* button;
	static Tile* topSnow;
	static Tile* ice;
	static Tile* snow;
	static Tile* cactus;
	static Tile* clay;
	static Tile* reeds;
	static Tile* recordPlayer;
	static Tile* fence;
	static Tile* stairs_brick;
	static Tile* fenceGateOak;
	static Tile* fenceGateSpruce;
	static Tile* fenceGateBirch;
	static Tile* fenceGateJungle;
	static Tile* fenceGateBigOak;
	static Tile* fenceGateAcacia;
	static Tile* pumpkin;
	static Tile* hellRock;
	static Tile* hellSand;
	static Tile* lightGem;
	static Tile* portalTile;
	static Tile* litPumpkin;
	static Tile* cake;
	static Tile* diode_off;
	static Tile* diode_on;
	static Tile* trapdoor;
	static Tile* monsterStoneEgg;
	static Tile* stoneBrickSmooth;
	static Tile* melon;
	static Tile* pumpkinStem;
	static Tile* melonStem;
	static Tile* netherReactor;
	static Tile* glowingObsidian;
	static Tile* woodSlab;
	static Tile* woodSlabHalf;
	static Tile* activatorRail;
	static Tile* cobbleWall;
	static Tile* carrots;
	static Tile* potatoes;
	static Tile* beetroot;
	static Tile* woodStairsDark;
	static Tile* woodStairsBirch;
	static Tile* woodStairsJungle;
	static Tile* woodStairsAcacia;
	static Tile* woodStairsBigOak;
	static Tile* hayBlock;
	static Tile* coalBlock;
	static Tile* woolCarpet;
	static Tile* deadBush;
	static Tile* leaves2;
	static Tile* log2;
	static Tile* stairs_stoneBrickSmooth;
	static Tile* netherBrick;
	static Tile* netherrack;
	static Tile* ironFence;
	static Tile* netherFence;
	static Tile* stairs_netherBricks;
	static Tile* stairs_sandStone;
	static Tile* quartzBlock;
	static Tile* stairs_quartz;
	static Tile* endPortalFrame;
	static Tile* mycelium;
	static Tile* brownMushroomBlock;
	static Tile* redMushroomBlock;
	static Tile* stainedClay;
	static Tile* hardenedClay;
	static Tile* vine;
	static Tile* packedIce;
	static Tile* cocoa;
	static Tile* doublePlant;
	static Tile* endStone;
	static Tile* waterlily;
	static Tile* emeraldOre;
	static Tile* emeraldBlock;
	static Tile* invisible_bedrock;
	static Tile* info_updateGame1;
	static Tile* info_updateGame2;
	static Tile* info_reserved6;
};
