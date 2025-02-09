#include <iostream>
using namespace std;
bool willend = 0;
string read() {
	string rtn;
	char c = getchar();
	while (c != '\n') {
		if (c == '\r') c = getchar(); // 可能用得上。
		if (c == EOF) {
			willend = 1;
			break;
		}
		rtn += c;
		c = getchar();

	}
	return rtn;
}
void write(string s) {
	for (auto i : s) putchar(i);
}
string s;
void findstr(int &i) {
	string ans = "";
	int len = s.size();
	int j;
	for (j = i + 1; j < len; ++j) {
		if (s[j] == '\"' && s[j - 1] != '\\') break;
	}
	printf("[STRING] ");
	for (; i <= j; ++i) {
		putchar(s[i]);
	}
	putchar('\n');
	--i;
}
void getname(int &i) {
	string ans = "";
	int len = s.size();
	for (; i < len; ++i) {
		if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9') || s[i] == '_') {
			ans += s[i];
		} else {
			break;
		}
	}
	--i;
	if (ans == "and" || ans == "break" || ans == "do" || ans == "else" || ans == "elseif" || ans == "end" || ans == "false" || ans == "for"
	        || ans == "function" || ans == "if" || ans == "in" || ans == "local" || ans == "nil" || ans == "not" || ans == "or" || ans == "repeat"
	        || ans == "return" || ans == "then" || ans == "true" || ans == "until" || ans == "while") {
		printf("[RESERVED] ");
	} else {
		printf("[NAME] ");
	}
	write(ans);
	putchar('\n');
}
void finde(int &i) {
	bool flag = 1;
	int len = s.size();
	if (i == len - 1) {
		//puts("QAQ");
		--i;//maybe
		putchar('\n');
		return ;
	}
	if (i != len - 2 && (s[i + 1] == '-' || s[i + 1] == '+') && (s[i + 2] < '0' || s[i + 2]  > '9')) {
		//puts("QAQ");
		--i;//maybe
		putchar('\n');
		return ;
	}
	if ((s[i + 1] != '+' && s[i + 1] != '-') && (s[i + 1] < '0' || s[i + 1] > '9')) {
		//puts("QAQ");
		--i;//maybe
		putchar('\n');
		return ;
	}
	if ((s[i + 1] == '+' || s[i + 1] == '-') && i == len - 2) {
		//puts("QAQ");
		--i;
		putchar('\n');
		return ;
	}
	if (s[i + 1] == '+' || s[i + 1] == '-') {
		flag = 0;
	}
	putchar('e');
	++i;
	for (; i < len; ++i) {
		if (s[i] >= '0' && s[i] <= '9') {
			putchar(s[i]);
		} else if (s[i] == '-' || s[i] == '+') {
			if (flag == 1) break;
			putchar(s[i]);
			flag = 1;
		} else {
			break;
		}
	}
	putchar('\n');
	--i;
}
void findfloat(int &i) {
	int len = s.size();
//	if(i == len-1){
//		putchar('\n');
//		return ;
//	}
	++i;
	if (s[i] == 'e') {
		putchar('.');
		finde(i);
		return ;
	}
	if (s[i] > '9' || s[i] < '0') {
		i -= 2;
		return ;
	}
	putchar('.');
	for (; i < len; ++i) {
		if (s[i] >= '0' && s[i] <= '9') {
			putchar(s[i]);
		} else if (s[i] == 'e') {
			//puts("QAQ");
			finde(i);
			return ;
		} else {
			--i;
			return ;
		}
	}
	--i;
	putchar('\n');
}
void findsixteen(int &i) {
	putchar(s[i]);
	putchar(s[i + 1]);
	i += 2;
	int len = s.size();
	for (; i < len; ++i) {
		if ((s[i] >= 'A' && s[i] <= 'F') || (s[i] >= 'a' && s[i] <= 'f') || (s[i] >= '0' && s[i] <= '9')) {
			putchar(s[i]);
		} else {
			break;
		}
	}
	--i;
	putchar('\n');
}
void findnum(int &i) {
	printf("[NUMBER] ");
	int len = s.size();
	if (i != len - 1 && s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'X')) {
		findsixteen(i);
		return ;
	}
	int ans = 0;
	for (; i < len; ++i) {
		if (s[i] >= '0' && s[i] <= '9') {
			ans = (ans << 1) + (ans << 3) + (s[i] - '0');
		} else if (s[i] == '.') {
			printf("%d", ans);
			findfloat(i);
			return ;
		} else if (s[i] == 'e') {
			printf("%d", ans);
			//puts("QAQ");
			finde(i);
			return ;
		} else {
			break;
		}
	}
	--i;
	printf("%d\n", ans);
}
void check() {
	int len = s.size();
	for (int i = 0; i < len; ++i) {
		if (s[i] == ' ') continue;
		if (i != len - 1 && s[i] == '.' && s[i + 1] >= '0' && s[i + 1] <= '9') {
			printf("[NUMBER] ");
			findfloat(i);
		} else if (s[i] == '"') {
			findstr(i);
		} else if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z')) {
			getname(i);
		} else if (i != len - 1 && s[i] == '-' && s[i + 1] == '-') {
			return ;
		} else if (i != len - 1 && s[i] == '~' && s[i + 1] == '=') {
			++i;
			puts("[SYMBOL] ~=");
		} else if (i != len - 2 && s[i] == '.' && s[i + 1] == '.' && s[i + 2] == '.') {
			i += 2;
			puts("[SYMBOL] ...");
		} else if (i != len - 1 && s[i] == '.' && s[i + 1] == '.') {
			++i;
			puts("[SYMBOL] ..");
		} else if (i != len - 1 && s[i] == '=' && s[i + 1] == '=') {
			++i;
			puts("[SYMBOL] ==");
		} else if (i != len - 1 && s[i] == '>' && s[i + 1] == '=') {
			++i;
			puts("[SYMBOL] >=");
		} else if (i != len - 1 && s[i] == '<' && s[i + 1] == '=') {
			++i;
			puts("[SYMBOL] <=");
		} else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '%' || s[i] == '#' || s[i] == '^' || s[i] == '<'
		           || s[i] == '>' || s[i] == '[' || s[i] == ']' || s[i] == '(' || s[i] == ')' || s[i] == '{' || s[i] == '}' || s[i] == '='
		           || s[i] == '.' || s[i] == ',' || s[i] == ':' || s[i] == ';') {
			printf("[SYMBOL] %c\n", s[i]);
		} else if (s[i] >= '0' && s[i] <= '9') {
			findnum(i);
		}
	}
}
int main() {
	while (1) {
		s = read();
		if (willend == 1) break;
		check();
		puts("[EOL]");
	}
	return 0;
}