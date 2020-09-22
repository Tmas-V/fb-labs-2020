#include "header.h"



int filter_text(char* f_raw_name, char* f_plain_name, int lang, bool ignoreBlanks)
{
	FILE* f_raw_txt = fopen(f_raw_name, "r");
	if (f_raw_txt == NULL)
	{
		printf("Error opening raw text file(This file doesn`t exist).\n");
		return 1;
	}
	FILE* f_plain_txt = fopen(f_plain_name, "w");
	if (f_plain_txt == NULL)
	{
		printf("Error creating file for plain text.\n");
		return 1;
	}

	unsigned int index_lang_first, index_lang_last, lang_upper_index_offset, lang_len;
	const map<unsigned int, unsigned int>* lang_mask = NULL;
	if ((lang != 0) && (lang != 1))
	{
		printf("Error: wrong .language\n");
		return 1;
	}
	else if (lang == 0)
	{
		index_lang_first = index_eng_first;
		index_lang_last = index_eng_last;
		lang_upper_index_offset = eng_upper_index_offset;
		lang_len = eng_len;
		lang_mask = &eng_lang_mask;
	}
	else
	{
		index_lang_first = index_rus_first;
		index_lang_last = index_rus_last;
		lang_upper_index_offset = rus_upper_index_offset;
		lang_len = rus_len;
		lang_mask = &rus_lang_mask;
	}

	//printf("%d, %d, %d, %d\n", index_lang_first, index_lang_last, lang_upper_index_offset, lang_len);

	wchar_t wchar_in;
	wchar_t wchar_out;
	unsigned int index;
	bool isPrevBlank = false;
	do {
		wchar_in = fgetwc(f_raw_txt);
		index = (unsigned int)wchar_in;
		if ((lang_mask->count(index) == 0)||(index == index_blank))
		{
			if (!ignoreBlanks)
			{
				if (!isPrevBlank)
				{
					fwprintf(f_plain_txt, L"%c", (wchar_t)index_blank);
				}
				isPrevBlank = true;
			}
		}
		else
		{
			index = lang_mask->at(index);
			wchar_out = wchar_in;
			if ((index_lang_first <= index) && (index <= index_lang_last))
			{
				wchar_out = wchar_in;
			}
			else if ((index_lang_first - lang_upper_index_offset <= index) && (index <= index_lang_last - lang_upper_index_offset))
			{
				wchar_out = (wchar_t)((index + lang_upper_index_offset));
			}
			fwprintf(f_plain_txt, L"%c", wchar_out);
			isPrevBlank = false;
		}
	} while(wchar_in != WEOF);

	fclose(f_plain_txt);
	fclose(f_raw_txt);
	return 0;
}
int get_text_length(char* f_plain_txt_name, unsigned long int& total_count, unsigned long int& blanks_count)
{
	FILE* f_plain_txt = fopen(f_plain_txt_name, "r");
	if (f_plain_txt == NULL)
	{
		printf("Error opening plain text file(This file doesn`t exist).\n");
		return 1;
	}

	wchar_t wchar_in;
	unsigned long int total_char_count = 0;
	blanks_count = 0;
	do {
		wchar_in = fgetwc(f_plain_txt);
		if ((unsigned int)wchar_in == index_blank)
		{
			blanks_count++;
		}
		total_char_count++;
	} while (wchar_in != WEOF);
	total_count = total_char_count;
	fclose(f_plain_txt);
	return 0;
}
int get_rate_of_gramm(wstring n_gramm, char* f_plain_txt_name,unsigned long int& n_gramm_count)
{
	if (n_gramm == L"")
	{
		printf("Error: NULL n-gramm .\n");
		return 1;
	}
	FILE* f_plain_txt = fopen(f_plain_txt_name, "r");
	if (f_plain_txt == NULL)
	{
		printf("Error opening plain text file(This file doesn`t exist).\n");
		return 1;
	}

	wchar_t wchar_in;
	unsigned long int ngramm_count = 0;
	unsigned int n = n_gramm.size(), current_coinc = 0;
	do {
		wchar_in = fgetwc(f_plain_txt);
		if (wchar_in == n_gramm[current_coinc])
		{
			current_coinc++;
			if (current_coinc == n)
			{
				ngramm_count++;
				if ((n > 1) && (wchar_in == n_gramm[0]))
				{
					current_coinc = 1;
				}
				else
				{
					current_coinc = 0;
				}
			}
		}
		else
		{
			current_coinc = 0;
		}
	} while (wchar_in != WEOF);
	n_gramm_count = ngramm_count;
	fclose(f_plain_txt);
	return 0;
}

