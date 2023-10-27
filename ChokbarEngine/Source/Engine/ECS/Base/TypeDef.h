#pragma once

#include <cstdint>
#include <bitset>

namespace Chokbar {

	using InstanceID = std::uint32_t;
	const InstanceID MAX_ENTITIES = 500;

	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 16;

	// The signature will be used to determine which components an entity has
	using Signature = std::bitset<MAX_COMPONENTS>;

}

