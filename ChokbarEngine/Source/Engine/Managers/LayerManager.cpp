#include "LayerManager.h"

PhysicLayer::PhysicLayer()
	: id(DEFAULT)
{
}

bool PhysicLayer::IsLayerInMask(std::bitset<8> bitset) const
{
	return (id & bitset) != 0;
}
