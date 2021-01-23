#include "defs.h"
#include "data.h"
#include "decl.h"

static int chrpos(char *s, int c)
{
  char *p;

  p = strchr(s, c);
  return (p ? p - s : -1);
}

static int next(void)
{
  int c;

  if (Putback)
  {
    c = Putback;
    Putback = 0;
    return (c);
  }

  c = fgetc(Infile);
  Pos++;

  if ('\n' == c)
  {
    Line++;
    Pos = 0;
  }
  return (c);
}

static void putback(int c)
{
  Putback = c;
}

int ind = 0;
static int skip(void)
{
  int c;

  c = next();
  if(c == '\n' && ind == 0)
    c = next();
  
  while (' ' == c || '\r' == c || '\f' == c)
  {
    c = next();
  }
  return (c);
}

static int scanint(int c)
{
  int k, val = 0;

  while ((k = chrpos("0123456789", c)) >= 0)
  {
    val = val * 10 + k;
    c = next();
  }

  putback(c);
  return (val);
}

static int scanident(int c, char *buf, int lim)
{
  int i = 0;

  while (isalpha(c) || isdigit(c) || '_' == c)
  {

    if (lim - 1 == i)
    {
      fatal("Too Long Identifier");
    }
    else if (i < lim - 1)
    {
      buf[i++] = c;
    }
    c = next();
  }

  putback(c);
  buf[i] = '\0';
  return (i);
}

static int keyword(char *s)
{
  switch (*s)
  {
  case 'c':
    if (!strcmp(s, "char"))
      return (T_CHAR);
    break;
  case 'e':
    if (!strcmp(s, "else"))
      return (T_ELSE);
    break;
  case 'f':
    if (!strcmp(s, "for"))
      return (T_FOR);
    break;
  case 'i':
    if (!strcmp(s, "if"))
      return (T_IF);
    if (!strcmp(s, "int"))
      return (T_INT);
    break;
  case 'l':
    if (!strcmp(s, "long"))
      return (T_LONG);
    break;
  case 'p':
    if (!strcmp(s, "print"))
      return (T_PRINT);
    break;
  case 'r':
    if (!strcmp(s, "return"))
      return (T_RETURN);
    break;
  case 'w':
    if (!strcmp(s, "while"))
      return (T_WHILE);
    break;
  case 'v':
    if (!strcmp(s, "void"))
      return (T_VOID);
    break;
  }
  return (0);
}

static struct token *Rejtoken = NULL;

void reject_token(struct token *t)
{
  if (Rejtoken != NULL)
    fatal("Can't reject token twice");
  Rejtoken = t;
}

int scan(struct token *t)
{
  int c, tokentype;

  if (Rejtoken != NULL)
  {
    t = Rejtoken;
    Rejtoken = NULL;
    return (1);
  }

  c = skip();

  switch (c)
  {
  case EOF:
    t->token = T_EOF;
    return (0);
  case '\t':
    t->token = T_TAB;
    break;
  case '\n':
    t->token = T_ENDL;
    break;
  case '+':
    t->token = T_PLUS;
    break;
  case '-':
    t->token = T_MINUS;
    break;
  case '*':
    t->token = T_STAR;
    break;
  case '/':
    t->token = T_SLASH;
    break;
  case ';':
    t->token = T_SEMI;
    break;
  case ':':
    t->token = T_COLON;
    break;
  case '{':
    t->token = T_LBRACE;
    break;
  case '}':
    t->token = T_RBRACE;
    break;
  case '(':
    t->token = T_LPAREN;
    break;
  case ')':
    t->token = T_RPAREN;
    break;
  case '=':
    if ((c = next()) == '=')
    {
      t->token = T_EQ;
    }
    else
    {
      putback(c);
      t->token = T_ASSIGN;
    }
    break;
  case '!':
    if ((c = next()) == '=')
    {
      t->token = T_NE;
    }
    else
    {
      fatalc("Unrecognised Character Input", c);
    }
    break;
  case '<':
    if ((c = next()) == '=')
    {
      t->token = T_LE;
    }
    else
    {
      putback(c);
      t->token = T_LT;
    }
    break;
  case '>':
    if ((c = next()) == '=')
    {
      t->token = T_GE;
    }
    else
    {
      putback(c);
      t->token = T_GT;
    }
    break;
  default:

    if (isdigit(c))
    {
      t->intvalue = scanint(c);
      t->token = T_INTLIT;
      break;
    }
    else if (isalpha(c) || '_' == c)
    {

      scanident(c, Text, TEXTLEN);

      if (tokentype = keyword(Text))
      {
        t->token = tokentype;
        break;
      }

      t->token = T_IDENT;
      break;
    }

    fatalc("Unrecognised Character Input", c);
  }

  return (1);
}
