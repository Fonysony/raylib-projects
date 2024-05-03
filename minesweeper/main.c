#include <stdbool.h>
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TILEWIDTH 4
#define TILEHEIGHT 3
#define TILESCALE 4
#define GAMEWIDTH 5
#define GAMEHEIGHT 5
#define GAMECOUNT GAMEWIDTH*GAMEHEIGHT
#define BOMBCOUNT 1

typedef struct Tile {
   int x;
   int y;
   int id;
   int index;
   bool isBomb;

}Tile;

Vector2 vertIndex(int tileIndex, int width);
int randomNumber(int min, int max);
Tile * createMap(Tile *tiles, int bombAmount);
Tile * createBombs(int bombAmount);
int vertPosToIndex(Vector2 tile);


Tile *tilesData;
float tileSize;
int randCount= 0;
Tile *gameTiles;
Tile *bombTiles;



int randomNumber(int min, int max)
{
   srand(time(NULL) * getpid() * ++randCount);
   int randNum = (rand() % max + min);

   return (int)randNum;
}

Tile * getTileData(const char *fileName)
{
   Texture2D tilesheet = LoadTexture(fileName);
   int totalTiles = TILEWIDTH * TILEHEIGHT;
   tileSize = ((float)tilesheet.width / TILEWIDTH)/TILESCALE;

   printf("TILE-SCALE: %f x %f\n", tileSize, tileSize);

   Tile *tiles = calloc(totalTiles, sizeof(Tile));

   for (int i = 0; i < totalTiles; i++)
   {
      Vector2 tile = vertIndex(i, TILEWIDTH); 
      tiles[i] = (Tile){(int)tile.x, (int)tile.y, i};
      printf("TILEDATA %i x: %i, y: %i\n", i, tiles[i].x, tiles[i].y);
   }
   return tiles;
}

Tile * createMap(Tile *tiles, int bombAmount)
{
   Tile *gameTiles = calloc(GAMECOUNT, sizeof(Tile));


   for (int i = 0; i < BOMBCOUNT; i++)
   {
   }
   int count = 0;

   for (int y = 0; y < GAMEHEIGHT; y++)
   {
      for (int x = 0; x < GAMEWIDTH; x++)
      {
         Tile tile = { x, y, count, 0, false };
         gameTiles[count] = tile;
         count++;
      }
   }
   return gameTiles;
}

Tile * createBombs(int bombAmount)
{
   Tile *bombArray = calloc(bombAmount, sizeof(Tile));
   
   Vector2 firstBomb = { randomNumber(0, GAMEWIDTH), randomNumber(0, GAMEHEIGHT) };
   int test = vertPosToIndex(firstBomb);
   printf("CREATED BOMB AT X: %f, Y: %f, gameTiles[%i]\n", firstBomb.x, firstBomb.y, test);
   gameTiles[test].isBomb = true;
   bombArray[0] = gameTiles[test];
   
   // ALGO for finding position for new mines

   return bombArray;
}

void renderTiles(Texture2D texture, int tileSize, Tile *gameTiles, int totalTiles)
{
   for (int i = 0; i < totalTiles; i++)
   {
      Tile tile = tilesData[gameTiles[i].index];

      // printf("x: %i, y: %i", tile.x, tile.y);
      
      DrawTexturePro(
         texture, 
         (Rectangle){tileSize*tile.x*TILESCALE, tileSize*tile.y*TILESCALE, tileSize*TILESCALE, tileSize*TILESCALE}, 
         (Rectangle){
            gameTiles[i].x*(float)tileSize, 
            gameTiles[i].y*(float)tileSize, 
            tileSize, tileSize
         },
         (Vector2){0, 0},
         0.0f,
         WHITE
      );
   }
}

Vector2 vertIndex(int tileIndex, int width)
{
   Vector2 tile;

   tile.x = (tileIndex % width);
   tile.y = (int)(tileIndex / width);
   return tile;
}

int vertPosToIndex(Vector2 tile)
{
   int index = ((int)tile.y*GAMEWIDTH)+(int)tile.x;

   return index;
}


Vector2 screenToWorld(Vector2 screenPos)
{
   Vector2 pos;
   pos.x = screenPos.x / tileSize;
   pos.y = screenPos.y / tileSize;
   return pos;
}

int getAdjecntTiles(Tile adjcentTiles[8], Tile clickedTile, Tile *gameTiles)
{
   int count = 0;
   for (int y = -1; y < 2; y++)
   {
      for (int x = -1; x < 2; x++)
      {
         Vector2 newTilePos = { (int)(clickedTile.x+x), (int)(clickedTile.y+y) };
         if (x == 0 && y == 0) continue;
         if (newTilePos.x >= 0 && newTilePos.x < GAMEWIDTH && newTilePos.y >= 0 && newTilePos.y < GAMECOUNT)
         {
            int tileIndex = vertPosToIndex(newTilePos);
            printf("Tile index: %i\n", tileIndex);
            // Tile adjTile = gameTiles[];
            if (tileIndex >= 0 && tileIndex < GAMECOUNT)
            {
               printf("TILE INDEX GO -> %i\n", tileIndex);
               adjcentTiles[count] = gameTiles[tileIndex];
               count++;
            }
         }
      }
   }
   return count;
}

int main(void)
{
   int screenWidth = 1000;
   int screenHeight = 500;

   InitWindow(screenWidth, screenHeight, "Minesweeper");

   Texture2D texture = LoadTexture("./tiles.png");

   tilesData = getTileData("./tiles.png");
   int tileLength = 12;

   gameTiles = createMap(tilesData, 10);
   bombTiles = createBombs(BOMBCOUNT);
   printf("Bomb %i: x: %i, y: %i, isBomb: %b\n", 0, bombTiles[0].x, bombTiles[0].y, bombTiles[0].isBomb);

   int clicks = 0;

   

   while (!WindowShouldClose())
   {
      BeginDrawing();
      ClearBackground(BLACK);
      if (GetMousePosition().x < tileSize*GAMEWIDTH && GetMousePosition().y < tileSize*GAMEHEIGHT)
      {
         if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
         {
            Vector2 pos = screenToWorld(GetMousePosition());
            printf("Mouse x: %f, y: %f\n", pos.x, pos.y);
            int tileIndex = vertPosToIndex(pos);
            Tile *tileClicked = &gameTiles[tileIndex];
            Tile adjTiles[8];
            int adjSize = getAdjecntTiles(adjTiles, *tileClicked, gameTiles);
            printf("adjTiles Size: %i\n", adjSize);

            if (gameTiles[tileIndex].index == 0)
            {
               tileClicked->index = 3;
            }
            printf("Tile Click %i x: %i, y: %i, index: %i\n", tileIndex, tileClicked->x, tileClicked->y, tileClicked->index); 
            clicks++;
         }else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
         {
            Vector2 pos = screenToWorld(GetMousePosition());
            int tileIndex = vertPosToIndex(pos);
            if (gameTiles[tileIndex].index == 0)
            {
               gameTiles[tileIndex].index = 1;
            } else if (gameTiles[tileIndex].index == 1){
               gameTiles[tileIndex].index = 0;
            }
         }

      }
      renderTiles(texture, tileSize, gameTiles, GAMECOUNT);
      DrawText(TextFormat("Button: %i Mouse: x: %f, y: %f", clicks, GetMousePosition().x, GetMousePosition().y), 200, 200, 12, WHITE);
      EndDrawing();
   }

   UnloadTexture(texture);

   CloseWindow();

   free(tilesData);
   free(gameTiles);
   free(bombTiles);

   return 0;
}
