#include <stdio.h>
#include <stdlib.h>
#include "babl/babl.h"

int main (int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf (stderr, "need two args, from and to babl-formats\n");
    return -1;
  }
  setenv ("BABL_DEBUG_CONVERSIONS", "1", 0);
  babl_init ();
  babl_fish (babl_format(argv[1]), babl_format (argv[2]));
  babl_exit ();

  return 0;
}