void ShowMapGame() //TODO
{
    ShowBitmap(0, 0, window.width, window.height, hBack);//������ ���

    for (int i = 0; i < enemycout; i++) {
        ShowBitmap(enemy1[i].x - enemy1[i].width / 2., enemy1[i].y, enemy1[i].width, enemy1[i].height, enemy1[i].hBitmap);
    }
    ShowBitmap(Mouse.x, Mouse.y, 1, 1, raketka_bmp);
}

void MapGame() {
    ShowMapGame();//������ ���, ������� � �����
    ShowScore();
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(16);//���� 16 ���������� (1/���������� ������ � �������) 
    for (int i = 0; i < enemycout; i++) {
        if (Mouse.L_butt) {
            if (CheckCollisionMouse(enemy1[i])) {
                switch (enemy1[i].type) {
                case Entity::lootchest:
                    game_mode = GameMode::loot;
                    break;
                case Entity::enemy:
                    game_mode = GameMode::battle;
                    break;
                case Entity::terminal:
                    game_mode = GameMode::terminal;
                    break;
                }
            }
        }
    }
}