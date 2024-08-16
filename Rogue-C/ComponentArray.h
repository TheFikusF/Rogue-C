#pragma once
#include <unordered_map>
#include "Entity.h"
#include "ASSERT.h"

namespace Core {
	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray {
	public:
		ComponentArray() {
	#if ARRAY_COMPONENT_ARRAYS
			for(Entity i = 0; i < MAX_ENTITIES; i++) {
				_sparseToDense[i] = MAX_ENTITIES;
				_denseToSparse[i] = MAX_ENTITIES;
			}
	#endif
			_components.reserve(MAX_ENTITIES);
		}

		void AddComponent(Entity entity, T component) {
			_sparseToDense[entity] = _size;
			_denseToSparse[_size] = entity;
	#ifdef MAP_COMPONENT_ARRAYS
			if(_size >= _components.size())
			{
				_components.push_back(component);
			}
			else
			{
				_components[_size] = component;
			}
	#elif ARRAY_COMPONENT_ARRAYS
			_components[_size] = component;
	#endif
			_size++;
		}
		
		void RemoveComponent(Entity entity) {
	#ifdef MAP_COMPONENT_ARRAYS
			ASSERT(_sparseToDense.contains(entity), "There is no such component on this entity");
	#elif ARRAY_COMPONENT_ARRAYS
			ASSERT(_sparseToDense[entity] < MAX_ENTITIES, "There is no such component on this entity");
	#endif
			Entity indexOfRemoved = _sparseToDense[entity];
			Entity indexOfLast = _size - 1;
			Entity entityOfLast = _denseToSparse[indexOfLast];

			_components[indexOfRemoved] = _components[indexOfLast];

			_sparseToDense[entityOfLast] = indexOfRemoved;
			_denseToSparse[indexOfRemoved] = entityOfLast;
			

	#ifdef MAP_COMPONENT_ARRAYS
			_sparseToDense.erase(entity);
			_denseToSparse.erase(entityOfLast);
	#elif ARRAY_COMPONENT_ARRAYS
			_sparseToDense[entity] = MAX_ENTITIES;
			_denseToSparse[entityOfLast] = MAX_ENTITIES;
	#endif

			_size--;
		}
		
		T& GetData(Entity entity) {
			//ASSERT(_sparseToDense.find(entity) != _sparseToDense.end(), std::format("Can't get component {}. There is no such component on this {}.", typeid(T).name(), entity));
			return _components[_sparseToDense[entity]];
		}
		
		void EntityDestroyed(Entity entity) override {
	#ifdef MAP_COMPONENT_ARRAYS
			if(_sparseToDense.contains(entity) == false) {
				return;
			}
	#elif ARRAY_COMPONENT_ARRAYS
			if(_sparseToDense[entity] >= MAX_ENTITIES) {
				return;
			}
	#endif
			RemoveComponent(entity);
		}

	private:
	#ifdef MAP_COMPONENT_ARRAYS
		std::unordered_map<Entity, Entity> _sparseToDense{ MAX_ENTITIES };
		std::unordered_map<Entity, Entity> _denseToSparse{ MAX_ENTITIES };
	#elif ARRAY_COMPONENT_ARRAYS
		std::array<Entity, MAX_ENTITIES> _sparseToDense{ MAX_ENTITIES };
		std::array<Entity, MAX_ENTITIES> _denseToSparse{ MAX_ENTITIES };
	#endif

		std::vector<T> _components;
		Entity _size = 0;
	};
}