void ShowLoot() {
    ShowBitmap(0, 0, window.width, window.height, InventoryhBack);//задний фон
    bool exit = Exit.Show();
    bool BootsInventory = Boots_inventory_butt.Show();
}
/*void InitLoot() {

}*/

void LootGame() {
    //InitLoot();
    ShowLoot();

    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }//done
}