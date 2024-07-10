#include "System.h"

template<typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
	const char* typeName = typeid(T).name();

	assert(_systems.find(typeName) == _systems.end() && "Registering system more than once.");

	auto system = std::make_shared<T>();
	_systems.insert({ typeName, system });
	return system;
}

template<typename T>
void SystemManager::SetSignature(Signature signature) {
	const char* typeName = typeid(T).name();

	assert(_systems.find(typeName) != _systems.end() && "System used before registered.");

	_signatures.insert({ typeName, signature });
}

void SystemManager::EntityDestroyed(Entity entity)
{
	for (auto const& pair : _systems)
	{
		auto const& system = pair.second;

		system->Entities.erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
	for (auto const& pair : _systems)
	{
		auto const& type = pair.first;
		auto const& system = pair.second;
		auto const& systemSignature = _signatures[type];

		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->Entities.insert(entity);
		}
		else
		{
			system->Entities.erase(entity);
		}
	}
}
