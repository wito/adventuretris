typedef char** piece;

typedef enum {
  IPiece = 0,
  SPiece = 1,
  ZPiece = 2,
  JPiece = 3,
  LPiece = 4,
  OPiece = 5,
  TPiece = 6,
  MaxPiece = 7,
  EmptyPiece = -1
} pieceType;

typedef enum {
  WHITE = 0,
  BLACK = 1
} pieceColor;

piece createPiece(pieceType type);
void destroyPiece(piece self);

piece rotatePiece(piece self);
