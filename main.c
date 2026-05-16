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
  int width = 30, height = 10;
  int px = 2, py = 2;
  int length = width * height;
  int size = 1;

  system("chcp 65001");
  printf("╭────╮\n│    │\n╰─●─╸╯\n");
  printf("╭────╮\n│    │\n╰─●╶─╯\n");
  // printf("%s\n", '╮');
          // ╭────────╮    
          // │        │  
          // │        │  
          // ╰───●────╯    
                     

  drawEmptyMap(width, height);

  int snake[length];
  int head = 0;
  int tail = 0;
  for (int i = 0; i < length; i++) {
    snake[i] = py * width + px;
  }

  while (lastChar != 3) { // Escape when user presses ctrl+c
    lastChar = _getch();
    // Arrow key was used
    if (lastChar == 224) lastChar += getch_noblock();

    if (lastChar == 97 || lastChar == 224 + 75) px = wrap(px, width, -1); // A or Left arrow
    else if (lastChar == 100 || lastChar == 224 + 77) px = wrap(px, width, 1); // D or Right arrow
    else if (lastChar == 119 || lastChar == 224 + 72) py = wrap(py, height, -1); // W or Up arrow
    else if (lastChar == 115 || lastChar == 224 + 80) py = wrap(py, height, 1); // S or Down arrow

    if (head - tail < 80 && head - tail >= 0) {
      add(snake, py * width + px, length, &head, &tail);
      size++;
    } else {
      set(snake, py * width + px, length, &head, &tail);
    }

    int y1 = snake[(head + (length - 1)) % length] / width;
    int x1 = snake[(head + (length - 1)) % length] - y1 * width;
    int y2 = snake[(head + (length - 2)) % length] / width;
    int x2 = snake[(head + (length - 2)) % length] - y2 * width;
          // ╭────────╮    
          // │        │  
          // │        │  
          // ╰───●────╯    

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

    int ty = snake[(tail + (length - 1)) % length] / width;
    int tx = snake[(tail + (length - 1)) % length] - ty * width;
    // Remove tail
    printf("\e[%dA\e[%dC \e[%dD\e[%dB", height - ty - 1, tx + 1, tx + 2, height - ty - 1);

    int pos = py * width + px;
    for (int j = 0; j < size - 1; j++) {
      if (snake[(tail + j) % length] == pos) return 0;
    }
  }
}
