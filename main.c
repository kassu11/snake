#include <conio.h>
#include <stdio.h>

#define clear() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

#ifdef _WIN32
#include <windows.h>
#define delay(ms) Sleep(ms)
#else
#include <unistd.h>
#define delay(ms) usleep((ms) * 1000)
#endif

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

void set(int *values, int value, int length, int *head, int *tail) {
  *head = (*head + 1) % length;
  *tail = (*tail + 1) % length;
  values[*head] = value;
}

void add(int *values, int value, int length, int *head, int *tail) {
  *head = (*head + 1) % length;
  values[*head] = value;
}

int main() {
  int lastChar = 0;
  int width = 30, height = 10;
  int length = width * height;

  drawEmptyMap(width, height);

  int snake[length];
  int head = 0;
  int tail = 0;
  for (int i = 0; i < length; i++) {
    snake[i] = 0;
  }

  int px = 2, py = 2;

  while (lastChar != 3) { // Escape when user presses ctrl+c
    lastChar = _getch();
    // Arrow key was used
    if (lastChar == 224) lastChar += getch_noblock();

    if (lastChar == 97 || lastChar == 224 + 75) px = (px + width - 3) % (width - 2); // A or Left arrow
    else if (lastChar == 100 || lastChar == 224 + 77) px = (px + 1) % (width - 2); // D or Right arrow
    else if (lastChar == 119 || lastChar == 224 + 72) py = (py + height - 3) % (height - 2); // W or Up arrow
    else if (lastChar == 115 || lastChar == 224 + 80) py = (py + 1) % (height - 2); // S or Down arrow

    // Move top, right print, left + 1, down
    printf("\e[%dA\e[%dC#\e[%dD\e[%dB", height - py - 1, px + 1, px + 2, height - py - 1);

    if (head - tail < 15 && head - tail >= 0) {
      add(snake, py * width + px, length, &head, &tail);
    } else {
      set(snake, py * width + px, length, &head, &tail);
      int y = snake[tail] / width;
      int x = snake[tail] - y * width;
      // Move top, right print, left + 1, down
      printf("\e[%dA\e[%dC.\e[%dD\e[%dB", height - y - 1, x + 1, x + 2, height - y - 1);
    }
  }
}
