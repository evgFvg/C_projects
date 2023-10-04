#include <stdio.h>
#include <assert.h>

#include "calc.h"


typedef struct test
{
	char str[50];
	double result;
} test_t;

typedef struct calc_error
{
    char str[70];
    calc_status_t c_status;
} error_status;

static void TestCalc(void);
static void TestErrors(void);
static void MentorTest(void);
static void TestExpressionExceeded(void);

int main()
{
	TestCalc();
	TestErrors();
	TestExpressionExceeded();
	MentorTest();
	
	printf("All tests have been passed\n");
	
	return 0;
}

static void TestCalc()
{
	calc_status_t status = 0;
	calc_t *new_c = NULL;
	double res = 0;
	size_t i = 0;
	size_t arr_size = 0;
	
	test_t test_arr[] =
		{
			{"3 ^ 2 + 4 ^ 2", 25},
			{"1 + 1", 2},
			{"2 * 3", 6},
			{"7 - 10", -3},
			{"1 / 4", 0.25},
			{"2 ^ 3", 8},
			{"2 + 5 +  3", 10},
			{"2 * 3 * 4", 24},
			{"-1 + 10", 9},
			{"+4 + 3", 7},
			{"1 + 3 - 7", -3},
			{"2 * 3 + 1", 7},
			{"1 + 2 * 3", 7},
			{"-1 + 8", 7},
			{"-1 - -8", 7},
			{"+9 - -8", 17},
			{"2 * (3 + 4)", 14},
			{"(3 + 4)*  2", 14},
			{"1 / 2 + 2", 2.5},
			{"2 + 1 / 2", 2.5},
			{"3*(2+1)", 9},
			{"10 - -5", 15},
			{"-5 + 10 + 15", 20},
			{"3 ^ 2", 9},
			{"9 / 3", 3},
			{"(5 + 3) * 2", 16},
			{"2 + 3 * 4", 14},
			{"(2 + 3) * 4", 20},
			{"2 ^ 0", 1},
			{"(2 + 3) ^ 2", 25},
			{"1 + 2 * 3 + 4 * 5 + 6", 33},
			{"(1 + 2) * (3 + 4)", 21},
			{"(1 + 2) * 3 + 4", 13},
			{"1 * 2 + 3 * 4", 14},
			{"3 ^ 2 + 4 ^ 2", 25},
			{"2 ^ 2 + 3 ^ 2 + 4 ^ 2", 29},
			{"(1 + 2) * 3", 9},
			{"(2 * 3) + 4", 10},
			{"(5 - 2) * (4 + 1)", 15},
			{"(10 - 5) / 5", 1},
			{"(3 + 5) / (2 * 2)", 2},
			{"(2 * 3) ^ 2", 36},
			{"(3 + 4) ^ 2", 49},
			{"(4 - 2) ^ 3", 8},
			{"2 * (3 + 4)", 14},
			{"4 / (2 + 2)", 1},
			{"(2 ^ 3) / 4", 2},
			{"3 * (4 + 5) / 2", 13.5},
			{"(3 + 4 * 5) / (2 - 3)", -23},
			{"(2 * 3 + 4) * 5", 50},
			{"(2 + 3) * (4 + 5)", 45},
			{"(1 + 2 + 3 + 4) * 5", 50},
			{"5 * (1 + 2 + 3 + 4)", 50},
			{"(2 + 3) * 4 - 5", 15},
			{"2 + (3 * 4 - 5)", 9},
			{"(1 + 2) ^ (3 - 1)", 9},
			{"2 ^ (1 + 3)", 16},
			{"(3 - 1) ^ (1 + 2)", 8},
			{"2 + 3 * 4 ^ 2", 50},
			{"(2 + 3) * 4 ^ 2", 80},
			{"(2 ^ 3) * (4 / 2)", 16},
			{"2 ^ (3 * 4) / 4", 1024},
			{"(2 * 3) + (4 * 5) - (6 / 3)", 24},
			{"(1 + 2) * (3 + 4) * (5 + 6)", 231},
			{"(1 + 1) ^ (0 + 2) ^ 3", 256},
			{"2 + (3 * 4) ^ 2 + 5", 151},
			{"(2 + 3 * 4) ^ 2 + 5", 201},
			{"(2 ^ 3) * (4 / 2) + 5", 21},
			{"2 ^ (3 * 4) / 4 - 5", 1019},
			{"(2 * 3) + (4 * 5) - (6 / 3) ^ 2", 22},
			{"(1 + 2) * (3 + 4) * (5 + 6) / 7", 33},
			{"3 ^ 2 ^ 2 * 3", 243},
			{"(((1 + 1)))", 2},
			{"  (  (((( 2 + 2 ) * 2)  ) ))", 8},
			{"-(1 + 2)", -3},
			{"+(1 + 2)", 3},
			{"((+3 - -2) * 4 - 2) / (5 - 1)", 4.5},
			{"(2 + (3 * 4)) / ((-5 + +9) * 2)", 1.75},
			{"((2 + 3) * (4 - 1)) / (5 - (2 + 1))", 7.5},
			{"((10 / 2) - 3) * ((+6 - 2)^2)", 32},
			{"((2 * 3) + 4) / ((5 - 1) * (2 - 1))", 2.5},
			{"((10 - (3 * 2)) / 4) + (6 - 2)^2", 17},
			{"((5 - 2) * ((3 + 4) / 2)) / (2 + 1)", 3.5}};

	arr_size = sizeof(test_arr) / sizeof(test_arr[0]);

	new_c = CalcCreate(100);

    	for (i = 0; i < arr_size; ++i)
  	{
       	    status = Calculator(new_c, test_arr[i].str, &res);
            assert(res == test_arr[i].result);
            assert(SUCCESS == status);
  	} 

    CalcDestroy(new_c);
}

