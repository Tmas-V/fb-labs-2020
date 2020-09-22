#include "header.h"
#include <cmath>


int read_ensemble_from_file(char* data_file_name, map<unsigned long int, float>* ensemble);
int calculate_ensemble_entropy(map<unsigned long int, float>* ensemble, float& entropy);