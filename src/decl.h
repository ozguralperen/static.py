void reject_token(struct token *t);
int scan(struct token *t);

struct ASTnode *mkastnode(int op, int type,
						  struct ASTnode *left,
						  struct ASTnode *mid,
						  struct ASTnode *right, int intvalue);
struct ASTnode *mkastleaf(int op, int type, int intvalue);
struct ASTnode *mkastunary(int op, int type,
						   struct ASTnode *left, int intvalue);
void PrintAST(struct ASTnode * root , int run);

int genlabel(void);
int genAST(struct ASTnode *n, int reg, int parentASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(int id);
int genprimsize(int type);
void genreturn(int reg, int id);

void freeall_registers(void);
void cgpreamble();
void cgfuncpreamble(int id);
void cgfuncpostamble(int id);
int cgloadint(int value, int type);
int cgloadglob(int id);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgcall(int r, int id);
int cgstorglob(int r, int id);
void cgglobsym(int id);
int cgcompare_and_set(int ASTop, int r1, int r2);
int cgcompare_and_jump(int ASTop, int r1, int r2, int label);
void cglabel(int l);
void cgjump(int l);
int cgwiden(int r, int oldtype, int newtype);
int cgprimsize(int type);
void cgreturn(int reg, int id);

struct ASTnode *funccall(void);
struct ASTnode *binexpr(int ptp);

struct ASTnode *compound_statement(void);

void match(int t, char *what);
void semi(void);
void lbrace(void);
void rbrace(void);
void lparen(void);
void rparen(void);
void col(void);
void endl(void);
void tab(void);
void ident(void);
void fatal(char *s);
void fatals(char *s1, char *s2);
void fatald(char *s, int d);
void fatalc(char *s, int c);

int findglob(char *s);
int addglob(char *name, int type, int stype, int endlabel);

void var_declaration(void);
struct ASTnode *function_declaration(void);

int type_compatible(int *left, int *right, int onlyright);
