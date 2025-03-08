struct Enemy_ { //done
    int HealthEnemy = 40000;
    int HealthEnemyMin = 0;
    int HealthEnemyMax = 50000;
    int ShieldEnemy = 40000;
    int ShieldEnemyMin = 0;
    int ShieldEnemyMax = 50000;
    int ShieldEnemy_bonus = 20;

    void adjustHealth(int v) {
        HealthEnemy -= rand() % v;  // Убираем лишнюю переменную
        HealthEnemy = clamp(HealthEnemy, HealthEnemyMin, HealthEnemyMax);
        AttackStartTime = AttackcurrentTime;
    }

    void adjustShield(int s) {
        ShieldEnemy -= (rand() % s) * 2;  // Убираем лишнюю переменную
        ShieldEnemy = clamp(ShieldEnemy, ShieldEnemyMin, ShieldEnemyMax);
        AttackStartTime = AttackcurrentTime;
    }
};

Enemy_ Enemy;
