#pragma once

#include <set>
#include "TypeDef.h"

namespace Chokbar {
	class System {

	public:

		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void Update(float delta) = 0;

	public:

		std::set<InstanceID> m_RegisteredEntities;

	};
}
