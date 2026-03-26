#pragma once
#include "Level1.h"
#include "Level2.h"
#include "level3.h"
#include "../engine/Sprite.h"
#include "../config/GameConfig.h"

// LevelGifts Funktionen
void handlePlayerMovement();
void handleGiftSpawning();
void updateLevelGifts(Level &lvl);
void checkLevelGiftCollisions(Sprite &player, Level &lvl);
void drawGift(LevelGift &g);

// drawSpriteTransparentXY für andere .cpp Dateien sichtbar machen
void drawSpriteTransparentXY(int x, int y, const uint16_t *bitmap, uint16_t transparentColor);