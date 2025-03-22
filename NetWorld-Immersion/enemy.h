class Enemy_ {
public:
    int HealthEnemy = 40000;
    int HealthEnemyMin = 0;
    int HealthEnemyMax = HealthEnemy * 2;
    int ShieldEnemy = 40000;
    int ShieldEnemyMin = 0;
    int ShieldEnemyMax = ShieldEnemy * 2;
    int ShieldEnemy_bonus = 20;
	int AttackEnemy = 40000;
    
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

    void adjustHealth() {
        HealthEnemy -= AttackEnemy + (rand() % 1000);  // доработать формулу урона по щиту и хп
        HealthEnemy = clamp(HealthEnemy, HealthEnemyMin, HealthEnemyMax);
         AttackStartTime = AttackcurrentTime; 
    }

    void adjustShield() {
        ShieldEnemy -= AttackEnemy + (rand() % 1000) * 2;  //доработать формулу урона по щиту и хп
        ShieldEnemy = clamp(ShieldEnemy, ShieldEnemyMin, ShieldEnemyMax);
         AttackStartTime = AttackcurrentTime; 
    }
};

// Создание объекта класса
int a = rand() % 5;
Enemy_ Enemy(a);
