
bool InitBattleFlag = false;

void InitBattle() {

    if (!InitBattleFlag)
    {
        PrimWeapon.Load("pw_butt.bmp", "pw_butt_glow.bmp", 1.65, 4.13, .09, .09);
        SpecWeapon.Load("sw_butt.bmp", "sw_butt_glow.bmp", 0.5, 4.13, .09, .09);
        DestructiveWeapon.Load("dw_butt.bmp", "dw_butt_glow.bmp", -0.7, 4.13, .09, .09);
        EnemyB.Load("Enemy_butt.bmp", "Enemy_butt_glow.bmp", 0.43, -0.56, .25, .60);
        Exit.Load("Exit_butt.bmp", "Exit_butt_glow.bmp", 12, -16, .04, .03);
        Heal_butt.Load("Heal_butt.bmp", "Heal_butt.bmp", -2.65, 5.12, .07, .07);
        Inventory_butt.Load("Heal_butt.bmp", "Heal_butt.bmp", 3.65, 5.12, .07, .07);
        Health.Load("Health_bar_back.bmp", "Health_bar_front.bmp", 0.45, 33.7, .28, .01);
        Shield.Load("Shield_bar_back.bmp", "Shield_bar_front.bmp", 0.45, 31.7, .28, .01);
        ShieldEnemy.Load("Shield_bar_back.bmp", "Shield_bar_front.bmp", 0.4, -42.4, .28, .01);
        HealthEnemy.Load("Health_bar_back.bmp", "Health_bar_front.bmp", 0.4, -40.4, .28, .01);

        Battlephon1_bmp = LoadBMP("Battlephon1.bmp");
        Exit.Load("Exit_butt.bmp", "Exit_butt_glow.bmp", 12, -16, .04, .03);

        InitBattleFlag = true;
    }
 
}


void ShowBattle() {

    ShowBitmap(0, 0, window.width, window.height, hBattleBack);//������ ���

    Health.ShowHealth(player.Health);
    Shield.ShowShield(player.Shield);
    ShieldEnemy.ShowShieldEnemy(Enemy.ShieldEnemy);
    HealthEnemy.ShowHealthEnemy(Enemy.HealthEnemy);

    bool pw = PrimWeapon.Show();
    bool sw = SpecWeapon.Show();
    bool dw = DestructiveWeapon.Show();
    bool exit = Exit.Show();
    bool enemy = EnemyB.Show();
    bool HealButt = Heal_butt.Show();
    bool InventoryButt = Inventory_butt.Show();

} 

void BattleGame() {//TODO ??????
    
    InitBattle();
    ShowBattle();//������ ���    
    if (Mouse.L_butt) {
        if (PrimWeapon.CheckCollisionMouse()) {
            if (AttackcurrentTime > AttackStartTime + AttackTime) {

                player.adjustHealth(Enemy.AttackEnemy);
                player.adjustShield(Enemy.AttackEnemyShield);
                Enemy.adjustHealth(player.Attack);
                Enemy.adjustShield(player.Attack);
                AttackStartTime = currentTime;
            }
        }

        if (SpecWeapon.CheckCollisionMouse()) {
            if (AttackcurrentTime > AttackStartTime + AttackTime) {
                player.adjustHealth(Enemy.AttackEnemy);
                player.adjustShield(Enemy.AttackEnemyShield);
                Enemy.adjustHealth(player.Attack);
                Enemy.adjustShield(player.Attack);
                AttackStartTime = currentTime;
            }
        }

        if (DestructiveWeapon.CheckCollisionMouse()) {
            if (AttackcurrentTime > AttackStartTime + AttackTime) {
                player.adjustHealth(Enemy.AttackEnemy);
                player.adjustShield(Enemy.AttackEnemyShield);
                Enemy.adjustHealth(player.Attack);
                Enemy.adjustShield(player.Attack);
                AttackStartTime = currentTime;
            }
        }
        if (Heal_butt.CheckCollisionMouseHeal());

        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }

    }

    
}