#include "defs.h"
#include "data.h"
#include "decl.h"

static struct ASTnode *single_statement(void);

static struct ASTnode *print_statement(void)
{
  struct ASTnode *tree;
  int lefttype, righttype;
  int reg;

  match(T_PRINT, "print");

  tree = binexpr(0);

  lefttype = P_INT;
  righttype = tree->type;
  if (!type_compatible(&lefttype, &righttype, 0))
    fatal("Incompatible Types");

  if (righttype)
    tree = mkastunary(righttype, P_INT, tree, 0);

  tree = mkastunary(A_PRINT, P_NONE, tree, 0);

  return (tree);
}

static struct ASTnode *assignment_statement(void)
{
  struct ASTnode *left, *right, *tree;
  int lefttype, righttype;
  int id;

  ident();

  if (Token.token == T_LPAREN)
    return (funccall());

  if ((id = findglob(Text)) == -1)
  {
    fatals("Undeclared Variable", Text);
  }
  right = mkastleaf(A_LVIDENT, Gsym[id].type, id);

  match(T_ASSIGN, "=");

  left = binexpr(0);

  lefttype = left->type;
  righttype = right->type;
  if (!type_compatible(&lefttype, &righttype, 1))
    fatal("Incompatible Types");

  if (lefttype)
    left = mkastunary(lefttype, right->type, left, 0);

  tree = mkastnode(A_ASSIGN, P_INT, left, NULL, right, 0);

  return (tree);
}

static struct ASTnode *if_statement(void)
{
  struct ASTnode *condAST, *trueAST, *falseAST = NULL;

  match(T_IF, "if");
  lparen();

  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  col();

  trueAST = compound_statement();

  if (Token.token == T_ELSE)
  {
    scan(&Token);
    falseAST = compound_statement();
  }

  return (mkastnode(A_IF, P_NONE, condAST, trueAST, falseAST, 0));
}

static struct ASTnode *while_statement(void)
{
  struct ASTnode *condAST, *bodyAST;

  match(T_WHILE, "while");
  lparen();

  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  col();

  bodyAST = compound_statement();

  return (mkastnode(A_WHILE, P_NONE, condAST, NULL, bodyAST, 0));
}

static struct ASTnode *for_statement(void)
{
  struct ASTnode *condAST, *bodyAST;
  struct ASTnode *preopAST, *postopAST;
  struct ASTnode *tree;

  match(T_FOR, "for");
  lparen();

  preopAST = single_statement();
  semi();

  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  semi();

  postopAST = single_statement();
  rparen();

  col();

  bodyAST = compound_statement();

  tree = mkastnode(A_GLUE, P_NONE, bodyAST, NULL, postopAST, 0);

  tree = mkastnode(A_WHILE, P_NONE, condAST, NULL, tree, 0);

  return (mkastnode(A_GLUE, P_NONE, preopAST, NULL, tree, 0));
}

static struct ASTnode *return_statement(void)
{
  struct ASTnode *tree;
  int returntype, functype;

  if (Gsym[Functionid].type == P_VOID)
    fatal("Can't return from a void function");

  match(T_RETURN, "return");
  lparen();

  tree = binexpr(0);

  returntype = tree->type;
  functype = Gsym[Functionid].type;
  if (!type_compatible(&returntype, &functype, 1))
    fatal("Incompatible types");

  if (returntype)
    tree = mkastunary(returntype, functype, tree, 0);

  tree = mkastunary(A_RETURN, P_NONE, tree, 0);

  rparen();
  return (tree);
}

static struct ASTnode *single_statement(void)
{
  switch (Token.token)
  {
  case T_PRINT:
    return (print_statement());
  case T_CHAR:
  case T_INT:
  case T_LONG:
    var_declaration();
    return (NULL);
  case T_IDENT:
    return (assignment_statement());
  case T_IF:
    return (if_statement());
  case T_WHILE:
    return (while_statement());
  case T_FOR:
    return (for_statement());
  case T_RETURN:
    return (return_statement());
  default:
    fatald("Syntax error, Token", Token.token);
  }
}

struct ASTnode *compound_statement(void)
{
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  while (1)
  {

    tab();

    tree = single_statement();

    if (tree != NULL && (tree->op == A_PRINT || tree->op == A_ASSIGN ||
                         tree->op == A_RETURN || tree->op == A_FUNCCALL))
      endl();

    if (tree != NULL)
    {
      if (left == NULL)
        left = tree;
      else
        left = mkastnode(A_GLUE, P_NONE, left, NULL, tree, 0);
    }

    if (Token.token != T_TAB)
    {
      return (left);
    }
  }
}
