#include "ECS.h"

ECS ECS::Instance;
//std::unique_ptr<EntityManager> ECS::_entityManager;
//std::unique_ptr<ComponentManager> ECS::_componentManager;
//std::unique_ptr<SystemManager> ECS::_systemManager;

//void ECS::Init() {
//	_entityManager = std::make_unique<EntityManager>();
//	_componentManager = std::make_unique<ComponentManager>();
//	_systemManager = std::make_unique<SystemManager>();
//}

//Entity ECS::CreateEntity() {
//	return _entityManager->New();
//}

//void ECS::DestoryEntity(Entity entity) {
//	_entityManager->Destroy(entity);
//	_componentManager->EntityDestroyed(entity);
//	_systemManager->EntityDestroyed(entity);
//}

//template<typename T>
//void ECS::RegisterSystem() {
//	_systemManager->RegisterSystem<T>();
//}
//
//template<typename T>
//void ECS::SetSystemSignature(Signature signature) {
//	_systemManager->SetSignature<T>(signature);
//}
//
//template<typename T>
//void ECS::RegisterComponent() {
//	_componentManager->RegisterComponent<T>();
//}
//
//template<typename T>
//void ECS::AddComponent(Entity entity, T component) {
//	_componentManager->AddComponent(entity, component);
//}
//
//template<typename T>
//void ECS::RemoveComponent(Entity entity) {
//	_componentManager->RemoveComponent<T>(entity);
//}
//
//template<typename T>
//T& ECS::GetComponent(Entity entity) {
//	return _componentManager->GetComponent<T>(entity);
//}