static void TestErrors()
{
    calc_t *new_c = CalcCreate(70);
    size_t i = 0;
    double res = 0;
    calc_status_t status = SUCCESS;
    size_t arr_size = 0;
    error_status test_arr[] = {

        {"5/0", DIVISION_BY_ZERO},
        {"5/(8-8)", DIVISION_BY_ZERO},
        {"5a8", SYNTAX_ERROR},
        {"5+ 8 - -9 / (45 # 56)", SYNTAX_ERROR},
        {")54-7", SYNTAX_ERROR},
        {"(5+6))", UNBALANCED_PARANTHESIS},
        {"((5+6 +   (56/7)", UNBALANCED_PARANTHESIS},
        {"100^100^100", RANGE_EXCEEDED},
        {"1 +++ 1",  SYNTAX_ERROR},
	{"1++ 1", SYNTAX_ERROR},
	{"1 ++ 1", SYNTAX_ERROR}};

    arr_size = sizeof(test_arr) / sizeof(test_arr[0]);

    for (i = 0; i < arr_size; ++i)
    {
        status = Calculator(new_c, test_arr[i].str, &res);
        assert(status == test_arr[i].c_status);
    }
    
    CalcDestroy(new_c);
}

static void TestExpressionExceeded(void)
{
    calc_t *new_c = CalcCreate(2);
    double res = 0;

    assert(EXPRESSION_EXCEEDED == Calculator(new_c, "(2 * 3) + (4 * 5) - (6 / 3)", &res));
    assert(SUCCESS == Calculator(new_c, "(2 * 3) + 3", &res));
    assert(EXPRESSION_EXCEEDED == Calculator(new_c, "(2 * 3) + 3 / 2", &res));

    CalcDestroy(new_c);
}


static void MentorTest()
{
    double res = 0;
    calc_t *calc = NULL;
    char str[100] = "8 + 7";
    char str2[100] = "8++8*3+-2^5";
    char str3[100] = "8+8*3-2^";
    char str4[100] = "2/0";
    char str5[100] = "8++8*((3-2)*5)";
    char str6[100] = "3-2)*5";
    char str7[100] = "(3-2)*5+ 5*(4+4+4";

    calc = CalcCreate(30);
    
    assert(SUCCESS == Calculator(calc, str, &res));
    assert(15 == res);

    assert(SUCCESS == Calculator(calc, str2,&res));
    assert(0 == res);

    assert(SYNTAX_ERROR == Calculator(calc, str3,&res));
    assert(0 == res);

    assert(DIVISION_BY_ZERO == Calculator(calc, str4,&res));
    assert(0 == res);
    
    assert(SUCCESS == Calculator(calc, str5,&res));
    assert(48 == res);
    
    assert(UNBALANCED_PARANTHESIS  == Calculator(calc, str6,&res));
    assert(0 == res);

    assert(UNBALANCED_PARANTHESIS  == Calculator(calc, str7,&res));
    assert(0 == res);

    printf("mentor test - SUCCESS\n");
    CalcDestroy(calc);
    
}



