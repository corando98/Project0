/*
About this program:
- This program counts words.
- The specific words that will be counted are passed in as command-line
  arguments.
- The program reads words (one word per line) from standard input until EOF or
  an input line starting with a dot '.'
- The program prints out a summary of the number of times each word has
  appeared.
- Various command-line options alter the behavior of the program.

E.g., count the number of times 'cat', 'nap' or 'dog' appears.
> ./main cat nap dog
Given input:
 cat
 .
Expected output:
 Looking for 3 words
 Result:
 cat:1
 nap:0
 dog:0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smp0_tests.h"

#define LENGTH(s) (sizeof(s) / sizeof(*s))

/* Structures */
typedef struct
{
  char *word;
  int counter;
} WordCountEntry;

int process_stream(WordCountEntry entries[], int entry_count)
{
  short line_count = 0;
  char buffer[30];
  char *tokenPointer;
  while (fgets(buffer, sizeof(buffer), stdin))
  {

    if (*buffer == '.')
      break;
    //printf("%s\n",buffer);
    tokenPointer = strtok(buffer, " \t\n");
    while (tokenPointer != NULL)
    {
      int i = 0;
      while (i < entry_count)
      {
        if (!strcmp(entries[i].word, tokenPointer))
          entries[i].counter++;
        i++;
      }
      //printf ("%s\n",tokenPointer);
      tokenPointer = strtok(NULL, " \t\n");
    }

    /* Compare against each entry */
    /*int i = 0;
    while (i < entry_count) {
      if (!strcmp(entries[i].word, buffer))
        entries[i].counter++;
      i++;
    }*/
    line_count++;
  }
  return line_count;
}

void print_result(WordCountEntry entries[], int entry_count /*FILE * output*/)
{
  fprintf(stdout, "Result:\n");
  int index = 0;
  for (index = 0; index < entry_count; index++)
  {
    fprintf(stdout, "%s:%d\n", entries[index].word, entries[index].counter);
  }
  //The reason why the error occurs is because the pointer will only point to the first item so that we can only get ./temp. Now I have changed the pointer to a normal one so that every time the loop runs it will point to different items.
}

void printHelp(const char *name)
{
  fprintf(stderr, "usage: %s [-h] <word1> ... <wordN>\n", name);
}

int main(int argc, char **argv)
{
  const char *prog_name = *argv;

  WordCountEntry *entries = malloc(sizeof(WordCountEntry) * argc);
  int entryCount = 0;

  //printf(argv[0]);
  /* Entry point for the testrunner program */
  if (argc > 1 && !strcmp(argv[1], "-test"))
  {
    //printf("trying");
    run_smp0_tests(argc - 1, argv + 1);
    return EXIT_SUCCESS;
  }

  argv++;

  while (*argv != NULL)
  {
    if (**argv == '-')
    {

      //printf("now we are gettign a - \n");
      switch ((*argv)[1])
      {
      case 'h':
        printHelp(prog_name);
        break;
      case 'f':
        stdout = fopen(*argv + 2, "w");
        break;
      default:
        fprintf(stderr, "%s: Invalid option %s. Use -h for help.\n",
                prog_name, *argv);
      }
    }
    else
    {
      if (entryCount < argc - 1)
      {

        entries[entryCount].word = *argv;
        entries[entryCount++].counter = 0;
      }
    }
    argv++;
  }
  if (entryCount == 0)
  {
    fprintf(stderr, "%s: Please supply at least one word. Use -h for help.\n",
            prog_name);
    return EXIT_FAILURE;
  }
  if (entryCount == 1)
  {
    fprintf(stdout, "Looking for a single word\n");
  }
  else
  {
    fprintf(stdout, "Looking for %d words\n", entryCount);
  }

  process_stream(entries, entryCount);
  print_result(entries, entryCount);

  return EXIT_SUCCESS;
}
