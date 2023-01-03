#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <unordered_map>

#include <vector>
#include <deque>

using namespace std;

enum Catagory { function, variable };
enum VaribleType { INT, FLOAT };

int OUTPUT = 0;

#define KEYWORDSUM 18    // 保留字数量
#define DOUBLESUM 8     // 双分界符数量
#define GLOBAL_ADDRESS 10000 // 定义全局变量起始地址,即局部变量终止地址

// 保留字表, 要求按照字典序有序，方便折半查找，不区分大小写
char keyword[][10] = { "break", "call", "case", "continue", "default", "do",
					  "else", "float", "for", "function", "if", "int", "read",
					  "return", "switch", "void", "while", "write"
};
// 单分界符，因为除号和注释符号相同，故另作讨论
char singleword[] = "+-*()[]{};,!><=:";
// 双分界符, 要求按照字典序排序，方便折半查找
char doubleword[][10] = { "!=", "&&", "++", "--", "<=", "==", ">=", "||" };

// 单词结构体由单词类别、名称、位置组成
struct Word
{
	int row;
	int col;
	char type[10];
	char token[40];
	Word(int row, int col, const char* token, const char* type);
};

Word::Word(int row, int col, const char* token, const char* type)
{
	this->row = row;
	this->col = col;
	strcpy(this->token, token);
	strcpy(this->type, type);
}

struct Code
{
	char opt[10];  // 操作码
	char operand1[20];  // 操作数1
	char operand2[20];  // 操作数2
	Code(const char* opt, char* operand1, char* operand2 = NULL);
	Code(const char* opt, int operand1, int operand2 = 0);
	Code(const char* opt);
	Code() {}
};

Code::Code(const char* opt, int operand1, int operand2)
{
	char t1[20], t2[20];
	itoa(operand1, t1, 10);
	itoa(operand2, t2, 10);
	strcpy(this->opt, opt);
	strcpy(this->operand1, t1);
	strcpy(this->operand2, t2);
}

Code::Code(const char* opt, char* operand1, char* operand2)
{
	strcpy(this->opt, opt);
	if (NULL == operand1) strcpy(this->operand1, "0");
	else strcpy(this->operand1, operand1);
	if (NULL == operand2) strcpy(this->operand2, "0");
	else strcpy(this->operand2, operand2);
}

Code::Code(const char* opt)
{
	strcpy(this->opt, opt);
	strcpy(this->operand1, "0");
	strcpy(this->operand2, "0");
}

struct Symbol
{
	char name[40];  //变量名
	int address;    //变量地址
	char type[20];  //变量类型
	int args;       //变量参数，对于Integer和Float，固定为4，对于数组，表示数组长度*4，对于函数，表示函数返回值参数
	vector<Code> init;   //初始化参数代码
	Symbol(const char* name, const char* type, int args = 0, int address = 0);
	Symbol() {}
};

Symbol::Symbol(const char* name, const char* type, int args, int address)
{
	this->address = address;
	this->args = args;
	strcpy(this->type, type);
	strcpy(this->name, name);
}

unordered_map<string, int> data_type_size;  // 存储数据类型的字节大小
unordered_map<string, VaribleType> data_type;  // 存储数据类型
unordered_map<string, vector<Symbol>> symbols; // 变量符号表
vector<Code> codes; // 中间代码
deque<Word> words;
char domain_name[40]; // 当前所处的函数名称，如果处于全局，则值为 global

#define MakeStringVectorPair(__string__) (pair<string, vector<Symbol>>(__string__, vector<Symbol>()))

// 确定成功则返回1，失败返回0
inline int CalculateSymbolAddress(Symbol& sym)
{
	unordered_map<string, vector<Symbol>>::iterator iter = symbols.find(domain_name);
	if (iter == symbols.end()) return 0;
	vector<Symbol>& vec = iter->second;
	int size = vec.size();
	int tsize = 0;
	for (int i = 1; i < size; i++)
	{
		if (!strcmp(vec[i].name, sym.name))
			break;
		tsize += vec[i].args;
	}
	if (strcmp(domain_name, "global")) sym.address = tsize + 8;
	else sym.address = tsize + GLOBAL_ADDRESS;
	return 1;
}

inline Symbol* lookup(const char* name, Catagory catagory)
{
	int i, size;
	unordered_map<string, vector<Symbol>>::iterator iter;
	unordered_map<string, vector<Symbol>>::iterator iter2 = symbols.find("global");
	if (catagory == function)
		iter = symbols.find(name);
	else
		iter = symbols.find(domain_name);
	if (iter != symbols.end())
	{
		vector<Symbol>& vec = iter->second;
		for (i = 0, size = vec.size(); i < size; i++)
			if (!strcmp(name, vec[i].name))
				return &vec[i];
	}
	if (iter2 != symbols.end())
	{
		vector<Symbol>& vec2 = iter2->second;
		for (i = 0, size = vec2.size(); i < size; i++)
			if (!strcmp(name, vec2[i].name))
				return &vec2[i];
	}
	return NULL;
}

int factor(int, int* _size = NULL, VaribleType* type = NULL);
int term(int, int* _size = NULL, VaribleType* type = NULL);
int additive_expr(int, int* _size = NULL, VaribleType* type = NULL);
int bool_stat(int, int* _size = NULL, VaribleType* type = NULL);
int bool_expr(int, int* _size = NULL, VaribleType* type = NULL);
int expression(int, int* _size = NULL);
int expression_stat(int, int* _size = NULL);
int compound_stat(int, int* _size = NULL);
int call_stat(int, int* _size = NULL, VaribleType* type = NULL);
int read_stat(int, int* _size = NULL);
int write_stat(int, int* _size = NULL);
int for_stat(int, int* _size = NULL);
int while_stat(int, int* _size = NULL);
int do_while_stat(int, int* _size = NULL);
int switch_stat(int, int* _size = NULL);
int if_stat(int, int* _size = NULL);
int statement(int, int* _size = NULL);
int statement_list(int, int* _size = NULL);
int declaration_body(int, char* data_type, int* _size = NULL);
int declaration_stat(int, int* _size = NULL);
int declaration_list(int, int* _size = NULL);
int function_body(int, int, char, int* _size = NULL);
int argument_stat(int, int* _size = NULL);
int argument_list(int, int* _size = NULL);
int fun_declaration(int, int* _size = NULL);
int main_declaration(int, int* _size = NULL);

// 读取文件
static FILE* inFile;
void inline TestRead(char*& _words_)
{
	int fsize, i = 0;
	if (NULL == inFile)
	{
		printf("fail to open input file！");
		exit(35);
	}
	fseek(inFile, 0, SEEK_END);
	fsize = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	_words_ = (char*)malloc(fsize + 5);
	while (~(_words_[i++] = getc(inFile)));
	_words_[i - 1] = '\0';
}

// 输出词法分析文件
static FILE* wordFile;
static char buff[1024], errors[500000];
int inline TestSave(int row, int col, const char* fmt, ...)
{
	int n;
	va_list args;
	va_start(args, fmt);
	n = vsprintf(buff, fmt, args);
	if (strstr(buff, "ERROR"))
		strcat(errors, buff);
	else
	{
		va_start(args, fmt);
		char* type = va_arg(args, char*);
		char* token = va_arg(args, char*);
		if (OUTPUT)
		{
			fprintf(wordFile, "%s", buff);
		}
		words.push_back(Word(row, col, token, type));
	}
	return n;
}

