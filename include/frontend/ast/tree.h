#ifndef TREE_H
#define TREE_H
#include "../lexer/token.h"
#include "../lexer/token_printer.h"

/****************   TYPES   ***************************/
typedef struct type {
   enum { primary_type, pointer_type, } kind;
   union {
      struct { enum {
                 IntType,
                 BooleanType,
                 StringType,
                 FloatType,
                 VoidType,
               } primary_type_kind;
               Token           primary_type_value;
      } primary_type;
               struct type*    pointer_type_value;
   } val;

} TYPE_node;

/****************   EXPRESSIONS   ***************************/
typedef struct Exp {
  enum { binary_exp, unary_exp, primary_exp, } kind;
  union { 
          struct { enum {
                     logical_exp,
                     equality_exp,
                     relational_exp,
                     additive_exp,
                     multiplicative_exp,
                     assign_exp,
                  } binary_exp_kind;
                   Token            operator;
                   struct Exp*      left;
                   struct Exp*      right;
          } binaryExp;

          struct { enum {
                     negation_exp,
                     inverse_exp,
                     reference_exp,    // <-- NOT IMPLEMENTED
                     dereference_exp,  // <-- NOT IMPLEMENTED
                  } unary_exp_kind;
                   Token            operator;
                   struct Exp*      operand;
          } unaryExp;

          struct { enum {
                     variable_exp,
                     string_exp,
                     integer_exp,
                     float_exp,
                     boolean_exp,
                     call_exp,
                  } primary_exp_kind;
                  union {
                     Token                                                 constantExp;
                     struct { Token name; struct EXP_list* arguments; }    callExp;
                  } primary_exp_val;
                   
          } primaryExp;

      } val;

} EXP_node;

/****************   EXPRESSIONS LIST   ***************************/
typedef struct EXP_list { 
  EXP_node*         elem;
  struct EXP_list*  next;
} EXP_node_list;

/****************   STATEMENTS    ***************************/
typedef struct Stmt {
  enum { var_decl, 
         input, output,
         loop_for, loop_for_in, loop_while, loop_do_while,
         statement_if,
         continue_stmt, break_stmt, return_stmt,
         expr_stmt,
  } kind;

  union { 
    struct {
      TYPE_node* type;
      Token name;
      EXP_node* decl_val;
    } varDecl;
    
    struct {
      Token* idents;
    } inputStmt;
    
    struct {
      EXP_node_list* expressions;
    } outputStmt;
    
    struct {
      EXP_node* init;
      EXP_node* cond;
      EXP_node* inc;
      struct STMTS_list* body;
    } loopFor;
    
    struct {
        Token name;
        EXP_node_list* expressions;
        struct STMTS_list* body;
    } loopForIn;
    
    struct {
      EXP_node* cond;
      struct STMTS_list* body;
    } loopWhile;
    
    struct {
      EXP_node* cond;
      struct STMTS_list* body;
    } loopDoWhile;
    
    struct {
      struct branch_list* branches;
      struct STMTS_list* else_body;
    } stmtIf;

    struct {
      Token lineno;
    } stmtBreak;

    struct {
      Token lineno;
    } stmtContinue;
    
    struct {
      EXP_node* expression;
    } stmtReturn;

    struct {
       EXP_node* expression;
    } exprStmt;
    
  } val;
  
} STMT_node;

/****************   STATEMENTS LIST    ***************************/
typedef struct STMTS_list { 
  STMT_node*            elem;
  struct STMTS_list*    next;
} STMT_node_list;

/****************   FUNCTION DECL   ***************************/
typedef struct fndecl {
  Token func_name;
  struct PARAMS_list* params;
  TYPE_node* ret_type;
  struct STMTS_list* body;
  int start_label;
} FNDECL_node;

/****************   FUNCTIONS LIST   ***************************/
typedef struct FUNCS_list { 
  struct fndecl*        elem;
  struct FUNCS_list*    next;
} FUNC_node_list;

/****************   PARAM   ***************************/
typedef struct param {
  Token name;
  TYPE_node* type;
} PARAM_node;

/****************   PARAMS LIST   ***************************/
typedef struct PARAMS_list { 
  struct param*           elem;
  struct PARAMS_list*     next;
} PARAM_node_list;

/****************   BRANCH  ***************************/
typedef struct branch {
   EXP_node* cond;
   STMT_node_list* body;
} BRANCH_node;

/****************   BRANCH LIST   ***************************/
typedef struct branch_list { 
  struct branch*           elem;
  struct branch_list*     next;
} BRANCH_node_list;

/****************   PROGRAM   ***************************/

typedef struct program {
  struct FUNCS_list* functions;
} PROGRAM_node;

/******************** DEFINITIONS **********************/
FUNC_node_list* add_func_to_list(FNDECL_node*);
FNDECL_node* make_func(Token, PARAM_node_list*, TYPE_node*, STMT_node_list*);
PARAM_node_list* add_param_to_list(PARAM_node*);
PARAM_node* make_param(Token, TYPE_node*);
STMT_node_list* add_stmt_to_list(STMT_node*);
STMT_node* make_if_stmt(BRANCH_node_list*, STMT_node_list*);
STMT_node* make_for_stmt(EXP_node*, EXP_node*, EXP_node*, STMT_node_list*);
BRANCH_node* make_branch(EXP_node*, STMT_node_list*);
BRANCH_node_list* add_branch_to_list(BRANCH_node*);
STMT_node* make_for_in_stmt(Token, EXP_node_list*, STMT_node_list*);
STMT_node* make_while_stmt(EXP_node*, STMT_node_list*);
STMT_node* make_do_while_stmt(EXP_node*, STMT_node_list*);
STMT_node* make_return_stmt(EXP_node*);
STMT_node* make_break_stmt(Token);
STMT_node* make_continue_stmt(Token);
STMT_node* make_var_decl_stmt(TYPE_node*, Token, EXP_node*);
STMT_node* make_var_assign_stmt(Token, Token, EXP_node*);
STMT_node* make_input_stmt(Token*);
STMT_node* make_output_stmt(EXP_node_list*);
STMT_node* make_expr_stmt(EXP_node*);
EXP_node_list* add_expr_to_list(EXP_node*);
EXP_node* make_binary_exp(int, Token, EXP_node*, EXP_node*);
EXP_node* make_unary_exp(int, Token, EXP_node*);
EXP_node* make_primary_const(Token, int);
EXP_node* make_primary_call(Token, EXP_node_list*);
TYPE_node* make_primary_type(int, Token);
TYPE_node* make_pointer_type(TYPE_node*);
PROGRAM_node* make_program_node(FUNC_node_list*);

#endif /* TREE_H */
