#include<bits/stdc++.h>
#include<ctype.h>
#include<stdio.h>
#define keywordSum 12
using namespace std;
string keyword[keywordSum] = { "case","do","else","for","function","if","int","main","read","switch","while","write" };
int keywordnum[keywordSum];
char singleword[50] = "+-*(){};,:&''";
char doubleword[10] = "><=!";
FILE* fin, * fout;//用于指向输入输出的文件指针
char token[40], token1[40], token2[40];
int lineindex;

int TESTparse();
int TESTscan();
bool match();

//关键字的简单集合，元素是排好序的，是唯一的
set<string>se;

struct node {
	string st;
	vector<node*>son;//动态数组
	//C++运行结构体中有函数的存在
	node* addSon(string s) {
		node* Son = new node();
		Son->st = s;
		//添加新的信息
		son.push_back(Son);
		return Son;
	}
} *Root;

int program();
int declaration_list(node* fa);
int declaration_stat(node* fa);
int statement_list(node* fa);
int statement(node* fa);
int if_stat(node* fa);
int while_stat(node* fa);
int for_stat(node* fa);
int read_stat(node* fa);
int write_stat(node* fa);
int compound_stat(node* fa);
int expression_stat(node* fa);
int expression(node* fa);
int bool_expr(node* fa);
int additive_expr(node* fa);
int term(node* fa);
int factor(node* fa);
int fun_declaration(node* fa);//
int fun_body(node* fa);//
int main_declaration(node* fa);//
int call_stat(node* fa);//
int dowhile_stat(node* fa);
int switch_stat(node* fa);

void out(node* p) {
	p->addSon(token1);
}

void shift(int x) {
	for (int i = 0; i < x; i++)
		fprintf(fout, "    ");
}

void tree_out(node* p, int x) {
	shift(x); //输出空格
	//c_str()生成一个const char*指针，指向以空字符终止的数组
	fprintf(fout, "%s\n", p->st.c_str());
	int nc = p->son.size();//个数
	if (nc == 0)
		return;

	//递归输出
	for (int i = 0; i < nc; i++) {
		tree_out(p->son[i], x + 1);
	}
	return;
}


int main()
{
	int es = 0;
	es = TESTscan();
	if (es > 0)
		cout << "词法分析失败" << endl;
	else
		cout << "词法分析成功" << endl;

	if (es == 0)  es = TESTparse();
	if (es == 0) {
		printf("语法分析成功\n");
	}
	else if (es != 0) printf("语法分析失败\n");

	if (es == 0)
		tree_out(Root, 0);

	fclose(fin);
	fclose(fout);
	return 0;
}

bool match()
{
	int low, high, mid;
	low = 0;
	high = keywordSum - 1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (keyword[mid] == token2)
		{
			return true;
		}
		else if (keyword[mid] > token2)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	return false;
}