// 将语法分析结果（中间代码，符号表）保存
static FILE* codeFile, * symbolFile;
void inline TestOut()
{
	int i, size;
	if (NULL == codeFile)
	{
		printf("fail to open the center file！");
		exit(35);
	}
	for (i = 0, size = codes.size(); i < size; i++)
	{
		fprintf(codeFile, "%s\t%s\t%s\n", codes[i].opt, codes[i].operand1, codes[i].operand2);
	}
	if (OUTPUT)
	{
		if (NULL == symbolFile)
		{
			printf("fail to open the symbols file！");
			exit(35);
		}
		for (auto iter = symbols.begin(); iter != symbols.end(); iter++)
		{
			auto t = iter->second;
			fprintf(symbolFile, "%s:\n", iter->first.c_str());
			for (i = 0, size = t.size(); i < size; i++)
				fprintf(symbolFile, "%s\t%d\t%s\t\t%d\n", t[i].name, t[i].address, t[i].type, t[i].args);
		}
	}
}

// 折半查找指定字符串是否在指定字符串数组中出现过，否返回0,是返回1，要求字符数组按字典序有序, ignore非0表示忽略大小写
char strsfind(char strs[][10], const char* sub, const int start, const int _end, const int ignore)
{
	if (_end < start) return 0;
	int i = 0, half = (start + _end) / 2;
	while (sub[i] && strs[half][i] && (sub[i] == strs[half][i] || (ignore && tolower(sub[i]) == strs[half][i]))) ++i;
	return sub[i] || strs[half][i] ? (!ignore && strs[half][i] > sub[i]) || (ignore && (strs[half][i] > tolower(sub[i]))) ?
		strsfind(strs, sub, start, half - 1, ignore) : strsfind(strs, sub, half + 1, _end, ignore) : 1;
}

// 查找字符是否在str中出现过
int strfind(const char* str, char sub)
{
	int i = 0;
	while (str[i] && str[i] != sub && ++i);
	return str[i] ? i + 1 : 0;
}

// 将src里从开始长度为len这段字符串拷贝到dst中
int strcopy(char* dst, const char* src, int len)
{
	int i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	return i;
}

char* strstolower(char* s)
{
	for (int i = 0; s[i]; i++) s[i] = tolower(s[i]);
	return s;
}

// 折半查找判断给定的字符串是否为保留字，否返回0
#define isKeyword(word) (strsfind(keyword, word, 0, KEYWORDSUM-1, 1))
// 判断给定的字符串是否为双分界符，否返回0
#define isDoubleword(word) (strsfind(doubleword, word, 0, DOUBLESUM-1, 0))
// 判断给定的字符是否为单分界符，否返回0
#define isSingleword(word) (strfind(singleword, word))
// 判断 ch1 和 ch2 是否为分隔符（单双分界符和wspace），否返回0
char isSplitword(char ch1, char ch2)
{
	char t[] = { ch1, ch2, '\0' };
	return iswspace(ch1) || isSingleword(ch1) || ch1 == '/' || isDoubleword(t);
}

// 判断数字状态是否有异常（判断在数字是否与非分隔字符连接）
int digitCheck(const char* words, char* token, int& i, int row, int& col)
{
	char error[400];
	int len;
	if (!isSplitword(words[i], words[i + 1]))
	{
		strcpy(error, token);
		len = strlen(error);
		while (!isSplitword(words[i], words[i + 1]))
			error[len] = words[i], ++i, ++col, ++len;
		TestSave(row, col - len + 1, "%s\tthe%drow%dcolumn has illegal sign:%s\n", "ERROR", row, col - len + 1, error);
		return 1;
	}
	return 0;
}

// 进行词法分析
void TestScan(const char* _words_)
{
	int i = 0, j, row = 1, col = 1, tcol;
	char token[41], flag = 0, type[10], t[3], * ttoken;
	while (_words_[i])
	{
		// 跳过空白字符
		if (iswspace(_words_[i]))
		{
			// 记录行列位置
			if ('\n' == _words_[i])
			{
				row++;
				col = 1;
			}
			else if ('\t' == _words_[i])
				col += 4;
			else
				col++;
			i++;
			continue;
		}
		// 进入识别标识符或保留字的状态,规定标识符和保留字为由字母数字下划线组成，且不能由数字开头，保留字位于保留字表
		else if (isalpha(_words_[i]) || '_' == _words_[i])
		{
			j = 1;
			token[0] = _words_[i];
			tcol = col;
			while (++col && (isalnum(_words_[++i]) || '_' == _words_[i]) && j < 40) token[j++] = _words_[i];
			token[j] = '\0';
			// 检查是否正常到达结束状态
			if (40 == j)
			{
				while (isalnum(_words_[i]) || '_' == _words_[i]) col++, i++, j++;
				ttoken = (char*)malloc(j + 1);
				strcopy(ttoken, _words_ + i - j, j);
				ttoken[j] = '\0';
				TestSave(row, tcol, "%s\tthe%drow%dcolumn the length of sign is too long:%s\n", "ERROR", row, tcol, ttoken);
				free(ttoken);
				continue;
			}
			// 如果是保留字
			if (isKeyword(token))
				TestSave(row, tcol, "%s\t%s\n", strstolower(token), token);
			// 否则就是用户自定义的标识符
			else
				TestSave(row, tcol, "%s\t%s\n", "ID", token);
		}
		// 实数识别
		else if (isdigit(_words_[i]) || '.' == _words_[i])
		{
			j = 1;
			token[0] = _words_[i];
			tcol = col;
			while (++col && (isdigit(_words_[++i]) || '.' == _words_[i]) && j < 40)
				token[j++] = _words_[i];
			token[j] = '\0';
			// 检查是否正常到达结束状态
			if (40 == j)
			{
				while (isdigit(_words_[i]) || '.' == _words_[i]) col++, i++, j++;
				ttoken = (char*)malloc(j + 1);
				strcopy(ttoken, _words_ + i - j, j);
				ttoken[j] = '\0';
				TestSave(row, tcol, "%s\t第%d行%d列的数字超出长度限制:%s\n", "ERROR", row, tcol, ttoken);
				free(ttoken);
				continue;
			}
			strcpy(type, "Integer");
			// q1 状态
			while (--j >= 0 && isdigit(token[j]));
			// q3 状态
			if ('.' == token[j])
			{
				strcpy(type, "Float");
				while (--j >= 0 && isdigit(token[j]));
			}
			if (j != -1)
				TestSave(row, tcol, "%s\t第%d行%d列无法识别的数字:%s\n", "ERROR", row, tcol, token);
			else if (!digitCheck(_words_, token, i, row, col))
				TestSave(row, tcol, "%s\t%s\n", type, token);
		}
		// 双分界符识别
		else if ((t[0] = _words_[i]) && (t[1] = _words_[i + 1]) && isDoubleword(t))
		{
			TestSave(row, col, "%s\t%s\n", t, t);
			i += 2;
			col += 2;
		}
		// 单分界符识别
		else if (isSingleword(_words_[i]))
		{
			token[0] = _words_[i++];
			token[1] = '\0';
			++col;
			TestSave(row, col - 1, "%s\t%s\n", token, token);
		}
		// 注释或者除号识别
		else if ('/' == _words_[i])
		{
			++i;
			++col;
			// 支持嵌套注释
			if ('*' == _words_[i])
			{
				flag = 1;
				++i;
				++col;
				while (flag && _words_[i])
				{
					if ('*' == _words_[i] && '/' == _words_[i + 1])
					{
						flag--;
						col += 2;
						i++;
					}
					else if ('/' == _words_[i] && '*' == _words_[i + 1])
					{
						flag++;
						col += 2;
						i++;
					}
					else if ('\n' == _words_[i])
					{
						row++;
						col = 1;
					}
					else if ('\t' == _words_[i])
					{
						col += 4;
					}
					else if (isascii(_words_[i]))
					{
						col++;
					}
					else
					{
						col++;
						i += 2;
					}
					i++;
				}
			}
			// 除号
			else
			{
				token[0] = '/';
				token[1] = '\0';
				TestSave(row, col - 1, "%s\t%s\n", token, token);
			}
		}
		// 非法字符
		else
		{
			j = 0;
			tcol = col;
			while (!isSplitword(_words_[i], _words_[i + 1]))
				token[j] = _words_[i], col++, i++, j++;
			token[j] = '\0';
			TestSave(row, tcol, "%s\t第%d行%d列含有非法字符:%s", "ERROR", row, tcol, token);
		}
	}
	Word __t = words.back();
	words.push_back(Word(__t.row, __t.col + strlen(__t.token), "#", "#"));
	for (i = 0, j = 1; errors[i]; i++)
		if (errors[i] == '\n')
			j++;
	if (i)
		printf("sum find%d errors，such as:\n%s", j, errors);
	//    fprintf(saveFile, "%s", errors);
}

