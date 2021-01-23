#include "defs.h"
#include "data.h"
#include "decl.h"

struct ASTnode *mkastnode(int op, int type,
                          struct ASTnode *left,
                          struct ASTnode *mid,
                          struct ASTnode *right, int intvalue)
{
  struct ASTnode *n;

  n = (struct ASTnode *)malloc(sizeof(struct ASTnode));
  if (n == NULL)
    fatal("There is no memory for mkastnode()");

  n->op = op;
  n->type = type;
  n->left = left;
  n->mid = mid;
  n->right = right;
  n->v.intvalue = intvalue;
  return (n);
}

struct ASTnode *mkastleaf(int op, int type, int intvalue)
{
  return (mkastnode(op, type, NULL, NULL, NULL, intvalue));
}

struct ASTnode *mkastunary(int op, int type, struct ASTnode *left,
                           int intvalue)
{
  return (mkastnode(op, type, left, NULL, NULL, intvalue));
}

void PrintAST(struct ASTnode *root, int run)
{
  if (root != NULL)
  {
    printf("--- Operation: %d , Generate :%d \n", root->op, root->type);

    for (int i = 0; i <= run + 1; i++)
      printf("---");

    if (root->left != NULL)
    {
      printf("--- Operation: %d , Generate :%d \n", root->left->op, root->left->type);
      PrintAST(root->left, run++);

      run = 0;
    }
    if (root->mid != NULL)
    {
      printf("--- Operation: %d , Generate :%d \n", root->mid->op, root->mid->type);
      PrintAST(root->mid, run++);
      run = 0;
    }
    if (root->right != NULL)
    {
      printf("--- Operation: %d , Generate :%d \n", root->right->op, root->right->type);
      PrintAST(root->right, run++);
    }
  }
}