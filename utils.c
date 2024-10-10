#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Sample *load_samples(const char *location, int *sample_count) {
  // read file
  FILE *fp = fopen(location, "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return NULL;
  }

  // read file into memory
  char *line = NULL;
  fseek(fp, 0, SEEK_END);
  long length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *buffer = malloc(length);
  fread(buffer, 1, length, fp);
  fclose(fp);

  // count the number of samples so we can allocate enough memory
  int lines = 1;
  for (int i = 0; i < length; i++)
    if (buffer[i] == '\n')
      lines++;

  *sample_count = lines;

  Sample *samples = malloc(sizeof(Sample) * lines);
  for (int i = 0; i < lines; i++) {
    samples[i].label = 0;
    samples[i].input = matrix_new(1, 784, 0);
  }

  // the current character we're looking at from the buffer
  int currentBufferIndex = 0;

  // create a buffer to store numbers
  char currentNumber[4];
  currentNumber[0] = '\0';
  currentNumber[1] = '\0';
  currentNumber[2] = '\0';
  currentNumber[3] = '\0';
  int currentNumberIndex = 0; // current index we're writing to

  // the current sample we're parsing for
  int currentSampleIndex = 0;
  int isParsingLabel = 1;
  int currentColumn = 0;

  while (currentBufferIndex < length) {
    // hit a comma so assign the thing inside the current number buffer into
    // either the label or current column in matrix
    if (buffer[currentBufferIndex] == ',') {
      currentNumber[3] = '\0';

      int number = atoi(currentNumber);
      double normalized = number / 255.0;

      if (isParsingLabel == 1) {
        samples[currentSampleIndex].label = number;
        isParsingLabel = 0;
      } else {
        matrix_set(samples[currentSampleIndex].input, 0, currentColumn++,
                   normalized);
      }

      currentNumberIndex = 0;
      currentNumber[0] = '\0';
      currentNumber[1] = '\0';
      currentNumber[2] = '\0';
      currentNumber[3] = '\0';
    }

    // reset variables so we're ready to parse the next sample
    else if (buffer[currentBufferIndex] == '\n') {
      currentNumberIndex = 0;
      currentNumber[0] = '\0';
      currentNumber[1] = '\0';
      currentNumber[2] = '\0';
      currentNumber[3] = '\0';
      currentColumn = 0;
      isParsingLabel = 1;
      currentSampleIndex++;
    }

    // parse things normally
    else if ('0' <= buffer[currentBufferIndex] &&
             buffer[currentBufferIndex] <= '9') {
      currentNumber[currentNumberIndex++] = buffer[currentBufferIndex];
    }

    currentBufferIndex++;
  }

  return samples;
}
