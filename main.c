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

int main() {
  int lastChar = 0;
  int width = 30, height = 10;
  char map[width * height + 1];

  clear();

  int px = 0, py = 0;

  while (lastChar != 3) { // Escape when user presses ctrl+c
    // Handle player movement
    lastChar = getch_noblock();
    if (lastChar == 224) { // Arrow key was used
      lastChar = getch_noblock();
      if (lastChar == 75) { // Left arrow
        px -= 1;
      } else if (lastChar == 77) { // Right arrow
        px += 1;
      } else if (lastChar == 72) { // Up arrow
        py -= 1;
      } else if (lastChar == 80) { // Down arrow
        py += 1;
      }
    } else if (lastChar == 97) { // A
      px -= 1;
    } else if (lastChar == 100) { // D
      px += 1;
    } else if (lastChar == 119) { // W
      py -= 1;
    } else if (lastChar == 115) { // S
      py += 1;
    }

    // Render map
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width - 1; x++) {
        if (px == x && py == y) {
          map[y * width + x] = '#';
        } else if (x == 0 || x == width - 2 || y == 0 || y == height - 1) {
          map[y * width + x] = '0';
        } else {
          map[y * width + x] = '.';
        }
      }
      map[y * width + width - 1] = '\n';
    }
    map[height * width] = '\0';

    // Move caret to top and print everything at once
    printf("\e[H%s", map);

    // lastChar = _getch();
    // printf("%d\n", lastChar);
  }
}
