#include <stddef.h>    /* size_t */
#include <stdlib.h>    /* malloc */
#include <stdio.h>     /* perror */
#include <string.h>    /* memcpy */
#include <assert.h>    /*assert  */
#include <ctype.h>     /* isdigit() */
#include <sys/types.h> /* ssize_t */
#include <math.h>      /* pow */

#include "calc.h"
#include "stack.h"

/***************************************** TYPEDEFS && ENUMS && MACROS ********************************************************/

#define NUM_OF_CHARS_IN_ASCII (256)
#define IS_INF(x) __builtin_isinf_sign(x)
#define LAST_PREC_INDEX (95)
#define LAST_OPERATOR_INDEX (95)
#define UNARY_SIZE (46)

typedef enum stack_type
{
    OPERAND = 0,
    OPERATOR = 1,
    NUM_OF_STACKS = 2
} stack_type_t;

typedef enum operator
{
    MULT = '*',
    DIV = '/',
    SUM = '+',
    SUB = '-',
    POW = '^',
    OPEN_PAR = '(',
    CLOSE_PAR = ')',
    SPACE = ' ',
    NUM_OF_OPERATOR = LAST_PREC_INDEX
} operator_t;

typedef enum state
{
    ERROR = -1,
    INIT,
    VALID,
    WAIT_FOR_OPERAND,
    END_OF_STRING,
    NUM_OF_STATES = 5
} state_t;

typedef double (*operate_t)(double x, double y);
typedef calc_status_t (*f_ptr)(calc_t *calc, char **str, state_t *state);

/***************************************** STRUCTS ********************************************************/

struct calculator
{
    stack_t *stack[NUM_OF_STACKS];
    f_ptr state_lut[NUM_OF_STATES][NUM_OF_CHARS_IN_ASCII];
    operate_t operator_lut[LAST_OPERATOR_INDEX];
    f_ptr prec_lut[NUM_OF_OPERATOR][NUM_OF_OPERATOR];
    size_t num_of_parentheses;
    f_ptr parentheses_lut[2];
    f_ptr unary_lut[UNARY_SIZE];
};

/***************************************** STATIC DECLARATIONS ********************************************************/

static void StateLUTInit(calc_t *calc);
static void OperatorLUTInit(calc_t *calc);
static void PrecendenceLutInit(calc_t *calc);
static void ParenthesesLutInit(calc_t *calc);
static void UnariLutInit(calc_t *calc);

static calc_status_t GetErrorState(calc_t *calc, char **str, state_t *state);
static calc_status_t PushOperandToOutputStack(calc_t *calc, char **str, state_t *state);
static calc_status_t PushOperatorToOperatorStack(calc_t *calc, char **str, state_t *state);
static calc_status_t SpaceHandle(calc_t *calc, char **str, state_t *state);
static calc_status_t UnaryPlusHandle(calc_t *calc, char **str, state_t *state);
static calc_status_t UnaryMinusHandle(calc_t *calc, char **str, state_t *state);
static calc_status_t GetEndOfString(calc_t *calc, char **str, state_t *state);
static calc_status_t PopParentheses(calc_t *calc, char **str, state_t *state);
static calc_status_t PopCalculatePush(calc_t *calc, char **str, state_t *state);
static calc_status_t PushOperator(calc_t *calc, char **str, state_t *state);
static calc_status_t ParenthesesCheck(calc_t *calc, char **str, state_t *state);
static calc_status_t PushOpenParentheses(calc_t *calc, char **str, state_t *state);
static calc_status_t PopParentheses(calc_t *calc, char **str, state_t *state);
static calc_status_t CallParenthesesCheck(calc_t *calc, char **str, state_t *state);

static double AddFunc(double x, double y);
static double SubFunc(double x, double y);
static double MultFunc(double x, double y);
static double DivFunc(double x, double y);
static double PowFunc(double x, double y);

/***************************************** PRIMARY FUNCTIONS ********************************************************/

calc_t *CalcCreate(size_t max_exp)
{
    calc_t *calc = (calc_t *)malloc(sizeof(calc_t));
    if (NULL == calc)
    {
        return NULL;
    }

    calc->stack[OPERAND] = StackCreate(max_exp, sizeof(double));

    if (NULL == calc->stack[OPERAND])
    {
        free(calc);
        return NULL;
    }

    calc->stack[OPERATOR] = StackCreate(max_exp, sizeof(char));

    if (NULL == calc->stack[OPERAND])
    {
        StackDestroy(calc->stack[OPERAND]);
        free(calc);
        return NULL;
    }

    StateLUTInit(calc);
    OperatorLUTInit(calc);
    PrecendenceLutInit(calc);
    calc->num_of_parentheses = 0;
    ParenthesesLutInit(calc);
    UnariLutInit(calc);

    return calc;
}