int TESTscan()
{
	char ch;
	int es = 0, e = 1, el[100], en = 0;
	string er[100];
	int j, n;

	if ((fin = fopen("datain.txt", "r")) == NULL)
	{
		cout << "没找到输入的文件！" << endl;
		return 1;
	}
	if ((fout = fopen("dataout.txt", "w")) == NULL)
	{
		cout << "没找到输出的文件!" << endl;
		return 2;
	}

	ch = getc(fin);

	while (ch != EOF)
	{
		while (ch == ' ' || ch == '\n' || ch == '\t')
		{
			if (ch == '\n')
				e++;
			ch = getc(fin);
		}

		if (isalpha(ch))//如果是字母
		{
			token[0] = ch;
			ch = (ch >= 'A') && (ch <= 'Z') ? ch + 32 : ch;
			token2[0] = ch;
			j = 1;
			ch = getc(fin);
			while (isalnum(ch))//如果是数字
			{
				token[j] = ch;
				ch = (ch >= 'A') && (ch <= 'Z') ? ch + 32 : ch;
				token2[j] = ch;
				j++;
				ch = getc(fin);
			}
			token[j] = '\0';
			token2[j] = '\0';

			if (match() == true)
			{
				cout << token2 << "\t" << token << endl;
				fprintf(fout, "%s\t%s\t%d\n", token2, token, e);
			}
			else
			{
				cout << "ID\t" << token << endl;
				fprintf(fout, "%s\t%s\t%d\n", "ID", token, e);
			}
		}

		else if (isdigit(ch))//查数字
		{
			token[0] = ch;
			j = 1;
			ch = getc(fin);
			if (isalpha(ch))
			{
				while (isalpha(ch))
				{
					token[j] = ch;
					j++;
					ch = getc(fin);
				}
				token[j] = '\0';
				es = 3;
				el[en] = e; er[en] = "字母和数字组成变量并数字在变量前!";
				en++;
				cout << "ERROR\t" << token << endl;
				fprintf(fout, "%s\t%s\t%d\n", "ERROR", token, e);
			}
			else
			{
				while (isdigit(ch))
				{
					token[j] = ch;
					j++;
					ch = getc(fin);
				}
				token[j] = '\0';
				cout << "NUM\t" << token << endl;
				fprintf(fout, "%s\t%s\t%d\n", "NUM", token, e);
			}
		}

		else if (strchr(singleword, ch) > 0)//查单分界符
		{
			token[0] = ch;
			token[1] = '\0';
			ch = getc(fin);
			cout << token << "\t" << token << endl;
			fprintf(fout, "%s\t%s\t%d\n", token, token, e);
		}
		else if (strchr(doubleword, ch) > 0)//查双分界符
		{
			token[0] = ch;
			ch = getc(fin);
			if (ch == '=')
			{
				token[1] = ch;
				token[2] = '\0';
				ch = getc(fin);
			}
			else
				token[1] = '\0';
			cout << token << "\t" << token << endl;
			fprintf(fout, "%s\t%s\t%d\n", token, token, e);
		}
		else if (ch == '/')//查注释
		{
			ch = getc(fin);
			if (ch == '*')
			{
				char ch1;
				ch1 = getc(fin);
				do {//删除注释
					ch = ch1;
					ch1 = getc(fin);
				} while ((ch != '*' || ch1 != '/') && ch1 != EOF);

				ch = getc(fin);
			}
			else
			{
				token[0] = '/';
				token[1] = '\0';
				cout << token << "\t" << token << endl;
				fprintf(fout, "%s\t%s\t%d\n", token, token, e);
			}
		}
		else//出错处理
		{
			token[0] = ch;
			token[1] = '\0';
			ch = getc(fin);
			es = 3;
			el[en] = e; er[en] = "未识别的标识符!";
			en++;
			cout << "ERROR" << "\t" << token << endl;
			fprintf(fout, "%s\t%s\t%d\n", "ERROR", token, e);
		}
	}
	if (en > 0)
	{
		for (int i = 0; i < en; i++)
		{
			cout << "第" << el[i] << "行出现错误，错误原因为：" << er[i] << endl;
		}
	}
	fclose(fin);
	fclose(fout);
	return es;
}

