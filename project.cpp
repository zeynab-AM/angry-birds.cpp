#include <iostream>
#include <cmath>
using namespace std;

const int H = 12;
const int W = 40;
const int MAX_SHOTS = 10;
const float G = 0.2;

// ساختارها
struct Projectile {
    float x, y;
    float vx, vy;
    bool active;
};

struct Enemy {
    int x, y;
    bool alive;
};

struct Game {
    char map[H][W];
    int launcherX, launcherY;
    Projectile proj;
    Enemy enemies[10];
    int enemyCount;
    int score;
    int shotsUsed;
    int level;
    int stars[3];
};

// توابع
void initGame(Game &game, int level) {
    game.level = level;
    game.score = 0;
    game.shotsUsed = 0;
    game.launcherX = 2;
    game.launcherY = H - 2;
    game.proj.active = false;
    game.enemyCount = 0;
    
    // پاک کردن نقشه
    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++) {
            game.map[i][j] = ' ';
        }
    }
    
    // ایجاد زمین
    for(int j = 0; j < W; j++) {
        game.map[H-1][j] = '#';
    }
    
    // ایجاد دشمنان بر اساس مرحله
    if(level == 1) {
        game.enemies[game.enemyCount++] = {20, H-2, true};
        game.enemies[game.enemyCount++] = {25, H-3, true};
        game.map[H-2][15] = '#';
    }
    else if(level == 2) {
        game.enemies[game.enemyCount++] = {18, H-2, true};
        game.enemies[game.enemyCount++] = {22, H-3, true};
        game.map[H-3][12] = '#';
        game.map[H-2][14] = '#';
    }
    else if(level == 3) {
        game.enemies[game.enemyCount++] = {20, H-2, true};
        game.enemies[game.enemyCount++] = {23, H-3, true};
        game.enemies[game.enemyCount++] = {25, H-4, true};
        game.map[H-2][10] = '#';
        game.map[H-3][15] = '#';
        game.map[H-4][18] = '#';
    }
    
    // قرار دادن دشمنان روی نقشه
    for(int i = 0; i < game.enemyCount; i++) {
        if(game.enemies[i].alive) {
            game.map[game.enemies[i].y][game.enemies[i].x] = 'E';
        }
    }
}

void drawGame(Game &game) {
    system("cls");
    
    cout << "========================================\n";
    cout << "Level: " << game.level << " | Score: " << game.score;
    cout << " | Shots: " << game.shotsUsed << "/" << MAX_SHOTS << "\n";
    cout << "========================================\n";
    
    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++) {
            // نمایش پرتابه
            if(game.proj.active && (int)game.proj.x == j && (int)game.proj.y == i) {
                cout << '*';
            }
            // نمایش پرتابگر
            else if(j == game.launcherX && i == game.launcherY) {
                cout << 'P';
            }
            else {
                cout << game.map[i][j];
            }
        }
        cout << "\n";
    }
    cout << "========================================\n";
    cout << "Controls: Enter=Next Step | Q=Quit\n";
}

void getShotInput(Game &game) {
    float angle, speed;
    
    cout << "\nShot " << (game.shotsUsed + 1) << ":\n";
    cout << "Enter angle (0-90): ";
    cin >> angle;
    cout << "Enter speed (1-15): ";
    cin >> speed;
    
    // اعتبارسنجی
    if(angle < 0) angle = 0;
    if(angle > 90) angle = 90;
    if(speed < 1) speed = 1;
    if(speed > 15) speed = 15;
    
    // تنظیم پرتابه
    game.proj.x = game.launcherX;
    game.proj.y = game.launcherY;
    game.proj.vx = speed * cos(angle * 3.14159 / 180.0);
    game.proj.vy = -speed * sin(angle * 3.14159 / 180.0);
    game.proj.active = true;
    
    game.shotsUsed++;
}

