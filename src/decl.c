#include "defs.h"
#include "data.h"
#include "decl.h"

int parse_type(int t)
{
  if (t == T_CHAR)
    return (P_CHAR);
  if (t == T_INT)
    return (P_INT);
  if (t == T_LONG)
    return (P_LONG);
  if (t == T_VOID)
    return (P_VOID);
  fatald("Illegal Type , Token", t);
}

void var_declaration(void)
{
  int id, type;

  type = parse_type(Token.token);
  scan(&Token);
  ident();

  id = addglob(Text, type, S_VARIABLE, 0);
  genglobsym(id);
}

struct ASTnode *function_declaration(void)
{
  struct ASTnode *tree, *finalstmt;
  int nameslot, type, endlabel;

  type = parse_type(Token.token);
  scan(&Token);
  ident();

  endlabel = genlabel();
  nameslot = addglob(Text, type, S_FUNCTION, endlabel);
  Functionid = nameslot;

  lparen();
  rparen();

  col();

  tree = compound_statement();

  if (type != P_VOID)
  {
    finalstmt = (tree->op == A_GLUE) ? tree->right : tree;
    if (finalstmt == NULL || finalstmt->op != A_RETURN)
      fatal("All of non-void functions must have a return statement");
  }

  return (mkastunary(A_FUNCTION, type, tree, nameslot));
}