// 提示错误信息并返回
#define err(_t, _es, _msg) (printf("第%d行%d列，%s\n", _t.row, _t.col, _msg),_es)
// 判断字符串是否相等
#define equals(str1, str2) (!strcmp(str1, str2))

// 获得并弹出队首元素
inline Word& wpop()
{
	Word& t = words.front();
	if (words.size() > 1)
		words.pop_front();
	return t;
}

int tree(const char* msg, int depth)
{
	if (OUTPUT)
	{
		for (int i = 0; i < depth; i++)
			printf("    ");
		printf("%d|%s\n", depth, msg);
	}
	return 1;
}

// <factor>::='('<additive_expr>')'|<call_stat>|Integer|Float|ID|++ID|--ID|ID'['Integer|ID']'
int factor(int depth, int* _size, VaribleType* type)
{
	tree("factor", depth++);
	int es = 0;
	char ts[45], msg[150];
	Word t = words.front();
	char flag = 0;
	if (equals(t.type, "Integer"))
	{
		flag = 1;
		codes.push_back(Code("LOADIi", t.token));
	}
	else if (equals(t.type, "Float"))
	{
		flag = 1;
		codes.push_back(Code("LOADIf", t.token));
	}
	if (flag)
	{
		if (NULL != _size) *_size = data_type_size[t.type];
		if (NULL != type) *type = data_type[t.type];
		wpop();
		tree(t.token, depth);
		return (es = 0);
	}
	if (equals(t.type, "ID"))
	{
		wpop();
		strcpy(ts, t.token);
		Symbol* sym = lookup(t.token, variable);
		if (NULL == sym) return err(t, 1, ("变量'" + string(t.token) + "'未定义").c_str());
		if (NULL != _size) *_size = sym->args;
		if (NULL != type) *type = data_type[sym->type];
		char oper1[50], oper2[50];
		sprintf(oper1, "%d", sym->address);
		sprintf(oper2, "%d", sym->args);
		t = words.front();
		if (equals(t.type, "["))
		{
			if (NULL != _size) *_size = data_type_size[sym->type];
			wpop();
			strcat(ts, t.token);
			t = wpop();
			if (!equals(t.type, "Integer") && !equals(t.type, "ID")) return err(t, 1, "数组的引用必须是数字或变量");
			if (!strstr(sym->type, "[]"))
			{
				sprintf(msg, "变量 '%s' 不是数组，不可以进行数组引用", sym->name);
				return err(t, 1, msg);
			}
			strcat(ts, t.token);
			if (equals(t.type, "Integer"))
			{
				int _t_ = atoi(t.token);
				if (_t_ >= sym->args / data_type_size[sym->type])
				{
					sprintf(msg, "%s 超过数组长度 %d", t.token, sym->args / data_type_size[sym->type]);
					return err(t, 1, msg);
				}
				sprintf(oper1, "%d", sym->address + _t_ * data_type_size[t.type]);
			}
			else
			{
				Symbol* sym2 = lookup(t.token, variable);
				if (NULL == sym2) return err(t, 1, ("变量'" + string(t.token) + "'未定义").c_str());
				if (!equals(sym2->type, "int")) return err(t, 1, "数组的引用必须是整型变量");
				sprintf(oper1, "&%d*%d+%d", sym2->address, data_type_size[t.type], sym->address);
			}
			t = wpop();
			if (!equals(t.type, "]")) return err(t, 1, "缺少右中括号:']'");
			strcat(ts, t.token);
			sprintf(oper2, "%d", data_type_size[sym->type]);
		}
		if (strstr(sym->type, "int"))
			codes.push_back(Code("LOADi", oper1, oper2));
		else
			codes.push_back(Code("LOADf", oper1, oper2));
		tree(ts, depth);
		return (es = 0);
	}
	else if ((equals(t.type, "++") || equals(t.type, "--")) && equals(words[1].type, "ID"))
	{
		Code tc("SUB");
		if (equals(t.type, "++")) strcpy(tc.opt, "ADD");
		wpop();
		memcpy(ts, t.token, 2);
		t = wpop();
		Symbol* sym = lookup(t.token, variable);
		if (NULL == sym) return err(t, 1, ("变量'" + string(t.token) + "未定义").c_str());
		if (data_type_size[sym->type] != sym->args) return err(t, 1, ("不可对数组名'" + string(t.token) + "'直接进行自增自减操作。").c_str());
		if (NULL != _size) *_size = sym->args;
		if (NULL != type) *type = data_type[sym->type];
		if (strstr(sym->type, "int"))
			codes.push_back(Code("LOADi", sym->address, sym->args));
		else
			codes.push_back(Code("LOADf", sym->address, sym->args));
		codes.push_back(Code("LOADIi", 1));
		codes.push_back(tc);
		strcpy(ts + 2, t.token);
		tree(ts, depth);
		return (es = 0);
	}
	else if (equals(t.type, "call"))
	{
		es = call_stat(depth, _size, type);
		return (es = 0);
	}
	if (!equals(t.type, "(")) return err(t, 1, "缺少左括号:'('");
	wpop();
	tree(t.token, depth);
	es = additive_expr(depth, _size, type);
	if (es) return es;
	t = wpop();
	if (!equals(t.type, ")")) return err(t, 1, "缺少右括号:')'");
	tree(t.token, depth);
	return es;
}

// <term>::=<factor>{(*|/)<factor>}
int term(int depth, int* _size, VaribleType* type)
{
	tree("term", depth++);
	int es = 0, tsize = 0, tsize2 = 0;
	VaribleType fac_type;
	es = factor(depth, &tsize, type);
	if (es) return es;
	Word t = words.front();
	while (equals(t.type, "*") || equals(t.type, "/"))
	{
		tree(t.token, depth);
		wpop();
		es = factor(depth, &tsize2, &fac_type);
		if (es) return es;
		if (tsize != tsize2) return err(t, 1, "数据类型不一致，无法做乘除运算");
		if (NULL != type && fac_type > *type) *type = FLOAT;  // int -> float
		if (equals(t.type, "*")) codes.push_back(Code("MULT"));
		else codes.push_back(Code("DIV"));
		t = words.front();
		tsize = tsize2;
	}
	if (NULL != _size) *_size = tsize;
	return es;
}

// <additive_expr>::=[+|-]<term>{(+|-)<term>}
int additive_expr(int depth, int* _size, VaribleType* type)
{
	tree("additive_expr", depth++);
	int es = 0, tsize = 0, tsize2 = 0;
	VaribleType term_type;
	char flag[10] = { '\0' };
	Word t = words.front();
	if (equals(t.type, "+") || equals(t.type, "-"))
	{
		wpop();
		tree(t.token, depth);
		codes.push_back(Code("LOADIi", 0));
		if (equals(t.type, "+")) strcpy(flag, "ADD");
		else strcpy(flag, "SUB");
	}
	es = term(depth, &tsize, type);
	if (es) return es;
	if (flag[0]) codes.push_back(Code(flag));
	t = words.front();
	while (equals(t.type, "+") || equals(t.type, "-"))
	{
		tree(t.token, depth);
		wpop();
		es = term(depth, &tsize2, &term_type);
		if (es) return es;
		if (tsize != tsize2) return err(t, 1, "数据类型不一致，无法做加减运算");
		if (NULL != type && term_type > *type) *type = FLOAT;  // int -> float
		if (equals(t.type, "+")) codes.push_back(Code("ADD"));
		else codes.push_back(Code("SUB"));
		t = words.front();
		tsize = tsize2;
	}
	if (NULL != _size) *_size = tsize;
	return es;
}

