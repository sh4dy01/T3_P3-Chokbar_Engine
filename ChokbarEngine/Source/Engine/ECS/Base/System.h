#pragma once

#include <set>
#include "TypeDef.h"

namespace Chokbar {
	class System {

	public:

		virtual void Update() = 0;

	public:

		std::set<Entity> m_AllEntities;

	};
}