int generate_gramm_rate_file(char* plain_file_name,char* output_file_name, unsigned long int text_analysis_params[4])
{
	if ((text_analysis_params[0] != 0) && (text_analysis_params[0] != 1))
	{
		printf("Error: wrong language.\n");
		return 1;
	}
	if (text_analysis_params[1] == 0)
	{
		printf("Error: wrong gramm length.\n");
		return 1;
	}
	unsigned int index_first = 0, index_last = 0;
	const map<unsigned int, wchar_t>* lang_encode = NULL;
	const map<unsigned int, unsigned int>* lang_mask = NULL;
	if (text_analysis_params[0] == 1)
	{
		index_first = index_rus_first, index_last = index_rus_last;
		lang_encode = &rus_lang_encoding;
		lang_mask = &rus_lang_mask;
	}
	else
	{
		index_first = index_eng_first, index_last = index_eng_last;
		lang_encode = &eng_lang_encoding;
		lang_mask = &eng_lang_mask;
	}
	unsigned long int  current_ngramm_count = 0;
	float current_ngramm_rate = 0.0f;
	FILE* gramm_output = fopen(output_file_name, "w");
	fprintf(gramm_output, "%d %d %d %d\n", text_analysis_params[0], text_analysis_params[1], text_analysis_params[2], text_analysis_params[3]);
	fclose(gramm_output);


	unsigned int* stack = new unsigned int[text_analysis_params[1]];
	unsigned long int top = text_analysis_params[1] - 1; bool isEnd = false;
	for (unsigned long int i = 0; i < text_analysis_params[1]; i++)
	{
		stack[i] = index_first;
	}
	while (!isEnd)
	{
		if (stack[top] == lang_mask->at(stack[top]))
		{
			wstring gramm = L"";
			for (unsigned long int i = 0; i<=top; i++)  gramm += { lang_encode->at(stack[i]) };
			if (get_rate_of_gramm(gramm, plain_file_name, current_ngramm_count))
			{
				printf("Error calculating n-gramm rate.\n");
				return 1;
			}
			current_ngramm_rate = (float)(current_ngramm_count) / (float)text_analysis_params[2];
			FILE* gramm_output = fopen(output_file_name, "a");
			fwprintf(gramm_output, L"%s %d %0.9f\n", gramm.c_str(), current_ngramm_count, current_ngramm_rate);
			fclose(gramm_output);
		}
		if (stack[top] == index_last)
		{
			stack[top] = index_blank;
			if (top != text_analysis_params[1] - 1)
			{
				top++;
			}
		}
		else if (stack[top] == index_blank)
		{
			while ((stack[top] == index_blank) && (top > 0))
			{
				stack[top] = index_first;
				top--;
			}
			if ((top == 0) && (stack[top] == index_blank))
			{
				isEnd = true;
			}
			else
			{
				if (stack[top] == index_last)
				{
					stack[top] = index_blank;
				}
				else
				{
					stack[top]++;
				}
				top = text_analysis_params[1] - 1;
			}
		}
		else
		{
			stack[top]++;
			if (top != text_analysis_params[1] - 1)
			{
				top++;
			}
		}
	}
	
	return 0;
}

