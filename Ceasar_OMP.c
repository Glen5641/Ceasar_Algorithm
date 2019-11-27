#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define DEBUG 0

/*
 *  Homework 5 Problem 1
 *  Ceasar Algorithm with OpenMP
 *  Generates a String of Characters and Takes The String and ****Adds a
 *  Displacement to Each Character Shifting it by a Certain Number****(TIMED).
 *  Once the String has been Shifted, the Program Shifts it Back to Ensure
 *  no Corruption in the Algorithm.
 *
 *  @Author Clayton Glenn
 *
 * Args   Thread Count 1 : 20
 *        Displacement any
 *        Character Count 0 : 16
 */
int main(int argc, char **argv) {

  /* Check if the Program has Sufficient Args */
  if (argc < 4) {
    fprintf(stderr, "Invalid Arguments\nCorrect Form: mpirun ceasar [# Threads] [Displacement] [# Characters]\n");
    return -1;
  }

  /* Grab the Variables From the Args */
  int thread_count  = strtol(argv[1], NULL, 10);
  int displacement  = strtol(argv[2], NULL, 10);
  int num_chars     = strtol(argv[3], NULL, 10);

  /* Ensure Valid Thread Count */
  if(thread_count < 1 || thread_count > 20) {
    fprintf(stderr, "Invalid Thread Count %d\n", thread_count);
    return -1;
  }

  /* Keep the Displacement Within Readable Values if Positive.
     Use Negative at your Own Risk. Immediate Unreadables     */
  if(displacement > 0) {
    displacement = displacement % 74;
  }

  /* Ensure Valid Character Count */
  if(num_chars < 1 || num_chars > 16) {
    fprintf(stderr, "Invalid Thread Count %d\n", num_chars);
    return -1;
  }

  /* Print Essential Locators */
  fprintf(stdout, "Thread Count: %d\n", thread_count);
  fprintf(stdout, "num_chars:    2^%d\n", num_chars);

  /* For Debug, Print all Variables and Pragma Space Entrance */
  if (DEBUG) {
    fprintf(stdout, "Displacement: %d\n", displacement);
    fprintf(stdout, "Entering Pragma Space\n");
  }

  num_chars = pow(2, num_chars);

  /* Declare a Set Array of Characters and a Counter */
  char string[num_chars];
  int index;

  /* Set all Values of the Character Array to Character Value Zero */
  for(index = 0; index < num_chars; index++) {
    string[index] = 48;
  }

  /* If Debug is Set, Print the Array to Standard Out */
  if (DEBUG) {
    for(index = 0; index < num_chars; index++) {
      fprintf(stdout, "%c", string[index]);
    }
    fprintf(stdout, "\n");
  }

  /* Start the Parallel Time */
  double start = omp_get_wtime();

  /* Add the Displacement to Each Character in the String in Parallel */
  index = 0;
  #pragma omp parallel for num_threads(thread_count) shared(string)
    for(index = 0; index < num_chars; index++) {
      string[index] += displacement;
    }

  /* Grab the Final Time and Print to Standard Out */
  double final_time = omp_get_wtime() - start;
  fprintf(stdout, "RunTime: %2.10f seconds\n", final_time);

  /* If Debug is set, Print the Shifted Characters */
  if (DEBUG) {
    for(index = 0; index < num_chars; index++) {
      fprintf(stdout, "%c", string[index]);
    }
    fprintf(stdout, "\n");
  }

  /* If Debug is set, Subtract the Displacement of the String
     to Get the Original Zeroed Array and Print the Array     */
  if (DEBUG) {
    for(index = 0; index < num_chars; index++) {
      string[index] -= displacement;
      fprintf(stdout, "%c", string[index]);
    }
    fprintf(stdout, "\n");
  }

  /* Print New Line and Finish Program with Success */
  fprintf(stdout, "\n");
  return 0;
}