// <bool_stat>::=<additive_expr>[(>|<|>=|<=|==|!=)<additive_expr>]
int bool_stat(int depth, int* _size, VaribleType* type)
{
	tree("bool_stat", depth++);
	int es = 0, tsize = 0, tsize2 = 0;
	es = additive_expr(depth, &tsize, type);
	if (es) return es;
	if (NULL != _size) *_size = tsize;
	Word t = words.front();
	char flag = 0;
	Code tcode("GT");
	if (equals(t.type, ">"))
	{
		flag = 1;
		strcpy(tcode.opt, "GT");
	}
	else if (equals(t.type, "<"))
	{
		flag = 1;
		strcpy(tcode.opt, "LES");
	}
	else if (equals(t.type, ">="))
	{
		flag = 1;
		strcpy(tcode.opt, "GE");
	}
	else if (equals(t.type, "<="))
	{
		flag = 1;
		strcpy(tcode.opt, "LE");
	}
	else if (equals(t.type, "=="))
	{
		flag = 1;
		strcpy(tcode.opt, "EQ");
	}
	else if (equals(t.type, "!="))
	{
		flag = 1;
		strcpy(tcode.opt, "NOTEQ");
	}
	if (flag)
	{
		tree(t.token, depth);
		wpop();
		es = additive_expr(depth, &tsize2);
		if (tsize != tsize2) return err(t, 1, "数据类型不一致，无法做比较运算");
		codes.push_back(tcode);
		if (NULL != _size) *_size = data_type_size["Integer"];  // 将比较结果视为 int 型
		if (NULL != type) *type = INT;
	}
	return es;
}

// bool_expr 的 FIRST 集
//static char first_bool_expr[][10] = {"Float", "ID", "Integer", "{"};
// <bool_expr>::=<bool_stat>{('&&'|'||')<bool_stat>}
// size里存放本次表达式的结果的字节大小，type表示应以何种方式解析内存
int bool_expr(int depth, int* _size, VaribleType* type)
{
	tree("bool_expr", depth++);
	int es = 0, tsize = 0, tsize2 = 0;
	es = bool_stat(depth, &tsize, type);
	if (es) return es;
	if (NULL != _size) *_size = tsize;
	Word t = words.front();
	while (equals(t.type, "&&") || equals(t.type, "||"))
	{
		tree(t.token, depth);
		wpop();
		es = bool_stat(depth, &tsize2);
		if (es) return es;
		if (tsize != tsize2) return err(t, 1, "数据类型不一致，无法做逻辑运算");
		if (equals(t.type, "&&")) codes.push_back(Code("AND"));
		else codes.push_back(Code("OR"));
		if (NULL != _size) *_size = data_type_size["Integer"];  // 将逻辑运算结果视为 int 型
		if (NULL != type) *type = INT;
		t = words.front();
		tsize = tsize2;
	}
	return es;
}

// <expression>::=ID(=<bool_expr>|'['Integer|ID']'=<bool_expr>)|<bool_expr>
int expression(int depth, int* _size)
{
	tree("expression", depth++);
	int es = 0, tsize = 0;
	char msg[150], msg2[80];
	Code* tc = NULL;
	Symbol* sym = NULL;
	Word t = words.front();
	if (equals(words[0].type, "ID") && (equals(words[1].type, "=") || equals(words[1].type, "[")))
	{
		sym = lookup(words[0].token, variable);
		if (NULL == sym)
		{
			sprintf(msg, "变量 '%s' 未定义", words[0].token);
			return err(words[0], 1, msg);
		}
		tree(words[0].token, depth);
		tree(words[1].token, depth);
		wpop();
		t = wpop();
		if (equals(t.type, "="))
		{
			tc = new Code("STO", sym->address, sym->args);
		}
		else
		{
			t = wpop();
			if (!equals(t.type, "Integer") && !equals(t.type, "ID")) return err(t, 1, "数组的引用必须是数字或变量");
			if (!strstr(sym->type, "[]"))
			{
				sprintf(msg, "变量 '%s' 不是数组，不可以进行数组引用", sym->name);
				return err(t, 1, msg);
			}
			tree(t.token, depth);
			if (equals(t.type, "Integer"))
			{
				int _t_ = atoi(t.token);
				if (_t_ >= sym->args / data_type_size[sym->type])
				{
					sprintf(msg, "%s 超过数组长度 %d", t.token, sym->args / data_type_size[sym->type]);
					return err(t, 1, msg);
				}
				tc = new Code("STO", sym->address + _t_ * data_type_size[sym->type], data_type_size[sym->type]);
			}
			else
			{
				Symbol* sym2 = lookup(t.token, variable);
				if (NULL == sym2) return err(t, 1, ("变量'" + string(t.token) + "'未定义").c_str());
				if (!equals(sym2->type, "int")) return err(t, 1, "数组的引用必须是整型变量");
				sprintf(msg, "&%d*%d+%d", sym2->address, data_type_size[sym->type], sym->address);
				sprintf(msg2, "%d", data_type_size[sym->type]);
				tc = new Code("STO", msg, msg2);
			}
			t = wpop();
			if (!equals(t.type, "]")) return err(t, 1, "缺少右中括号:]");
			tree(t.token, depth);
			t = wpop();
			if (!equals(t.type, "=")) return err(t, 1, "缺少等于号:=");
			tree(t.token, depth);
		}
	}
	if (NULL != tc)
	{
		if (NULL != _size) *_size = 0;
		es = bool_expr(depth, &tsize);
		if (tsize != atoi(tc->operand2)) return err(words[0], 1, "非法的赋值操作：变量数据类型不一致");
	}
	else
	{
		es = bool_expr(depth, &tsize);
		if (NULL != _size) *_size = tsize;
	}
	if (tc) codes.push_back(*tc);
	delete tc;
	return es;
}

// <expression_stat>::=<expression>;|;
int expression_stat(int depth, int* _size)
{
	tree("expression_stat", depth++);
	int es = 0;
	Word t = words.front();
	if (equals(t.type, ";"))
	{
		wpop();
	}
	else
	{
		es = expression(depth, _size);
		if (es) return es;
		t = wpop();
		if (!equals(t.type, ";")) return err(t, es, "缺少分号:;");
	}
	tree(t.token, depth);
	return es;
}

// <compound_stat>::='{'<statement_list>'}'
int compound_stat(int depth, int* _size)
{
	tree("compound_stat", depth++);
	int es = 0;
	Word t = wpop();
	if (!equals(t.type, "{")) return err(t, 1, "缺少左花括号:'{'");
	tree(t.token, depth);
	es = statement_list(depth, _size);
	if (es) return es;
	t = wpop();
	if (!equals(t.type, "}")) return err(t, 1, "缺少右花括号:'}'");
	tree(t.token, depth);
	return es;
}

