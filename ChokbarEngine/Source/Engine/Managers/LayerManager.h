#pragma once


class LayerManager
{
	friend class Engine;
public:
	~LayerManager();

	static std::bitset<8> GetNextLayer();

private:


	LayerManager();

	static std::bitset<MAX_LAYERS> m_NextLayer;
	
};

