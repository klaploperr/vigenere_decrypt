#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <Windows.h>
#define SIZE 20

unsigned char alphavit[] = { 'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И','Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С','Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь','Э', 'Ю', 'Я' };
unsigned char Vij_square[33][33];
unsigned long long time1 = 0, time2 = 0;
//Это просто структура которая хранит в первой ячейке букву, а во второй количество её появлений в зашифрованном тексте 
struct rasp {
	unsigned char letter;
	int kolichestvo_poyavlenii;
} typedef rasp;

struct shift {
	int j;
	int i;
	int r;
};

short my_strlen(unsigned char* str) {
	unsigned char* p = str;
	while (*p++);
	return p - str - 1;
}

//Пробелы удаляет
void delete_spaces(unsigned char str[], unsigned char cipher[]) {
	short i = 0, j = 0;
	while (str[i] != '\0') {
		/*if (str[i] != ' ') {
			cipher[j] = str[i];
			i++, j++;
		}
		else i++;*/
		__asm {
			movsx eax, word ptr[i]
			mov ecx, dword ptr[str]
			movzx edx, byte ptr [ecx+eax]
			cmp edx, 20h
			je iskl
			movsx ecx, word ptr[j]
			mov esi, dword ptr[cipher]
			mov dword ptr[ecx+esi], edx
			inc ecx
			mov word ptr[j], cx
				iskl:
			inc eax
			mov word ptr[i], ax
			jmp delete_spaces + 34h
		}
	}
	cipher[j] = '\0';
}

void decode(unsigned char key[], unsigned char mes[]) {
	
	unsigned char* sss;
	unsigned int N = strlen(mes);
	unsigned short key_size = (unsigned short)strlen(key);
	unsigned short alph_size = (unsigned short)strlen(alphavit);
	short d = 0;
	short j, f;
	for (unsigned short i = 0; i < N; i++) {
		for (f = 0; f < alph_size; f++) {
			if (key[d] == alphavit[f]) {
				break;
			}
		}
		d++;
		if (d > key_size - 1) {
			d = 0;
		}
		for (j = 0; j < alph_size; j++) {
			if (mes[i] == Vij_square[f][j]) {
				break;
			}
		}
		mes[i] = alphavit[j];
	}
	mes[N] = '\0';
	sss = (unsigned char*)malloc(sizeof(unsigned char) * N);
	if (sss) {
		strncpy(sss, mes, (size_t)N);
	}
	printf("%s", sss);
	free(sss);
}

