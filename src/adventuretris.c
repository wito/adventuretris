#define _BSD_SOURCE

#include <stdlib.h>
#include <stdio.h>

#include "piece.h"

#define WIDTH 10
#define DEPTH 20

typedef char **field;

typedef enum {
  left = -1,
  right = 1
} direction;

typedef struct {
  int x;
  int y;
} loc;

loc location = {0,0};

int movePiece (piece, field);
int nudgePiece (piece, field, direction);
piece turnPiece (piece, field);

int collidePiece (piece, field);
void blitPiece (piece, field);

int spawnPiece (field, piece*);

void printField (field);
void printPiece (piece);

int main () {
  field gameField = NULL;
  piece currentPiece = NULL;

  // --- Set up field --- //

  gameField = calloc(DEPTH, sizeof(char *));

  for (int i = 0; i < DEPTH; i++) {
    gameField[i] = calloc(WIDTH, sizeof(char));
  }

  // --- Main game --- //

  while (spawnPiece(gameField, &currentPiece)) {
    while (movePiece(currentPiece,gameField)) {
      currentPiece = turnPiece(currentPiece,gameField);

#ifdef DEBUG
      printPiece(currentPiece);
#endif

      nudgePiece(currentPiece,gameField,(random() % 3) - 1);
    }

#ifdef DEBUG
    printField(gameField);
#endif

    destroyPiece(currentPiece);
  }

  destroyPiece(currentPiece);

  // --- Free field --- //

  for (int i = 0; i < DEPTH; i++) {
    free(gameField[i]);
  }

  free(gameField);

  return 0;
}


int movePiece (piece p, field f) {
  location.y++;

  if (!collidePiece(p,f)) {
    location.y--;
    blitPiece (p,f);

    return 0;
  }

  return 1;
}

int nudgePiece (piece p, field f, direction dir) {
  location.x += dir;

  if (!collidePiece(p,f)) {
    location.x -= dir;
    return 0;
  }

  return 1;
}


int collidePiece (piece p, field f) {
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (p[py][px]) {
        if (location.x + px >= WIDTH || location.x < 0 || location.y + py >= DEPTH || location.y < 0) // edges
          return 0;

        if (f[location.y + py][location.x + px]) // field pieces
          return 0;
      }
    }
  }

  return 1;
}

void blitPiece (piece p, field f) {
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (location.x + px >= WIDTH || location.x < 0 || location.y + py >= DEPTH || location.y < 0) // edges
        continue;

      f[location.y + py][location.x + px] = (f[location.y + py][location.x + px] || p[py][px]);
    }
  }
}


int spawnPiece (field f, piece *p) {
  location.x = 3;
  location.y = 0;

  int type = random() % MaxPiece;

#ifdef DEBUG
  printf("piece: %d\n", type);
#endif

  *p = createPiece(type);

  return collidePiece(*p, f);
}


void printField (field f) {
  for (int y = 0; y < DEPTH; y++) {
    printf("|");
    for (int x = 0; x < WIDTH; x++) {
      if (f[y][x])
        printf("*");
      else
        printf(" ");
    }
    printf("|\n");
  }
  printf("\n");
}

void printPiece (piece p) {
  for (int y = 0; y < 4; y++) {
    printf("|");
    for (int x = 0; x < 4; x++) {
      if (p[y][x])
        printf("*");
      else
        printf(" ");
    }
    printf("|\n");
  }
  printf("\n");
}

piece turnPiece (piece p, field f) {
  piece rotPiece = rotatePiece(p);

  if (collidePiece(rotPiece,f)) {
    destroyPiece(p);
    return rotPiece;
  } else {
    destroyPiece(rotPiece);
    return p;
  }
}
