#include "LayerManager.h"

LayerManager::LayerManager()
{
	m_NextLayer = 0;
}

std::bitset<8> LayerManager::GetNextLayer()
{
	std::bitset<8> temp = m_NextLayer;

	return m_NextLayer <<= 1;
}
