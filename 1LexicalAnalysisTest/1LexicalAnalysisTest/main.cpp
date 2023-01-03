#include <iostream>
#include <algorithm>
#include <set>
#define keywordsum 10
#define oformat "%s\t %s\n"
using namespace std;
int row = 1, column = 0;

//初始化，输入并打开源程序文件和目标程序文件，初始化保留字表。
int initscanner(FILE** fin, FILE** fout, char keyword[keywordsum][10], char* singleword, char* doubleword) {
	char ifn[50] = "Test.txt", ofn[50] = "lexical analysis.txt";//输入文件名source.txt，输出文件名lexical analysis.txt

	if ((*fin = fopen(ifn, "r")) == NULL) {
		printf("Cannot open the input file!");
		return (1);
	}
	if ((*fout = fopen(ofn, "w")) == NULL) {
		printf("Cannot open the output file!");
		return (1);
	}

	//初始化保留字表，单字符表，双字符表
	string kw[keywordsum] = { "if","else","for","while","call","int","read","write","function","main" };
	sort(kw, kw + keywordsum);//将保留字表排序
	int len;
	for (int i = 0; i < keywordsum; i++) {
		len = kw[i].length();
		kw[i].copy(keyword[i], len);
		keyword[i][len] = '\0';
	}
	strcpy(singleword, "+-*(){};,:");
	strcpy(doubleword, "><=!");
}
//识别保留字和标识符
int r_alpha(char* ch, FILE** fin, FILE** fout, char keyword[keywordsum][10], set<string>* idlist) {
	int i = 0;
	char token[30], kw[30];
	while (((*ch >= 65 && *ch <= 90) or (*ch >= 97 && *ch <= 122) or (*ch >= 48 && *ch <= 57)) && *ch != EOF) {
		//组合标识符直到遇到不是字符或数字
		token[i++] = *ch;
		*ch = getc(*fin);
	}
	token[i] = '\0';
	strcpy(kw, token);
	strlwr(kw);//将 token 值复制后全部转换为小写

	/*/遍历查找关键字
	for (int j = 0; j < keywordsum; j++)
		if (strcmp(kw, keyword[j]) == 0){
			fprintf(*fout, oformat, kw, kw);
			return 0;
		}
	/*///折半查找关键字
	int m = 0, n = keywordsum, s;
	while (m <= n) {
		s = strcmp(kw, keyword[(m + n) / 2]);
		if (s == 0) {
			fprintf(*fout, oformat, kw, kw);
			return 0;
		}
		else if (s > 0)
			m = (m + n) / 2 + 1;
		else
			n = (m + n) / 2 - 1;
	}
	//*/

	fprintf(*fout, oformat, "id", token);
	idlist->insert(token);
	return 0;
}
//识别整数或实数
int r_number(char* ch, FILE** fin, FILE** fout) {
	int i = 0;
	bool isreal = false;
	char token[40];
	while (((*ch >= 48 && *ch <= 57) || *ch == '.') && *ch != EOF) {//组合标识符直到遇到不是数字
		if (*ch == '.')
			if (!isreal)isreal = true;
			else return -3;//如果一个数字中出现两个小数点则返回非法数字代码-3
		token[i++] = *ch;
		*ch = getc(*fin);
	}
	token[i] = '\0';

	if (isreal)
		fprintf(*fout, oformat, "NUM", token);
	else
		fprintf(*fout, oformat, "NUM", token);
	if ((*ch >= 65 && *ch <= 90) or (*ch >= 97 && *ch <= 122))return -2;//如果后面一个字符为字母则返回非法字符错误代码-2
	return 0;
}
//处理纯单分符
int r_single(char* ch, FILE** fin, FILE** fout) {
	char token[2];
	token[0] = *ch;
	token[1] = '\0';
	fprintf(*fout, oformat, token, token);
	*ch = getc(*fin);
	return 0;
}
//处理双分符
int r_double(char* ch, FILE** fin, FILE** fout) {
	char token[3];
	token[0] = *ch;
	*ch = getc(*fin);
	if (*ch == '=') {
		token[1] = '=';
		token[2] = '\0';
		fprintf(*fout, oformat, token, token);
		*ch = getc(*fin);
	}
	else {
		token[1] = '\0';
		fprintf(*fout, oformat, token, token);
	}
	return 0;
}
//处理除号/和注释
int r_anotation(char* ch, FILE** fin, FILE** fout) {
	char token[2] = "/", pre;
	*ch = getc(*fin);
	if (*ch == '*') {
		do {
			pre = *ch;
			*ch = getc(*fin);
			if (*ch == '/' && pre == '*')break;
		} while (*ch != EOF);
		if (*ch == EOF)return -4;//若直到文件结束为止均为结束注释，则返回注释未结束错误代码-4
		*ch = getc(*fin);
	}
	else fprintf(*fout, oformat, token, token);
	return 0;
}
//输出错误信息到屏幕
void error(char ch, int er, FILE** fin) {
	long int lct = ftell(*fin) - column;//lct means location
	switch (er) {
	case -1://读入未知字符
		printf("!Unknown letter '%c' in  [%d,%d]!\n", ch, row, lct);
		break;
	case -2://非法标识符（以数字开头）
		printf("!Illegal ID '%c' in [%d,%d]!\n", ch, row, lct);
		break;
	case -3://非法实数（多个小数点）
		printf("!Illegal real number in [%d,%d]!\n(Maybe:more than one dot)\n", row, lct);
		break;
	case -4://注释未结束
		printf("!The anotation did not end!\n");
		break;
	}
}
void output_list(set<string> idlist) {
	if (!idlist.size()) {
		printf("idlist is null");
		return;
	}
	printf("ID List:\n");
	set<string>::iterator i = idlist.begin();
	for (i; i != idlist.end(); i++) {
		cout << *i << endl;
	}
}

