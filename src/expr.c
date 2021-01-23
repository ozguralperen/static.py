#include "defs.h"
#include "data.h"
#include "decl.h"

struct ASTnode *funccall(void)
{
  struct ASTnode *tree;
  int id;

  if ((id = findglob(Text)) == -1)
  {
    fatals("Undeclared Function Call", Text);
  }

  lparen();

  tree = binexpr(0);

  tree = mkastunary(A_FUNCCALL, Gsym[id].type, tree, id);

  rparen();
  return (tree);
}

static struct ASTnode *primary(void)
{
  struct ASTnode *n;
  int id;

  switch (Token.token)
  {
  case T_INTLIT:

    if ((Token.intvalue) >= 0 && (Token.intvalue < 256))
      n = mkastleaf(A_INTLIT, P_CHAR, Token.intvalue);
    else
      n = mkastleaf(A_INTLIT, P_INT, Token.intvalue);
    break;

  case T_IDENT:

    scan(&Token);

    if (Token.token == T_LPAREN)
      return (funccall());

    reject_token(&Token);

    id = findglob(Text);
    if (id == -1)
      fatals("Call Unknown Variable", Text);

    n = mkastleaf(A_IDENT, Gsym[id].type, id);
    break;

  default:
    fatald("Syntax Error, Token", Token.token);
  }

  scan(&Token);
  return (n);
}

static int arithop(int tokentype)
{
  if (tokentype > T_EOF && tokentype < T_INTLIT)
    return (tokentype);
  fatald("Syntax Error , Token", tokentype);
}

static int OpPrec[] = {
    0, 10, 10,
    20, 20,
    30, 30,
    40, 40, 40, 40};

static int op_precedence(int tokentype)
{
  int prec = OpPrec[tokentype];
  if (prec == 0)
    fatald("Syntax Error , Token", tokentype);
  return (prec);
}

struct ASTnode *binexpr(int ptp)
{
  struct ASTnode *left, *right;
  int lefttype, righttype;
  int tokentype;

  left = primary();

  tokentype = Token.token;
  if (tokentype == T_SEMI || tokentype == T_RPAREN)
    return (left);

  while (op_precedence(tokentype) > ptp)
  {

    scan(&Token);

    right = binexpr(OpPrec[tokentype]);

    lefttype = left->type;
    righttype = right->type;
    if (!type_compatible(&lefttype, &righttype, 0))
      fatal("Operation in Incompatible Types");

    if (lefttype)
      left = mkastunary(lefttype, right->type, left, 0);
    if (righttype)
      right = mkastunary(righttype, left->type, right, 0);

    left = mkastnode(arithop(tokentype), left->type, left, NULL, right, 0);

    tokentype = Token.token;
    if (tokentype == T_SEMI || tokentype == T_RPAREN)
      return (left);
  }

  return (left);
}
