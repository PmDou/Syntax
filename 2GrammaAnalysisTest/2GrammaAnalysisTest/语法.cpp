#include<stdio.h>   //my2
#include<ctype.h>
#include<conio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
int TESTscan;
int TESTpause;
char token[20], token1[40];//token数组
char Scanin[300], Scanout[300];//输入输出文件名
#define keywordSum 12

int errornum = 0;//错误编号
int n = 1;//行号
int M = 0;//错误
FILE* fin, * fout;// 指向文件的指针 
char ch;
int S;  //output的编号
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
	printf("请输入词法分析输出文件：");
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
	while (M == 0 && (strcmp(token, "int") == 0 || strcmp(token, "function") == 0)) {//当是function的时候
		while (M == 0 && (strcmp(token, "int") == 0)) {
			string a[20] = {  };
			M = declaration_list(a);
			g = 1;
		}
		while (M == 0 && (strcmp(token, "function") == 0)) {//当是function的时候
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
	if (strcmp(token, "ID"))//如果不是（
	{
		M = 14;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "("))//如果不是（
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
	if (strcmp(token, "ID")) {//如果不是ID
		M = 4;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if(strcmp(token, ",") == 0){
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	while (strcmp(token, "int") == 0) {//如果是int
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
	if (strcmp(token, "{"))//是否是{
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
	
	if (strcmp(token, "}"))//如果不是}
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
	if (strcmp(token, ";"))//如果不是;
	{
		M = 5;
		error();
		return M;
	}
	return M;
}
int declaration_list(string a[20]) {//<declaration_list>::={<declaration_stat>}声明序列=声明序列+声明语句||声明语句
	while (strcmp(token, "int") == 0) {//如果是int
		M = declaration_stat(a);
		if (M > 0) return(M);

	}
	return (M);
}
int declaration_stat(string a[20]) {//声明语句=int+变量 <declaration_stat>::=int ID; 
	int i = 1;
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (M>0&&strcmp(token, "ID")) {//如果不是ID
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
		if (M == 0 && strcmp(token, "[") == 0) {//如果是[就是进入数组判断
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
			if (M == 0 && strcmp(token, "NUM") == 0) {//如果是数字就是数组
				fscanf(fp, "%s%s\n", &token, &token1);
				printf("%s     %s\n", token, token1);
				if (M == 0 && strcmp(token, "]") == 0) {//如果是]就是数组
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
		if (M == 0 && strcmp(token, "=") == 0) {//如果是=就是初始化
			M = init_stat();
	}
		while (M == 0 && strcmp(token, ",") == 0) {//如果是逗号
			fscanf(fp, "%s%s\n", &token, &token1);
			printf("%s     %s\n", token, token1);
			if (M == 0 && strcmp(token, "ID")) {//如果不是ID
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
			if (M == 0 && strcmp(token, "=") == 0) {//如果是=就是初始化
				M = init_stat();
			}
		}
		if (M>0&&strcmp(token, ";")) {//如果不是分号
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
int statement_list() {//语句序列=语句序列+语句||不写  	<statement_list>::={<statement>}
	while (strcmp(token,"}")&& strcmp(token, "function")&& strcmp(token, "main")&& strcmp(token, "return")) {//当后面不是} 或function  时
		M = statement();
		if (M > 0) {
			return(M);
		}
	}
	return (M);
}
int statement() {//(8).	<statement>::=<if_stat>|<while_stat>|<for_stat>|<read_stat> | <write_stat> | <compound_stat> | <expression_stat> | < call _stat>   语句=if||for||while||read||write||复合语句||表达式语句
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
int if_stat() {//<if_stat>::= if ‘(‘<expr>’)’<statement>[else<statement>]
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (strcmp(token, "(")) {//如果不是左括号
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
int while_stat() {//<while_stat>::= while ‘(‘<expr >’)’ < statement > 
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
int for_stat() {//<for_stat>::= for’(‘<expr>;<expr>;<expr>’)’<statement>
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
int compound_stat() {//(<compound_stat>::=’{‘<statement_list>’}’
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
		if (strcmp(token, "ID")) {          //不是标识符 
			M = 4;             //设置错误代码 
			error();             //报错 
			return M;
		}
		fscanf(fp, "%s %s \n", &token, &token1);
		printf("%s      %s\n", token, token1);
		if (strcmp(token, "(")) {          //如果不是'(' 
			M = 6;             //设置错误代码 
			error();             //报错 
			return M;
		}
		fscanf(fp, "%s %s \n", &token, &token1);
		printf("%s      %s\n", token, token1);
		if (strcmp(token, ")")) {          //如果不是')' 
			M = 7;             //设置错误代码 
			error();             //报错 
			return M;
		}
		fscanf(fp, "%s %s \n", &token, &token1);
		printf("%s      %s\n", token, token1);
		if (strcmp(token, ";") == 0) {          //如果是';' 
			fscanf(fp, "%s %s \n", &token, &token1);
			printf("%s      %s\n", token, token1);
			return M;
		}
		else {                 //不是';' 
			M = 5;             //设置错误代码 
			error();             //报错 
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
	if (strcmp(token, "ID")) {//如果token不是ID
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
	if (M == 0 && strcmp(token, "{")) {//如果token不是{
		M = 2;
		error();
		return M;
	}
	fscanf(fp, "%s%s\n", &token, &token1);
	printf("%s     %s\n", token, token1);
	if (M==0&&strcmp(token, "case")) {//如果token不是case
		M = 10;
		error();
		return M;
	}
	while (strcmp(token, "case") == 0) {
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (M == 0 && strcmp(token, "NUM")) {//如果token不是:
			M = 11;
			error();
			return M;
		}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (M == 0 && strcmp(token, ":")) {//如果token不是:
			M = 11;
			error();
			return M;
		}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		if (M == 0 && strcmp(token, "{")) {//如果token不是{
			M = 2;
			error();
			return M;
		}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
		M = expression_stat();
		if (M == 0 && strcmp(token, "}")) {//如果token不是}
			M = 3;
			error();
			return M;
		}
		fscanf(fp, "%s%s\n", &token, &token1);
		printf("%s     %s\n", token, token1);
	}
	if (M == 0 && strcmp(token, "}")) {//如果token不是case
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
	if (strcmp(token, "ID") == 0) {//如果是ID 
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
	if (strcmp(token, "(") == 0 )//如果是（
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
		case 1: printf("\nERROR\t打开语法分析输入文件出错！\n"); break;  exit(0);    //打开输入文件出错 
		case 2: printf("\nERROR\t缺少{\n"); break;
		case 3: printf("\nERROR\t缺少}\n"); break;
		case 4: printf("\nERROR\t缺少标识符\n"); break;
		case 5: printf("\nERROR\t缺少分号\n"); break;
		case 6: printf("\nERROR\t缺少(\n"); break;
		case 7: printf("\nERROR\t缺少)\n"); break;
		case 8: printf("\nERROR\t缺少操作数\n"); break;
		case 10: printf("\nERROR\t 缺少case语句\n"); break;
		case 11: printf("\nERROR\t case语句缺少冒号\n"); break;
		case 12: printf("\nERROR\t case语句缺少判断条件\n"); break;
		case 13: printf("\nERROR\t 函数变量名重复\n"); break;
		case 14: printf("\nERROR\t 函数没有函数名\n"); break;
		case 16: printf("\nERROR\t 逗号后面没有变量\n"); break;
		case 17: printf("\nERROR\t 数组没有数字\n"); break;
		case 18: printf("\nERROR\t 数组没有]\n"); break;
	}
	return M;
}
int main() {
	TESTparse(); 
	printf("程序运行完成");
	return 0;
}