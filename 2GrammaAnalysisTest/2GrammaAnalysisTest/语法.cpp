#include<stdio.h>   //my2
#include<ctype.h>
#include<conio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
int TESTscan;
int TESTpause;
char token[20], token1[40];//token����
char Scanin[300], Scanout[300];//��������ļ���
#define keywordSum 12

int errornum = 0;//������
int n = 1;//�к�
int M = 0;//����
FILE* fin, * fout;// ָ���ļ���ָ�� 
char ch;
int S;  //output�ı��
FILE* fp;
int g = 1;
int TESTparse();
int program();
int fun_declaration(string a[20]);
int function(string a[20]);
int main_declaration(string a[20]);
int function_body(string a[20]);
int return_stat();
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
int doile_stat();
int switch_stat();
int expression();
int bool_expr();
int additive_expr();
int term();
int factor();
int error();
int init_stat();
int TESTparse() {
	printf("������ʷ���������ļ���");
	cin >> Scanout;
	if ((fp = fopen(Scanout, "r"))== NULL) {//if ((fin = fopen(Scanin, "r")) == NULL)
		M = 1;
		error();
		return M;
	}
	if(M==0){
	program();
	fclose(fp);
	}
	return M;
}
int program() //(1).	<program> ::={fun_declaration }<main_declaration>
{
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	while (M == 0 && (strcmp(token, "int") == 0 || strcmp(token, "function") == 0)) {//����function��ʱ��
		while (M == 0 && (strcmp(token, "int") == 0)) {
			string a[20] = {  };
			M = declaration_list(a);
			g = 1;
		}
		while (M == 0 && (strcmp(token, "function") == 0)) {//����function��ʱ��
			string a[20] = {  };
			M = fun_declaration(a);
			g = 1;
		}
}

	if (M == 0 && strcmp(token, "main") == 0) {
		string a[20] = {  };
	M = main_declaration( a );
		g = 1;
	}
	return M;
}
int fun_declaration(string a[20]) {
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "ID"))//������ǣ�
	{
		M = 14;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "("))//������ǣ�
	{
		M = 6;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "int") == 0) {
		function(a);
	}
	if (M==0&&strcmp(token, ")"))
	{
		M = 7;
		error();
		return M;
	}
	if (M > 0)return M;
	function_body(a);
	return M;
}
int function(string a[20]) {
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "ID")) {//�������ID
		M = 4;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if(strcmp(token, ",") == 0){
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	while (strcmp(token, "int") == 0) {//�����int
		function(a);
		if (M > 0) {
			error();
			return M;
		}
	}
	}
	return (M);
}
int main_declaration(string a[20]) {
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "("))
	{
		M = 6;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, ")"))
	{
		M = 7;
		error();
		return M;
	}
	function_body(a);
}
int function_body(string a[20]) {
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "{"))//�Ƿ���{
	{
		M = 2;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = declaration_list(a);
	if (M > 0)return(M);
	M = statement_list();
	if (M > 0)return (M);
	if (strcmp(token, "return") == 0) {
		M  = return_stat();
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	}
	
	if (strcmp(token, "}"))//�������}
	{
		M = 3;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	return M;
}
int  return_stat() {
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
	}
	else {
		M = 8;
		error();
		return M;
	}
	if (M > 0)return M;
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
	{
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
		}
		if (M > 0)return M;
	}
	if (M > 0)return M;
	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
	{
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
		}
		else {
			M = 8;
			error();
			return M;
		}
		if (M > 0)return M;
		while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
		{
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
			if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
				fscanf(fp, "%s%s\n", &token, &token1);
				printf("%s     %s\n", token, token1);
			}
			else {
				M = 8;
				error();
				return M;
			}
			if (M > 0)return M;
		}
		if (M > 0)return M;
	}
	if (strcmp(token, ";"))//�������;
	{
		M = 5;
		error();
		return M;
	}
	return M;
}
int declaration_list(string a[20]) {//<declaration_list>::={<declaration_stat>}��������=��������+�������||�������
	while (strcmp(token, "int") == 0) {//�����int
		M = declaration_stat(a);
		if (M > 0) return(M);

	}
	return (M);
}
int declaration_stat(string a[20]) {//�������=int+���� <declaration_stat>::=int ID; 
	int i = 1;
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (M>0&&strcmp(token, "ID")) {//�������ID
		M = 4;
		error();
		return M;
	}
	for (i = 1; i <= 20; i++) {
		if (token1 == a[i]) {
			M = 13;
			error();
			return M;
		}
	}
	a[g] = token1;
	g++;
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (M == 0 && strcmp(token, "[") == 0) {//�����[���ǽ��������ж�
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
			if (M == 0 && strcmp(token, "NUM") == 0) {//��������־�������
				fscanf(fp, "%s%s\n", &token, &token1);
				printf("%s     %s\n", token, token1);
				if (M == 0 && strcmp(token, "]") == 0) {//�����]��������
					fscanf(fp, "%s%s\n", &token, &token1);
					printf("%s     %s\n", token, token1);
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
			M = init_stat();
	}
		while (M == 0 && strcmp(token, ",") == 0) {//����Ƕ���
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
			if (M == 0 && strcmp(token, "ID")) {//�������ID
				M = 16;
				error();
				return M;
			}
			for (i = 1; i <= 20; i++) {
				if (token1 == a[i]) {
					M = 13;
					error();
					return M;
				}
			}
			a[g] = token1;
			g++;
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
			if (M == 0 && strcmp(token, "=") == 0) {//�����=���ǳ�ʼ��
				M = init_stat();
			}
		}
		if (M>0&&strcmp(token, ";")) {//������Ƿֺ�
			M = 5;
			error();
			return M;
		}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		return M;
}
int init_stat() {
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
	}
	else {
		M = 8;
		error();
		return M;
	}
	if (M > 0)return M;
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
	{
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
			return M;
		}
		if (M > 0)return M;
	}
	if (M > 0)return M;
	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
	{
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
		}
		else {
			M = 8;
			error();
			return M;
		}
		if (M > 0)return M;
		while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
		{
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
			if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
				fscanf(fp, "%s%s\n", &token, &token1);
				printf("%s     %s\n", token, token1);
				return M;
			}
			else {
				M = 8;
				error();
				return M;
			}
			if (M > 0)return M;
		}
		return M;
		if (M > 0)return M;
	}
	return M;
}
int statement_list() {//�������=�������+���||��д  	<statement_list>::={<statement>}
	while (strcmp(token,"}")&& strcmp(token, "function")&& strcmp(token, "main")&& strcmp(token, "return")) {//�����治��} ��function  ʱ
		M = statement();
		if (M > 0) {
			return(M);
		}
	}
	return (M);
}
int statement() {//(8).	<statement>::=<if_stat>|<while_stat>|<for_stat>|<read_stat> | <write_stat> | <compound_stat> | <expression_stat> | < call _stat>   ���=if||for||while||read||write||�������||���ʽ���
	if (M == 0 && strcmp(token, "if") == 0) { M = if_stat(); return M;
	}
	if (M == 0 && strcmp(token, "while") == 0) { M = while_stat(); return M;
	}
	if (M == 0 && strcmp(token, "for") == 0) { M = for_stat(); return M;
	}
	if (M == 0 && strcmp(token, "read") == 0) { M = read_stat(); return M;
	}
	if (M == 0 && strcmp(token, "write") == 0) { M = write_stat(); return M;
	}
	if (M == 0 && strcmp(token, "{") == 0) { M = compound_stat(); return M;
	}
	if (M == 0 && (strcmp(token, "ID")==0||strcmp(token, "NUM")==0|| strcmp(token, "(") == 0)) { M = expression_stat(); return M;
	}
	if (M == 0 && strcmp(token, "call") == 0) { M = call_stat(); return M;
	}
	if (M == 0 && strcmp(token, "do") == 0) { M = doile_stat(); return M;
	}
	if (M == 0 && strcmp(token, "switch") == 0) { M = switch_stat(); return M;
	}
	return M;
}
int if_stat() {//<if_stat>::= if ��(��<expr>��)��<statement>[else<statement>]
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "(")) {//�������������
		M = 6;
		error();
		return 0;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = expression();
	if (M > 0) {
		return(M);
	}
	if (strcmp(token, ")")) 
	{
		return (M = 6);
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = statement();
	if (M > 0)
	{
		return M;
	}
	if (strcmp(token, "}")) {
		M = 2;
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "else") == 0) {
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		M = statement_list();
		if (strcmp(token, "}")) {
					M = 2;
					return M;
			}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
	}	

	return M;
}
int while_stat() {//<while_stat>::= while ��(��<expr >��)�� < statement > 
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "("))return (M = 6);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = expression();
	if (M > 0) { return M; }
	if (strcmp(token, ")")) { 
		M = 7;
		error();
		return M ; }
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	 M = statement();
	 fscanf(fp, "%s%s\n", &token, &token1);
	 printf("%s     %s\n", token, token1);
	return M;
}
int for_stat() {//<for_stat>::= for��(��<expr>;<expr>;<expr>��)��<statement>
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "("))
	{
		return (M = 6);
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1); 
	M = expression();
	if (M > 0) return M;
	if (strcmp(token, ";"))
	{
		return (M = 5);
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = expression();
	if (M > 0) return M;
	if (strcmp(token, ";"))    return (M = 5);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = expression();
	if (M > 0) return M;
	if (strcmp(token, ")"))    return (M = 7);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = statement();
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	return M;
}
int write_stat() {//<write_stat>::=write <expr>; 
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = expression();
	if (M > 0) return M;
	if (strcmp(token, ";"))    return (M = 5);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	return M;
}
int read_stat() {//<read_stat>::=read ID; 
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "ID"))    return (M = 4);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, ";"))    return (M = 5);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	return M;
}
int compound_stat() {//(<compound_stat>::=��{��<statement_list>��}��
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = statement_list();
	return M;
}
int expression_stat() {//<expression_stat>::=< expr >;|; 
	if (strcmp(token, ";") == 0) {
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		return (M = 5);
	}
	M = expression();
	if (M > 0) { return M; }
	if (M==0&&(strcmp(token, ";") == 0)) {
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		return (M);
	}
	else {
		M = 5;
		error();
		return M;
	}
}
int  call_stat() {
		fscanf(fp, "%s %s\n", &token, &token1);
		printf("%s      %s\n", token, token1);
		if (strcmp(token, "ID")) {          //���Ǳ�ʶ�� 
			M = 4;             //���ô������ 
			error();             //���� 
			return M;
		}
		fscanf(fp, "%s %s \n", &token, &token1);
		printf("%s      %s\n", token, token1);
		if (strcmp(token, "(")) {          //�������'(' 
			M = 6;             //���ô������ 
			error();             //���� 
			return M;
		}
		fscanf(fp, "%s %s \n", &token, &token1);
		printf("%s      %s\n", token, token1);
		if (strcmp(token, ")")) {          //�������')' 
			M = 7;             //���ô������ 
			error();             //���� 
			return M;
		}
		fscanf(fp, "%s %s \n", &token, &token1);
		printf("%s      %s\n", token, token1);
		if (strcmp(token, ";") == 0) {          //�����';' 
			fscanf(fp, "%s %s \n", &token, &token1);
			printf("%s      %s\n", token, token1);
			return M;
		}
		else {                 //����';' 
			M = 5;             //���ô������ 
			error();             //���� 
			return M;
		}
		return M;
}
int doile_stat() {
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = statement();
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (M == 0 && strcmp(token, "while") == 0) {
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "("))return (M = 6);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	M = expression();
	if (M > 0) return M;
	if (strcmp(token, ")"))return (M = 7);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, ";"))return (M = 5);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	}
	return M;
}
int switch_stat() {
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "("))return (M = 6);
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "ID")) {//���token����ID
		M = 4;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (M > 0) return M;
	if (strcmp(token, ")")) {
		M = 7;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (M == 0 && strcmp(token, "{")) {//���token����{
		M = 2;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (M==0&&strcmp(token, "case")) {//���token����case
		M = 10;
		error();
		return M;
	}
	while (strcmp(token, "case") == 0) {
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (M == 0 && strcmp(token, "NUM")) {//���token����:
			M = 11;
			error();
			return M;
		}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (M == 0 && strcmp(token, ":")) {//���token����:
			M = 11;
			error();
			return M;
		}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (M == 0 && strcmp(token, "{")) {//���token����{
			M = 2;
			error();
			return M;
		}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		M = expression_stat();
		if (M == 0 && strcmp(token, "}")) {//���token����}
			M = 3;
			error();
			return M;
		}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
	}
	if (M == 0 && strcmp(token, "}")) {//���token����case
		M = 3;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	return M;
}
int expression() {//< expr >::= ID= <bool_expr>|<bool_expr> 
	int fileadd;
	char token2[20], token3[40];
	if (strcmp(token, "ID") == 0) {//�����ID 
		fileadd = ftell(fp);
		fscanf(fp, "%s%s\n", &token2, &token3);
		printf("%s     %s\n", token2, token3);
		if (strcmp(token2, "=") == 0) {
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
			M = bool_expr();
			if (M > 0) return M;
		}
		else {
			fseek(fp, fileadd, 0);
			printf("%s     %s\n", token, token1);
			M = bool_expr();
			if (M > 0)return M;
		}
	}else M= bool_expr();
	return M;
}
int bool_expr() {//<bool_expr>::=<additive_expr>{(>|<|>=|<=|==|!=)<additive_expr>}
	M = additive_expr();
	if (M > 0)return M;
	if (strcmp(token, ">") == 0||strcmp(token,">=")==0||strcmp(token,"<")==0||strcmp(token, "<=") == 0 || strcmp(token, "==") == 0 || strcmp(token, "!=") == 0){
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		M = additive_expr();
		if (M > 0)return M;
	}
	return M;
}
int additive_expr() {//< additive_expr>::=<term>{(+|-)< term >}
	M = term();
	if(M>0)return M;
	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
	{
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		M = term();
		if (M > 0)return M;
	}
	return M;
}
int term() {//< term >::=<factor>{(*| /)< factor >}
	M = factor();
	if (M > 0)return M;
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
	{
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		M = factor();
		if (M > 0)return M;
	}
	return M;
}
int factor() {
	if (strcmp(token, "(") == 0 )//����ǣ�
	{
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		M = expression();
		if (strcmp(token, ")") == 0)return (M=7);
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
	}
	else {
		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0){
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
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
		case 1: printf("\nERROR\t���﷨���������ļ�����\n"); break;  exit(0);    //�������ļ����� 
		case 2: printf("\nERROR\tȱ��{\n"); break;
		case 3: printf("\nERROR\tȱ��}\n"); break;
		case 4: printf("\nERROR\tȱ�ٱ�ʶ��\n"); break;
		case 5: printf("\nERROR\tȱ�ٷֺ�\n"); break;
		case 6: printf("\nERROR\tȱ��(\n"); break;
		case 7: printf("\nERROR\tȱ��)\n"); break;
		case 8: printf("\nERROR\tȱ�ٲ�����\n"); break;
		case 10: printf("\nERROR\t ȱ��case���\n"); break;
		case 11: printf("\nERROR\t case���ȱ��ð��\n"); break;
		case 12: printf("\nERROR\t case���ȱ���ж�����\n"); break;
		case 13: printf("\nERROR\t �����������ظ�\n"); break;
		case 14: printf("\nERROR\t ����û�к�����\n"); break;
		case 16: printf("\nERROR\t ���ź���û�б���\n"); break;
		case 17: printf("\nERROR\t ����û������\n"); break;
		case 18: printf("\nERROR\t ����û��]\n"); break;
	}
	return M;
}
int main() {
	TESTparse(); 
	printf("�����������");
	return 0;
}