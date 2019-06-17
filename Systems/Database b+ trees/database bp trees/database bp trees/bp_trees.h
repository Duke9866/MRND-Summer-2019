static int time_stamp[3] = { 0 };

typedef struct _page_info_ref {
	char page_type, unused[3];
	int child[4];
	int value[3];
} PAGE_INFO_REF;

typedef struct _page_info {
	int page_size:8;
	char unused[4];
	int sid[3];
	char name[3][5];
} PAGE_INFO;

void time_stamp_counter() {
	int i;
	for (i = 0; i < 3; i++) {
		time_stamp[i]++;
	}
}

void read_lp() {
	int i, j;
	void* buffer;
	FILE* fin, *fout;
	PAGE_INFO lp;
	if ((fin = fopen("input.txt", "r")) == 0) {
		exit(0);
	}
	if ((fout = fopen("memory.bin", "wb")) == 0) {
		exit(0);
	}
	buffer = malloc(sizeof(PAGE_INFO));
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			fscanf(fin, "%d %s", &lp.sid[j], lp.name[j]);
		}
		memcpy(buffer, &lp, sizeof(PAGE_INFO));
		fwrite(buffer, sizeof(PAGE_INFO), 1, fout);
	}
	fclose(fin);
	fclose(fout);
}

void build_nlp(PAGE_INFO_REF* nlp) {
	int i;
	FILE* fin;
	PAGE_INFO lp;

	if ((fin = fopen("memory.bin", "rb")) == 0) {
		exit(0);
	}

	nlp->child[0] = 0;
	for (i = 1; i < 4; i++) {
		fread(&lp, sizeof(PAGE_INFO), 1, fin);
		nlp->child[i] = i;
		nlp->value[i - 1] = lp.sid[2];
	}
	fclose(fin);
}

int remove_page() {
	int i, page_no = 0, max = time_stamp[0];
	for (i = 1; i < 3; i++) {
		if (time_stamp[i] > max) {
			max = time_stamp[i];
			page_no = i;
		}
	}
	time_stamp[page_no] = 0;
	return page_no;
}

PAGE_INFO* get_page(PAGE_INFO pages[3], PAGE_INFO_REF* page_ref, int sid) {
	int i, j, replace_page_no;
	int low, high, mid;
	FILE* fin;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (pages[i].sid[j] == sid) {
				time_stamp_counter();
				return &pages[i];
			}
		}
	}
	time_stamp_counter();
	replace_page_no = remove_page();
	
	//search page offset
	low = 0, high = 2;
	while (low <= high) {
		mid = (low + high) / 2;
		if (sid > page_ref->value[mid]) {
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}
	
	//read page offset from file
	if ((fin = fopen("memory.bin", "rb")) == NULL) {
		exit(0);
	}

	fseek(fin, low * sizeof(PAGE_INFO), SEEK_SET);
	fread(&pages[replace_page_no], sizeof(PAGE_INFO), 1, fin);

	for (i = 0; i < 3; i++) {
		if (pages[replace_page_no].sid[i] == sid) {
			fclose(fin);
			return &pages[replace_page_no];
		}
	}
	fclose(fin);
	return NULL;
}