/*
 * main.c
 *
 * The main entry point to the program.
 *
 * Tom Leaman (tom@tomleaman.co.uk)
 */

#include <stdlib.h>
#include <stdio.h>

void print_menu() {
  printf("\n");
  printf("Please select from the following options:\n");
  printf("\n");
  printf("\t1. Locate a competitor\n");
  printf("\t2. Show how many competitors have not yet started\n");
  printf("\t3. Show how many competitors are currently on the course\n");
  printf("\t4. Show how many competitors have finished\n");
  printf("\t5. Supply checkpoint times manually\n");
  printf("\t6. Supply checkpoint times from a file\n");
  printf("\t7. Display results list\n");
  printf("\t8. Exit the program\n");
  printf("\n");
}

int prompt() {
  int result = -1;
  printf(">>  ");
  scanf("%d", &result);
  return result;
}

int main(int argc, char* argv[]) {
  int input;

  /* Init data structures and whatnot */

  /* The main program loop, shows the menu and prompts for input */
  input = -1;
  while (1) {
    print_menu();
    input = prompt();
    if (input == 1) { // locate competitor

    } else if (input == 2) { // show not started

    } else if (input == 3) { // show on course

    } else if (input == 4) { // show finished

    } else if (input == 5) { // supply checkpoints manually

    } else if (input == 6) { // read from file

    } else if (input == 7) { // show results

    } else if (input == 8) { // quit
      return EXIT_SUCCESS;
    } else {
      // invalid input, do nothing
    }
  }

  return EXIT_SUCCESS;
}
