#include <stdio.h>
#include <cstdlib>
#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <cctype>
#include <stack>

using namespace std;

#define GLOBAL_ADDRESS 10000 // 定义全局变量起始地址,即局部变量终止地址

struct Code
{
	char opt[10];  // 操作码
	char operand1[20];  // 操作数1
	char operand2[20];  // 操作数2
};

enum Opt {
	NUL, LOADi, LOADIi, STO, ADD, SUB, MULT, DIV, BR, BRF,
	EQ, NOTEQ, GT, LES, GE, LE, AND, OR, INi, OUTi, RETURN,
	ENTER, CAL, CPY, ALLOCi, LOADf, LOADIf, INf, OUTf, ALLOCf
};


int* Memory = (int*)calloc(20000, 4);


enum MemoryType { INT, FLOAT };
vector<MemoryType> mtypes;

unordered_map<string, int> Map;
vector<Code> codes;

void initMap()
{
	Map["LOADi"] = LOADi;
	Map["LOADIi"] = LOADIi;
	Map["LOADf"] = LOADf;
	Map["LOADIf"] = LOADIf;
	Map["STO"] = STO;
	Map["ADD"] = ADD;
	Map["SUB"] = SUB;
	Map["MULT"] = MULT;
	Map["DIV"] = DIV;
	Map["BR"] = BR;
	Map["BRF"] = BRF;
	Map["EQ"] = EQ;
	Map["NOTEQ"] = NOTEQ;
	Map["GT"] = GT;
	Map["LES"] = LES;
	Map["GE"] = GE;
	Map["LE"] = LE;
	Map["AND"] = AND;
	Map["OR"] = OR;
	Map["INi"] = INi;
	Map["INf"] = INf;
	Map["OUTi"] = OUTi;
	Map["OUTf"] = OUTf;
	Map["RETURN"] = RETURN;
	Map["ENTER"] = ENTER;
	Map["CAL"] = CAL;
	Map["CPY"] = CPY;
	Map["ALLOCi"] = ALLOCi;
	Map["ALLOCf"] = ALLOCf;
}

void ReadCodes(const char* codesPath)
{
	FILE* codesFile = fopen(codesPath, "r");
	Code code;
	if (codesFile == NULL)
	{
		printf("fail to open the center file\n");
		exit(-1);
	}
	while (~fscanf(codesFile, "%s\t%s\t%s", code.opt, code.operand1, code.operand2))
		codes.push_back(code);
}

char cmp(char a, char b)
{
	switch (a)
	{
	case '#':
		return ('#' == b) ? '=' : '<';
		break;
	case '-':
	case '+':
		if ('*' == b || '/' == b || '(' == b)
			return '<';
		else
			return '>';
		break;
	case '*':
	case '/':
		return ('(' == b) ? '<' : '>';
		break;
	case '(':
		return (')' == b) ? '=' : '<';
		break;
	default:
		throw "error:unkown operator";
	}
}

int calc(int a, int b, char op)
{
	switch (op)
	{
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	case '/':
		if (0 == b)
			throw "error: the divisor shoud not be negtive.";
		else
			return a / b;
	default:
		throw "error: unknown operator.";
	}
}

bool isoptr(char c)
{
	static string optrs("+-*/()#");
	if (optrs.find(c) == string::npos)
	{
		if (isdigit(c))
			return false;
		else
			throw "error: unknown char.";
	}
	return true;
}

//求计算式e的值
int eval(string e)
{
	e += "#";
	stack<int> opnd;
	stack<char> optr;
	optr.push('#');
	int i = 0;
	int num = 0;
	while (e[i] != '#' || optr.top() != '#')
	{
		if (!isoptr(e[i]))
		{
			num = 0;
			while (!isoptr(e[i]))
			{
				num *= 10;
				num += e[i] - '0';
				++i;
			}
			opnd.push(num);
		}
		else
		{
			if (optr.empty())
				throw "error: optr is empty";
			switch (cmp(optr.top(), e[i]))
			{
			case '<':
				optr.push(e[i]);
				++i;
				break;
			case '=':
				optr.pop();
				++i;
				break;
			case '>':
				if (opnd.empty())
					throw "error: opnd is empty.";
				int a = opnd.top();
				opnd.pop();
				if (opnd.empty())
					throw "error: opnd is empty.";
				int b = opnd.top();
				opnd.pop();
				opnd.push(calc(b, a, optr.top()));
				optr.pop();
				break;
			}
		}
	}
	return opnd.top();
}

