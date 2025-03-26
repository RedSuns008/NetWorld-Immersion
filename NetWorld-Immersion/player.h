struct Player_ { //done
   float Health = 460000;
   float HealthMin = 0;
   float HealthMax = 500000;
   float Shield = 460000;
   float ShieldMin = 0;
   float ShieldMax = 500000;
   float Shield_bonus = 2; //
   float Attack = 20000;

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