int TESTparse() {
	int es = 0;

	if ((fin = fopen("dataout.txt", "r")) == NULL) {
		printf("打开文件错误\n");
		es = 10;
	}
	fout = fopen("dataout2.txt", "w");
	if (es == 0)
		es = program();

	printf("=====语法分析结果：======\n");
	switch (es) {
	case 0:
		printf("语法分析成功\n");
		break;
	case 10:
		cout << "打开输入文件失败" << endl;
		break;
	case 1:
		cout << "缺少 { !" << endl;
		break;
	case 2:
		cout << "缺少 } !" << endl;
		break;
	case 3:
		cout << "缺少标识符!" << endl;
		break;
	case 4:
		cout << "缺少分号!" << endl;
		break;
	case 5:
		cout << "缺少 ( !" << endl;
		break;
	case 6:
		cout << "缺少 ) !" << endl;
		break;
	case 7:
		cout << "缺少操作数!" << endl;
		break;
	case 8:
		cout << "重复定义！" << endl;
		break;
	case 9:
		cout << "缺少 main 函数!" << endl;
		break;
	case 11:
		cout << "非法声明,缺少 fun/main !" << endl;
		break;
	case 12:
		cout << "使用未声明变量！" << endl;
		break;
	case 13:
		cout << "dowhile缺少 while !" << endl;
		break;
	case 14:
		cout << "case缺少标识符!" << endl;
		break;
	case 15:
		cout << "case缺少判断条件!" << endl;
		break;
	case 16:
		cout << "case缺少 : !" << endl;
		break;
	default:
		cout << es << endl;
	}
	if (es != 0) {
		printf("错误在%d行\n", lineindex - 1);
	}

	return es;
}

int program() {
	Root = new node();
	Root->st = "<programe>";

	int es = 0;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	if (strcmp(token, "function") && strcmp(token, "main")) {
		return es = 11;
	}
	while (strcmp(token, "function") == 0) {

		es = fun_declaration(Root);
		if (es > 0) return es;
	}
	if (strcmp(token, "main")) {
		return es = 9;
	}
	es = main_declaration(Root);
	if (es > 0) return es;

	return es;
}

int fun_declaration(node* fa) {
	node* p = fa->addSon("<fun_declaration>");
	out(p);

	int es = 0;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, "ID")) return es = 3;
	out(p);

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, "(")) return es = 5;
	out(p);

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, ")")) return es = 5;
	out(p);

	es = fun_body(p);
	if (es > 0) return es;

	return es;
}

int main_declaration(node* fa) {
	node* p = fa->addSon("<main_declaration>");
	out(p);

	int es = 0;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, "(")) return es = 5;
	out(p);

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, ")")) return es = 6;
	out(p);

	es = fun_body(p);
	if (es > 0) return es;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	return es;
}

int fun_body(node* fa) {
	node* p = fa->addSon("fun_body");

	int es = 0;

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, "{"))
		return es = 1;
	out(p);

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	es = declaration_list(p);
	if (es > 0)
		return es;

	es = statement_list(p);
	if (es > 0)
		return es;
	if (strcmp(token, "}"))
		return es = 2;
	out(p);

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	se.clear();//清空一个识别完成的函数体内的变量
	return es;
}

