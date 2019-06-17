#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma pack(1)

#include <iostream> //for exit function only, testing the file opening.
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "bp_trees.h"

//lp - stands for leaf page
//nlp - stands for non leaf page

int main() {
	int i, j, sid;
	char input;
	PAGE_INFO_REF* nlp = (PAGE_INFO_REF*)malloc(sizeof(PAGE_INFO_REF));
	PAGE_INFO* pages = (PAGE_INFO*)calloc(3, sizeof(PAGE_INFO));
	PAGE_INFO* requested_page;

	read_lp();
	build_nlp(nlp);
	
	while (true) {
		printf("request sid: ");
		scanf("%d", &sid);
		if (sid == -1) {
			break;
		}
		requested_page = get_page(pages, nlp, sid);
		if (requested_page != NULL) {
			for (i = 0; i < 3; i++) {
				if (requested_page->sid[i] == sid) {
					printf("%d %s\n", requested_page->sid[i], requested_page->name[i]);
					break;
				}
			}
		}
		else {
			printf("record not found.\n");
		}

	}
	_getch();
	return 0;
}