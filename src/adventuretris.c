#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>

#define DEBUG

#include <strings.h>

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

typedef enum {
  C_VOID = -1,
  C_LEFT = 0,
  C_RIGHT = 1,
  C_DROP = 2,
  C_TURN = 3,
  C_QUIT = 4
} command;

static const char *commandStrings[] = {
  "left",
  "right",
  "drop",
  "turn",
  "quit"
};

loc location = {0,0};

int movePiece (piece, field);
int nudgePiece (piece, field, direction);
piece turnPiece (piece, field);

int collidePiece (piece, field);
void blitPiece (piece, field);

int spawnPiece (field, piece*);

void printField (field);
void printPiece (piece);

command parseCommand (const char *);

void zapLines (field);
int dropField (field);

void updateField (field);

field createField ();

int main () {
  field gameField = NULL;
  piece currentPiece = NULL;

  // --- Set up field --- //

  gameField = createField();

  // --- Main game --- //

  size_t nBytes = 40;
  char *commandString = malloc(nBytes);

  size_t bytes_read = 0;

  while (spawnPiece(gameField, &currentPiece)) {
    while (movePiece(currentPiece,gameField)) {
      bytes_read = getline(&commandString, &nBytes, stdin);

      printf("%s\n", commandString);

      command currentCommand = parseCommand(commandString);

      switch (currentCommand) {
      case C_VOID:
      case C_DROP:
        break;
      case C_TURN:
        currentPiece = turnPiece(currentPiece,gameField);
        break;
      case C_QUIT:
        exit(0);
      case C_LEFT:
        nudgePiece(currentPiece,gameField,left);
        break;
      case C_RIGHT:
        nudgePiece(currentPiece,gameField,right);
        break;
      }

#ifdef DEBUG
      printPiece(currentPiece);
#endif
    }

    updateField(gameField);

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

    printf("PIECE_LAND\n");

    return 0;
  } else {
    printf("PIECE_MOVE\n");
  }

  return 1;
}

int nudgePiece (piece p, field f, direction dir) {
  location.x += dir;

  if (!collidePiece(p,f)) {
    location.x -= dir;

    printf("NUDGE_FAIL\n");

    return 0;
  } else {
    printf("NUDGE\n");
  }

  return 1;
}


int collidePiece (piece p, field f) {
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (p[py][px]) {
        if (location.x + px >= WIDTH || location.x + px < 0 || location.y + py >= DEPTH || location.y < 0) // edges
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
      if (location.x + px >= WIDTH || location.x + px < 0 || location.y + py >= DEPTH || location.y + py < 0) // edges
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

  int retval = collidePiece(*p, f);

  if (retval) {
    printf("SPAWN_PIECE: %s\n", pieceStrings[type]);
  } else {
    printf("SPAWN_FAIL\n");
  }

  return retval;
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
    printf("ROTATE_PIECE\n");

    destroyPiece(p);
    return rotPiece;
  } else {
    printf("ROTATE_FAIL\n");

    destroyPiece(rotPiece);
    return p;
  }
}

void zapLines (field f) {
  int lc = 0;

  for (int y = 0; y < DEPTH; y++) {
    int zap = 1;

    for (int x = 0; x < WIDTH; x++) {
      zap = (zap && f[y][x]);
    }

    if (zap) {
      free(f[y]);
      f[y] = NULL;

      lc += 1;
    }
  }

  if (lc) {
    printf("ZAP_LINE\n");
  }
}

int dropField (field f) {
  int retval = 0;

  for (int y = DEPTH - 1; y >= 0; --y) {
    if (f[y])
      continue;

    retval++;

    if (y) {
      f[y] = f[y - 1];
      f[y - 1] = NULL;
    } else {
      f[y] = calloc(WIDTH, sizeof(char));
    }
  }

  return retval;
}

void updateField (field f) {
  zapLines(f);
  while (dropField(f));
}

field createField () {
  field gameField = calloc(DEPTH, sizeof(char *));

  for (int i = 0; i < DEPTH; i++) {
    gameField[i] = calloc(WIDTH, sizeof(char));
  }

  printf("CREATE_FIELD\n");

  return gameField;
}

command parseCommand (const char *command) {
  for (int c = 0; c < 5; c++) {
    if (!strncasecmp(command,commandStrings[c],4))
      return c;
  }

  return -1;
}