int declaration_list(node* fa) {
	node* p = fa->addSon("declaration_list");
	int es = 0;
	while (strcmp(token, "int") == 0) {
		es = declaration_stat(p);
		if (es > 0) return es;
	}
	return es;
}
int declaration_stat(node* fa) {
	node* p = fa->addSon("<declaration_stat>");
	int es = 0;
	out(p);

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	if (strcmp(token, "ID"))
		return es = 3;
	//用来查找set中某个值出现的次数
	if (se.count(token1))
		return es = 8;

	//插入token1
	se.insert(token1);
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	if (strcmp(token, ";"))
		return es = 4;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	return es;
}
int statement_list(node* fa) {
	node* p = fa->addSon("<state_ment>");
	int es = 0;
	while (strcmp(token, "}")) {
		es = statement(p);
		if (es > 0)
			return es;
	}
	return es;
}
int statement(node* fa) {
	node* p = fa->addSon("<statement>");
	int es = 0;
	if (es == 0 && strcmp(token, "if") == 0)
		es = if_stat(p);
	if (es == 0 && strcmp(token, "while") == 0)
		es = while_stat(p);
	if (es == 0 && strcmp(token, "for") == 0)
		es = for_stat(p);
	if (es == 0 && strcmp(token, "read") == 0)
		es = read_stat(p);
	if (es == 0 && strcmp(token, "write") == 0)
		es = write_stat(p);
	if (es == 0 && strcmp(token, "{") == 0)
		es = compound_stat(p);
	if (es == 0 && strcmp(token, "call") == 0)
		es = call_stat(p);
	if (es == 0 && strcmp(token, "do") == 0)
		es = dowhile_stat(p);
	if (es == 0 && strcmp(token, "switch") == 0)
		es = switch_stat(p);
	if (es == 0 && (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0))
		es = expression_stat(p);

	return es;
}
int if_stat(node* fa) {
	node* p = fa->addSon("<if_stat>");
	int es = 0;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	if (strcmp(token, "("))
		return es = 5;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	es = expression(p);
	if (es > 0)
		return es;
	if (strcmp(token, ")"))
		return es = 6;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	es = statement(p);
	if (es > 0)
		return es;

	if (strcmp(token, "else") == 0) {
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

		es = statement(p);
		return es;
	}
	return es;
}
int while_stat(node* fa) {
	node* p = fa->addSon("<while_stat>");
	int es = 0;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	out(p);
	if (strcmp(token, "("))
		return es = 5;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	es = expression(p);
	if (es > 0)
		return es;
	out(p);
	if (strcmp(token, ")")) return es = 6;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	es = statement(p);
	return es;
}
int for_stat(node* fa) {
	node* p = fa->addSon("<for_stat>");
	out(p);
	int es = 0;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	out(p);
	if (strcmp(token, "("))
		return es = 5;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	es = expression(p);
	if (es > 0)
		return es;
	if (strcmp(token, ";"))
		return es = 4;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	es = expression(p);
	if (es > 0)
		return es;
	if (strcmp(token, ";"))
		return es = 4;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	es = expression(p);
	if (es > 0)
		return es;
	if (strcmp(token, ")"))
		return es = 6;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	es = statement(p);
	return es;
}
int write_stat(node* fa) {
	node* p = fa->addSon("<write_stat>");
	out(p);
	int es = 0;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	//	printf("%s %s\n",token,token1);
	es = expression(p);
	if (es > 0) return es;
	if (strcmp(token, ";")) return es = 4;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	//	printf("%s %s\n",token,token1);
	return es;
}
int read_stat(node* fa) {
	node* p = fa->addSon("<read_stat>");
	out(p);
	int es = 0;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	//	printf("%s %s\n",token,token1);
	if (strcmp(token, "ID")) return es = 3;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	//	printf("%s %s\n",token,token1);
	if (strcmp(token, ";")) return es = 4;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	//	printf("%s %s\n",token,token1);
	return es;
}
int compound_stat(node* fa) {
	node* p = fa->addSon("<compound_stat>");
	out(p);
	int es = 0;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	//	printf("%s %s\n",token,token1);
	es = statement_list(p);
	return es;
}
int expression_stat(node* fa) {
	node* p = fa->addSon("<expression_stat>");
	//out(p);
	int es = 0;
	if (strcmp(token, ";") == 0) {
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		//		printf("%s %s\n",token,token1);
		return es;
	}
	es = expression(p);
	if (es > 0) return es;
	if (es == 0 && strcmp(token, ";") == 0) {
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		//		printf("%s %s\n",token,token1);
		return es;
	}
	else {
		return es = 4;
	}
}
int call_stat(node* fa) {
	node* p = fa->addSon("<call_stat>");
	int es = 0;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, "ID")) return es = 3;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, "(")) return es = 5;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, ")")) return es = 6;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	return es;
}

int dowhile_stat(node* fa) {
	node* p = fa->addSon("<dowhile_stat>");
	int es = 0;
	out(p);

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	out(p);
	if (strcmp(token, "{"))  return es = 1;

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	es = statement(p);

	out(p);
	if (strcmp(token, "}"))  return es = 2;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);

	if (strcmp(token, "while")) return es = 13;


	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	out(p);
	if (strcmp(token, "("))  return es = 5;

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	es = expression(p);
	if (es > 0)
		return es;
	out(p);
	if (strcmp(token, ")"))  return es = 6;

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	if (strcmp(token, ";"))   return es = 4;
	out(p);

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	return es;
}

