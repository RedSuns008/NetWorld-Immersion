class Enemy_ {
public:
    float HealthEnemy = 40000;
    float HealthEnemyMin = 0;
    float HealthEnemyMax = HealthEnemy * 2;
    float ShieldEnemy = 40000;
    float ShieldEnemyMin = 0;
    float ShieldEnemyMax = ShieldEnemy * 2;
    float ShieldEnemy_bonus = 20;
	float AttackEnemy = 40000;
	float AttackEnemyShield = (AttackEnemy * 3) / 4;
    
    Enemy_(int a) {

		switch (a)
		{
		case 1:
			//std::cout << "Haker\n";
			this->AttackEnemy += 5555;
			this->HealthEnemy += 5000;
			this->ShieldEnemy += 20000;

			break;
		case 2:
			//std::cout << "Killer\n";
			this->AttackEnemy += 15000;
			this->HealthEnemy += 10000;
			this->ShieldEnemy += 12000;
			break;
		case 3:
			//std::cout << "Tank\n";
			this->AttackEnemy += 5000;
			this->HealthEnemy += 25000;
			this->ShieldEnemy += 20000;

			break;
		case 4:
			//std::cout << "Vydo\n";
			this->AttackEnemy += 20000;
			this->HealthEnemy += 25000;
			this->ShieldEnemy += 22000;
			break;
		}
    }


	void adjustHealth(int v) {
		HealthEnemy -= v + rand() % v;
		HealthEnemy = clamp(HealthEnemy, HealthEnemyMin, HealthEnemyMax);
		AttackStartTime = AttackcurrentTime;
	}

	void adjustShield(int s) {
		ShieldEnemy -= s + (rand() % s); // Учитываем бонус сразу
		ShieldEnemy = clamp(ShieldEnemy, ShieldEnemyMin, ShieldEnemyMax);
		AttackStartTime = AttackcurrentTime;
	}





};

// Создание объекта класса
int a = rand() % 5;
Enemy_ Enemy(a);
