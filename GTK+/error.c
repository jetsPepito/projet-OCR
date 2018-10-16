#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void print_info (char *format, ...)
{
  va_list va;

  va_start(va, format);
  printf("Information : ");
  vprintf(format, va);
  printf("\n");
}

void print_warning (char *format, ...)
{
  va_list va;

  va_start(va, format);
  fprintf(stderr, "Erreur : ");
  vfprintf(stderr, format, va);
  fprintf(stderr, "\n");
}

void print_error (char *format, ...)
{
  va_list va;

  va_start(va, format);
  fprintf(stderr, "Erreur fatale : ");
  vfprintf(stderr, format, va);
  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}