void CalcDestroy(calc_t *calc)
{
    size_t i = 0;

    assert(NULL != calc);

    for (i = 0; i < NUM_OF_STACKS; ++i)
    {
        StackDestroy(calc->stack[i]);
    }

    free(calc);
}

calc_status_t Calculator(calc_t *calc, const char *expression, double *result)
{
    state_t curr_state = INIT;
    calc_status_t c_status = SUCCESS;
    char *str = NULL;
    *result = 0;

    assert(NULL != calc);
    assert(NULL != expression);
    assert(NULL != result);

    str = (char *)expression;

    while (curr_state != END_OF_STRING && curr_state != ERROR && SUCCESS == c_status)
    {
        c_status = calc->state_lut[curr_state][(unsigned char)*str](calc, &str, &curr_state);
    }

    if (SUCCESS == c_status)
    {
        *result = *(double *)StackPeek(calc->stack[OPERAND]);
    }
    
    calc->num_of_parentheses = 0;
    
    while (0 == StackIsEmpty(calc->stack[OPERATOR]))
    {
        StackPop(calc->stack[OPERATOR]);
    }

    while (0 == StackIsEmpty(calc->stack[OPERAND]))
    {
        StackPop(calc->stack[OPERAND]);
    }

    return c_status;
}

/********************************************** STATIC FUNCTIONS ***********************************************************/

static void OperatorLUTInit(calc_t *calc)
{
    assert(NULL != calc);
    
    calc->operator_lut[SUM] = AddFunc;
    calc->operator_lut[SUB] = SubFunc;
    calc->operator_lut[MULT] = MultFunc;
    calc->operator_lut[DIV] = DivFunc;
    calc->operator_lut[POW] = PowFunc;
}

static void PrecendenceLutInit(calc_t *calc)
{
    size_t i = 0;
    size_t j = 0;
    
    assert(NULL != calc);

    for (i = 0; i < NUM_OF_OPERATOR; ++i)
    {
        for (j = 0; j < NUM_OF_OPERATOR; ++j)
        {
            calc->prec_lut[i][j] = PushOperator;
        }
    }
    calc->prec_lut['+']['-'] = PopCalculatePush;
    calc->prec_lut['+']['+'] = PopCalculatePush;

    calc->prec_lut['-']['+'] = PopCalculatePush;
    calc->prec_lut['-']['-'] = PopCalculatePush;

    calc->prec_lut['*']['-'] = PopCalculatePush;
    calc->prec_lut['*']['+'] = PopCalculatePush;
    calc->prec_lut['*']['*'] = PopCalculatePush;
    calc->prec_lut['*']['/'] = PopCalculatePush;

    calc->prec_lut['/']['-'] = PopCalculatePush;
    calc->prec_lut['/']['+'] = PopCalculatePush;
    calc->prec_lut['/']['*'] = PopCalculatePush;
    calc->prec_lut['/']['/'] = PopCalculatePush;

    calc->prec_lut['^']['-'] = PopCalculatePush;
    calc->prec_lut['^']['+'] = PopCalculatePush;
    calc->prec_lut['^']['*'] = PopCalculatePush;
    calc->prec_lut['^']['/'] = PopCalculatePush;

    calc->prec_lut['+'][')'] = ParenthesesCheck;
    calc->prec_lut['-'][')'] = ParenthesesCheck;
    calc->prec_lut['*'][')'] = ParenthesesCheck;
    calc->prec_lut['/'][')'] = ParenthesesCheck;
    calc->prec_lut['^'][')'] = ParenthesesCheck;
    calc->prec_lut['('][')'] = ParenthesesCheck;
}

