#include <stdio.h>
#include <conio.h>

int main() {
  int lastChar = 0;
  while (lastChar != 3) { // Escape when user presses ctrl+c
    lastChar = _getch();
    printf("%d\n", lastChar - 'a' + 1);
  }

  // strcmp("12", "21") == 1 -1 = 0;

  printf("Hello world");
}

