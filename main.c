#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

float randomFloat() {
  float r = (float)rand() / (float)RAND_MAX;
  return r;
}

enum Direction { RIGHT, LEFT, DOWN, UP };

void drawEmptyMap(int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (x == 0 || x == width - 1 || y == 0 || y == height - 1) printf("0");
      else printf(" ");
    }
    printf("\n");
  }
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

int wrap(int a, int b, int c) {
  return (a + b + (c - 2)) % (b - 2);
}

int main() {
  int lastChar = 0;
  int width = 30, height = 15;
  int px = 0, py = 0;
  int ax = (width - 2) / 2, ay = (height - 2) / 2;
  int maxScore = (width - 2) * (height - 2);
  int length = width * height;
  int size = 1;

  srand(time(NULL));
  system("chcp 65001");
  printf("\e[A");

  drawEmptyMap(width, height);

  printf("\e[%dA\e[%dC@\e[%dD\e[%dB", height - ay - 1, ax + 1, ax + 2, height - ay - 1);

  int snake[length];
  memset(snake, py * width + px, sizeof snake);
  int head = 0, tail = 0;

  while (lastChar != 3) { // Escape when user presses ctrl+c
    lastChar = _getch();
    // Arrow key was used
    if (lastChar == 224) lastChar += getch_noblock();

    if (lastChar == 97 || lastChar == 224 + 75) px = wrap(px, width, -1); // A or Left arrow
    else if (lastChar == 100 || lastChar == 224 + 77) px = wrap(px, width, 1); // D or Right arrow
    else if (lastChar == 119 || lastChar == 224 + 72) py = wrap(py, height, -1); // W or Up arrow
    else if (lastChar == 115 || lastChar == 224 + 80) py = wrap(py, height, 1); // S or Down arrow
    else continue;

    if (px == ax && py == ay) {
      add(snake, py * width + px, length, &head, &tail);
      size++;
      int apos = 0;
      int failed = 0;

      if (size < maxScore) {
        appleSearch:
        failed++;
        ax = randomFloat() * (width - 3);
        ay = randomFloat() * (height - 3);
        apos = ay * width + ax;
        for (int j = 0; j <= size; j++) {
          if (snake[(tail + j) % length] == apos) goto appleSearch;
        }
      } else goto restart;

    } else {
      set(snake, py * width + px, length, &head, &tail);
    }

    int y1 = snake[(head + (length - 1)) % length] / width;
    int x1 = snake[(head + (length - 1)) % length] - y1 * width;
    int y2 = snake[(head + (length - 2)) % length] / width;
    int x2 = snake[(head + (length - 2)) % length] - y2 * width;

    int ty = snake[(tail + (length - 1)) % length] / width;
    int tx = snake[(tail + (length - 1)) % length] - ty * width;
    // Remove tail
    printf("\e[%dA\e[%dC \e[%dD\e[%dB", height - ty - 1, tx + 1, tx + 2, height - ty - 1);

    // Move top, right print, left + 1, down
    // Render head
    if (y1 == wrap(py, height, -1)) {
      printf("\e[%dA\e[%dC╹\e[%dD\e[%dB", height - py - 1, px + 1, px + 2, height - py - 1);
    }
    else if (y1 == wrap(py, height, 1)) {
      printf("\e[%dA\e[%dC╻\e[%dD\e[%dB", height - py - 1, px + 1, px + 2, height - py - 1);
    }
    else if (x1 == wrap(px, width, -1)) {
      printf("\e[%dA\e[%dC╸\e[%dD\e[%dB", height - py - 1, px + 1, px + 2, height - py - 1);
    }
    else if (x1 == wrap(px, width, 1)) {
      printf("\e[%dA\e[%dC╺\e[%dD\e[%dB", height - py - 1, px + 1, px + 2, height - py - 1);
    }

    if (size > 1) {
      // Render curve behind head
      if ((y1 == wrap(py, height, -1) && x2 == wrap(px, width, -1)) || (y2 == wrap(py, height, 1) && x1 == wrap(px, width, 1))) {
        printf("\e[%dA\e[%dC╮\e[%dD\e[%dB", height - y1 - 1, x1 + 1, x1 + 2, height - y1 - 1);
      }
      else if ((y1 == wrap(py, height, 1) && x2 == wrap(px, width, 1)) || (y2 == wrap(py, height, -1) && x1 == wrap(px, width, -1))) {
        printf("\e[%dA\e[%dC╰\e[%dD\e[%dB", height - y1 - 1, x1 + 1, x1 + 2, height - y1 - 1);
      }
      else if ((y1 == wrap(py, height, -1) && x2 == wrap(px, width, 1)) || (y2 == wrap(py, height, 1) && x1 == wrap(px, width, -1))) {
        printf("\e[%dA\e[%dC╭\e[%dD\e[%dB", height - y1 - 1, x1 + 1, x1 + 2, height - y1 - 1);
      }
      else if ((y1 == wrap(py, height, 1) && x2 == wrap(px, width, -1)) || (y2 == wrap(py, height, -1) && x1 == wrap(px, width, 1))) {
        printf("\e[%dA\e[%dC╯\e[%dD\e[%dB", height - y1 - 1, x1 + 1, x1 + 2, height - y1 - 1);
      }
      else if (y1 == py) {
        printf("\e[%dA\e[%dC─\e[%dD\e[%dB", height - y1 - 1, x1 + 1, x1 + 2, height - y1 - 1);
      }
      else if (x1 == px) {
        printf("\e[%dA\e[%dC│\e[%dD\e[%dB", height - y1 - 1, x1 + 1, x1 + 2, height - y1 - 1);
      }
    }

    // Render apple
    printf("\e[%dA\e[%dC@\e[%dD\e[%dB", height - ay - 1, ax + 1, ax + 2, height - ay - 1);

    int pos = py * width + px;
    for (int j = 0; j < size - 1; j++) {
      // Reset game
      if (snake[(tail + j) % length] == pos) {
        goto restart;
      }
    }

    continue;

    restart: 
    printf("Game has ended with a score of: %d.\nPress any key to continue.\n", size);
    size = 1;
    py = 0;
    px = 0;
    head = 0;
    tail = 0;
    memset(snake, py * width + px, sizeof snake);
    lastChar = _getch();

    printf(
      "\e[A                                            \n"
      "\e[2A                                           \n"
      "\e[%dA",
      height + 1
    );
    drawEmptyMap(width, height);
  }

  printf("Exited game with a score of: %d", size);
}