static void StateLUTInit(calc_t *calc)
{
    size_t i = 0;
    size_t j = 0;
    
    assert(NULL != calc);

    for (i = 0; i < NUM_OF_STATES; i++)
    {
        for (j = 0; j < NUM_OF_CHARS_IN_ASCII; ++j)
        {
            calc->state_lut[i][j] = GetErrorState;
        }
    }

    /* INIT */
    for (i = '0'; i <= '9'; ++i)
    {
        calc->state_lut[INIT][i] = PushOperandToOutputStack;
    }
    calc->state_lut[INIT]['('] = PushOpenParentheses;
    calc->state_lut[INIT][' '] = SpaceHandle;
    calc->state_lut[INIT]['+'] = UnaryPlusHandle;
    calc->state_lut[INIT]['-'] = UnaryMinusHandle;

    /* VALID */
    calc->state_lut[VALID]['+'] = PushOperatorToOperatorStack;
    calc->state_lut[VALID]['-'] = PushOperatorToOperatorStack;
    calc->state_lut[VALID]['/'] = PushOperatorToOperatorStack;
    calc->state_lut[VALID]['*'] = PushOperatorToOperatorStack;
    calc->state_lut[VALID]['^'] = PushOperatorToOperatorStack;
    calc->state_lut[VALID]['('] = GetErrorState;
    calc->state_lut[VALID][')'] = CallParenthesesCheck;
    calc->state_lut[VALID]['\0'] = GetEndOfString;
    calc->state_lut[VALID][' '] = SpaceHandle;

    /* WAIT_FOR_OPERAND */
    for (i = '0'; i <= '9'; ++i)
    {
        calc->state_lut[WAIT_FOR_OPERAND][i] = PushOperandToOutputStack;
    }

    calc->state_lut[WAIT_FOR_OPERAND]['('] = PushOpenParentheses;
    calc->state_lut[WAIT_FOR_OPERAND][')'] = GetErrorState;
    calc->state_lut[WAIT_FOR_OPERAND][' '] = SpaceHandle;
    calc->state_lut[WAIT_FOR_OPERAND]['+'] = UnaryPlusHandle;
    calc->state_lut[WAIT_FOR_OPERAND]['-'] = UnaryMinusHandle;

    /* END_OF_STRING */
    for (i = 0; i < NUM_OF_CHARS_IN_ASCII; ++i)
    {
        calc->state_lut[END_OF_STRING][i] = GetEndOfString;
    }
}

static void ParenthesesLutInit(calc_t *calc)
{
    assert(NULL != calc);

    calc->parentheses_lut[0] = GetErrorState;
    calc->parentheses_lut[1] = PopParentheses;
}

static void UnariLutInit(calc_t *calc)
{
    assert(NULL != calc);

    calc->unary_lut['+'] = UnaryPlusHandle;
    calc->unary_lut['-'] = UnaryMinusHandle;
}

/************************** State Functions ************************************/

static calc_status_t GetErrorState(calc_t *calc, char **str, state_t *state)
{
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);
    
    calc = (void *)calc;
    str = (void *)str;
    *state = ERROR;

    return SYNTAX_ERROR;
}

static calc_status_t SpaceHandle(calc_t *calc, char **str, state_t *state)
{
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);
    
    calc = (void *)calc;
    str = (void *)str;
    state = (void *)state;
    ++(*str);

    return SUCCESS;
}

static calc_status_t UnaryPlusHandle(calc_t *calc, char **str, state_t *state)
{
    char next_symbol = 0;
    
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);

    (void)calc;
    (void)str;

    next_symbol = *(*str + 1);

    if ('+' == next_symbol || '-' == next_symbol || ' ' == next_symbol)
    {
        *state = ERROR;
        return SYNTAX_ERROR;
    }

    *state = WAIT_FOR_OPERAND;

    if ((StackSize(calc->stack[OPERAND])) == StackCapacity(calc->stack[OPERAND]) ||
        (StackSize(calc->stack[OPERATOR])) == StackCapacity(calc->stack[OPERATOR]))
    {
        return EXPRESSION_EXCEEDED;
    }

    ++(*str);

    return SUCCESS;
}

static calc_status_t UnaryMinusHandle(calc_t *calc, char **str, state_t *state)
{
    double zero = 0;
    char minus = '-';
    char next_symbol = 0;

    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);

    (void)calc;
    (void)str;

    next_symbol = *(*str + 1);

    if ('+' == next_symbol || '-' == next_symbol || ' ' == next_symbol)
    {
        *state = ERROR;
        return SYNTAX_ERROR;
    }

    *state = WAIT_FOR_OPERAND;

    if ((StackSize(calc->stack[OPERAND])) == StackCapacity(calc->stack[OPERAND]) ||
        (StackSize(calc->stack[OPERATOR])) == StackCapacity(calc->stack[OPERATOR]))
    {
        return EXPRESSION_EXCEEDED;
    }

    StackPush(calc->stack[OPERAND], &zero);
    StackPush(calc->stack[OPERATOR], &minus);
    ++(*str);

    return SUCCESS;
}

static calc_status_t PushOperandToOutputStack(calc_t *calc, char **str, state_t *state)
{
    double digit = 0;
    
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);
    
    while (isdigit(**str))
    {
        digit *= 10;
        digit += (double)(**str - '0');
        ++(*str);
    }

    if (StackSize(calc->stack[OPERAND]) == StackCapacity(calc->stack[OPERAND]))
    {
        return EXPRESSION_EXCEEDED;
    }

    StackPush(calc->stack[OPERAND], &digit);
    *state = VALID;

    return SUCCESS;
}