int switch_stat(node* fa) {
	node* p = fa->addSon("<switch_stat>");
	out(p);
	int es = 0;

	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	out(p);

	if (strcmp(token, "(")) return es = 5;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	out(p);
	if (strcmp(token, "ID")) return es = 3;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	out(p);
	if (es > 0) return es;
	if (strcmp(token, ")")) return es = 6;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	out(p);
	if (es == 0 && strcmp(token, "{")) return es = 1;
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	out(p);
	if (es == 0 && strcmp(token, "case")) return es = 14;
	while (strcmp(token, "case") == 0) {
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		out(p);
		if (es == 0 && strcmp(token, "NUM")) return es = 15;
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		out(p);
		if (es == 0 && strcmp(token, ":")) return es = 16;
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		if (es == 0 && strcmp(token, "{")) return es = 1;
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		es = expression_stat(p);
		if (es == 0 && strcmp(token, "}")) return es = 2;
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	}

	if (es == 0 && strcmp(token, "}")) return es = 2;
	out(p);
	fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
	return es;
}

int expression(node* fa) {
	node* p = fa->addSon("<expression>");
	int es = 0, fileadd;
	char token2[20], token3[40];
	if (strcmp(token, "ID") == 0) {
		if (!se.count(token1)) return es = 12;
		fileadd = ftell(fin);
		fscanf(fin, "%s\t%s\t%d\n", token2, token3, &lineindex);
		//		printf("%s %s\n",token2,token3);...
		if (strcmp(token2, "=") == 0) {
			out(p);
			//			shift(p+1);
			//			fprintf(fout,"%s\n",token2);
			p->addSon(token2);

			fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
			es = bool_expr(p);
			if (es > 0) return es;
		}
		else {
			fseek(fin, fileadd, 0);
			es = bool_expr(p);
			if (es > 0) return es;
		}
	}
	else es = bool_expr(p + 1);
	return es;
}
int bool_expr(node* fa) {
	node* p = fa->addSon("<bool_expr>");
	int es = 0;
	es = additive_expr(p);
	if (es > 0) return es;
	if (strcmp(token, ">") == 0 || strcmp(token, ">=") == 0 || strcmp(token, "<") == 0 || strcmp(token, "<=") == 0 || strcmp(token, "==") == 0 || strcmp(token, "!=") == 0) {
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		//			printf("%s %s\n",token,token1);
		es = additive_expr(p);
		if (es > 0) return es;
	}
	return es;
}
int additive_expr(node* fa) {
	node* p = fa->addSon("<additive_expr>");
	int es = 0;
	es = term(p);
	if (es > 0) return es;
	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0) {
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		//		printf("%s %s\n",token,token1);
		es = term(p);
		if (es > 0) return es;
	}
	return es;
}
int term(node* fa) {
	node* p = fa->addSon("<term>");
	int es = 0;
	es = factor(p);
	if (es > 0) return es;
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		//		printf("%s %s\n",token,token1);
		es = factor(p);
		if (es > 0) return es;
	}
	return es;
}
int factor(node* fa) {
	node* p = fa->addSon("<factor>");
	int es = 0;

	if (strcmp(token, "(") == 0) {
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		//		printf("%s %s\n",token,token1);
		es = expression(p);
		if (es > 0) return es;
		if (strcmp(token, ")")) return es = 6;
		out(p);
		fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
		//		printf("%s %s\n",token,token1);
	}
	else {

		if (strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0) {
			out(p);
			fscanf(fin, "%s\t%s\t%d\n", token, token1, &lineindex);
			//			printf("%s %s\n",token,token1);
			return es;
		}
		else {
			es = 7;
			return es;
		}
	}
	return es;
}
