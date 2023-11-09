#pragma once

enum LayerID
{
	DEFAULT = 0,
	PLAYER = 1,
	ASTEROID = 2,
	PROJECTILE = 4,
	OBSTACLE = 8,
	ITEM = 256,
	ENVIRONMENT = 512,
	ALL = 1023
};

class PhysicLayer
{
public:

	PhysicLayer();

	void AddLayer(const LayerID id) { this->id |= id; }
	void SetLayer(const LayerID id) { this->id = id; }

	std::bitset<MAX_LAYERS> GetLayer() const { return id; }
	bool IsLayerInMask(std::bitset<8> bitset) const;

private:

	std::bitset<MAX_LAYERS> id;
};

