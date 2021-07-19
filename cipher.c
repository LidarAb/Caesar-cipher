#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define COMMAND_1 "encode"
#define COMMAND_2 "decode"
#define COMMAND_3 "check"
#define COMMAND_ERR "The given command is invalid\n"
#define NUM_ARGV_ERR_1 "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n"
#define NUM_ARGV_ERR_2 "Usage: cipher <check> <source path file> <output path file>\n"
#define FILE_ERR "The given file is invalid\n"
#define INVALID_CHECK "Invalid encrypting\n"
#define VALID_CHECK "Valid encrypting with k = %d\n"
#define FIVE_ARGV 5
#define FOUR_ARGV 4
#define NUM_OF_LETTERS 26
#define MIN_SMALL_LETTERS 97
#define MAX_SMALL_LETTERS 122
#define MIN_CAPITAL_LETTERS 65
#define MAX_CAPITAL_LETTERS 90
#define MIN_K -25
#define MAX_K 25
#define RANDOM_VAL 300

int check_validity (int argc, char *argv[])
/// This function checks if there are invalid situations before running the
/// program.
/// \param argc int represents the number of args we get from the CML
/// \param argv the argv we got from the CML - the command, files paths and the
/// offset parameter, k.
/// \return EXIT_FAILURE when there is a problem, and zero when everything is valid.
{
  // if the command is not one of the three.
  if ((argc == 1) || ((strcmp (COMMAND_1, argv[1]) != 0) &&
                      (strcmp (COMMAND_2, argv[1]) != 0) &&
                      (strcmp (COMMAND_3, argv[1]) != 0)))
    {
      fprintf (stderr, COMMAND_ERR);
      return EXIT_FAILURE;
    }
  // check validity when the command is "encode" or "decode".
  if ((strcmp (COMMAND_1, argv[1]) == 0) || (strcmp (COMMAND_2, argv[1]) == 0))
    {

      if (argc != FIVE_ARGV)
        {
          fprintf (stderr, NUM_ARGV_ERR_1);
          return EXIT_FAILURE;
        }
      FILE *f = fopen (argv[3], "r");
      if (f == NULL)
        {
          fprintf (stderr, FILE_ERR);
          return EXIT_FAILURE;
        }
      fclose (f);
    }
  // check validity when the command is "check".
  if ((strcmp (COMMAND_3, argv[1]) == 0))
    {
      if (argc != FOUR_ARGV)
        {
          fprintf (stderr, NUM_ARGV_ERR_2);
          return EXIT_FAILURE;
        }
      FILE *f1 = fopen (argv[2], "r");
      FILE *f2 = fopen (argv[3], "r");
      if (f1 == NULL)
        {
          fprintf (stderr, FILE_ERR);
          return EXIT_FAILURE;
        }
      fclose (f1);
      if (f2 == NULL)
        {
          fprintf (stderr, FILE_ERR);
          return EXIT_FAILURE;
        }
      fclose (f2);
    }

  return 0;
}

void encode_decode (char *argv[])
/// The function encrypts text according to the offset parameter it receives,
/// and according to the command it receives: "encode" or "decode".
/// if the command is "encode", the function encrypts the alphabet according
/// to the k parameter and thr ascii table. if the command is "decode",
///// we execute encode with -k.
/// The original text is called from an input file and the encrypted text is
/// written to an output file, two paths that given in the CML.
/// \param argv the argv we got from the CML
{
  int k = atoi (argv[2]);
  // if the command is "decode" - we change the sign of k
  if ((strcmp (COMMAND_2, argv[1]) == 0))
    k = -1 * k;
  // if k is negative, we change it to positive without changing the offset.
  if (k < 0)
    k = (k % NUM_OF_LETTERS) + NUM_OF_LETTERS;
  // make sure k is between 0 to 26
  else
    k = k % NUM_OF_LETTERS;
  FILE *source_f = fopen (argv[3], "r");
  FILE *output_f = fopen (argv[4], "w");
  char input;
  while ((input = (char) getc (source_f)) != EOF)
    {
      int curr;
      char new;

      // if the char is not a letter, we write it with no change
      if (!((MIN_CAPITAL_LETTERS <= (int) input)
            && ((int) input <= MAX_CAPITAL_LETTERS))
          && !((MIN_SMALL_LETTERS <= (int) input)
               && ((int) input <= MAX_SMALL_LETTERS)))
        {
          new = input;
        }
      else
        {
          // if the char is a small letter
          if ((MIN_SMALL_LETTERS <= (int) input)
              && ((int) input <= MAX_SMALL_LETTERS))
            {
              curr = ((int) input + k);
              if (curr > MAX_SMALL_LETTERS)
                new = (char) ((MIN_SMALL_LETTERS - 1)
                              + (curr - MAX_SMALL_LETTERS));
              else
                new = (char) curr;
            }
            // if the char is a capital letter
          else
            {
              curr = ((int) input + k);
              if (curr > MAX_CAPITAL_LETTERS)
                new = (char) ((MIN_CAPITAL_LETTERS - 1)
                              + (curr - MAX_CAPITAL_LETTERS));
              else
                new = (char) curr;
            }
        }
      fprintf (output_f, "%c", new);
    }
  fclose (source_f);
  fclose (output_f);
}