int generate_gramm_rate_file_MAP(char* plain_file_name, char* output_file_name, unsigned long int text_analysis_params[4])
{
	if ((text_analysis_params[0] != 0) && (text_analysis_params[0] != 1))
	{
		printf("Error: wrong language.\n");
		return 1;
	}
	if (text_analysis_params[1] == 0)
	{
		printf("Error: wrong gramm length.\n");
		return 1;
	}
	unsigned int index_first = 0, index_last = 0;
	const map<unsigned int, wchar_t>* lang_encode = NULL;
	const map<unsigned int, unsigned int>* lang_mask = NULL;
	if (text_analysis_params[0] == 1)
	{
		index_first = index_rus_first, index_last = index_rus_last;
		lang_encode = &rus_lang_encoding;
		lang_mask = &rus_lang_mask;
	}
	else
	{
		index_first = index_eng_first, index_last = index_eng_last;
		lang_encode = &eng_lang_encoding;
		lang_mask = &eng_lang_mask;
	}
	
	FILE* gramm_output = fopen(output_file_name, "w");
	fprintf(gramm_output, "%d %d %d %d\n", text_analysis_params[0], text_analysis_params[1], text_analysis_params[2], text_analysis_params[3]);
	fclose(gramm_output);


	unsigned int* stack = new unsigned int[text_analysis_params[1]];
	unsigned long int top = text_analysis_params[1] - 1; bool isEnd = false;
	map<wstring, unsigned long int> hash_table;
	deque<wchar_t> current_gramm;
	for (unsigned long int i = 0; i < text_analysis_params[1]; i++)
	{
		stack[i] = index_first;
	}
	while (!isEnd)
	{
		if (stack[top] == lang_mask->at(stack[top]))
		{
			wstring gramm = L"";
			for (unsigned long int i = 0; i <= top; i++)  gramm += { lang_encode->at(stack[i]) };
			hash_table[gramm] = 0;			
		}
		if (stack[top] == index_last)
		{
			stack[top] = index_blank;
			if (top != text_analysis_params[1] - 1)
			{
				top++;
			}
		}
		else if (stack[top] == index_blank)
		{
			while ((stack[top] == index_blank) && (top > 0))
			{
				stack[top] = index_first;
				top--;
			}
			if ((top == 0) && (stack[top] == index_blank))
			{
				isEnd = true;
			}
			else
			{
				if (stack[top] == index_last)
				{
					stack[top] = index_blank;
				}
				else
				{
					stack[top]++;
				}
				top = text_analysis_params[1] - 1;
			}
		}
		else
		{
			stack[top]++;
			if (top != text_analysis_params[1] - 1)
			{
				top++;
			}
		}
	}
	FILE* plain_file = fopen(plain_file_name, "r");
	wchar_t wchar_in;
	for (unsigned int i = 0; i < text_analysis_params[1]; i++)
	{
		wchar_in = fgetwc(plain_file);
		current_gramm.push_back(wchar_in);
	}
	do {
		wchar_in = fgetwc(plain_file);
		if (wchar_in != WEOF)
		{
			wstring gramm = L"";
			for (deque<wchar_t>::iterator i = current_gramm.begin(); i != current_gramm.end(); i++)  gramm += { lang_encode->at(*i) };
			if (hash_table.count(gramm) > 0)
			{
				hash_table[gramm]++;
			}
			if ((lang_mask->count((unsigned int)wchar_in) > 0) && ((unsigned int)wchar_in == lang_mask->at((unsigned int)wchar_in)))
			{
				current_gramm.pop_front();
				current_gramm.push_back(wchar_in);
			}
		}
	} while (wchar_in != WEOF);
	fclose(plain_file);

	unsigned long int  current_ngramm_count = 0;
	float current_ngramm_rate = 0.0f;
	FILE* gramm_output_new = fopen(output_file_name, "a");
	map<wstring, float> map_for_sort;
	for (map<wstring, unsigned long int>::iterator i = hash_table.begin(); i != hash_table.end(); i++)
	{
		current_ngramm_count = i->second;
		current_ngramm_rate = (float)(current_ngramm_count) / (float)text_analysis_params[2];
		map_for_sort[i->first] = current_ngramm_rate;
		fwprintf(gramm_output_new, L"%s %d %0.9f\n", i->first.c_str(), current_ngramm_count, current_ngramm_rate);
	}
	fclose(gramm_output_new);

	const char* sort_f_name = "_-gramm_rate_data_SORTED__.txt";
	char* f_name = new char[strlen(sort_f_name) + 1];
	for (unsigned int i = 0; i < strlen(sort_f_name); i++)
	{
		f_name[i] = sort_f_name[i];
	}
	f_name[strlen(sort_f_name)] = '\0';
	f_name[0] = (char)(text_analysis_params[1] + (int)'1' - 1);
	if (text_analysis_params[3] > 0)
	{
		f_name[25] = '1';
	}
	else
	{
		f_name[25] = '0';
	}
	output_ngramm_rate_sorted(text_analysis_params[0], text_analysis_params[1], &map_for_sort, f_name);
	return 0;
}

int sort_map_by_rate(map<wstring, float>* hash_table, vector<map<wstring, float>::iterator>& sorted_arr)
{
	if (hash_table == NULL)
	{
		printf("Error: NULL map in sort.\n");
		return 1;
	}

	for (map<wstring, float>::iterator i = hash_table->begin(); i != hash_table->end(); i++)
	{
		sorted_arr.push_back(i);
	}
	quickSort(sorted_arr, 0, sorted_arr.size() - 1);
	return 0;
}

