//!!! THIS MAUST BE REWORK&CHANGED  !!!

void ShowInventory() {
    ShowBitmap(0, 0, window.width / 2., window.height / 2., InventoryhBack);//������ ���
    bool BootsInventory = Boots__inventory_butt.ShowInv();
    }
void InitInventory() {
    Boots__inventory_butt.LoadInv("Boots__inventory_butt.bmp", 0.5, 3., .4, .07);
    Exit.Load("Exit_butt.bmp", "Exit_butt_glow.bmp", 12, -16, .04, .03);
    Inventory_butt.Load("Heal_butt.bmp", "Heal_butt.bmp", 3.65, 5.12, .07, .07);
    Inventoryphon1_bmp = LoadBMP("Inventoryphon1.bmp");
}