void Vizhioner_square(int FirstChar, unsigned char keyword[]) {
	unsigned char* p;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for (int i = 0; i < 33; i++) {
		memcpy(Vij_square[i], alphavit, (size_t)(strlen(alphavit)));
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	unsigned char tmp;
	tmp = strchr(alphavit, keyword[0])-alphavit;

	/*int tmp = 0;
	for (int i = 0; i < 33; i++) {
		if (keyword[0] == alphavit[i]) {
			tmp = i;
			break;
		}
	}*/
	unsigned char Start[34];
	short k = FirstChar;
	int i;
	for (i = 0; i < 33; i++) {
		if (k == 33) {
			k = 0;
		}
		Start[i] = alphavit[k];
		k++;
	}
	Start[33] = '\0';
	i = tmp;
	for (short counter = 0; counter < 33; i++, counter++) {
		if (i == 33) {
			i = 0;
		}
		short p = counter;
		for (short j = 15, counter2 = 0; counter2 < 33; j++, counter2++) {
			if (p == 33) {
				p = 0;
			}
			if (j == 33) {
				j = 0;
			}
			Vij_square[i][j] = Start[p];
			p++;

		}
	}
	short count = 0;
	printf("   ");
	//unsigned char* p;
	for (p = &alphavit[0]; p < &alphavit[33]; p++) {
		printf("%c ", *p++);
	}
	printf("\n");
	printf("---------------------------------------------------------------------\n");
	for (i = 0; i < 33; i++) {
		printf("%c |", alphavit[count]);
		for (short j = 0; j < 33; j++) {
			printf("%c ", Vij_square[i][j]);
		}
		printf("\n");
		count++;
	}
}


//Вычисляет индекс совпадения
int IS(unsigned char str[], int* buf) {
	
	short counter = 0;
	for (short i = 1; i <= SIZE; i++) //предполагаем длину слова 
	{
		rasp** distribution;
		distribution = (rasp**)malloc((size_t)(sizeof(rasp*) * i));
		if (distribution) {
			for (short p = 0; p < i; p++) {
				distribution[p] = (rasp*)malloc((size_t)(sizeof(rasp) * 33));
				if (distribution[p]) {
					for (short k = 0; k < 33; k++) {
						distribution[p][k].letter = alphavit[k];
						distribution[p][k].kolichestvo_poyavlenii = 0;
					}
				}
			}
		}
		float* is_j;
		is_j = (float*)malloc((size_t)(sizeof(float) * i));
		//вынос инварианта из цикла
		unsigned short str_size = (unsigned short)strlen(str);
		for (short j = 1; j <= i; j++) //вычисляем i значений различных ИС
		{
			double N = 0;
			for (short k = 0; ((j + i * k) - 1) < str_size; k++)
			{
				short n = 0;
				short p = 0;
				while (n < 33)
				{
					if (distribution && distribution[j - 1] && str[(j + i * k) - 1] == distribution[j - 1][n].letter)
					{
						//!!!!!!!!!НЕ ЛОМАЙСЯ ПЛИЗ!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						//distribution[j - 1][n].kolichestvo_poyavlenii += 1;
						distribution[j - 1][n].kolichestvo_poyavlenii++;
						break;
					}
					n++;
				}
				N++;
			}
			float IS_j = 0;
			for (int f = 0; f < 33; f++)
			{
				float x;
				if (distribution) {
					if (distribution[j - 1]) {
						double y = (float)distribution[j - 1][f].kolichestvo_poyavlenii;
						x = (y * (y - 1)) / (N * (N - 1));
						IS_j += x;
					}

				}
			}
			if (is_j) {
				is_j[j - 1] = (float)IS_j;
			}
		}
		short flag = 0;
		for (short k = 0; k < i; k++) {
			if (is_j && is_j[k] > 0.045) {
				__asm {
					mov         ax, word ptr[ebp - 0D0h]
					inc         ax
					mov         word ptr[ebp - 0D0h], ax
				}
				//flag++;
			}
		}
		if (flag == i || flag == i - 1) {
			buf[counter] = i;
			counter++;
		}

		free(distribution);

	}
	return(counter);
}

void hz(unsigned char str[], int buf, int counter) {
	
	int N = strlen(str);
	short amount_of_groups = (N / buf) + 1;
	unsigned char** groups = (unsigned char**)malloc((size_t)(sizeof(unsigned char*) * amount_of_groups));
	for (short i = 0; i < amount_of_groups; i++) {
		groups[i] = (unsigned char*)malloc((size_t)(sizeof(unsigned char) * (buf + 1)));
	}
	short pointer = 0;
	short char_counter = 0;
	for (short j = 0; j < amount_of_groups; j++) {
		for (short k = 0; k <= buf; k++) {
			if (k == buf || char_counter == N) {
				groups[j][k] = '\0';
			}
			else {
				groups[j][k] = str[pointer];
				pointer++;
				char_counter++;
			}
		}
	}
	//Строим распределение для всех букв 
	rasp** distribution = (rasp**)malloc(sizeof(rasp*) * (size_t)buf);
	//!!!!!!!!!!!!!!!!!!вынесла условие 
	if (distribution) {
		for (int p = 0; p < buf; p++) {
				distribution[p] = (rasp*)malloc(sizeof(rasp) * (size_t)33);
				if (distribution[p] != NULL) {
					for (short k = 0; k < 33; k++) {
						distribution[p][k].letter = alphavit[k];
						distribution[p][k].kolichestvo_poyavlenii = 0;
					}
				}

		}
	}
	
	for (short i = 0; i < buf; i++) {
		for (short j = 0; j < amount_of_groups; j++) {
			short n = 0;
			short p = 0;
			while (n < 33)
			{
				if (groups[j][i] == distribution[i][n].letter)
				{
					//!!!!!!!!!!!!!!!!
					distribution[i][n].kolichestvo_poyavlenii++;
					break;
				}
				n++;
			}
		}
	}
	//R i_j_r вычисляю здесь
	short isk[10];
	short count = 0;
	short SSgroup = 0;
	memset(isk, 111, 10);
	short a = 0;
	struct shift* SHIFTS = (struct shift*)malloc((size_t)((buf + 1) * sizeof(struct shift)));
	//!!!!!!!!! вынесла условие (жоска)
	if (distribution) {
		while (a != buf - 1) {
			short max_R = 0, max_i = 0, max_j = 0, max_r = 0;
			for (short i = 0; i < buf; i++) {
				//!!!!!!!!тож подняла условие
				if (distribution[i]) {
					for (short j = i + 1; j < buf; j++) {
						//!!!!!!!!!!!!аналогично 
						if (distribution[j]) {
							for (short r = 0; r < 33; r++) {
								short flag = 0;
								for (short p = 0; p <= count; p++) {
									if (i == isk[p] || j == isk[p]) {
										flag = 1;
										break;
									}
								}
								if (flag) {
									break;
								}
								short R_i_j_r = 0;
								for (short b = 0, tmp = b + r; b < 33; b++, tmp++) {
									if (tmp > 32) {
										tmp = 0;
									}
									//!!!!!!!!!!!!!!!!! отсюда вынесла if
									R_i_j_r += distribution[i][b].kolichestvo_poyavlenii * distribution[j][tmp].kolichestvo_poyavlenii;
								}
								//printf("Значение Rijr для групп %d и %d с предположительным сдвигом %d - %d\n", i+1, j+1, r, R_i_j_r);
								if (R_i_j_r > max_R) {
									max_R = R_i_j_r;
									max_i = i;
									max_j = j;
									max_r = r;
								}
							}
						}

					}
				}
				
			}
			//!!!!!!!!!!!!!!!! не работает ни панимаю предупреждение ухади (
			if (SHIFTS) {
				SHIFTS[a].j = max_j;
				SHIFTS[a].i = max_i;
				SHIFTS[a].r = max_r;
			}
			//!!!!!!!!!!!!!!!! вынесла условия из цикла 
			if (distribution && distribution[max_i] && distribution[max_j]) {
				for (short i = 0, tmp = i + max_r; i < 33; i++, tmp++) {
					if (tmp == 33) {
						tmp = 0;
					}
					distribution[max_i][i].kolichestvo_poyavlenii = distribution[max_i][i].kolichestvo_poyavlenii + distribution[max_j][tmp].kolichestvo_poyavlenii;
				}
			}
			isk[count] = max_j;
			count++;
			a++;
			SSgroup = max_i;
		}
	}
	printf("\n");
	for (short i = 0; i < 32; i++) {
		for (short j = 32; j > i; j--) {// для всех элементов после i-ого
			if (distribution && distribution[SSgroup] && distribution[SSgroup][j - 1].kolichestvo_poyavlenii < distribution[SSgroup][j].kolichestvo_poyavlenii) {// если текущий элемент меньше предыдущего
				short temp = distribution[SSgroup][j - 1].kolichestvo_poyavlenii; // меняем их местами
				unsigned char bukovka = distribution[SSgroup][j - 1].letter;
				distribution[SSgroup][j - 1].kolichestvo_poyavlenii = distribution[SSgroup][j].kolichestvo_poyavlenii;
				distribution[SSgroup][j - 1].letter = distribution[SSgroup][j].letter;
				distribution[SSgroup][j].kolichestvo_poyavlenii = temp;
				distribution[SSgroup][j].letter = bukovka;
			}
		}
	}
	short FirstChar = 0;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (distribution && distribution[SSgroup]) {
		unsigned char tmp;
		tmp = strchr(alphavit, distribution[SSgroup][0].letter) - alphavit;
		FirstChar = tmp;

	}
	/*while (distribution[SSgroup][0].letter != alphavit[i]) {
		i++;
	}*/
	printf("%d\n", FirstChar);
	if (SHIFTS) {
		short original_i = SHIFTS[0].i, original_r, check = 0;
		for (int i = 1; i < buf - 1; i++) { //тут я тупо сортирую все сдвиги, чтобы они были все относительно одной буквы
			if (SHIFTS[i].i != SHIFTS[i - 1].i) {
				if (!check) {
					original_r = 33 - SHIFTS[i].r;
					SHIFTS[i].r = original_r;
					original_i = SHIFTS[i].i;
					SHIFTS[i].i = SHIFTS[i - 1].i;
					SHIFTS[i].j = original_i;
					check = 1;
				}
				else if (original_i == SHIFTS[i].j) {
					original_r = original_r - SHIFTS[i].r;
					SHIFTS[i].r = original_r;
					SHIFTS[i].j = SHIFTS[i].i;
					SHIFTS[i].i = SHIFTS[i - 1].i;
				}
				else {
					SHIFTS[i].r = original_r + SHIFTS[i].r;
					SHIFTS[i].i = SHIFTS[i - 1].i;
				}
				if (SHIFTS[i].r < 0) SHIFTS[i].r = 33 + SHIFTS[i].r;
			}
		}
		unsigned char keywords[33][20];
		for (short i = 0, j = 0; i < 33; i++) {
			keywords[i][SHIFTS[j].i] = alphavit[i];
			for (j = 0; j < buf - 1; j++) {
				short r = SHIFTS[j].r + i;
				if (r > 32) r = r - 33;
				keywords[i][SHIFTS[j].j] = alphavit[r];
			}
			keywords[i][buf] = '\0';
			printf("%2d.Возможное ключевое слово: %s\n", i + 1, keywords[i]);
			j = 0;
		}
		printf("Какое ключевое слово подходит? Введите цифру: ");
		short choise;
		time1 = __rdtsc();
		scanf_s("%hd", &choise);
		time2 = (__rdtsc() - time1) + time2;
		printf("\n");
		Vizhioner_square(FirstChar, keywords[choise - 1]);
		printf("\n");
		decode(keywords[choise - 1], str);
	}
	
	free(distribution);
	free(SHIFTS);
}

int main() {
	int buf[SIZE];
	unsigned char str[2000], cipher[2000];
	system("chcp 1251");
	printf("Введите закодированное сообщение\n");
	time1 = __rdtsc();
	fgets(str, 2000, stdin);
	time2 = (__rdtsc() - time1) + time2;
	short l = my_strlen(str);
	str[l] = '\0';
	delete_spaces(str, cipher);
	int counter = IS(cipher, &buf[0]);
	printf("Какую длину ключевого слова рассмотрим?\n");
	for (int i = 1; i < counter; i++) {
		printf("Press %d - %d\n", i, buf[i]);
	}
	short choice = 0;
	time1 = __rdtsc();
	scanf_s("%hd", &choice);
	time2 = (__rdtsc() - time1) + time2;
	hz(cipher, buf[choice], counter);
	unsigned long long time = __rdtsc() - time2;
	printf("\n time is %llu", time);
	return 0;
}