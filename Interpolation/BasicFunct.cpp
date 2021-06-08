#include "BasicFunct.h";
int htoi(char arr[]) {
	int i = 0, n = 0;
	unsigned char c;
	if (arr[i] == '0' && (arr[i + 1] == 'x' || arr[i + 1] == 'X')) {
		i += 2;
		for (; ((c = arr[i]) >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); i++) {
			if (c >= 'A' && c <= 'Z') {
				c += 'a' - 'A';
			}
			if (c >= '0' && c <= '9') {
				n = 16 * n + (c - '0');
			}
			else n = 16 * n + (c - 'a') + 10;
		}
	}
	return n;
}

void squeeze(char s1[], char s2[]) {
	bool flag = 0;
	int k, i;
	for (i = 0, k = 0; s1[i] != 0; i++) {
		flag = 0;
		for (int j = 0; s2[j] != 0; j++) {
			if (s1[i] == s2[j]) flag = 1;
		}
		if (!flag) {
			s1[k++] = s1[i];
		}
	}
	s1[k] = 0;
	printf(s1);
}

int any(char s1[], char s2[]) {
	int i = 0, n = -1;
	for (; s1[i] != '\0' && n == -1; i++) {
		for (int k = 0; s2[k] != '\0'; k++)
			if (s1[i] == s2[k]) n = i;
	}
	return n;
}

int setbits(unsigned int x, int p, int n, int y) {
	return (x & ~(getbits(~0, n - 1, n) << (p - n + 1)) | (getbits(y, n - 1, n) << (p - n + 1)));
}

unsigned int getbits(unsigned x, int p, int n) {
	return (x >> (p + 1 - n)) & ~(~0 << n);
}

int invert(unsigned int x, int p, int n) {
	return setbits(x, p, n, ~x);
}

int rightrot(unsigned int x, int n) {
	if (n == 0)
		return x;
	return rightrot(setbits(x >> 1, 31, 1, getbits(x, 1, 1)), --n);
}

int bitcount2(unsigned int x) {
	int i;
	for (i = 0; x != 0; i++) {
		x &= x - 1;
	}
	return i;
}

int bitcount1(unsigned int x) {
	int b;
	for (b = 0; x != 0; x >>= 1)
		if (x & 01)
			b++;
	return b;
}

void lower2(char str[]) {
	char c;
	for (int i = 0; str[i] != '\0'; i++) {
		str[i] = ((c = str[i]) <= 'Z' && c >= 'A') ? str[i] - 'A' + 'a' : str[i];
	}
	printf(str);
}

int binsearch(int x, int v[], int n) {
	int low, high, mid;
	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (high + low) / 2;
		if (x < v[mid])
			high = mid - 1;
		else if (x > v[mid])
			low = mid + 1;
		else return mid;
	}
	return -1;
}

int binsearchv2(int x, int v[], int n) {
	int low = 0;
	int high = n - 1;
	while (low < high) {
		int mid = (high + low) / 2;
		if (x <= v[mid])
			high = mid;
		else low = mid + 1;
	}
	if (v[high] == x) return high;
	else return -1;
}

void escape(char s[], char t[]) {
	int i, j;
	for (i = 0, j = 0; t[i] != '\0'; i++, j++)
		switch (t[i])
		{
		case '\t':
			s[j] = '\\';
			j++;
			s[j] = 't';
			break;
		case '\n':
			s[j] = '\\';
			j++;
			s[j] = 'n';
			break;
		default:
			s[j] = t[i];
			break;
		}
	s[j] = '\0';
}

void revescape(char s[], char t[]) {
	int i, j;
	for (i = 0, j = 0; t[i] != '\0'; i++, j++)
		if (t[i] == '\\') {
			i++;
			switch (t[i])
			{
			case 't':
				s[j] = '\t';
				break;
			case 'n':
				s[j] = '\n';
				break;
			default:
				s[j] = '\\';
				s[j + 1] = t[i];
				break;
			}
		}
		else {
			s[j] = t[i];
		}
	s[j] = '\0';
}

bool in(char sym, char start, char end) {
	return (sym >= start && sym <= end);
}

bool isupcase(char sym) {
	return in(sym, 'A', 'Z');
}

bool islowcase(char sym) {
	return in(sym, 'a', 'z');
}

bool issymbol(char sym) {
	return islowcase(sym) || isupcase(sym);
}

bool isdigit(char sym) {
	return in(sym, '0', '9');
}

bool expandable(char sym1, char sym2) {
	if (isupcase(sym1) && isupcase(sym2))
		return true;
	if (islowcase(sym1) && islowcase(sym2))
		return true;
	if (isdigit(sym1) && isdigit(sym2))
		return true;
	return false;
}

void expand(char s1[], char s2[]) {
	char prev = 0;
	int i, j;
	for (i = 0, j = 0; s1[i] != '\0'; i++, j++) {
		if (s1[i] == '-' && expandable(prev, s1[i + 1])) {
			int k;
			char c;
			for (c = prev + 1; c != s1[i + 1]; c++, j++) {
				s2[j] = c;
			}
			j--;
		}
		else s2[j] = s1[i];
		prev = s1[i];
	}
	s2[j] = '\0';
}

void reverse(char s[]) {
	int c, i, j;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void itoa(int num, char s[]) {
	int i, sign;
	unsigned int n;
	if ((sign = num) < 0)
		n = -(num + 1) + 1;
	else n = num;
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

void buftostr(int buf[], char s[], int len) {
	int i;
	for (i = 0; i < len; i++) {
		if (buf[i] < 10) {
			s[i] = '0' + buf[i];
		}
		else {
			s[i] = 'A' - 10 + buf[i];
		}
	}
	s[i] = '\0';
}

void itob(int num, char s[], int d) {
	if (d < 25) {
		int i, sign;
		int buf[100];
		unsigned int n;
		if ((sign = num) < 0)
			n = -(num + 1) + 1;
		else n = num;
		i = 0;
		do {
			buf[i++] = n % d;
		} while ((n /= d) > 0);
		buftostr(buf, s, i);
		if (sign < 0)
			s[i++] = '-';
		s[i] = '\0';
		reverse(s);
	}
}

void itoam(int n, char s[], int min) {
	itoa(n, s);
	int l;
	if ((l = strlen(s)) < min) {
		reverse(s);
		while (l < min) {
			s[l] = ' ';
			l++;
		}
		s[l] = '\0';
		reverse(s);
	}
}

double atof(char* s, int &i) {
	double val, power = 1;
	int sign;
	for (; isspace(s[i]); i++);
	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	for (val = 0.0; isdigit(s[i]); i++)
		val = 10.0 * val + (s[i] - '0');
	if (s[i] == '.') {
		i++;
		for (power = 1.0; isdigit(s[i]); i++) {
			val = 10.0 * val + (s[i] - '0');
			power *= 10.0;
		}
	}
	if (s[i] == 'e' || s[i] == 'E') {
		i++;
		double dpower = 0;
		if (s[i] == '-') {
			i++;
			for (; isdigit(s[i]); i++) {
				dpower = 10.0 * dpower + (s[i] - '0');
			}
			for (int j = 0; j < dpower; j++) {
				power *= 10.0;
			}
		}
		else {
			i++;
			for (; isdigit(s[i]); i++) {
				dpower = 10.0 * dpower + (s[i] - '0');
			}
			for (int j = 0; j < dpower; j++) {
				power /= 10.0;
			}
		}
	}
	return sign * val / power;
}