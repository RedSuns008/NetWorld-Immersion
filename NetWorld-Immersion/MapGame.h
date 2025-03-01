void ShowMapGame() //TODO
{
    ShowBitmap(0, 0, window.width, window.height, hBack);//задний фон

    for (int i = 0; i < enemycout; i++) {
        ShowBitmap(enemy1[i].x - enemy1[i].width / 2., enemy1[i].y, enemy1[i].width, enemy1[i].height, enemy1[i].hBitmap);
    }
    ShowBitmap(Mouse.x, Mouse.y, 1, 1, raketka_bmp);
}

void MapGame() {
    ShowMapGame();//рисуем фон, ракетку и шарик
    ShowScore();
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
    Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду) 
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