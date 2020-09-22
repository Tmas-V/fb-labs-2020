#include "complete_funcs.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    system("cd");
    calculate_text_entropy_for_ngramm_MAP(1, 2, false);
    calculate_text_entropy_for_ngramm_MAP(1, 2, true);
    return 0;
}

