#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copy_str(char* src, char* dest, int dest_index) {
	int src_index = 0;
	while (src[src_index]) {
		dest[dest_index] = src[src_index];
		src_index++;
		dest_index++;
	}
	return src_index;
}

char* getCode(int num, char ch) {
	int i = 0;
	char* res = (char*)calloc(10, sizeof(char));
	if (num == 1) {
		res[0] = ch;
		res[1] = '\0';
		return res;
	}
	while (num) {
		res[i++] = num % 10 + '0';
		num /= 10;
	}
	res[i++] = ch;
	res[i] = '\0';
	_strrev(res);
	return res;
}

char* compress(char* str) {
	int i, len = 0, count = 1;
	char ch = str[0];
	for (i = 1; str[i]; i++) {
		if (str[i] != ch) {
			len += copy_str(getCode(count, ch), str, len);
			ch = str[i];
			count = 1;
		}
		else {
			count++;
		}
	}
	if (str[i] != ch) {
		len += copy_str(getCode(count, ch), str, len);
	}
	str[len] = '\0';
	return str;
}

char* input() {
	char* ans = (char*)malloc(sizeof(char) * 50);
	
	//char* inp1 = "aaabbbccc";
	char* inp2 = "aaaaaaaaaabbbbbbbbbbcccccccccc";

	//strcpy(ans, inp1);
	strcpy(ans, inp2);

	return ans;
}

void init() {
	char* ans = input();
	printf("%s\n", ans);
	ans = compress(ans);
	printf("%s\n", ans);
}