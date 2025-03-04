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

    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
    Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }//done
}