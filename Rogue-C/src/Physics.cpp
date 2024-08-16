#include "Physics.h"

#include <omp.h>
#include "LOG.h"

namespace Physics
{
    using namespace Core;
    
    Cell::Cell(Vec2 vec) : hash(std::floor(vec.x / GRID_SIZE) + std::floor(vec.y / GRID_SIZE)) {}

	bool Cell::operator==(const Cell other) const {
		return hash == other.hash;
	}

	std::size_t CellHasher::operator()(const Cell& k) const {
		return k.hash;
	}

    std::uint32_t updateGridTime = 0;
    std::uint32_t findCollisionsTime = 0;
    std::uint32_t lastRawDetectionTime = 0;
    std::uint32_t correctTime = 0;
    std::uint32_t lastIterationsCount = 0;

    std::uint32_t rawDetectionTime = 0;
    std::uint32_t iterationsCount = 0;

    PhysicsSystem::PhysicsSystem() {
        for (std::uint8_t i = 0; i < MAX_LAYERS; i++) {
            std::fill_n(layers[i], MAX_LAYERS, true);
        }
        signature.set(ECS::GetComponentType<MTransform>());
        signature.set(ECS::GetComponentType<Collider2D>());
        collisions.reserve(MAX_ENTITIES);
        _grid.reserve(MAX_ENTITIES);
    }

    void PhysicsSystem::SetLayer(Layer a, Layer b, bool flag) {
        layers[a][b] = flag;
        layers[b][a] = flag;
    }

    bool PhysicsSystem::WillCollide(Layer a, Layer b) { return layers[a][b]; }

    void PhysicsSystem::PhysicsUpdate(float dt) {
        collisions.clear();
        auto findClock = std::chrono::high_resolution_clock::now();
        UpdateGrid();
        auto resolveClock = std::chrono::high_resolution_clock::now();
        rawDetectionTime = 0;
        FindCollisions();
        lastRawDetectionTime = rawDetectionTime;
        auto updateClock = std::chrono::high_resolution_clock::now();
        ECS::HandleCollisions(collisions);
        ResolveCollisions();
        UpdateVelocities(dt);
        auto endClock = std::chrono::high_resolution_clock::now();

        updateGridTime = (resolveClock - findClock).count();
        findCollisionsTime = (updateClock - resolveClock).count();
        correctTime = (endClock - updateClock).count();
    }

    std::shared_ptr<PhysicsSystem> PhysicsSystem::RegisterPhysics() {
        ECS::RegisterComponent<Collider2D>();
        return ECS::RegisterSystem<PhysicsSystem>();
    }

    void PhysicsSystem::UpdateVelocities(float dt) {
        static const float gravity = 9.81f;

        for (auto const& entity : Entities) {
            MTransform& tr = ECS::GetComponent<MTransform>(entity);
            Collider2D& collider = ECS::GetComponent<Collider2D>(entity);

            if (collider.isStatic) {
                continue;
            }

            if (collider.useGravity) {
                collider.velocity.y -= gravity * dt;
            }

            tr.position += collider.velocity * dt;

            if (collider.kinematic) {
                collider.velocity = Vec2();
            }
        }
    }

    void PhysicsSystem::FindCollisions() {
        // std::vector<Entity> EntitiesVector(Entities.begin(), Entities.end());

        // #pragma omp parallel for
        lastIterationsCount = iterationsCount;
        iterationsCount = 0;
        
        for (Entity const entity : Entities) {
            const MTransform& tr = ECS::GetComponent<MTransform>(entity);
            const Collider2D& collider = ECS::GetComponent<Collider2D>(entity);

            //LOG(std::format("entities {}", Cell(tr.position).hash));
            GetNeighbors(entity, tr.position, collider, tr);
        }
    }

    inline void PhysicsSystem::CheckEntity(const Entity entity2, const Entity entity1, const Collider2D& collider1, const MTransform& tr1) {
            
        if (entity1 == entity2) {
            return;
        }


        auto updateClock = std::chrono::high_resolution_clock::now();
        const MTransform& tr2 = ECS::GetComponent<MTransform>(entity2);
        rawDetectionTime += (std::chrono::high_resolution_clock::now() - updateClock).count();
        const Collider2D& collider2 = ECS::GetComponent<Collider2D>(entity2);

        if (WillCollide(collider1.layer, collider2.layer) == false) {
            return;
        }

        if (collider1.isStatic == true && collider2.isStatic == true) {
            return;
        }
     
        Vec2 posA = _positions[entity1];
        Vec2 posB = _positions[entity2];

        Collision2D collision = IsColliding(posA, posB, tr1.scale, tr2.scale);
        collision.isTrigger = collider1.isTrigger || collider2.isTrigger;
        collision.a = entity1;
        collision.b = entity2;

        if (collision.hasCollision) {
            // #pragma omp critical
            collisions.emplace_back(collision);
        }
    }