int TESTmachine()
{
	int oper1, oper2, i, t, es, top, base, ip, mtypes_size;
	float foper1;
	char result[20];
	es = top = base = ip = 0;
	Code instruction;
	do
	{
		instruction = codes[ip++];
		if (instruction.operand1[0] == '#')
		{
			if (instruction.operand1[1] == 'T')
			{
				itoa(top, result, 10);
				strcat(result, instruction.operand1 + 4);
				oper1 = eval(string(result));
			}
			else
			{
				itoa(base, result, 10);
				strcat(result, instruction.operand1 + 5);
				oper1 = eval(string(result));
			}
		}
		else if (instruction.operand1[0] == '&')
		{
			for (i = 1; instruction.operand1[i] && isdigit(instruction.operand1[i]); i++)
				result[i - 1] = instruction.operand1[i];
			result[i - 1] = '\0';
			oper1 = atoi(result);
			if (oper1 < GLOBAL_ADDRESS) oper1 += base;
			t = *((int*)(Memory + oper1));
			itoa(t, result, 10);
			strcat(result, instruction.operand1 + i);
			oper1 = eval(string(result));
		}
		else if (!strcmp(instruction.opt, "LOADIf"))
		{
			foper1 = atof(instruction.operand1);
		}
		else
		{
			oper1 = atoi(instruction.operand1);
		}
		oper2 = atoi(instruction.operand2);
		t = NUL;
		mtypes_size = mtypes.size() - 1;

		//#define TEST
#ifdef TEST
		printf("ip=%d, opt=%s,oper1=%d,oper2=%d,foper1=%f,top=%d,base=%d\n", ip, instruction.opt, oper1, oper2, foper1, top, base);
		getchar();
#endif // TEST

		switch (Map[instruction.opt])
		{
		case LOADi:
			for (i = 0; i < oper2; i += 4) mtypes.push_back(INT);
			t = LOADIi;
		case LOADf:
			if (t == NUL)
				for (i = 0; i < oper2; i += 4) mtypes.push_back(FLOAT);
			if (oper1 < GLOBAL_ADDRESS) oper1 += base;
			memcpy(Memory + top, Memory + oper1, oper2);
			top += oper2;
			break;
		case LOADIi:
			*(int*)(Memory + top) = oper1;
			mtypes.push_back(INT);
			top += 4;
			break;
		case LOADIf:
			*(float*)(Memory + top) = foper1;
			mtypes.push_back(FLOAT);
			top += 4;
			break;
		case STO:
			if (oper1 < GLOBAL_ADDRESS) oper1 += base;
			memcpy(Memory + oper1, Memory + top - oper2, oper2);
			for (i = 0; i < oper2; i += 4) mtypes.pop_back();
			top -= oper2;
			break;
		case ADD:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) + *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT && (mtypes[mtypes_size - 1] == FLOAT))
				*(float*)(Memory + top - 4) = *(int*)(Memory + top - 4) + *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(float*)(Memory + top - 4) = *(float*)(Memory + top - 4) + *(int*)(Memory + top);
			else
				*(float*)(Memory + top - 4) = *(float*)(Memory + top - 4) + *(float*)(Memory + top);
			mtypes.pop_back();
			break;
		case SUB:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) - *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT && (mtypes[mtypes_size - 1] == FLOAT))
				*(float*)(Memory + top - 4) = *(int*)(Memory + top - 4) - *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(float*)(Memory + top - 4) = *(float*)(Memory + top - 4) - *(int*)(Memory + top);
			else
				*(float*)(Memory + top - 4) = *(float*)(Memory + top - 4) - *(float*)(Memory + top);
			mtypes.pop_back();
			break;
		case MULT:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) * *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT && (mtypes[mtypes_size - 1] == FLOAT))
				*(float*)(Memory + top - 4) = *(int*)(Memory + top - 4) * *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(float*)(Memory + top - 4) = *(float*)(Memory + top - 4) * *(int*)(Memory + top);
			else
				*(float*)(Memory + top - 4) = *(float*)(Memory + top - 4) * *(float*)(Memory + top);
			mtypes.pop_back();
			break;
		case DIV:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) / *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT && (mtypes[mtypes_size - 1] == FLOAT))
				*(float*)(Memory + top - 4) = *(int*)(Memory + top - 4) / *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(float*)(Memory + top - 4) = *(float*)(Memory + top - 4) / *(int*)(Memory + top);
			else
				*(float*)(Memory + top - 4) = *(float*)(Memory + top - 4) / *(float*)(Memory + top);
			mtypes.pop_back();
			break;
		case BR:
			ip = oper1;
			break;
		case BRF:
			top -= 4;
			mtypes.pop_back();
			if (mtypes[mtypes_size - 1] == FLOAT && *(float*)(Memory + top) == 0)
				ip = oper1;
			else if (*(int*)(Memory + top) == 0)
				ip = oper1;
			break;
		case EQ:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) == *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) == *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) == *(int*)(Memory + top);
			else
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) == *(float*)(Memory + top);
			mtypes.pop_back();
			mtypes.back() = INT;
			break;
		case NOTEQ:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) != *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) != *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) != *(int*)(Memory + top);
			else
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) != *(float*)(Memory + top);
			mtypes.pop_back();
			mtypes.back() = INT;
			break;
		case GT:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) > *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) > *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) > *(int*)(Memory + top);
			else
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) > *(float*)(Memory + top);
			mtypes.pop_back();
			mtypes.back() = INT;
			break;
		case LES:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) < *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) < *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) < *(int*)(Memory + top);
			else
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) < *(float*)(Memory + top);
			mtypes.pop_back();
			mtypes.back() = INT;
			break;
		case GE:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) >= *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) >= *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) >= *(int*)(Memory + top);
			else
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) >= *(float*)(Memory + top);
			mtypes.pop_back();
			mtypes.back() = INT;
			break;
		case LE:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) <= *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) <= *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) <= *(int*)(Memory + top);
			else
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) <= *(float*)(Memory + top);
			mtypes.pop_back();
			mtypes.back() = INT;
			break;
		case AND:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) && *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) && *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) && *(int*)(Memory + top);
			else
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) && *(float*)(Memory + top);
			mtypes.pop_back();
			mtypes.back() = INT;
			break;
		case OR:
			top -= 4;
			if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) || *(int*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == INT && mtypes[mtypes_size] == FLOAT)
				*(int*)(Memory + top - 4) = *(int*)(Memory + top - 4) || *(float*)(Memory + top);
			else if (mtypes[mtypes_size - 1] == FLOAT && mtypes[mtypes_size] == INT)
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) || *(int*)(Memory + top);
			else
				*(int*)(Memory + top - 4) = *(float*)(Memory + top - 4) || *(float*)(Memory + top);
			mtypes.pop_back();
			mtypes.back() = INT;
			break;
		case INi:
			printf("please input %d integer: ", oper1 / 4);
			for (i = 0; i < oper1; i += 4, top += 4, mtypes.push_back(INT))
				scanf("%d", (int*)(Memory + top));
			break;
		case INf:
			printf("please input %d float: ", oper1 / 4);
			for (i = 0; i < oper1; i += 4, top += 4, mtypes.push_back(FLOAT))
				scanf("%f", (float*)(Memory + top));
			break;
		case OUTi:
			printf("result: ");
			for (i = top - oper1; i < top; i += 4) printf("%d  ", *(int*)(Memory + i));
			putchar('\n');
			top -= oper1;
			for (i = 0; i < oper1; i += 4) mtypes.pop_back();
			break;
		case OUTf:
			printf("result: ");
			for (i = top - oper1; i < top; i += 4) printf("%f  ", *(float*)(Memory + i));
			putchar('\n');
			top -= oper1;
			for (i = 0; i < oper1; i += 4) mtypes.pop_back();
			break;
		case ENTER:
			top += oper1;
			break;
		case RETURN:
			ip = *(int*)(Memory + base + 4);
			top = base;
			base = *(int*)(Memory + top);
			break;
		case CAL:
			*(int*)(Memory + top) = base;
			*(int*)(Memory + top + 4) = ip;
			base = top;
			top += 8;
			ip = oper1;
			break;
		case ALLOCi:
			top += oper1;
			for (i = 0; i < oper1; i += 4) mtypes.push_back(INT);
			break;
		case ALLOCf:
			top += oper1;
			for (i = 0; i < oper1; i += 4) mtypes.push_back(FLOAT);
			break;
		case CPY:
			memcpy(Memory + oper1, Memory + top - oper2, oper2);
			break;
		}
	} while (ip != 0);
	return es = 0;
}

int main(int argc, char* argv[])
{
	system("color F0");

	//    #define TEST
#ifndef TEST
	if (argc < 2)
	{
		printf("please input the center code file path\n");
		//exit(-52);
	}
	ReadCodes("D:\\LinYuFeng\\Documents\\Coder Life\\VSrepos\\4VirtualMachine\\4VirtualMachine\\codes.txt");
#else
	ReadCodes("D:\\LinYuFeng\\Documents\\Coder Life\\VSrepos\\4VirtualMachine\\4VirtualMachine\\codes.txt");
#endif
	initMap();
	TESTmachine();
	return 0;
}

