#include <cstdlib>   // Для rand()

class Enemy_ {
public:
    int HealthEnemy = 40000;
    int HealthEnemyMin = 0;
    int HealthEnemyMax = 50000;
    int ShieldEnemy = 40000;
    int ShieldEnemyMin = 0;
    int ShieldEnemyMax = 50000;
    int ShieldEnemy_bonus = 20;

    Enemy_(int name) : HealthEnemy(10) {
        // Здесь нет необходимости повторно объявлять переменные
    }

    Enemy_(){}

    void adjustHealth(int v) {
        HealthEnemy -= v + rand() % v;  // Убираем лишнюю переменную
        HealthEnemy = clamp(HealthEnemy, HealthEnemyMin, HealthEnemyMax);
         AttackStartTime = AttackcurrentTime; // Убедитесь, что эти переменные определены
    }

    void adjustShield(int s) {
        ShieldEnemy -= s + (rand() % s) * 2;  // Убираем лишнюю переменную
        ShieldEnemy = clamp(ShieldEnemy, ShieldEnemyMin, ShieldEnemyMax);
         AttackStartTime = AttackcurrentTime; // Убедитесь, что эти переменные определены
    }
};

// Создание объекта класса
Enemy_ Enemy;
int a;
Enemy_ killer(a);
