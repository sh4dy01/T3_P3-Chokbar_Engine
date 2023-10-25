#pragma once

#include <cstdint>
#include <bitset>

namespace Chokbar {

	using Entity = std::uint32_t;
	const Entity MAX_ENTITIES = 1000;

	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	// The signature will be used to determine which components an entity has
	using Signature = std::bitset<MAX_COMPONENTS>;

}