// <call_stat>::= call ID'('{<bool_expr>}')'
int call_stat(int depth, int* _size, VaribleType* type)
{
	tree("call_stat", depth++);
	int es = 0;
	int arg_size = 0, tsize;
	Symbol* sym;
	Word t = wpop();
	if (!equals(t.type, "call")) return err(t, 1, "缺少关键字:'call'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "ID")) return err(t, 1, "函数调用缺少函数名。");
	sym = lookup(t.token, function);
	if (NULL == sym) return err(t, 1, ("函数'" + string(t.token) + "'未定义。").c_str());
	if (NULL != _size) *_size = sym->args;
	if (NULL != type) *type = data_type[sym->type];
	if (0 != sym->args)
	{
		if (strstr(sym->type, "int"))
			codes.push_back(Code("ALLOCi", sym->args));
		else
			codes.push_back(Code("ALLOCf", sym->args));
	}
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "(")) return err(t, 1, "缺少左括号:'('");
	tree(t.token, depth);
	while (!equals(words.front().type, ")"))
	{
		es = bool_expr(depth, &tsize);
		arg_size += tsize;
		if (es) return es;
		if (equals(words.front().type, ","))
			wpop();
	}
	t = wpop();
	if (!equals(t.type, ")")) return err(t, 1, "缺少右括号:')'");
	if (0 != arg_size)
	{
		char oper1[20], oper2[20];
		strcpy(oper1, "#TOP+8");
		sprintf(oper2, "%d", arg_size);
		codes.push_back(Code("CPY", oper1, oper2));
	}
	codes.push_back(Code("CAL", sym->address));
	tree(t.token, depth);
	return es;
}