//根据读入的单词的第一个字符确定调用不同的单词识别函数类型
int lexscan(char ch, char* singleword, char* doubleword) {
	if (ch == ' ' || ch == '\t')return 0;//是否为空字
	if ((ch >= 65 && ch <= 90) or (ch >= 97 && ch <= 122))
		return 1;//是否为字母
	if (ch >= 48 && ch <= 57)return 2;//是否为数字
	for (int i = 0; i < strlen(singleword); i++)
		if (ch == singleword[i])return 3;//是否为纯单分符
	for (int i = 0; i < strlen(doubleword); i++)
		if (ch == doubleword[i])return 4;//是否为双分符
	if (ch == '/')return 5;//是否为注释斜杠
	if (ch == '\n')return 6;//是否为换行符
	return -1;
}
//若文件结束，反复调用 Lexscan 函数识别单词
void scanner(FILE** fin, FILE** fout, char keyword[keywordsum][10], char* singleword, char* doubleword) {
	char ch, token[40];
	set<string> idlist;//标识符表
	int er = 0;
	ch = getc(*fin);
	while (ch != EOF) {//反复读取字符直到文件结束
		switch (lexscan(ch, singleword, doubleword)) {
		case 0://为空字
			ch = getc(*fin);
			break;
		case 1://为字母
			er = r_alpha(&ch, fin, fout, keyword, &idlist);
			break;
		case 2://为数字
			er = r_number(&ch, fin, fout);
			break;
		case 3://为纯单分符
			er = r_single(&ch, fin, fout);
			break;
		case 4://为双分符
			er = r_double(&ch, fin, fout);
			break;
		case 5://为注释斜杠
			er = r_anotation(&ch, fin, fout);
			break;
		case 6:
			fprintf(*fout, oformat, "enter", "enter");
			row++;
			ch = getc(*fin);
			column = ftell(*fin);//记录当前行尾坐标
			break;
		case -1:er = -1;
		}
		if (er != 0) {
			error(ch, er, fin);//调用错误处理函数
			ch = getc(*fin);
		}
		if (ch == ' ')ch = getc(*fin);
	}
	//output_list(idlist);
}

int main()
{
	FILE* fin = NULL, * fout = NULL;
	char keyword[keywordsum][10], singleword[50] = "", doubleword[10] = "";
	initscanner(&fin, &fout, keyword, singleword, doubleword);

	//fprintf(fout, oformat, "TYPE", "VALUE");

	scanner(&fin, &fout, keyword, singleword, doubleword);
}