static calc_status_t PushOperatorToOperatorStack(calc_t *calc, char **str, state_t *state)
{
    char stack_operator = 0;
    calc_status_t c_status = SUCCESS;

    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);

    if (1 != StackIsEmpty(calc->stack[OPERATOR]))
    {
        stack_operator = *(char *)StackPeek(calc->stack[OPERATOR]);

        while (calc->prec_lut[(int)stack_operator][(int)**str] == PopCalculatePush && SUCCESS == c_status)
        {
            c_status = calc->prec_lut[(int)stack_operator][(int)**str](calc, str, state);

            if (1 == StackIsEmpty(calc->stack[OPERATOR]))
            {
                break;
            }
            else
            {
                stack_operator = *(char *)StackPeek(calc->stack[OPERATOR]);
            }
        }
    }

    c_status = PushOperator(calc, str, state);

    return c_status;
}

static calc_status_t PushOpenParentheses(calc_t *calc, char **str, state_t *state)
{
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);
    
    ++(calc->num_of_parentheses);
    PushOperator(calc, str, state);

    return SUCCESS;
}

static calc_status_t GetEndOfString(calc_t *calc, char **str, state_t *state)
{
    calc_status_t c_status = SUCCESS;
    
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);

    while (0 == StackIsEmpty(calc->stack[OPERATOR]) && SUCCESS == c_status)
    {
        c_status = PopCalculatePush(calc, str, state);
    }

    *state = END_OF_STRING;

    return c_status;
}

static calc_status_t PopParentheses(calc_t *calc, char **str, state_t *state)
{
    calc_status_t c_status = SUCCESS;
    
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);

    while ('(' != *(char *)StackPeek(calc->stack[OPERATOR]) && SUCCESS == c_status)
    {
        c_status = PopCalculatePush(calc, str, state);
    }

    StackPop(calc->stack[OPERATOR]);
    --(calc->num_of_parentheses);
    ++(*str);
    *state = VALID;

    return c_status;
}

static calc_status_t CallParenthesesCheck(calc_t *calc, char **str, state_t *state)
{
    return ParenthesesCheck(calc, str, state);
}

static calc_status_t PushOperator(calc_t *calc, char **str, state_t *state)
{
    calc = (void *)calc;
    
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);

    if ((StackSize(calc->stack[OPERATOR]) + 1) > StackCapacity(calc->stack[OPERATOR]))
    {
        return EXPRESSION_EXCEEDED;
    }

    StackPush(calc->stack[OPERATOR], *str);
    *state = WAIT_FOR_OPERAND;
    ++(*str);

    return SUCCESS;
}

static calc_status_t PopCalculatePush(calc_t *calc, char **str, state_t *state)
{
    double x = 0;
    double y = 0;
    double res = 0;
    char stack_operator = 0;
    
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);
    
    state = (void *)state;
    str = (void *)str;

    stack_operator = *(char *)StackPeek(calc->stack[OPERATOR]);
    StackPop(calc->stack[OPERATOR]);

    if ('(' == stack_operator)
    {
        return UNBALANCED_PARANTHESIS;
    }

    y = *(double *)StackPeek(calc->stack[OPERAND]);
    StackPop(calc->stack[OPERAND]);
    x = *(double *)StackPeek(calc->stack[OPERAND]);
    StackPop(calc->stack[OPERAND]);

    if ('/' == stack_operator && 0 == y)
    {
        return DIVISION_BY_ZERO;
    }

    res = calc->operator_lut[(int)stack_operator](x, y);

    if (0 != IS_INF(res))
    {
        return RANGE_EXCEEDED;
    }

    StackPush(calc->stack[OPERAND], &res);

    return SUCCESS;
}

static calc_status_t ParenthesesCheck(calc_t *calc, char **str, state_t *state)
{
    assert(NULL != calc);
    assert(NULL != str);
    assert(NULL != state);
    
    if (0 == calc->num_of_parentheses && ')' == **str)
    {
        return UNBALANCED_PARANTHESIS;
    }

    calc->parentheses_lut[!!((int)calc->num_of_parentheses)](calc, str, state);

    return SUCCESS;
}

/************************** Operate Functions ************************************/

static double AddFunc(double x, double y)
{
    return x + y;
}

static double SubFunc(double x, double y)
{
    return x - y;
}

static double MultFunc(double x, double y)
{
    return x * y;
}

static double DivFunc(double x, double y)
{
    return x / y;
}

static double PowFunc(double x, double y)
{
    return pow(x, y);
}