// <read_stat>::=read ID['['Integer|ID']'];
int read_stat(int depth, int* _size)
{
	tree("read_stat", depth++);
	int es = 0;
	char msg[150];
	Word t = wpop();
	Code* code = NULL;
	if (!equals(t.type, "read")) return err(t, 1, "缺少关键字:'read'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "ID")) return err(t, 1, "read缺少运算对象");
	tree(t.token, depth);
	Symbol* sym = lookup(t.token, variable);
	if (NULL == sym) return err(t, 1, ("变量'" + string(t.token) + "未定义").c_str());
	if (NULL != _size) *_size = 0;
	if (equals(words.front().type, "["))
	{
		tree(words.front().token, depth);
		wpop();
		t = wpop();
		if (!equals(t.type, "Integer") && !equals(t.type, "ID")) return err(t, 1, "数组的引用必须是数字或变量");
		if (!strstr(sym->type, "[]"))
		{
			sprintf(msg, "变量 '%s' 不是数组，不可以进行数组引用", sym->name);
			return err(t, 1, msg);
		}
		if (equals(sym->type, "int[]"))
			codes.push_back(Code("INi", data_type_size[sym->type]));
		else
			codes.push_back(Code("INf", data_type_size[sym->type]));
		if (equals(t.type, "Integer"))
		{
			int _t_ = atoi(t.token);
			if (_t_ >= sym->args / data_type_size[sym->type])
			{
				sprintf(msg, "%s 超过数组长度 %d", t.token, sym->args / data_type_size[sym->type]);
				return err(t, 1, msg);
			}
			code = new Code("STO", _t_ * data_type_size[sym->type] + sym->address, data_type_size[sym->type]);
			codes.push_back(*code);
			delete code;
		}
		else
		{
			Symbol* sym2 = lookup(t.token, variable);
			if (NULL == sym2) return err(t, 1, ("变量'" + string(t.token) + "'未定义").c_str());
			if (!equals(sym2->type, "int")) return err(t, 1, "数组的引用必须是整型变量");
			code = new Code("STO", sym->address, data_type_size[sym->type]);
			sprintf((*code).operand1, "&%d*%d+%d", sym2->address, data_type_size[sym->type], sym->address);
			codes.push_back(*code);
			delete code;
		}
		tree(t.token, depth);
		t = wpop();
		if (!equals(t.type, "]")) return err(t, 1, "缺少右中括号:]");
		tree(t.token, depth);
		return es;
	}
	else if (equals(sym->type, "int") || equals(sym->type, "int[]"))
	{
		codes.push_back(Code("INi", sym->args));
	}
	else if (equals(sym->type, "float") || equals(sym->type, "float[]"))
	{
		codes.push_back(Code("INf", sym->args));
	}
	codes.push_back(Code("STO", sym->address, sym->args));
	return es;
}

// <write_stat>::=write <bool_expr>;
int write_stat(int depth, int* _size)
{
	tree("write_stat", depth++);
	int es = 0, tsize = 0;
	VaribleType bool_type;
	Word t = wpop();
	if (!equals(t.type, "write")) return err(t, 1, "缺少关键字:'write'");
	tree(t.token, depth);
	es = bool_expr(depth, &tsize, &bool_type);
	if (NULL != _size) *_size = 0;
	if (bool_type == INT) codes.push_back(Code("OUTi", tsize));
	else codes.push_back(Code("OUTf", tsize));
	return es;
}

//  <for_stat>::= for'('<expr>;<bool_expr>;<expr>')'<statement>
int for_stat(int depth, int* _size)
{
	tree("for_stat", depth++);
	int es = 0, cx_expr2, cx_expr3, expr2_brf, expr2_br;
	Word t = wpop();
	if (!equals(t.type, "for")) return err(t, 1, "缺少关键字:'for'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "(")) return err(t, 1, "缺少左括号:'('");
	tree(t.token, depth);
	es = expression(depth);
	if (es) return es;
	t = wpop();
	if (!equals(t.type, ";")) return err(t, 1, "缺少第二个表达式");
	tree(t.token, depth);
	cx_expr2 = codes.size();
	es = bool_expr(depth);
	if (es) return es;
	expr2_brf = codes.size();
	codes.push_back(Code("BRF"));
	expr2_br = codes.size();
	codes.push_back(Code("BR"));
	t = wpop();
	if (!equals(t.type, ";")) return err(t, 1, "缺少第三个表达式");
	tree(t.token, depth);
	cx_expr3 = codes.size();
	es = expression(depth);
	codes.push_back(Code("BR", cx_expr2));
	if (es) return es;
	t = wpop();
	if (!equals(t.type, ")")) return err(t, 1, "缺少右括号:')'");
	tree(t.token, depth);
	sprintf(codes[expr2_br].operand1, "%d", (int)codes.size());
	es = statement(depth);
	codes.push_back(Code("BR", cx_expr3));
	sprintf(codes[expr2_brf].operand1, "%d", (int)codes.size());
	return es;
}

// <while_stat>::= while '('<bool_expr>')' <statement>
int while_stat(int depth, int* _size)
{
	tree("while_stat", depth++);
	int es = 0, expr_brf, cx_expr;
	Word t = wpop();
	if (!equals(t.type, "while")) return err(t, 1, "缺少关键字:'while'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "(")) return err(t, 1, "缺少左括号:'('");
	tree(t.token, depth);
	cx_expr = codes.size();
	es = bool_expr(depth);
	if (es) return es;
	expr_brf = codes.size();
	codes.push_back(Code("BRF"));
	t = wpop();
	if (!equals(t.type, ")")) return err(t, 1, "缺少右括号:')'");
	tree(t.token, depth);
	es = statement(depth);
	codes.push_back(Code("BR", cx_expr));
	sprintf(codes[expr_brf].operand1, "%d", (int)codes.size());
	return es;
}

// <do_while_stat>::=do<statement> while '('<bool_expr>');'
int do_while_stat(int depth, int* _size)
{
	tree("do_while_stat", depth++);
	int es = 0, cx_stat, expr_brf;
	Word t = wpop();
	if (!equals(t.type, "do")) return err(t, 1, "缺少关键字:'do'");
	tree(t.token, depth);
	cx_stat = codes.size();
	es = statement(depth);
	if (es) return es;
	t = wpop();
	if (!equals(t.type, "while")) return err(t, 1, "缺少关键字:'while'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "(")) return err(t, 1, "缺少左括号:'('");
	tree(t.token, depth);
	es = bool_expr(depth);
	if (es) return es;
	expr_brf = codes.size();
	codes.push_back(Code("BRF"));
	codes.push_back(Code("BR", cx_stat));
	sprintf(codes[expr_brf].operand1, "%d", (int)codes.size());
	t = wpop();
	if (!equals(t.type, ")")) return err(t, 1, "缺少右括号:')'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, ";")) return err(t, 1, "缺少分号:';'");
	tree(t.token, depth);
	return es;
}

// <switch_stat>::=switch'('ID')' '{'{case Integer:<statement_list>[break;]}[default:<statement_list>[break;]]'}'
int switch_stat(int depth, int* _size)
{
	tree("switch_stat", depth++);
	int es = 0;
	int loc_BRF;
	vector<int> loc_break;
	Word t = wpop();
	if (!equals(t.type, "switch")) return err(t, 1, "缺少关键字:'switch'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "(")) return err(t, 1, "缺少左括号:'('");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "ID")) return err(t, 1, "语句缺少switch跳转关键字。");
	Symbol* sym = lookup(t.token, variable);
	if (NULL == sym) return err(t, 1, ("变量'" + string(t.token) + "'未定义").c_str());
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, ")")) return err(t, 1, "缺少右括号:')'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "{")) return err(t, 1, "缺少左花括号:'{'");
	tree(t.token, depth);
	t = words.front();
	while (equals(t.type, "case"))
	{
		tree(t.token, depth);
		if (strstr(sym->type, "int"))
			codes.push_back(Code("LOADi", sym->address, 4));
		else
			codes.push_back(Code("LOADf", sym->address, 4));
		wpop();
		t = wpop();
		if (!equals(t.type, "Integer")) return err(t, 1, "错误的case关键字，必须为整数。");
		codes.push_back(Code("LOADIi", t.token));
		codes.push_back(Code("EQ"));
		loc_BRF = codes.size();
		codes.push_back(Code("BRF"));
		tree(t.token, depth);
		t = wpop();
		if (!equals(t.type, ":")) return err(t, 1, "缺少冒号:':'");
		tree(t.token, depth);
		es = statement_list(depth);
		if (es) return es;
		t = words.front();
		if (equals(t.type, "break"))
		{
			loc_break.push_back(codes.size());
			codes.push_back(Code("BR"));
			tree(t.token, depth);
			wpop();
			t = wpop();
			if (!equals(t.type, ";")) return err(t, 1, "缺少分号:';'");
			tree(t.token, depth);
		}
		t = words.front();
		sprintf(codes[loc_BRF].operand1, "%d", (int)codes.size());
	}
	if (equals(t.type, "default"))
	{
		tree(t.token, depth);
		wpop();
		t = wpop();
		if (!equals(t.type, ":")) return err(t, 1, "缺少冒号:':'");
		tree(t.token, depth);
		es = statement_list(depth);
		if (es) return es;
		t = words.front();
		if (equals(t.type, "break"))
		{
			loc_break.push_back(codes.size());
			codes.push_back(Code("BR"));
			tree(t.token, depth);
			wpop();
			t = wpop();
			if (!equals(t.type, ";")) return err(t, 1, "缺少分号:';'");
			tree(t.token, depth);
		}
	}
	int l = codes.size();
	for (int i = 0, _size = loc_break.size(); i < _size; i++)
	{
		sprintf(codes[loc_break[i]].operand1, "%d", l);
	}
	t = wpop();
	if (!equals(t.type, "}")) return err(t, 1, "缺少右花括号:'}'");
	tree(t.token, depth);
	return es;
}

// <if_stat>::= if '('<bool_expr>')' <statement> [else <statement>]
int if_stat(int depth, int* _size)
{
	tree("if_stat", depth++);
	int es = 0, stat_br, expr_brf;
	Word t = wpop();
	if (!equals(t.type, "if")) return err(t, 1, "缺少关键字:'if'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "(")) return err(t, 1, "缺少左括号:'('");
	tree(t.token, depth);
	es = bool_expr(depth);
	if (es) return es;
	expr_brf = codes.size();
	codes.push_back(Code("BRF"));
	t = wpop();
	if (!equals(t.type, ")")) return err(t, 1, "缺少右括号:')'");
	tree(t.token, depth);
	es = statement(depth);
	if (es) return es;
	sprintf(codes[expr_brf].operand1, "%d", (int)codes.size());
	stat_br = codes.size();
	codes.push_back(Code("BR"));
	t = words.front();
	if (equals(t.type, "else"))
	{
		tree(t.token, depth);
		wpop();
		sprintf(codes[expr_brf].operand1, "%d", (int)codes.size());
		es = statement(depth);
	}
	sprintf(codes[stat_br].operand1, "%d", (int)codes.size());
	return es;
}

// statement的FIRST集
static char first_statement[][10] = { "(", ";", "Float", "ID", "Integer", "call", "do", "for", "if", "read", "switch", "while", "write", "{" };

// <statement>::=<if_stat>|<while_stat>|<do_while_stat>|<switch_stat>|<for_stat>|<read_stat>|<write_stat>|<compound_stat>|<expression_stat>|<call_stat>
int statement(int depth, int* _size)
{
	tree("statement", depth++);
	int es = 0;
	Word t = words.front();
	if (equals(t.type, "if"))
		es = if_stat(depth, _size);
	else if (equals(t.type, "while"))
		es = while_stat(depth, _size);
	else if (equals(t.type, "for"))
		es = for_stat(depth, _size);
	else if (equals(t.type, "read"))
		es = read_stat(depth, _size);
	else if (equals(t.type, "write"))
		es = write_stat(depth, _size);
	else if (equals(t.type, "{"))
		es = compound_stat(depth, _size);
	else if (equals(t.type, "call"))
		es = call_stat(depth, _size);
	else if (equals(t.type, "do"))
		es = do_while_stat(depth, _size);
	else if (equals(t.type, "switch"))
		es = switch_stat(depth, _size);
	else if (equals(t.type, "(") || equals(t.type, ";") || equals(t.type, "ID") || equals(t.type, "Integer") || equals(t.type, "Float"))
		es = expression_stat(depth, _size);
	else
		return err(t, 1, "错误的语句。");
	return es;
}

// <statement_list>::={<statement>}
int statement_list(int depth, int* _size)
{
	tree("statement_list", depth++);
	int es = 0;
	// 如果下个词属于 statement 的FIRST集合，那么就可以进入statement
	while (strsfind(first_statement, words.front().type, 0, 13, 0))
	{
		es = statement(depth, _size);
		if (es) return es;
	}
	return es;
}

// <declaration_body>::=ID[=<bool_expr>|'['Integer']'[='{'<bool_expr>{,<bool_expr>}'}']
// 对于声明来说，size表示声明了多大的数据
int declaration_body(int depth, char* data_type, int* _size)
{
	tree("declaration_body", depth++);
	vector<Code> vec;
	int es = 0, i = 0, tsize = 0, flag = 0;
	int len = 0; // 数组长度
	Word t = wpop();
	if (!equals(t.type, "ID")) return err(t, 1, "错误的变量声明。");
	if (lookup(t.token, variable)) return err(t, 1, ("重复的变量声明: " + string(t.token)).c_str());
	Symbol sym(t.token, data_type, data_type_size[data_type]);
	if (!CalculateSymbolAddress(sym)) return err(t, 1, "符号表插入失败。");
	tree(t.token, depth);
	t = words.front();
	if (equals(t.type, "="))
	{
		flag = 1;
		tree(t.token, depth);
		wpop();
		es = bool_expr(depth, &tsize);
		if (es) return es;
		if (0 == tsize || sym.args != tsize) return err(t, 1, "错误的变量初始化。");
	}
	else if (equals(t.type, "["))
	{
		tree(t.token, depth);
		wpop();
		t = wpop();
		if (equals(t.type, "]"))
		{
			tree(t.token, depth);
			t = wpop();
			if (!equals(t.type, "=")) return err(t, 1, "当不指定数组长度时，必须对此数组进行初始化。");
			flag = 1;
			tree(t.token, depth);
			t = wpop();
			if (!equals(t.type, "{")) return err(t, 1, "缺少左花括号:'{'");
			tree(t.token, depth);
			do
			{
				len++;
				es = bool_expr(depth);
				if (es) return es;
			} while (equals(words.front().type, ",") && (tree(t.token, depth)) && (&wpop()));
			t = wpop();
			if (!equals(t.type, "}")) return err(t, 1, "缺少右花括号:'}'");
			tree(t.token, depth);
		}
		else if (equals(t.type, "-"))
		{
			return err(t, 1, "数组长度必须是正数。");
		}
		else if (equals(t.type, "+"))
		{
			wpop();
			t = wpop();
		}
		if (equals(t.type, "Integer"))
		{
			tree(t.token, depth);
			len = atoi(t.token);
			t = wpop();
			if (!equals(t.type, "]")) return err(t, 1, "缺少右中括号:']'");
			tree(t.token, depth);
			t = words.front();
			if (equals(t.type, "="))
			{
				flag = 1;
				tree(t.token, depth);
				wpop();
				t = wpop();
				if (!equals(t.type, "{")) return err(t, 1, "缺少左花括号:'{'");
				tree(t.token, depth);
				do
				{
					i++;
					es = bool_expr(depth);
					if (es) return es;
				} while (equals(words.front().type, ",") && (tree(t.token, depth)) && (&wpop()));
				t = wpop();
				if (!equals(t.type, "}")) return err(t, 1, "缺少右花括号:'}'");
				tree(t.token, depth);
				if (i > len) return err(t, 1, "数组初始化值数量必须小于等于数组长度。");
			}
		}
		else return err(t, 1, "数组声明长度必须是整数。");
		strcat(sym.type, "[]");
		sym.args *= len;
	}
	if (NULL != _size) *_size = sym.args;
	if (flag)
	{
		codes.push_back(Code("STO", sym.address, sym.args));
		if (equals(domain_name, "global"))
			while (!(equals(codes.back().opt, "RETURN") || equals(codes.back().opt, "BR")))
				sym.init.push_back(codes.back()), codes.pop_back();
	}
	symbols[domain_name].push_back(sym);
	return es;
}

// <declaration_stat>::=(int|float)<declaration_body>{,declaration_body};
int declaration_stat(int depth, int* _size)
{
	tree("declaration_stat", depth++);
	int es = 0, tsize1 = 0, tsize2 = 0;
	char data_type[20];
	Word t = wpop();
	if (!equals(t.type, "int") && !equals(t.type, "float")) return err(t, 1, "声明缺少数据类型。");
	strcpy(data_type, t.type);
	tree(t.token, depth);
	do
	{
		es = declaration_body(depth, data_type, &tsize1);
		tsize2 += tsize1;
		if (es) return es;
	} while (equals(words.front().type, ",") && (tree(words.front().token, depth)) && (&wpop()));
	t = wpop();
	if (!equals(t.type, ";")) return err(t, 1, "缺少分号:';'");
	tree(t.token, depth);
	if (NULL != _size) *_size = tsize2;
	return es;
}

// <declaration_list>::={<declaration_stat>}
int declaration_list(int depth, int* _size)
{
	tree("declaration_list", depth++);
	int es = 0, tsize1 = 0, tsize2 = 0;
	while (equals(words.front().type, "int") || equals(words.front().type, "float"))
	{
		es = declaration_stat(depth, &tsize1);
		tsize2 += tsize1;
		if (es) return es;
	}
	if (NULL != _size) *_size = tsize2;
	return es;
}

// <function_body>::=<declaration_list><statement_list>
int function_body(int depth, int arg_size, char isMain, int* _size)
{
	tree("function_body", depth++);
	int es = 0, tsize1 = 0, tsize2 = 0, loc_enter;
	loc_enter = codes.size();
	codes.push_back(Code("ENTER"));
	es = declaration_list(depth, &tsize1);
	if (es) return es;
	sprintf(codes[loc_enter].operand1, "%d", arg_size + tsize1 + 8);
	if (isMain)
	{
		vector<Symbol>& vec = symbols["global"];
		for (int i = 0, size = vec.size(); i < size; i++)
		{
			vector<Code>& tc = vec[i].init;
			while (!tc.empty())
				codes.push_back(tc.back()), tc.pop_back();
		}
	}
	es = statement_list(depth, &tsize2);
	if (NULL != _size) *_size = tsize1;
	return es;
}

// <argument_stat>::=(int|float) ID['['Integer']']
int argument_stat(int depth, int* _size)
{
	tree("argument_stat", depth++);
	int es = 0;
	Word t = wpop();
	Symbol sym;
	if (!equals(t.token, "int") && !equals(t.token, "float")) return err(t, 1, "未指定函数参数数据类型。");
	strcpy(sym.type, t.token);
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "ID")) return err(t, 1, "错误的函数参数定义。");
	strcpy(sym.name, t.token);
	sym.args = data_type_size[sym.type];
	if (!CalculateSymbolAddress(sym)) return err(t, 1, "符号表查找失败。");
	tree(t.token, depth);
	t = words.front();
	if (equals(t.type, "["))
	{
		tree(t.token, depth);
		wpop();
		t = wpop();
		if (!equals(t.type, "Integer")) return err(t, 1, "函数参数数组必须指定大小。");
		sym.args *= atoi(t.token);
		tree(t.token, depth);
		t = wpop();
		if (!equals(t.type, "]")) return err(t, 1, "缺少右中括号:']'");
		tree(t.token, depth);
		strcat(sym.type, "[]");
	}
	if (NULL != _size) *_size = sym.args;
	symbols[domain_name].push_back(sym);
	return es;
}

// <argument_list>::=[<argument_stat>{,<argument_stat>}]
int argument_list(int depth, int* _size)
{
	tree("argument_list", depth++);
	int es = 0, tsize1 = 0, tsize2 = 0;
	if (equals(words.front().type, ")")) return (es = 0);
	do
	{
		es = argument_stat(depth, &tsize1);
		if (es) return es;
		tsize2 += tsize1;
	} while (equals(words.front().type, ",") && (tree(words.front().token, depth)) && (&wpop()));
	if (NULL != _size) *_size = tsize2;
	return es;
}

// <fun_declaration>::=(float|int)['['Integer']'] function ID'('<argument_list>')''{'<function_body>return<bool_expr>'}'|
// (void|ε) function ID'('<argument_list>')''{'<function_body>[return;]'}'
int fun_declaration(int depth, int* _size)
{
	tree("fun_declaration", depth++);
	int es = 0, arg_size = 0, func_size = 0, tsize = 0;
	char oper1[20], oper2[20];
	Word t = wpop();
	Symbol sym("", "void"); // 函数返回值类型，若未指定则默认为void
	if (equals(t.type, "float") || equals(t.type, "int"))
	{
		sym.args = data_type_size[t.token];
		strcpy(sym.type, t.type);
		tree(t.token, depth);
		t = wpop();
		if (equals(t.type, "["))
		{
			t = wpop();
			if (!equals(t.type, "Integer")) return err(t, 1, "函数返回值数组必须指定大小。");
			sym.args *= atoi(t.token);
			strcat(sym.type, "[]");
			t = wpop();
			if (!equals(t.type, "]")) return err(t, 1, "缺少右中括号:']'");
			t = wpop();
		}
	}
	else if (equals(t.type, "void"))
	{
		tree(t.token, depth);
		t = wpop();
	}
	if (!equals(t.type, "function")) return err(t, 1, "错误的函数定义。");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "ID")) return err(t, 1, "错误的函数定义。");
	strcpy(sym.name, t.token);
	strcpy(domain_name, t.token);
	sym.address = codes.size();
	if (symbols.find(sym.name) != symbols.end()) return err(t, 1, "重复的函数定义。");
	symbols.insert(MakeStringVectorPair(domain_name));
	symbols[domain_name].push_back(sym);
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "(")) return err(t, 1, "缺少左括号:'('");
	tree(t.token, depth);
	es = argument_list(depth, &arg_size);
	if (es) return es;
	t = wpop();
	if (!equals(t.type, ")")) return err(t, 1, "缺少右括号:')'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "{")) return err(t, 1, "缺少左花括号:'{'");
	tree(t.token, depth);
	es = function_body(depth, arg_size, 0, &func_size);
	if (es) return es;
	if (!equals(sym.type, "void"))
	{
		t = wpop();
		if (!equals(t.type, "return")) return err(t, 1, "缺少返回语句:return");
		tree(t.token, depth);
		es = bool_expr(depth, &tsize);
		if (es) return es;
		if (tsize != symbols[domain_name][0].args) return err(t, 1, "错误的返回值类型。");
		sprintf(oper1, "#BASE-%d", tsize);
		sprintf(oper2, "%d", tsize);
		codes.push_back(Code("CPY", oper1, oper2));
		t = wpop();
		if (!equals(t.type, ";")) return err(t, 1, "缺少分号:';'");
		tree(t.token, depth);
	}
	else if (equals(words.front().type, "return"))
	{
		t = wpop();
		tree(t.token, depth);
		t = wpop();
		if (!equals(t.type, ";")) return err(t, 1, "缺少分号:';'");
		tree(t.token, depth);
	}
	codes.push_back(Code("RETURN"));
	t = wpop();
	if (!equals(t.type, "}")) return err(t, 1, "缺少右花括号:'}'");
	tree(t.token, depth);
	strcpy(domain_name, "global");
	return es;
}

// <main_declaration>::=main'(' ')''{'<function_body>[return;]'}'
int main_declaration(int depth, int* _size)
{
	tree("main_declaration", depth++);
	int es = 0;
	Word t = wpop();
	Symbol sym("main", "void");
	strcpy(domain_name, "main");
	if (equals(t.type, "#")) return err(t, 1, "main函数未定义。");
	if (!equals(t.type, "ID")) return err(t, 1, "错误的main函数定义。");
	if (!equals(t.token, "main")) return err(t, 1, "错误的main函数定义。");
	sym.address = codes.size();
	symbols.insert(MakeStringVectorPair(domain_name));
	symbols[domain_name].push_back(sym);
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "(")) return err(t, 1, "缺少左括号:'('");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, ")")) return err(t, 1, "缺少右括号:')'");
	tree(t.token, depth);
	t = wpop();
	if (!equals(t.type, "{")) return err(t, 1, "缺少左花括号:'{'");
	tree(t.token, depth);
	es = function_body(depth, 0, 1, _size);
	if (es) return es;
	t = wpop();
	if (equals(t.type, "return"))
	{
		tree(t.token, depth);
		t = wpop();
		if (!equals(t.type, ";")) return err(t, 1, "缺少分号:';'");
		tree(t.token, depth);
	}
	codes.push_back(Code("RETURN"));
	if (!equals(t.type, "}")) return err(t, 1, "缺少右花括号:'}'");
	tree(t.token, depth);
	strcpy(domain_name, "global");
	return es;
}

// <program>::={<fun_declaration>|<declaration_stat>}<main_declaration>
// 全局变量的声明必须引用之前
int program(int depth)
{
	tree("program", depth++);
	int es = 0;
	codes.push_back(Code("BR"));
	Word t = words.front();
	strcpy(domain_name, "global");
	symbols.insert(MakeStringVectorPair(domain_name));
	symbols[domain_name].push_back(Symbol("global", "void", 0, GLOBAL_ADDRESS));
	symbols[domain_name].push_back(Symbol("__var__global1__", "int", 4, GLOBAL_ADDRESS));
	symbols[domain_name].push_back(Symbol("__var__global2__", "float", 4, 10004));
	while (equals(t.type, "float") || equals(t.type, "int") || equals(t.type, "void") || equals(t.type, "function"))
	{
		if (!equals(t.type, "function") && equals(words[1].type, "ID"))
			es = declaration_stat(depth);
		else
			es = fun_declaration(depth);
		if (es) return es;
		t = words.front();
	}
	int tsize = codes.size();
	sprintf(codes[0].operand1, "%d", tsize);
	es = main_declaration(depth);
	if (es) return es;
	if (words.size() - 1)
	{
		while (words.size() - 1)
		{
			t = wpop();
			printf("第%d行%d列，多余的语句。\n", t.row, t.col);
		}
		return (es = 1);
	}
	return es;
}

int main(int argc, char* argv[])
{
	int es, i, j, len, t;
	// 存储程序所有字符
	char* _words_;
	char infile[500], wordfile[500], codefile[500], symbolfile[500], basePath[400], codefile_suffix[10];
	system("color F0");
	// 切换编码为 UTF-8，防止中文乱码
	system("chcp 65001>nul");
	// 指定输出文件后缀名
	strcpy(codefile_suffix, "test");
	// 指定基本数据类型大小
	data_type_size["Integer"] = 4;
	data_type_size["Float"] = 4;
	data_type_size["int"] = 4;
	data_type_size["float"] = 4;
	data_type_size["int[]"] = 4;
	data_type_size["float[]"] = 4;

	data_type["Integer"] = INT;
	data_type["Float"] = FLOAT;
	data_type["int"] = INT;
	data_type["float"] = FLOAT;
	data_type["int[]"] = INT;
	data_type["float[]"] = FLOAT;
	//#define TEST
#ifdef TEST
	strcpy(infile, "D:\\LinYuFeng\\Documents\\Coder Life\\VSrepos\\4VirtualMachine\\4VirtualMachine\\in.txt");
#else
	if (argc < 2)
	{
		printf("please input the source file");
	}
	strcpy(infile, "D:\\LinYuFeng\\Documents\\Coder Life\\VSrepos\\4VirtualMachine\\4VirtualMachine\\in.txt");
#endif // TEST
	len = strlen(infile);
	for (i = len - 1; i != -1 && infile[i] != '\\'; i--)
		codefile[i] = infile[i];
	for (j = len - 3, t = 0, len = strlen(codefile_suffix); t < len; j++, t++)
		codefile[j] = codefile_suffix[t];
	if (i == -1)
		strcpy(basePath, ".\\");
	else
	{
		for (j = 0; j <= i; j++)
			basePath[j] = codefile[j] = infile[j];
		basePath[j] = '\0';
	}
	strcpy(wordfile, basePath);
	strcat(wordfile, "words.txt");
	strcpy(symbolfile, basePath);
	strcat(symbolfile, "symbols.txt");
	strcpy(codefile, basePath);
	strcat(codefile, "codes.txt");
#ifdef TEST
	OUTPUT = 1;
#else
	if (argc < 3 || !strcmp(argv[2], "-o"))
		OUTPUT = 1;
#endif // TEST
	inFile = fopen(infile, "r");
	if (OUTPUT)
		wordFile = fopen(wordfile, "w");
	TestRead(_words_);
	fclose(inFile);
	if (OUTPUT)
		printf("Cifa analysis start\n");
	TestScan(_words_);
	if (OUTPUT)
		fclose(wordFile);
	if (OUTPUT)
	{
		printf("\nCifa analysis over\n");
		printf("---------------------------------------------\n");
		printf("Yufa analysis start\n");
	}
	es = program(0);
	if (es)
		return 0;
	if (OUTPUT)
		printf("\nYufa analysis over\n");
	codeFile = fopen(codefile, "w");
	if (OUTPUT)
		symbolFile = fopen(symbolfile, "w");
	TestOut();
	fclose(codeFile);
	if (OUTPUT)
		fclose(symbolFile);
}