int check (char *argv[])
/// This function checks if the text that in the output file is
/// valid encrypting of the text that in the source file, with k between 0 to 25.
/// \param argv the argv we got from the CML
/// \return returns EXIT_SUCCESS anyway
{
  FILE *source_f = fopen (argv[2], "r");
  FILE *output_f = fopen (argv[3], "r");
  int k = RANDOM_VAL;
  int tempt;
  char input1 = (char) getc (source_f);
  char input2 = (char) getc (output_f);
  while ((input1 != EOF) && (input2 != EOF))
    {
      // if the first char is not letter
      if (!((MIN_CAPITAL_LETTERS <= input1) && (input1 <= MAX_CAPITAL_LETTERS))
          && !((MIN_SMALL_LETTERS <= input1) && (input1 <= MAX_SMALL_LETTERS)))
        {
          if (input1 != input2)
            {
              fprintf (stdout, INVALID_CHECK);
              fclose (source_f);
              fclose (output_f);
              return EXIT_SUCCESS;
            }
        }
      // if the first char is letter, but the second char is not letter
      else if (
          !((MIN_CAPITAL_LETTERS <= input2) && (input2 <= MAX_CAPITAL_LETTERS))
          && !((MIN_SMALL_LETTERS <= input2) && (input2 <= MAX_SMALL_LETTERS)))
        {
          fprintf (stdout, INVALID_CHECK);
          fclose (source_f);
          fclose (output_f);
          return EXIT_SUCCESS;
        }
        // if both chars are letters
      else
        {
          tempt = (int) input2 - (int) input1;
          // if k is not in the range 0-25 - invalid
          if (!((MIN_K <= tempt) && (tempt <= MAX_K)))
            {
              fprintf (stdout, INVALID_CHECK);
              fclose (source_f);
              fclose (output_f);
              return EXIT_SUCCESS;
            }
          // if k is valid and negative, we make it positive
          else if (tempt < 0)
            {
              tempt += NUM_OF_LETTERS;
            }
          if ((k == RANDOM_VAL) || (k == tempt))
            k = tempt;
          // if k and tempt are not equal - invalid
          else
            {
              fprintf (stdout, INVALID_CHECK);
              fclose (source_f);
              fclose (output_f);
              return EXIT_SUCCESS;
            }
        }
      input1 = (char) getc (source_f);
      input2 = (char) getc (output_f);
    }
  // we check if both files end together
  if ((input1 != EOF && input2 == EOF)
      || (input1 == EOF && input2 != EOF))
    {
      fprintf (stdout, INVALID_CHECK);
    }
  // if the encrypting is valid but k did not change - all chars was not letters
  // so k is zero.
  else if (k == RANDOM_VAL)
    fprintf (stdout, VALID_CHECK, 0);
  else
    fprintf (stdout, VALID_CHECK, k);
  fclose (source_f);
  fclose (output_f);
  return EXIT_SUCCESS;
}

int main (int argc, char *argv[])
/// \param argc int represents the number of args we get from the CML
/// \param argv the argv we got from the CML - the command, files paths and the
/// offset parameter, k.
/// \return EXIT_FAILURE when there is a problem, and EXIT_SUCCESS when the
/// program run good.

{
  if (check_validity (argc, argv) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  else if ((strcmp (COMMAND_1, argv[1]) == 0)
           || (strcmp (COMMAND_2, argv[1]) == 0))
    {
      encode_decode (argv);
    }
  else if ((strcmp (COMMAND_3, argv[1]) == 0))
    {
      check (argv);
    }

  return EXIT_SUCCESS;
}