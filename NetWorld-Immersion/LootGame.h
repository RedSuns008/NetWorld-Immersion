void ShowLoot() {
    ShowBitmap(0, 0, window.width, window.height, InventoryhBack);//������ ���
    bool exit = Exit.Show();
    bool BootsInventory = Boots_inventory_butt.Show();
}
/*void InitLoot() {

}*/

void LootGame() {
    //InitLoot();
    ShowLoot();

    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }//done
}