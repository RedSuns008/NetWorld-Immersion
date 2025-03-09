#include "math.h";

void InitBattle() {
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
 
}


void ShowBattle() {

    ShowBitmap(0, 0, window.width, window.height, hBattleBack);//задний фон

    Health.ShowHealth(player.Health);
    Shield.ShowShield(player.Shield);
    ShieldEnemy.ShowShield(Enemy.ShieldEnemy);
    HealthEnemy.ShowHealth(Enemy.HealthEnemy);

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
    ShowBattle();//рисуем фон 
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
    Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
         // добавить цикл whilе. условие выхода наше хп=0 или хп врага =0
        // 
    //while ((player.Health >= 0) || (Enemy.HealthEnemy >= 0)) {
        if (Mouse.L_butt) {
            if (PrimWeapon.CheckCollisionMouse()) {
                if (AttackcurrentTime > AttackStartTime + AttackTime) {

                    player.adjustHealth(4000);
                    player.adjustShield(6500);
                    Enemy.adjustHealth(5000);
                    Enemy.adjustShield(6500);
                    AttackStartTime = currentTime;
                }
            }

            if (SpecWeapon.CheckCollisionMouse()) {
                if (AttackcurrentTime > AttackStartTime + AttackTime) {
                    player.adjustHealth(7500);
                    player.adjustShield(9000);
                    Enemy.adjustHealth(8000);
                    Enemy.adjustShield(12000);
                    AttackStartTime = currentTime;
                }
            }

            if (DestructiveWeapon.CheckCollisionMouse()) {
                if (AttackcurrentTime > AttackStartTime + AttackTime) {
                    player.adjustHealth(10000);
                    player.adjustShield(15000);
                    Enemy.adjustHealth(17000);
                    Enemy.adjustShield(23000);
                    AttackStartTime = currentTime;
                }
            }
            if (Heal_butt.CheckCollisionMouseHeal());

            if (Exit.CheckCollisionMouse()) {
                game_mode = GameMode::map;
            }

        }

    //}
}