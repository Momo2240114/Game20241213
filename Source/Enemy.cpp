#include "Enemy.h"
#include "Enemymanager.h"

void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}
