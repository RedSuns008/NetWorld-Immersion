void ShowTerminal() {
    ShowBitmap(0, 0, window.width, window.height, TerminalhBack);//задний фон
    bool exit = Exit.Show();
}

void TerminalGame() { //TODO
    ShowTerminal();
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
    Sleep(16);
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }
}