void quickSort(vector<map<wstring, float>::iterator>& iterators_array, unsigned long int left, unsigned long int right)
{
	for (unsigned long int i = left; i < right; i++)
	{
		for (unsigned long int j = i + 1; j <= right; j++)
		{
			if (iterators_array[j]->second > iterators_array[i]->second)
			{
				map<wstring, float>::iterator tmp = iterators_array[j];
				iterators_array[j] = iterators_array[i];
				iterators_array[i] = tmp;
			}
		}
	}
}

int output_ngramm_rate_sorted(unsigned long int lang, unsigned long int n, map<wstring, float>* hash_table, char* output_f_name)
{
	if (lang != 1)
	{
		return 1;
	}
	if (n > 2)
	{
		return 1;
	}
	else if (n == 1)
	{
		vector<map<wstring, float>::iterator> sorted_arr;
		sort_map_by_rate(hash_table, sorted_arr);
		FILE* output_file = fopen(output_f_name, "w");
		for (unsigned long int i = 0; i < sorted_arr.size(); i++)
		{
			if (sorted_arr[i]->first.c_str()[0] == ' ') fwprintf(output_file, L"'%s' %0.9f\n", sorted_arr[i]->first.c_str(), sorted_arr[i]->second);
			else fwprintf(output_file, L"%s %0.9f\n", sorted_arr[i]->first.c_str(), sorted_arr[i]->second);
		}
		fclose(output_file);
	}
	else
	{
		FILE* output_file = fopen(output_f_name, "w");
		fwprintf(output_file, L"     ");
		for (unsigned int i = index_rus_first; i <= index_rus_last; i++)
		{
			if (i != rus_lang_mask.at(i)) continue;
			fwprintf(output_file, L"|      %c       ", rus_lang_encoding.at(i));
		}
		fwprintf(output_file, L"|      '%c'      |\n", rus_lang_encoding.at(index_blank));
		//                                                                                         columns
		for (unsigned int i = index_rus_first; i <= index_rus_last; i++)
		{
			if (i != rus_lang_mask.at(i)) continue;
			fwprintf(output_file, L"----");
			for (unsigned int j = index_rus_first; j <= index_rus_last; j++)
			{
				if (j != rus_lang_mask.at(j)) continue;
				fwprintf(output_file, L"--------------");
			}
			fwprintf(output_file, L"-------------\n");
			
			fwprintf(output_file, L"%c   ", rus_lang_encoding.at(i));
			for (unsigned int j = index_rus_first; j <= index_rus_last; j++)
			{
				if (j != rus_lang_mask.at(j)) continue;
				wstring gramm = L"";
				gramm += { rus_lang_encoding.at(i) };
				gramm += { rus_lang_encoding.at(j) };
				fwprintf(output_file, L" | %0.9f ", hash_table->at(gramm));
			}
			wstring gramm = L"";
			gramm += { rus_lang_encoding.at(i) };
			gramm += { rus_lang_encoding.at(index_blank) };
			fwprintf(output_file, L"| %0.9f |\n", hash_table->at(gramm));
		}
		//                                                                                 low-border + floats
		for (unsigned int j = index_rus_first; j <= index_rus_last; j++)
		{
			if (j != rus_lang_mask.at(j)) continue;
			fwprintf(output_file, L"---------------");
		}
		fwprintf(output_file, L"-------------\n");
		fwprintf(output_file, L"' '");
		for (unsigned int j = index_rus_first; j <= index_rus_last; j++)
		{
			if (j != rus_lang_mask.at(j)) continue;
			wstring gramm = L"";
			gramm += { rus_lang_encoding.at(index_blank) };
			gramm += { rus_lang_encoding.at(j) };
			fwprintf(output_file, L" | %0.9f ", hash_table->at(gramm));
		}
		fwprintf(output_file, L"| %0.9f |\n", hash_table->at(L"  "));
		//                                                                                 low-border + row with blanks
		fwprintf(output_file, L"----");
		for (unsigned int j = index_rus_first; j <= index_rus_last; j++)
		{
			if (j != rus_lang_mask.at(j)) continue;
			fwprintf(output_file, L"---------------");
		}
		fwprintf(output_file, L"-------------");
		//                                                                                  final low-border
		fclose(output_file);
	}
	return 0;
}
