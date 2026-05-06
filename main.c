#include <conio.h>
#include <stdio.h>

#define clear() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

int getch_noblock() {
  if (_kbhit())
    return _getch();
  else
    return -1;
}

enum Direction { RIGHT, LEFT, DOWN, UP };

void drawEmptyMap(int width, int height) {
  width += 1;
  char map[width * height + 1];

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width - 1; x++) {
      if (x == 0 || x == width - 2 || y == 0 || y == height - 1) {
        map[y * width + x] = '0';
      } else {
        map[y * width + x] = '.';
      }
    }
    map[y * width + width - 1] = '\n';
  }
  map[height * width] = '\0';

  printf("%s", map);
}

int main() {
  int lastChar = 0;
  int width = 10, height = 10;

  drawEmptyMap(width, height);

  int px = 2, py = 2;

  while (lastChar != 3) { // Escape when user presses ctrl+c
    // Handle player movement
    lastChar = getch_noblock();
    if (lastChar == 224) { // Arrow key was used
      lastChar = getch_noblock();
      if (lastChar == 75) { // Left arrow
        px = (px + width - 3) % (width - 2);
      } else if (lastChar == 77) { // Right arrow
        px = (px + 1) % (width - 2);
      } else if (lastChar == 72) { // Up arrow
        py = (py + height - 3) % (height - 2);
      } else if (lastChar == 80) { // Down arrow
        py = (py + 1) % (height - 2);
      }
    } else if (lastChar == 97) { // A
      px = (px + width - 3) % (width - 2);
    } else if (lastChar == 100) { // D
      px = (px + 1) % (width - 2);
    } else if (lastChar == 119) { // W
      py = (py + height - 3) % (height - 2);
    } else if (lastChar == 115) { // S
      py = (py + 1) % (height - 2);
    }

    px += 1;
    py += 1;

    // Move top, right print, left + 1, down
    printf("\e[%dA\e[%dC#\e[%dD\e[%dB", height - py, px, px + 1, height - py);

    px -= 1;
    py -= 1;
  }
}
