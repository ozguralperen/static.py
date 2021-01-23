#include "defs.h"
#define extern_
#include "data.h"
#undef extern_
#include "decl.h"
#include <errno.h>

static void init()
{
  Line = 1;
  Putback = '\n';
}

static void usage()
{
  fprintf(stderr, "Usage: spy < file >\n");
  exit(1);
}

void main(int argc, char *argv[])
{
  struct ASTnode *tree;

  if (argc != 2)
    usage();

  init();

  if ((Infile = fopen(argv[1], "r")) == NULL)
  {
    fprintf(stderr, "Unable to open source file %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  if ((Outfile = fopen("out.s", "w")) == NULL)
  {
    fprintf(stderr, "Unable to create assembly output : %s\n", strerror(errno));
    exit(1);
  }

  addglob("printint", P_CHAR, S_FUNCTION, 0);

  scan(&Token);
  genpreamble();
  while (1)
  {
    tree = function_declaration();
    PrintAST(tree , 0);
    getchar();
    
    genAST(tree, NOREG, 0);
    if (Token.token == T_EOF)
      break;
  }
  fclose(Outfile);
  exit(0);
}
