#ifndef TILE_H
#define TILE_H

class Tile {
public:
  Tile() {
    row = -1;
    col = -1;
  }

  Tile(int r, int c) {
    row = r;
    col = c;
  }

  int row;
  int col;
};

#endif