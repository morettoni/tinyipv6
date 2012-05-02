/*
   Copyright (c) 2005  Morettoni Luca <luca@morettoni.net>
   All rights reserved.
   
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
   SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LOBYTE(x) (x & 0xff)
#define HIBYTE(x) ((x >> 8) & 0xff)

int comment = 0;

void usage (void)
{
  fprintf (stderr, "usage: add-host6 [-c] host IPv6\n");
  fprintf (stderr, "with -c option add comment about IPv6 address\n");
  _exit (1);
}

int parseip (char *base, char *in)
{
  char *fp[8];
  char val[5];
  int fn = 0;
  int c, n;
  int expand = 0;
  int oct[8];
  int pos;

  fp[fn++] = in;
  for (c = 1; *(in+c); c++) 
    if (*(in+c) == ':' && *(in+c+1)) fp[fn++] = in+c+1;

  if (fn < 2 || fn > 8) return 0;

  pos = 0;
  for (c = 0; c < fn; c++) {
    for (n = 0; n < 4 && *(fp[c]+n) != ':'; n++)
      val[n] = *(fp[c]+n);
    val[n] = 0x00;

    if (!n) {
      if (expand) return 0;

      expand = 1;
      for (n = 0; n < 8-fn+1; n++) oct[pos++] = 0;
    } else
      oct[pos++] = strtol(val, (char **) NULL, 16);
  }

  if (comment) {
    printf ("# %s AAAA ", base);
    for (c = 0; c < 7; c++) printf ("%x:", oct[c]);
    printf ("%x\n", oct[7]);
  }

  printf (":%s:28:", base);
  for (c = 0; c < 8; c++) printf ("\\%03o\\%03o", HIBYTE(oct[c]), LOBYTE(oct[c]));
  printf ("\n");

  return 1;
}

int main (int argc, char *argv[])
{
  if (argc != 3) usage ();

  if (strcmp (argv[1], "-c") == 0) {
    comment = 1;
    argc--;
    argv++;
  }

  if (!parseip (argv[1], argv[2])) {
    fprintf (stderr, "\"%s\" don't seem IPv6 address.\n", argv[2]);
    _exit (1);
  }
  
  _exit (0);
}
