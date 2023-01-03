#include <iostream>
#include <algorithm>
#include <set>
#define keywordsum 10
#define oformat "%s\t %s\n"
using namespace std;
int row = 1, column = 0;

//��ʼ�������벢��Դ�����ļ���Ŀ������ļ�����ʼ�������ֱ�
int initscanner(FILE** fin, FILE** fout, char keyword[keywordsum][10], char* singleword, char* doubleword) {
	char ifn[50] = "Test.txt", ofn[50] = "lexical analysis.txt";//�����ļ���source.txt������ļ���lexical analysis.txt

	if ((*fin = fopen(ifn, "r")) == NULL) {
		printf("Cannot open the input file!");
		return (1);
	}
	if ((*fout = fopen(ofn, "w")) == NULL) {
		printf("Cannot open the output file!");
		return (1);
	}

	//��ʼ�������ֱ����ַ���˫�ַ���
	string kw[keywordsum] = { "if","else","for","while","call","int","read","write","function","main" };
	sort(kw, kw + keywordsum);//�������ֱ�����
	int len;
	for (int i = 0; i < keywordsum; i++) {
		len = kw[i].length();
		kw[i].copy(keyword[i], len);
		keyword[i][len] = '\0';
	}
	strcpy(singleword, "+-*(){};,:");
	strcpy(doubleword, "><=!");
}
//ʶ�����ֺͱ�ʶ��
int r_alpha(char* ch, FILE** fin, FILE** fout, char keyword[keywordsum][10], set<string>* idlist) {
	int i = 0;
	char token[30], kw[30];
	while (((*ch >= 65 && *ch <= 90) or (*ch >= 97 && *ch <= 122) or (*ch >= 48 && *ch <= 57)) && *ch != EOF) {
		//��ϱ�ʶ��ֱ�����������ַ�������
		token[i++] = *ch;
		*ch = getc(*fin);
	}
	token[i] = '\0';
	strcpy(kw, token);
	strlwr(kw);//�� token ֵ���ƺ�ȫ��ת��ΪСд

	/*/�������ҹؼ���
	for (int j = 0; j < keywordsum; j++)
		if (strcmp(kw, keyword[j]) == 0){
			fprintf(*fout, oformat, kw, kw);
			return 0;
		}
	/*///�۰���ҹؼ���
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
//ʶ��������ʵ��
int r_number(char* ch, FILE** fin, FILE** fout) {
	int i = 0;
	bool isreal = false;
	char token[40];
	while (((*ch >= 48 && *ch <= 57) || *ch == '.') && *ch != EOF) {//��ϱ�ʶ��ֱ��������������
		if (*ch == '.')
			if (!isreal)isreal = true;
			else return -3;//���һ�������г�������С�����򷵻طǷ����ִ���-3
		token[i++] = *ch;
		*ch = getc(*fin);
	}
	token[i] = '\0';

	if (isreal)
		fprintf(*fout, oformat, "NUM", token);
	else
		fprintf(*fout, oformat, "NUM", token);
	if ((*ch >= 65 && *ch <= 90) or (*ch >= 97 && *ch <= 122))return -2;//�������һ���ַ�Ϊ��ĸ�򷵻طǷ��ַ��������-2
	return 0;
}
//�������ַ�
int r_single(char* ch, FILE** fin, FILE** fout) {
	char token[2];
	token[0] = *ch;
	token[1] = '\0';
	fprintf(*fout, oformat, token, token);
	*ch = getc(*fin);
	return 0;
}
//����˫�ַ�
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
//�������/��ע��
int r_anotation(char* ch, FILE** fin, FILE** fout) {
	char token[2] = "/", pre;
	*ch = getc(*fin);
	if (*ch == '*') {
		do {
			pre = *ch;
			*ch = getc(*fin);
			if (*ch == '/' && pre == '*')break;
		} while (*ch != EOF);
		if (*ch == EOF)return -4;//��ֱ���ļ�����Ϊֹ��Ϊ����ע�ͣ��򷵻�ע��δ�����������-4
		*ch = getc(*fin);
	}
	else fprintf(*fout, oformat, token, token);
	return 0;
}
//���������Ϣ����Ļ
void error(char ch, int er, FILE** fin) {
	long int lct = ftell(*fin) - column;//lct means location
	switch (er) {
	case -1://����δ֪�ַ�
		printf("!Unknown letter '%c' in  [%d,%d]!\n", ch, row, lct);
		break;
	case -2://�Ƿ���ʶ���������ֿ�ͷ��
		printf("!Illegal ID '%c' in [%d,%d]!\n", ch, row, lct);
		break;
	case -3://�Ƿ�ʵ�������С���㣩
		printf("!Illegal real number in [%d,%d]!\n(Maybe:more than one dot)\n", row, lct);
		break;
	case -4://ע��δ����
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

//���ݶ���ĵ��ʵĵ�һ���ַ�ȷ�����ò�ͬ�ĵ���ʶ��������
int lexscan(char ch, char* singleword, char* doubleword) {
	if (ch == ' ' || ch == '\t')return 0;//�Ƿ�Ϊ����
	if ((ch >= 65 && ch <= 90) or (ch >= 97 && ch <= 122))
		return 1;//�Ƿ�Ϊ��ĸ
	if (ch >= 48 && ch <= 57)return 2;//�Ƿ�Ϊ����
	for (int i = 0; i < strlen(singleword); i++)
		if (ch == singleword[i])return 3;//�Ƿ�Ϊ�����ַ�
	for (int i = 0; i < strlen(doubleword); i++)
		if (ch == doubleword[i])return 4;//�Ƿ�Ϊ˫�ַ�
	if (ch == '/')return 5;//�Ƿ�Ϊע��б��
	if (ch == '\n')return 6;//�Ƿ�Ϊ���з�
	return -1;
}
//���ļ��������������� Lexscan ����ʶ�𵥴�
void scanner(FILE** fin, FILE** fout, char keyword[keywordsum][10], char* singleword, char* doubleword) {
	char ch, token[40];
	set<string> idlist;//��ʶ����
	int er = 0;
	ch = getc(*fin);
	while (ch != EOF) {//������ȡ�ַ�ֱ���ļ�����
		switch (lexscan(ch, singleword, doubleword)) {
		case 0://Ϊ����
			ch = getc(*fin);
			break;
		case 1://Ϊ��ĸ
			er = r_alpha(&ch, fin, fout, keyword, &idlist);
			break;
		case 2://Ϊ����
			er = r_number(&ch, fin, fout);
			break;
		case 3://Ϊ�����ַ�
			er = r_single(&ch, fin, fout);
			break;
		case 4://Ϊ˫�ַ�
			er = r_double(&ch, fin, fout);
			break;
		case 5://Ϊע��б��
			er = r_anotation(&ch, fin, fout);
			break;
		case 6:
			fprintf(*fout, oformat, "enter", "enter");
			row++;
			ch = getc(*fin);
			column = ftell(*fin);//��¼��ǰ��β����
			break;
		case -1:er = -1;
		}
		if (er != 0) {
			error(ch, er, fin);//���ô�������
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
