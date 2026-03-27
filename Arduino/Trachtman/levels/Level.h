#pragma once
#include <Arduino.h>
#include "../engine/Sprite.h"
#include "../config/GameConfig.h"

// ===== Gift-Typen =====
enum GiftType : uint8_t {
    DOGPOOP1,
    DOGPOOP2,
    DOG,
    QUESTION,
    WURST,
    BRETZEL,
    BIER,
    GOLDKISTE
};

// ===== LevelGift-Struktur =====
struct LevelGift {
    GiftType type;
    int x, y;
    bool active;
    unsigned long spawnTime;
    unsigned long lifetime;
};

// ===== Level-Struktur =====
struct Level {
    uint8_t tiles[6][8];        // 6 Zeilen × 8 Spalten
    LevelGift levelGifts[8];    // max 8 Gifts pro Level
};
