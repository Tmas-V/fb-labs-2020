#include "cryptography_formulas_header.h"

int calculate_ensemble_entropy(map<unsigned long int, float>* ensemble,unsigned int& n, float& entropy)
{
	if (ensemble == NULL)
	{
		printf("Error: NULL ensemble ptr.\n");
		return 1;
	}

	entropy = 0.0f;
	for (map<unsigned long int, float>::iterator iter = ensemble->begin(); iter != ensemble->end(); iter++)
	{
		if (iter->second > 0.0f)
		{
			entropy += -1 * iter->second * log2(iter->second);
		}
	}
	entropy = entropy / (float)n;
	return 0;
}
int read_ensemble_from_file(char* data_file_name, map<unsigned long int, float>* ensemble)
{
	if (ensemble == NULL)
	{
		printf("Error: NULL ensemble ptr.\n");
		return 1;
	}
	FILE* data_file = fopen(data_file_name, "r");
	if (!data_file)
	{
		printf("Error: could not open plain text file.\n");
		return 1;
	}
	unsigned long int lang = 0, n = 0, total_count = 0, blanks_count = 0;

	int tmp = 0;
	tmp = fwscanf(data_file, L"%u %u %u %u",&lang, &n, &total_count, &blanks_count);
	wchar_t* gramm = new wchar_t[n];
	unsigned long int i = 1;
	unsigned long int count = 0;
	float rate = 0.0f;
	while (!feof(data_file))
	{
		if ((tmp = fwscanf(data_file, L"%s %u %f", gramm, &count, &rate)) > 0)
		{
			ensemble->insert(pair<unsigned long int, float>(i, rate));
			i++;
		}
		else
		{
			//printf("Error reading line.\n");
		}
	}
	fclose(data_file);
	return 0;
}