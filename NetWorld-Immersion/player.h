struct Player_ { //done
    int Health = 46000;
    int HealthMin = 0;
    int HealthMax = 50000;
    int Shield = 46000;
    int ShieldMin = 0;
    int ShieldMax = 50000;
    int Shield_bonus = 2; //TODO

    void adjustHealth(int v) {
        Health -= rand() % v;
        Health = clamp(Health, HealthMin, HealthMax);
    }

    void adjustShield(int s) {
        Shield -= (rand() % s) * Shield_bonus; // Учитываем бонус сразу
        Shield = clamp(Shield, ShieldMin, ShieldMax);
    }

    void adjustHeal() {
        Health = clamp(Health + 1500, HealthMin, HealthMax);
        Shield = clamp((Shield + 1500) * Shield_bonus, ShieldMin, ShieldMax);
        healStartTime = currentTime;
    }
};

Player_ player;