#include <stdlib.h>
#include <assert.h>

#include "piece.h"

piece createPiece(pieceType type) {
  piece self = calloc(4, sizeof(char *));

  for (int i = 0; i < 4; i++) {
    self[i] = calloc(4, sizeof(char));
  }

  if (type == IPiece) {
    self[0][1] = BLACK;
    self[1][1] = BLACK;
    self[2][1] = BLACK;
    self[3][1] = BLACK;
  } else if (type == OPiece) {
    self[1][1] = BLACK;
    self[1][2] = BLACK;
    self[2][1] = BLACK;
    self[2][2] = BLACK;
  } else if (type == SPiece) {
    self[0][1] = BLACK;
    self[1][1] = BLACK;
    self[1][2] = BLACK;
    self[2][2] = BLACK;
  } else if (type == ZPiece) {
    self[0][2] = BLACK;
    self[1][2] = BLACK;
    self[1][1] = BLACK;
    self[2][1] = BLACK;
  } else if (type == LPiece) {
    self[0][1] = BLACK;
    self[1][1] = BLACK;
    self[2][1] = BLACK;
    self[2][2] = BLACK;
  } else if (type == JPiece) {
    self[0][2] = BLACK;
    self[1][2] = BLACK;
    self[2][2] = BLACK;
    self[2][1] = BLACK;
  } else if (type == TPiece) {
    self[1][2] = BLACK;
    self[2][1] = BLACK;
    self[2][2] = BLACK;
    self[3][2] = BLACK;
  } else if (type == EmptyPiece) {
  } else assert(0);

  return self;
}

void destroyPiece(piece self) {
  for (int i = 0; i < 4; i++) {
    free(self[i]);
  }

  free(self);
}


piece rotatePiece(piece self) {
  piece retval = createPiece(EmptyPiece);

  retval[0][0] = self[3][0];
  retval[0][1] = self[2][0];
  retval[0][2] = self[1][0];
  retval[0][3] = self[0][0];

  retval[1][0] = self[3][1];
  retval[1][1] = self[2][1];
  retval[1][2] = self[1][1];
  retval[1][3] = self[0][1];

  retval[2][0] = self[3][2];
  retval[2][1] = self[2][2];
  retval[2][2] = self[1][2];
  retval[2][3] = self[0][2];

  retval[3][0] = self[3][3];
  retval[3][1] = self[2][3];
  retval[3][2] = self[1][3];
  retval[3][3] = self[0][3];

  return retval;
}
