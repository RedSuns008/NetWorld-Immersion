void ShowLoot() {
    ShowBitmap(0, 0, window.width, window.height, InventoryhBack);//������ ���
    bool exit = Exit.Show();
    bool BootsInventory = Boots__inventory_butt.Show();
}


void LootGame() {
    ShowLoot();

    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(20);//���� 16 ���������� (1/���������� ������ � �������)
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }//done
}