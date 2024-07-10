#pragma once
#include "ComponentArray.h"
#include <unordered_map>
#include "Entity.h"
#include <memory>

class ComponentManager {
public:
	template<typename T>
	void RegisterComponent() {

	}

private:
	std::unordered_map<const char*, ComponentType> mComponentTypes;

	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays;

	ComponentType mNextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}

};