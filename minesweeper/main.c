#include <stdbool.h>
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define TILEWIDTH 4
#define TILEHEIGHT 3
#define TILESCALE 4
#define GAMEWIDTH 5
#define GAMEHEIGHT 5
#define GAMECOUNT GAMEWIDTH*GAMEHEIGHT

void vertIndex(int tileIndex, int *x, int *y);

typedef struct Tile {
   int x;
   int y;
   int index;
   bool isBomb;

}Tile;

Tile * getTileData(const char *fileName, int tileWidth, int tileHeight)
{
   Texture2D tilesheet = LoadTexture(fileName);
   int totalTiles = tileWidth * tileHeight;
   int tileSize = tilesheet.width / tileWidth;
   printf("TILESIZE: %i, IMAGE-SCALE: %f x %f\n", (int)tileSize, (float)tileSize/4, (float)tileSize/4);

   Tile *tiles = calloc(totalTiles, sizeof(Tile));

   for (int i = 0; i < totalTiles; i++)
   {
      int x, y;
      vertIndex(i, &x, &y); 
      tiles[i] = (Tile){x, y, i};
   }
   return tiles;
}

Tile * createMap(Tile *tiles, int bombAmount)
{
   Tile *gameTiles = calloc(GAMECOUNT, sizeof(Tile));

   int bombArray[bombAmount];
   int count = 0;

   for (int y = 0; y < GAMEHEIGHT; y++)
   {
      for (int x = 0; x < GAMEWIDTH; x++)
      {
         Tile tile = { x, y, 0, false };
         gameTiles[count] = tile;
         count++;
      }
   }
   return gameTiles;
}

void renderTiles(Texture2D texture, int tileSize, Tile *gameTiles, int totalTiles)
{
   for (int i = 0; i < totalTiles; i++)
   {
      DrawTexturePro(
         texture, 
         (Rectangle){tileSize*0, tileSize*0, tileSize, tileSize}, 
         (Rectangle){
            gameTiles[i].x*((float)tileSize/TILESCALE), 
            gameTiles[i].y*((float)tileSize/TILESCALE), 
            (float)tileSize/TILESCALE, (float)tileSize/TILESCALE
         },
         (Vector2){0, 0},
         0.0f,
         WHITE
      );
   }
}

void vertIndex(int tileIndex, int *x, int *y)
{
   *x = tileIndex % TILEWIDTH;
   *y = tileIndex / TILEHEIGHT;
}

int main(void)
{
   int screenWidth = 1000;
   int screenHeight = 500;

   InitWindow(screenWidth, screenHeight, "Minesweeper");

   Texture2D texture = LoadTexture("./tiles.png");

   Tile *tiles = getTileData("./tiles.png", TILEWIDTH, TILEHEIGHT);
   int tileLength = 12;

   float tileSize = (float)texture.width / 4;

   Tile *gameTiles = createMap(tiles, 10);

   for (int i = 0; i < 25; i++)
   {
      printf("Tile %i x: %i, y: %i\n", i, gameTiles[i].x, gameTiles[i].y);
   }

   while (!WindowShouldClose())
   {
      BeginDrawing();
         ClearBackground(BLACK);
         renderTiles(texture, tileSize, gameTiles, GAMECOUNT);

      EndDrawing();
   }

   UnloadTexture(texture);

   CloseWindow();

   free(tiles);
   free(gameTiles);

   return 0;
}
