#include <stdio.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#define maxvartablep 1000
#define keywordSum 12
using namespace std;
const int max_len = 12;
char tree_map[100][2000];
int TESTscan;
string a_type;
int TESTpause;
char token[20], token1[40];//token����
char Scanin[300], Scanout[300];//��������ļ���

int errornum = 0;//������
int n = 1;//�к�
int M = 0;//����
FILE * fout;// ָ���ļ���ָ��
char ch;
int S;  //output�ı��
FILE* fp;
int g = 1;
string domain_mark = "Global";
struct {
	char name[8];
	int address;
	string domain;
	string type;
	bool jud;
}vartable[maxvartablep];
typedef struct Code
{
	char opt[10];    //������
	int operand;     //������
} Code;
int vartablep = 0, labelp = 0, datap = 0;
int labelmain = labelp++;
struct func {
	string func_list;
	int address;
}func_[100];

int funcp = 0;

int start;
int name_def(char* name, string domain, string type);
int TESTparse();
int program();
int fun_declaration(string a[20] );
int function_(string a[20]);
int main_declaration(string a[20]);
int function_body(string a[20]);
int  return_stat();
int declaration_list(string a[20]);
int declaration_stat(string a[20]);
int statement_list();
int statement();
int if_stat();
int while_stat();
int for_stat();
int write_stat();
int read_stat();
int compound_stat();
int expression_stat();
int call_stat();
int dowhile_stat();
int switch_stat();
int expression();
int bool_expr();
int additive_expr();
int term();
int factor();
int error();
int init_stat();
int lookup(char* name, int* paddress, string domain);
int flag = 0;
int func_def(char* name) {
	int es = 0;
	if (funcp > 100) {
		return 21;
	}
	for (int i = funcp - 1; i >= 0; i--) {
		if (func_[i].func_list == name) {
			es = 22;
			break;
		}
	}
	if (es > 0) {
		return es;
	}

	func_[funcp].func_list = name;
	func_[funcp].address = labelp;
	labelp++;
	labelp++;
	funcp++;
	return 0;
}
int lookup_func(string name, int* paddress) {
	int i, es = 0;
	for (i = 0; i < funcp; i++) {
		if (name == func_[i].func_list) {
			*paddress = func_[i].address;
			return es;
		}

	}
	es = 23;
	return es;
}
int name_def(char* name, string domain, string type) {
	int i, es = 0;
	if (vartablep >= maxvartablep) {
		return 21;
	}
	for (i = vartablep - 1; i >= 0; i--) {// ����Ϊi==0
		if (strcmp(vartable[i].name, name) == 0 && domain == vartable[i].domain) {
			es = 22;
			break;
		}
	}
	if (es > 0) {
		return es;
	}
	strcpy(vartable[vartablep].name, name);
	vartable[vartablep].domain = domain;
	vartable[vartablep].address = datap;
	vartable[vartablep].type = a_type;

	datap++;
	vartablep++;
	return es;
}
int lookup(char* name, int* paddress, string domain) {
	int i, es = 0;
	for (i = 0; i < vartablep; i++) {
		if (strcmp(vartable[i].name, name) == 0 && (vartable[i].domain == domain || vartable[i].domain == "Global")) {
			*paddress = vartable[i].address;
			return es;
		}

	}
	es = 23;
	return es;
}
int TESTparse() {
	printf("������ʷ���������ļ���(����·��):");
	cin >> Scanin;
	printf("�����������������ļ���(����·��):");
	cin >> Scanout;
	if ((fp = fopen(Scanin, "r")) == NULL) {
		M = 1;
		error();
		return 1;
	}
	fout = fopen(Scanout, "w");
	if (M == 0) {
		program();
	}
	return M;
}
int program() //(1).	<program> ::={fun_declaration }<main_declaration>
{
	fprintf(fout, "\tBR LABEL%d\n", labelmain);

	fscanf(fp, "%s%s%d\n", &token, &token1 ,&n);
	printf("%s     %s     %d\n", token, token1, n);
	while (M == 0 && (strcmp(token, "int") == 0 || strcmp(token, "function") == 0)) {//����function��ʱ��
		while (M == 0 && (strcmp(token, "int") == 0)) {
			string a[20] = {  };
			domain_mark = "Global";
			M = declaration_list(a);
			g = 1;
		}
		while (M == 0 && (strcmp(token, "function") == 0)) {//����function��ʱ��

			string a[20] = {  };
			M = fun_declaration(a);
			g = 1;
		}
	}
	fprintf(fout, "LABEL%d:\n", labelmain);

	if (M == 0 && strcmp(token, "main") == 0) {
		string a[20] = {  };
		M = main_declaration(a);
		g = 1;
	}
	fprintf(fout, "\tSTOP\n");
	return M;
}
string function_name;
int fun_declaration(string a[20]) {
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "ID"))//�������ID
	{
		M = 14;
		error();
		return M;
	}

	M = func_def(token1);
	function_name = token1;
	if (M > 0) {
		error();
		return M;
	}
	fprintf(fout, "LABEL%d:\n", labelp - 2);
	domain_mark = token1;
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "("))//������ǣ�
	{
		M = 6;
		error();
		return M;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "int") == 0) {

		function_(a);
	}
	if (M == 0 && strcmp(token, ")"))
	{
		M = 7;
		error();
		return M;
	}
	if (M > 0) {
		return(M);
	}
	function_body(a);
	return M;
}
int function_(string a[20]) {
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "ID")) {//�������ID
		M = 4;
		error();
		return M;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, ",") == 0) {

		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		while (strcmp(token, "int") == 0) {//�����int
			function_(a);
			if (M > 0) {
				error();
				return M;
			}
		}
	}
	return (M);
}
int main_declaration(string a[20]) {
	flag = 1;
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	domain_mark = "main";
	if (strcmp(token, "("))
	{
		M = 6;
		error();
		return M;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, ")")){
		M = 7;
		error();
		return M;
	}
	function_body(a);
}
int function_body(string a[20]) {
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "{"))//�Ƿ���{
	{
		M = 2;
		error();
		return M;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = declaration_list(a);
	if (M > 0) {
		error();
		return(M);
	}
	M = statement_list();
	if (M > 0) {
		error();
		return (M);
	}

	if (strcmp(token, "return") == 0) {
		M = return_stat();
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
	}

	if (strcmp(token, "}"))//�������{
	{
		M = 3;
		error();
		return M;
	}

	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	int address;
	lookup_func(function_name, &address);
	if (flag == 0) {
		fprintf(fout, "\tBR LABEL%d\n", address + 1);
	}


	return M;
}
int  return_stat() {
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);

	if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
	}
	else {
		M = 8;
		error();
		return M;
	}

	if (M > 0) {
		error();
		return M;
	}
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
	{
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);

		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			fscanf(fp, "%s%s%d\n", &token, &token1, &n);
			printf("%s     %s     %d\n", token, token1, n);

		}
		if (M > 0) {
			error();
			return M;
		}

	}
	if (M > 0) {
		return M;
	}

	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
	{
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);

		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			fscanf(fp, "%s%s%d\n", &token, &token1, &n);
			printf("%s     %s     %d\n", token, token1, n);

		}
		else {
			M = 8;
			error();
			return M;
		}
		if (M > 0) {
			return M;
		}

		while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
		{
			fscanf(fp, "%s%s%d\n", &token, &token1, &n);
			printf("%s     %s     %d\n", token, token1, n);

			if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
				fscanf(fp, "%s%s%d\n", &token, &token1, &n);
				printf("%s     %s     %d\n", token, token1, n);

				return M;
			}
			else {
				M = 8;
				error();
				return M;
			}
			if (M > 0) {
				return(M);
			}
		}
		return M;
		if (M > 0) {
			return(M);
		}
	}
	if (strcmp(token, ";"))//�������{
	{
		M = 5;
		error();
		return M;
	}
	return M;
}
int declaration_list(string a[20]) {//<declaration_list>::={<declaration_stat>}��������=��������+�������||�������
	while (strcmp(token, "int") == 0) {//�����int
		a_type = token;

		M = declaration_stat(a);
		if (M > 0) {
			return(M);
		}
	}
	return (M);
}
string markID;
int m_addr;
int declaration_stat(string a[20]) {//�������=int+���� <declaration_stat>::=int ID;
	int i = 1;
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (M > 0 && strcmp(token, "ID")) {//�������ID
		M = 4;
		error();
		return M;
	}
	//����
	M = name_def(token1, domain_mark, a_type);
	markID = token1;
	m_addr = datap - 1;
	if (M > 0) {
		error();
		return(M);
	}
	for (i = 0; i < 20; i++) {
		if (token1 == a[i]) {
			M = 13;
			error();
			return M;
		}
	}
	a[g] = token1;
	g++;
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (M == 0 && strcmp(token, "[") == 0) {//�����[���ǽ��������ж�
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		if (M == 0 && strcmp(token, "NUM") == 0) {//��������־�������
			fscanf(fp, "%s%s%d\n", &token, &token1, &n);
			printf("%s     %s     %d\n", token, token1, n);
			if (M == 0 && strcmp(token, "]") == 0) {//�����]��������
				fscanf(fp, "%s%s%d\n", &token, &token1, &n);
				printf("%s     %s     %d\n", token, token1, n);
			}
			else
			{
				M = 18;
				error();
				return M;
			}
		}
		else {
			M = 17;
			error();
			return M;
		}
	}
	if (M == 0 && strcmp(token, "=") == 0) {//�����=���ǳ�ʼ��
		vartable[m_addr].jud = 1;
		M = init_stat();
	}
	while (M == 0 && strcmp(token, ",") == 0) {//����Ƕ���
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		if (M == 0 && strcmp(token, "ID")) {//�������ID
			M = 16;
			error();
			return M;
		}

		M = name_def(token1, domain_mark, a_type);
		markID = token1;
		m_addr = datap - 1;
		if (M > 0) {
			error();
			return(M);
		}

		for (i = 0; i < 20; i++) {
			if (token1 == a[i]) {
				M = 13;
				error();
				return M;
			}
		}
		a[g] = token1;
		g++;
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s      %d\n", token, token1, n);

		if (M == 0 && strcmp(token, "=") == 0) {//�����=���ǳ�ʼ��
			vartable[m_addr].jud = 1;
			M = init_stat();
		}
	}
	if (M > 0 && strcmp(token, ";")) {//������Ƿֺ�
		M = 5;
		error();
		return M;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	return M;
}
int init_stat() {
	int init_mark;
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {

		int address;
		if (strcmp(token, "ID") == 0) {
			M = lookup(token1, &address, domain_mark);
			if (M > 0) {
				error();
				return M;
			}

			if (vartable[address].jud == 0) {
				M = 24;
				error();
				return M;
			}

			fprintf(fout, "\tLOAD%d\n", address);
		}
		if (strcmp(token, "NUM") == 0) {
			fprintf(fout, "\tLOADI%s\n", token1);
		}


		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
	}
	else {
		M = 8;
		error();
		return M;
	}
	if (M > 0) {
		return(M);
	}
	string token_st1, token_st2;
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
	{
		token_st1 = token;
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			int address;
			if (strcmp(token, "ID") == 0) {
				M = lookup(token1, &address, domain_mark);
				if (M > 0) {
					error();
					return M;
				}
				if (vartable[address].jud == 0) {
					M = 24;
					error();
					return M;
				}

				fprintf(fout, "\tLOAD%d\n", address);
			}
			if (strcmp(token, "NUM") == 0) {
				fprintf(fout, "\tLOADI%s\n", token1);
			}

			fscanf(fp, "%s%s%d\n", &token, &token1, &n);
			printf("%s     %s     %d\n", token, token1, n);

			if (token_st1 == "*") {

				fprintf(fout, "\tMULT\n");
			}
			if (token_st1 == "/") {
				fprintf(fout, "\tDIV\n");
			}
			//return M;
		}
		if (M > 0) {
			return(M);
		}
	}
	if (M > 0) {
		return(M);
	}
	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
	{
		token_st1 = token;

		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			int address;
			if (strcmp(token, "ID") == 0) {
				M = lookup(token1, &address, domain_mark);
				if (M > 0) {
					error();
					return M;
				}


				fprintf(fout, "\tLOAD%d\n", address);
			}
			if (strcmp(token, "NUM") == 0) {
				fprintf(fout, "\tLOADI%s\n", token1);
			}
			fscanf(fp, "%s%s%d\n", &token, &token1, &n);
			printf("%s     %s     %d\n", token, token1, n);
		}
		else {
			M = 8;
			error();
			return M;
		}
		if (M > 0) {
			return(M);
		}
		while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
		{
			token_st2 = token;

			fscanf(fp, "%s%s%d\n", &token, &token1, &n);
			printf("%s     %s     %d\n", token, token1, n);
			if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
				int address;
				if (strcmp(token, "ID") == 0) {
					M = lookup(token1, &address, domain_mark);
					if (M > 0) {
						error();
						return M;
					}
					fprintf(fout, "\tLOAD%d\n", address);
				}
				if (strcmp(token, "NUM") == 0) {
					fprintf(fout, "\tLOADI%s\n", token1);
				}
				fscanf(fp, "%s%s%d\n", &token, &token1, &n);
				printf("%s     %s     %d\n", token, token1, n);

				if (token_st2 == "*") {
					fprintf(fout, "\tMULT\n");
				}
				if (token_st2 == "/") {
					fprintf(fout, "\tDIV\n");
				}
			}
			else {
				M = 8;
				error();
				return M;
			}
			if (M > 0) {
				return(M);
			}
		}
		if (token_st1 == "+") {
			fprintf(fout, "\tADD\n");
		}
		if (token_st1 == "-") {
			fprintf(fout, "\tSUB\n");
		}
		if (M > 0) {
			return(M);
		}
	}
	fprintf(fout, "\tSTO\t%d\n", m_addr);
	return M;
}
int statement_list() {//�������=�������+���||��д  	<statement_list>::={<statement>}
	while (strcmp(token, "}") && strcmp(token, "function") && strcmp(token, "main") && strcmp(token, "return")) {//�����治��} ��function  ʱ
		M = statement();
		if (M > 0) {
			return(M);
		}
	}
	return (M);
}
int statement() {//(8).	<statement>::=<if_stat>|<while_stat>|<for_stat>|<read_stat> | <write_stat> | <compound_stat> | <expression_stat> | < call _stat>   ���=if||for||while||read||write||�������||���ʽ���
	if (M == 0 && strcmp(token, "if") == 0) {


		M = if_stat();
		return M;
	}
	if (M == 0 && strcmp(token, "while") == 0) {

		M = while_stat();
		return M;
	}
	if (M == 0 && strcmp(token, "for") == 0) {

		M = for_stat();
		return M;
	}
	if (M == 0 && strcmp(token, "read") == 0) {
		M = read_stat();
		return M;
	}
	if (M == 0 && strcmp(token, "write") == 0) {
		M = write_stat();
		return M;
	}
	if (M == 0 && strcmp(token, "{") == 0) {
		M = compound_stat();
		return M;
	}
	if (M == 0 && (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0)) {
		M = expression_stat();
		return M;
	}
	if (M == 0 && strcmp(token, "call") == 0) {

		M = call_stat();
		return M;
	}
	if (M == 0 && strcmp(token, "do") == 0) {
		M = dowhile_stat();
		return M;
	}
	if (M == 0 && strcmp(token, "switch") == 0) {
		M = switch_stat();
		return M;
	}
	return M;
}
int if_stat() {//<if_stat>::= if ��(��<expr>��)��<statement>[else<statement>]
	int label1, label2;
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);

	if (strcmp(token, "(")) {//�������������
		M = 6;
		error();
		return 0;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = expression();
	if (M > 0) {
		return(M);
	}

	if (strcmp(token, ")"))
	{
		return (M = 6);
	}
	label1 = labelp++;
	fprintf(fout, "\tBRF LABEL%d\n", label1);
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = statement();
	if (M > 0)
	{
		return M;
	}
	label2 = labelp++;
	fprintf(fout, "\tBR LABEL%d\n", label2);
	fprintf(fout, "LABEL%d:\n", label1);
	if (strcmp(token, "}")) {
		M = 2;
		return M;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "else") == 0) {
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		M = statement_list();

		if (strcmp(token, "}")) {
			M = 2;
			return M;
		}
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
	}

	fprintf(fout, "LABEL%d:\n", label2);

	return M;
}
int while_stat() {//<while_stat>::= while ��(��<expr >��)�� < statement >

	int label1, label2;
	label1 = labelp++;
	fprintf(fout, "LABEL%d:\n", label1);

	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);

	if (strcmp(token, "(")) {
		return (M = 6);
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = expression();
	if (M > 0) {
		return M;
	}
	if (strcmp(token, ")")) {
		M = 7;
		error();
		return M;
	}
	label2 = labelp++;
	fprintf(fout, "\tBRF LABEL%d\n", label2);

	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = statement();
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);

	fprintf(fout, "\tBR LABEL%d\n", label1);
	fprintf(fout, "LABEL%d:\n", label2);

	return M;
}
int for_stat() {//<for_stat>::= for��(��<expr>;<expr>;<expr>��)��<statement>

	int label1, label2, label3, label4;


	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);

	if (strcmp(token, "("))
	{
		return (M = 6);
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = expression();
	if (M > 0) return(M);
	fprintf(fout, "\tPOP\n");

	if (strcmp(token, ";"))return (M = 5);
	label1 = labelp++;
	fprintf(fout, "LABEL%d:\n", label1);
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = expression();
	if (M > 0) {
		return(M);
	}
	label2 = labelp++;
	fprintf(fout, "\tBRF LABEL%d\n", label2);
	label3 = labelp++;
	fprintf(fout, "\tBR LABEL%d\n", label3);
	if (strcmp(token, ";")) {
		return (M = 5);
	}
	label4 = labelp++;
	fprintf(fout, "LABEL%d:\n", label4);

	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = expression();
	if (M > 0) return(M);
	fprintf(fout, "\tPOP\n");
	fprintf(fout, "\tLR LABEL%d\n", label1);
	if (strcmp(token, ")")) {
		return (M = 7);
	}
	fprintf(fout, "LABEL%d:\n", label3);
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = statement();
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	fprintf(fout, "\tBR LABEL%d\n", label4);
	fprintf(fout, "LABEL%d:\n", label2);
	return M;
}
int write_stat() {//<write_stat>::=write <expr>;
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = expression();
	if (M > 0) {
		return(M);
	}
	if (strcmp(token, ";")) {
		return (M = 5);
	}
	//
	fprintf(fout, "\tOUT\n");

	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	return M;
}
int read_stat() {//<read_stat>::=read ID;
	int address;
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "ID")) 	return (M = 4);
	M = lookup(token1, &address, domain_mark);
	if (M > 0) {
		return M;
	}

	fprintf(fout, "\tIN\n");
	fprintf(fout, "\tSTO\t%d\n", address);
	fprintf(fout, "\tPOP\n");

	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, ";")) {
		return (M = 5);
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	return M;
}
int compound_stat() {//(<compound_stat>::=��{��<statement_list>��}��
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	M = statement_list();
	return M;
}
int expression_stat() {//<expression_stat>::=< expr >;|;
	if (strcmp(token, ";") == 0) {
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		return M;
	}
	M = expression();
	if (M > 0) {
		return M;
	}

	fprintf(fout, "\tPOP\n");
	if (M == 0 && (strcmp(token, ";") == 0)) {
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);

		return (M);
	}
	else {
		M = 5;
		error();
		return M;
	}

}
int  call_stat() {
	start = labelp++;
	fscanf(fp, "%s %s%d\n", &token, &token1, &n);
	printf("%s      %s     %d\n", token, token1, n);
	if (strcmp(token, "ID")) {          //���Ǳ�ʶ��
		M = 4;             //���ô������
		error();             //����
		return M;
	}
	int address;
	M = lookup_func(token1, &address);

	if (M > 0) {
		error();
		return M;
	}



	fscanf(fp, "%s %s %d\n", &token, &token1, &n);
	printf("%s      %s     %d\n", token, token1, n);
	if (strcmp(token, "(")) {          //�������'('
		M = 6;             //���ô������
		error();             //����
	}
	fscanf(fp, "%s %s %d\n", &token, &token1, &n);
	printf("%s      %s     %d\n", token, token1, n);
	if (strcmp(token, ")")) {          //�������')'
		M = 7;             //���ô������
		error();             //����
	}
	fscanf(fp, "%s %s %d\n", &token, &token1, &n);
	printf("%s      %s     %d\n", token, token1, n);
	if (strcmp(token, ";") == 0) {          //�����';'
		fscanf(fp, "%s %s %d\n", &token, &token1, &n);
		printf("%s      %s     %d\n", token, token1, n);
	}
	else {                 //����';'
		M = 5;             //���ô������
		error();             //����
		return M;
	}

	fprintf(fout, "\tBR LABEL%d\n", address);
	fprintf(fout, "LABEL%d:\n", address + 1);
	return M;
}
int dowhile_stat() {
	int label1, label2;

	label1 = labelp++;
	label2 = labelp++;
	fprintf(fout, "LABEL%d:\n", label1);
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s      %d\n", token, token1, n);
	M = statement();
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (M == 0 && strcmp(token, "while") == 0) {
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);

		if (strcmp(token, "(")) {
			return (M = 6);
		}
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		M = expression();
		if (M > 0) {
			return M;
		}
		fprintf(fout, "\tBRF LABEL%d\n", label2);
		fprintf(fout, "\tBR LABEL%d\n", label1);

		if (strcmp(token, ")")) return (M = 7);
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		if (strcmp(token, ";")) {
			return (M = 5);
		}
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
	}
	fprintf(fout, "LABEL%d:\n", label2);
	return M;
}
int switch_stat() {
	int label1, label2;
	label1 = labelp++;
	label2 = labelp++;
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "(")) return (M = 6);

	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (strcmp(token, "ID")) {//���token����ID
		M = 4;
		error();
		return M;
	}
	int address;
	M = lookup(token1, &address, domain_mark);
	if (M > 0) {
		error();
		return M;
	}



	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (M > 0) {
		return M;
	}
	if (strcmp(token, ")")) {
		M = 7;
		error();
		return M;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s    %d\n", token, token1, n);
	if (M == 0 && strcmp(token, "{")) {//���token����{
		M = 2;
		error();
		return M;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	if (M == 0 && strcmp(token, "case")) {//���token����case
		M = 10;
		error();
		return M;
	}
	while (strcmp(token, "case") == 0) {
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		if (M == 0 && strcmp(token, "NUM")) {//���token����NUM
			M = 11;
			error();
			return M;
		}
		fprintf(fout, "\tLOAD%d\n", address);
		fprintf(fout, "\tLOADI%s\n", token1);
		fprintf(fout, "\tEQ\n");
		fprintf(fout, "\tBRF LABEL%d\n", label1);
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		if (M == 0 && strcmp(token, ":")) {//���token����:
			M = 11;
			error();
			return M;
		}
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		if (M == 0 && strcmp(token, "{")) {//���token����{
			M = 2;
			error();
			return M;
		}
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		M = expression_stat();
		if (M == 0 && strcmp(token, "}")) {//���token����}
			M = 3;
			error();
			return M;
		}
		fprintf(fout, "\tBR LABEL%d\n", label2);
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);


		fprintf(fout, "LABEL%d:\n", label1);
		label1 = labelp++;
	}
	if (M == 0 && strcmp(token, "}")) {//���token����case
		M = 3;
		error();
		return M;
	}
	fscanf(fp, "%s%s%d\n", &token, &token1, &n);
	printf("%s     %s     %d\n", token, token1, n);
	fprintf(fout, "LABEL%d:\n", label2);
	return M;
}
int expression() {//< expr >::= ID= <bool_expr>|<bool_expr>
	int fileadd;
	char token2[20], token3[40];
	if (strcmp(token, "ID") == 0) {//�����ID
		fileadd = ftell(fp);

		fscanf(fp, "%s%s%d\n", &token2, &token3, &n);
		if (strcmp(token2, "=") == 0) {

			int address;
			M = lookup(token1, &address, domain_mark);
			if (M > 0) {
				return M;
			}
			fscanf(fp, "%s%s%d\n", &token, &token1, &n);
			printf("%s     %s     %d\n", token, token1, n);
			M = bool_expr();

			if (M > 0) {
				return M;
			}

			fprintf(fout, "\tSTO\t%d\n", address);
		}
		else {
			fseek(fp, fileadd, 0);
			M = bool_expr();
			if (M > 0) {
				return M;
			}
		}
	}
	else {
		M = bool_expr();
	}

	return M;
}
int bool_expr() {//<bool_expr>::=<additive_expr>{(>|<|>=|<=|==|!=)<additive_expr>}
	string store;
	M = additive_expr();
	if (M > 0) {
		return M;
	}
	if (strcmp(token, ">") == 0 || strcmp(token, ">=") == 0 || strcmp(token, "<") == 0
		|| strcmp(token, "<=") == 0 || strcmp(token, "==") == 0 || strcmp(token, "!=") == 0) {
		store = token;
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		M = additive_expr();
		if (store == ">") {


			fprintf(fout, "\tGT\n");
		}
		if (store == ">=") {
			fprintf(fout, "\tGE\n");
		}
		if (store == "<") {
			fprintf(fout, "\tLES\n");
		}
		if (store == "<=") {
			fprintf(fout, "\tLE\n");
		}
		if (store == "==") {
			fprintf(fout, "\tEQ\n");
		}
		if (store == "!=") {
			fprintf(fout, "\tNOTEQ\n");
		}
		if (M > 0) {
			return M;
		}

	}
	return M;
}
int additive_expr() {//< additive_expr>::=<term>{(+|-)< term >}
	M = term();
	if (M > 0) {
		return M;
	}
	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
	{
		char token4[20];
		strcpy(token4, token);
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		M = term();
		if (M > 0) {
			return M;
		}

		if (strcmp(token4, "+") == 0) {
			fprintf(fout, "\tADD\n");
		}
		if (strcmp(token, "-") == 0) {
			fprintf(fout, "\tSUB\n");
		}


	}
	return M;
}
int term() {//< term >::=<factor>{(*| /)< factor >}
	M = factor();
	if (M > 0) {
		return M;
	}
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
	{
		char token4[20];
		strcpy(token4, token);


		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		M = factor();
		if (M > 0) {
			return M;
		}

		if (strcmp(token4, "*") == 0) {
			fprintf(fout, "\tMULT\n");
		}
		if (strcmp(token, "-") == 0) {
			fprintf(fout, "\tDIV\n");
		}
	}
	return M;
}
int factor() {
	if (strcmp(token, "(") == 0)//����ǣ�
	{
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
		M = expression();
		if (strcmp(token, ")") == 0) {
			return (M = 7);
		}
		fscanf(fp, "%s%s%d\n", &token, &token1, &n);
		printf("%s     %s     %d\n", token, token1, n);
	}
	else {
		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			if (strcmp(token, "ID") == 0) {
				int address;
				M = lookup(token1, &address, domain_mark);
				if (M > 0) {
					return M;
				}

				if (vartable[address].jud == 0) {
					M = 24;
					error();
					return M;
				}

				fprintf(fout, "\tLOAD%d\n", address);
			}
			if (strcmp(token, "NUM") == 0) {
				fprintf(fout, "\tLOADI%s\n", &token1);
			}

			fscanf(fp, "%s%s%d\n", &token, &token1, &n);
			printf("%s     %s     %d\n", token, token1, n);
			return M;
		}
		else {
			M = 8;
			error();
			return M;
		}
	}
	return M;
}
int error() {
	switch (M) {
	case 1: 	printf("\nERROR\t���﷨���������ļ�����\n"); break;  exit(0);                           //�������ļ�����
	case 2: printf("\nERROR\t��%d��ȱ��{\n", n); break;
	case 3: printf("\nERROR\t��%d��ȱ��}\n", n); break;
	case 4: printf("\nERROR\t��%d��ȱ�ٱ�ʶ��\n", n); break;
	case 5: printf("\nERROR\t��%d��ȱ�ٷֺ�\n", n); break;
	case 6: printf("\nERROR\t��%d��ȱ��(\n", n); break;
	case 7: printf("\nERROR\t��%d��ȱ��)\n", n); break;
	case 8: printf("\nERROR\t��%d��ȱ�ٲ�����\n", n); break;
	case 10: printf("\nERROR\t ��%d��ȱ��case���\n", n); break;
	case 11: printf("\nERROR\t ��%d��case���ȱ��ð��\n", n); break;
	case 12: printf("\nERROR\t ��%d��case���ȱ���ж�����\n", n); break;
	case 13: printf("\nERROR\t ��%d�к����������ظ�\n", n); break;
	case 14: printf("\nERROR\t ��%d�к���û�к�����\n", n); break;
	case 16: printf("\nERROR\t ��%d�ж��ź���û�б���\n", n); break;
	case 17: printf("\nERROR\t ��%d������û������\n", n); break;
	case 18: printf("\nERROR\t ��%d������û��]\n", n); break;
	case 21: printf("\nERROR\t ��%d�з��ű����\n", n); break;
	case 22: printf("\nERROR\t ��%d�б����ظ�����\n", n); break;
	case 23: printf("\nERROR\t ��%d�б���δ����\t%s\n", n, token1); break;
	case 24: printf("\nERROR\t ��%d�б���δ��ֵ\t%s\n",n, token1); break;
	}
	return M;
}
int main() {
	TESTparse();
	printf("�����������\n");
	printf("���ű�\n");
	for (int i = 0; i < vartablep; i++) {
		printf("\t%s\t%d\t", vartable[i].name, vartable[i].address);
		cout << vartable[i].domain << '\t';
//		cout << vartable[i].jud << '\t';
		cout << '\t' << vartable[i].type << endl;
	}
	printf("������\n");
	for (int i = 0; i < funcp; i++) {
		cout << func_[i].func_list << '\t' << func_[i].address << endl;
	}
	fclose(fp);
	fclose(fout);
	return 0;
}
