#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <string>
#include <windows.h>
#include <map>
#include <deque>
#include <vector>

#define index_eng_first 97
#define index_eng_last 122
#define eng_upper_index_offset 32
#define eng_len 26
#define index_blank 32
#define index_rus_first 1072
#define index_rus_last 1103
#define rus_upper_index_offset 32
#define rus_len 32

#define fname_buff_size 256
#define word_len 50
using namespace std;

const map<unsigned int, wchar_t> rus_lang_encoding = {
	{1040, L'À'},
	{1041, L'Á'},
	{1042, L'Â'},
	{1043, L'Ã'},
	{1044, L'Ä'},
	{1045, L'Å'},
	{1025, L'¨'},
	{1046, L'Æ'},
	{1047, L'Ç'},
	{1048, L'È'},
	{1049, L'É'},
	{1050, L'Ê'},
	{1051, L'Ë'},
	{1052, L'Ì'},
	{1053, L'Í'},
	{1054, L'Î'},
	{1055, L'Ï'},
	{1056, L'Ð'},
	{1057, L'Ñ'},
	{1058, L'Ò'},
	{1059, L'Ó'},
	{1060, L'Ô'},
	{1061, L'Õ'},
	{1062, L'Ö'},
	{1063, L'×'},
	{1064, L'Ø'},
	{1065, L'Ù'},
	{1066, L'Ü'},
	{1067, L'Ú'},
	{1068, L'Û'},
	{1069, L'Ý'},
	{1070, L'Þ'},
	{1071, L'ß'},
	{32, L' '},
	{1072, L'à'},
	{1073, L'á'},
	{1074, L'â'},
	{1075, L'ã'},
	{1076, L'ä'},
	{1077, L'å'},
	{1105, L'¸'},
	{1078, L'æ'},
	{1079, L'ç'},
	{1080, L'è'},
	{1081, L'é'},
	{1082, L'ê'},
	{1083, L'ë'},
	{1084, L'ì'},
	{1085, L'í'},
	{1086, L'î'},
	{1087, L'ï'},
	{1088, L'ð'},
	{1089, L'ñ'},
	{1090, L'ò'},
	{1091, L'ó'},
	{1092, L'ô'},
	{1093, L'õ'},
	{1094, L'ö'},
	{1095, L'÷'},
	{1096, L'ø'},
	{1097, L'ù'},
	{1098, L'ü'},
	{1099, L'ú'},
	{1100, L'û'},
	{1101, L'ý'},
	{1102, L'þ'},
	{1103, L'ÿ'}
};
const map<unsigned int, unsigned int> rus_lang_mask = {
	{1040, 1040},
	{1041, 1041},
	{1042, 1042},
	{1043, 1043},
	{1044, 1044},
	{1045, 1045},
	{1025, 1045},
	{1046, 1046},
	{1047, 1047},
	{1048, 1048},
	{1049, 1049},
	{1050, 1050},
	{1051, 1051},
	{1052, 1052},
	{1053, 1053},
	{1054, 1054},
	{1055, 1055},
	{1056, 1056},
	{1057, 1057},
	{1058, 1058},
	{1059, 1059},
	{1060, 1060},
	{1061, 1061},
	{1062, 1062},
	{1063, 1063},
	{1064, 1064},
	{1065, 1065},
	{1066, 1066},
	{1067, 1066},
	{1068, 1068},
	{1069, 1069},
	{1070, 1070},
	{1071, 1071},
	{32, 32},
	{1072, 1072},
	{1073, 1073},
	{1074, 1074},
	{1075, 1075},
	{1076, 1076},
	{1077, 1077},
	{1105, 1077},
	{1078, 1078},
	{1079, 1079},
	{1080, 1080},
	{1081, 1081},
	{1082, 1082},
	{1083, 1083},
	{1084, 1084},
	{1085, 1085},
	{1086, 1086},
	{1087, 1087},
	{1088, 1088},
	{1089, 1089},
	{1090, 1090},
	{1091, 1091},
	{1092, 1092},
	{1093, 1093},
	{1094, 1094},
	{1095, 1095},
	{1096, 1096},
	{1097, 1097},
	{1098, 1098},
	{1099, 1098},
	{1100, 1100},
	{1101, 1101},
	{1102, 1102},
	{1103, 1103}
};

const map<unsigned int, wchar_t> eng_lang_encoding = {};
const map<unsigned int, unsigned int> eng_lang_mask = {};

int filter_text(char* f_raw_name, char* f_plain_name, int lang,bool ignoreBlanks);
int get_text_length(char* f_plain_txt_name, unsigned long int& total_count, unsigned long int& blanks_count);
int get_rate_of_gramm(wstring n_gramm, char* f_plain_txt_name, unsigned long int& n_gramm_count);
int generate_gramm_rate_file(char* plain_file_name, char* output_file_name, unsigned long int text_analysis_params[4]);
int generate_gramm_rate_file_MAP(char* plain_file_name, char* output_file_name, unsigned long int text_analysis_params[4]);

int sort_map_by_rate(map<wstring, float>* hash_table, vector<map<wstring, float>::iterator>& sorted_arr);
void quickSort(vector<map<wstring, float>::iterator>& iterators_array, unsigned long int left, unsigned long int right);
int output_ngramm_rate_sorted(unsigned long int lang, unsigned long int n, map<wstring, float>* hash_table, char* output_f_name);

