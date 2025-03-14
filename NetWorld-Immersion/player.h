struct Player_ { //done
    int Health = 46000;
    int HealthMin = 0;
    int HealthMax = 50000;
    int Shield = 46000;
    int ShieldMin = 0;
    int ShieldMax = 50000;
    int Shield_bonus = 2; //TODO

    void adjustHealth(int v) {
        Health -= v + rand() % v;
        Health = clamp(Health, HealthMin, HealthMax);
        AttackStartTime = AttackcurrentTime;
    }

    void adjustShield(int s) {
        Shield -= s + (rand() % s) * Shield_bonus; // Учитываем бонус сразу
        Shield = clamp(Shield, ShieldMin, ShieldMax);
        AttackStartTime = AttackcurrentTime;
    }

    void adjustHeal() {
        Health = clamp(Health + HealthMax / 100., HealthMin, HealthMax);
        Shield = clamp((Shield + ShieldMax / 100.) * Shield_bonus, ShieldMin, ShieldMax);
        healStartTime = AttackcurrentTime;
    }
};

Player_ player;