#include "Enemy.h"
#include "./include/raylib/raylib.h"

// Enemy::Enemy(Vec2 position, float speed, float size, int maxHealth, const Player* player) 
//     : position(position), speed(speed), size(size), player(player), id(), health(maxHealth, [this]() -> void { this->Die(); }) {
    
// }

// void Enemy::Die() {
//     //enemies.erase(enemies.begin() + id);
// }

// void Enemy::ProcessAll(float ds) {
//     // for(int i = enemies.size() - 1; i >= 0; i--) {
//     //     enemies[i].Process(ds);
//     // }
// }

// void Enemy::DrawAll() {
//     for(const Enemy &enemy : enemies) {
//         DrawCircle(enemy.position.x, enemy.position.y, enemy.size, RED);
//     }
// }

// void Enemy::Process(float ds) {
//     position += (player->position - position).GetNormalized() * speed * ds;
// }

// void SpawnEnemy(Vec2 position, const Player* player) { 
//     Enemy enemy(position, 40, 20, 5, player);
// }