void updateProjectile(Game &game) {
    if(!game.proj.active) return;
    
    // حرکت پرتابه
    game.proj.x += game.proj.vx;
    game.proj.y += game.proj.vy;
    game.proj.vy += G;
    
    int ix = (int)game.proj.x;
    int iy = (int)game.proj.y;
    
    // بررسی مرزها
    if(ix < 0 || ix >= W || iy < 0 || iy >= H) {
        game.proj.active = false;
        return;
    }
    
    // بررسی برخورد با دیوار
    if(game.map[iy][ix] == '#') {
        game.proj.active = false;
        return;
    }
    
    // بررسی برخورد با دشمن
    for(int i = 0; i < game.enemyCount; i++) {
        if(game.enemies[i].alive && 
           game.enemies[i].x == ix && 
           game.enemies[i].y == iy) {
            game.enemies[i].alive = false;
            game.map[iy][ix] = ' ';
            game.score += 100;
            game.proj.active = false;
            return;
        }
    }
}

bool checkLevelComplete(Game &game) {
    for(int i = 0; i < game.enemyCount; i++) {
        if(game.enemies[i].alive) {
            return false;
        }
    }
    return true;
}

int calculateStars(Game &game) {
    int parShots[3] = {4, 5, 6}; // شلیک استاندارد برای هر مرحله
    
    if(game.shotsUsed <= parShots[game.level-1]) return 3;
    if(game.shotsUsed <= parShots[game.level-1] + 1) return 2;
    if(game.shotsUsed <= parShots[game.level-1] + 3) return 1;
    return 0;
}

void showMenu() {
    system("cls");
    cout << "============================\n";
    cout << "     ANGRY BIRDS GAME      \n";
    cout << "============================\n";
    cout << "1. Start Game\n";
    cout << "2. How to Play\n";
    cout << "3. Exit\n";
    cout << "============================\n";
    cout << "Select: ";
}

void showHelp() {
    system("cls");
    cout << "HOW TO PLAY:\n";
    cout << "============\n";
    cout << "1. Enter angle and speed for each shot\n";
    cout << "2. Press Enter to move projectile step by step\n";
    cout << "3. Destroy all enemies (E) with limited shots\n";
    cout << "4. Avoid walls (#)\n";
    cout << "5. Get stars by using few shots\n\n";
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}

void playLevel(Game &game, int level) {
    initGame(game, level);
    
    while(game.shotsUsed < MAX_SHOTS) {
        drawGame(game);
        
        // اگر پرتابه فعال نیست، شلیک جدید
        if(!game.proj.active) {
            getShotInput(game);
        }
        
        // شبیه‌سازی پرتابه
        while(game.proj.active) {
            drawGame(game);
            cout << "Press Enter for next step (Q to quit)...";
            
            char ch;
            cin.ignore();
            ch = cin.get();
            
            if(ch == 'q' || ch == 'Q') {
                return;
            }
            if(ch == '\n' || ch == '\r') {
                updateProjectile(game);
            }
        }
        
        // بررسی پایان مرحله
        if(checkLevelComplete(game)) {
            break;
        }
    }
}

int main() {

    Game game;  //یک متغیر به نام game از نوعGame میسازه
    
    // مقداردهی اولیه ستاره‌ها
    for(int i = 0; i < 3; i++) {
        game.stars[i] = 0;
    }
    
    int choice;
    do {
        showMenu();
        cin >> choice;
        
        if(choice == 1) {
            // بازی اصلی - 3 مرحله
            for(int level = 1; level <= 3; level++) {
                playLevel(game, level);
                
                // اگر کاربر Q زد
                if(game.shotsUsed == 0 && level == 1) {
                    break;
                }
                
                // نمایش نتایج مرحله
                int stars = calculateStars(game);
                game.stars[level-1] = stars;
                
                system("cls");
                cout << "============================\n";
                cout << "   LEVEL " << level << " COMPLETE!   \n";
                cout << "============================\n";
                cout << "Stars: " << stars << "\n";
                cout << "Shots used: " << game.shotsUsed << "\n";
                cout << "Total score: " << game.score << "\n";
                cout << "============================\n";
                
                if(level < 3) {cout << "Press Enter for next level...";
                    cin.ignore();
                    cin.get();
                }
            }
            
            // نمایش نتایج نهایی
            system("cls");
            cout << "============================\n";
            cout << "      GAME COMPLETE!       \n";
            cout << "============================\n";
            cout << "Final Score: " << game.score << "\n";
            cout << "Stars: ";
            for(int i = 0; i < 3; i++) {
                cout << game.stars[i] << " ";
            }
            cout << "\n============================\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
        else if(choice == 2) {
            showHelp();
        }
        
    } while(choice != 3);
    
    cout << "\nThanks for playing!\n";
    return 0;
}