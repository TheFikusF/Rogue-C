#pragma once
#include "Vec2.h"
#include "Player.h"
#include "Health.h"
#include <vector>

class Enemy
{
private:
    void Process(float ds);

public:
    int id;
    Vec2 position;
    float speed;
    float size;
    Health health;
    const Player* player;

    Enemy(Vec2 position, float speed, float size, int maxHealth, const Player* player);

    void Die();
    
public:
    static Enemy enemies[];
    static void ProcessAll(float ds);
    static void DrawAll();
};

void SpawnEnemy(Vec2 position, const Player* player);