    void PhysicsSystem::ResolveCollisions() {
        for (Collision2D const& collision : collisions) {
            Collider2D& collider1 = ECS::GetComponent<Collider2D>(collision.a);
            Collider2D& collider2 = ECS::GetComponent<Collider2D>(collision.b);

            if (collider1.isTrigger == true || collider2.isTrigger == true) {
                continue;
            }

            MTransform& tr1 = ECS::GetComponent<MTransform>(collision.a);
            MTransform& tr2 = ECS::GetComponent<MTransform>(collision.b);

            CorrectPositions(collider1, collider2, collision, tr1, tr2);

            CorrectVelocities(collider1, collider2, collision);
        }
    }

    void PhysicsSystem::CorrectPositions(const Collider2D& a, const Collider2D& b, const Collision2D& collision, MTransform& tr1, MTransform& tr2) {
        float totalMass = a.mass + b.mass;
        Vec2 correction = collision.normal * (collision.depth / totalMass);
        tr1.position = tr1.position - correction * (a.mass / totalMass);
        tr2.position = tr2.position + correction * (b.mass / totalMass);
    }

    void PhysicsSystem::CorrectVelocities(Collider2D& a, Collider2D& b, const Collision2D& collision) {
        Vec2 relativeVelocity = a.velocity - b.velocity;
        float velocityAlongNormal = Vec2::Dot(relativeVelocity, collision.normal);

        if (velocityAlongNormal > 0) {
            return;
        }

        float e = 1.0f;  // coefficient of restitution (1 for elastic, 0 for inelastic)
        float j = -(1 + e) * velocityAlongNormal / (1 / a.mass + 1 / b.mass);

        Vec2 impulse = collision.normal * j;
        a.velocity -= impulse / a.mass;
        a.velocity += impulse / b.mass;
    }

    void PhysicsSystem::UpdateGrid() {
        _grid.clear();
        for (Entity const entity : Entities) {
            const MTransform& tr = ECS::GetComponent<MTransform>(entity);
            Vec2 pos = MTransformSystem::GetRealPosition(entity);
            
            _positions[entity] = pos;
            _grid[pos + tr.scale].insert(entity);
            _grid[pos - tr.scale].insert(entity);
            _grid[pos + Vec2(tr.scale.x, -tr.scale.y)].insert(entity);
            _grid[pos + Vec2(-tr.scale.x, tr.scale.y)].insert(entity);
        }
    }

    inline void PhysicsSystem::GetNeighbors(Entity entityToCheck, Vec2 position, const Collider2D& collision, const MTransform& transform) {
        // lastIterationsCount = iterationsCount;
        // iterationsCount = 0;
        // for(std::int8_t y = 0; y < 1; ++y) {
        //     for(std::int8_t x = 0; x < 1; ++x) {
        for(std::int8_t y = -1; y < 2; ++y) {
            for(std::int8_t x = -1; x < 2; ++x) {
                Cell cell(position + Vec2(x * GRID_SIZE, y * GRID_SIZE));
                if(_grid.find(cell) == _grid.end()) {
                    continue;
                }

                for (Entity const entity : _grid[cell]) {
                    iterationsCount++;
                    CheckEntity(entity, entityToCheck, collision, transform);   
                }
            }    
        }
    }

    inline void PhysicsSystem::GetCell(Entity entityToCheck, Vec2 position, const Collider2D& collision, const MTransform& transform) {
        for (Entity const entity : _grid[position]) {
            iterationsCount++;
            CheckEntity(entity, entityToCheck, collision, transform);
        }
    }

    Collision2D IsColliding(Vec2 posA, Vec2 posB, Vec2 scaleA, Vec2 scaleB) {
        Vec2 pointA = posA + posB - posA;
        Vec2 pointB = posB + posA - posB;

        return Collision2D{
            .hasCollision = Vec2::DistanceSquared(posA, posB) <=
                            scaleA.x * scaleA.x + scaleB.x * scaleB.x,
            .a = 0,
            .b = 0,
            .pointA = pointA,
            .pointB = pointB,
            .normal = (posB - posA).GetNormalized(),
            .depth = Vec2::Distance(pointA, pointB),
        };
    }   
}