#include "complete_funcs.h"

int calculate_text_entropy_for_ngramm(unsigned int lang, unsigned int n_for_gramm, bool ignoreBlanks)
{
    if (n_for_gramm > 9)
    {
        printf("Error: too long gramm.\n");
        return 1;
    }
    if (!ignoreBlanks) printf("////////////////////////\nAnalysing text including blanks...\n////////////////////////\n");
    else printf("////////////////////////\nAnalysing text NOT including blanks...\n////////////////////////\n");
    char c = '\n';
    printf("Do you want to use custom i/o files(y/n): ");
    while (c=='\n') c = getchar();
    char* raw_file_name_buffer;
    char* plain_file_name_buffer;
    char* entropy_out_f_name;
    if (c == 'y')
    {
        raw_file_name_buffer = new char[fname_buff_size];
        plain_file_name_buffer = new char[fname_buff_size];
        printf("Enter filename|path of text file with raw text: ");
        scanf("%s", raw_file_name_buffer);
        printf("Enter filename|path for plain text file : ");
        scanf("%s", plain_file_name_buffer);

        entropy_out_f_name = new char[fname_buff_size];
        printf("Enter filename|path of text file for entropy output: ");
        scanf("%s", entropy_out_f_name);
    }
    else
    {
        const char* raw_f_name = "raw_text.txt";
        raw_file_name_buffer = new char[strlen(raw_f_name)+1];
        for (unsigned int i = 0; i < strlen(raw_f_name); i++)
        {
            raw_file_name_buffer[i] = raw_f_name[i];
        }
        raw_file_name_buffer[strlen(raw_f_name)] = '\0';

        const char* plain_f_name = "plain_text.txt";
        plain_file_name_buffer = new char[strlen(plain_f_name)+1];
        for (unsigned int i = 0; i < strlen(plain_f_name); i++)
        {
            plain_file_name_buffer[i] = plain_f_name[i];
        }
        plain_file_name_buffer[strlen(plain_f_name)] = '\0';

        const char* entropy_o_f_name = "entropy_out.txt";
        entropy_out_f_name = new char[strlen(entropy_o_f_name) + 1];
        for (unsigned int i = 0; i < strlen(entropy_o_f_name); i++)
        {
            entropy_out_f_name[i] = entropy_o_f_name[i];
        }
        entropy_out_f_name[strlen(entropy_o_f_name)] = '\0';
    }
    FILE* entropy_file = fopen(entropy_out_f_name, "a");
    
    //                                                                                       preparing  for entropy calculation
    printf("Text files: %s, %s, %s\n", raw_file_name_buffer, plain_file_name_buffer, entropy_out_f_name);
    //                                                                                       filtering raw text from ,.? etc..
    if (filter_text(raw_file_name_buffer, plain_file_name_buffer, lang, ignoreBlanks))
    {
        printf("Error filtering raw text.\n");
        return 1;
    }
    unsigned long int plain_text_len = 0, blanks_count = 0;
    //                                                                                       counting total characters count
    if (get_text_length(plain_file_name_buffer, plain_text_len, blanks_count))
    {
        printf("Error calculating plain text length.\n");
        return 1;
    }
    printf("Plain text length: %d\n\n", plain_text_len);
    fprintf(entropy_file, "lang=%u ; N=%u ; n_blanks=%u\n", lang, plain_text_len, blanks_count);
    //                                                                                       n-gramm rate calculation
    unsigned long int tmp_arr[4] = { lang, 1, plain_text_len , blanks_count };
    const char* f_name = "1-gramm_data_____blanks.txt";
    char* gramm_data_f_name = new char[strlen(f_name) + 1];
    for (unsigned int i = 0; i < strlen(f_name); i++)
    {
        gramm_data_f_name[i] = f_name[i];
    }
    gramm_data_f_name[strlen(f_name)] = '\0';
    for (unsigned int i = 1; i <= n_for_gramm; i++)
    {
        map<unsigned long int, float>* ensemble = new map<unsigned long int, float>();
        float entropy = 0.0f;
        tmp_arr[1] = i;
        if (ignoreBlanks)
        {
            gramm_data_f_name[14] = 'n';
            gramm_data_f_name[15] = 'o';
        }
        else
        {
            gramm_data_f_name[13] = 'a';
            gramm_data_f_name[14] = 'n';
            gramm_data_f_name[15] = 'd';
        }
        gramm_data_f_name[0] = (char)(i + (int)'1' - 1);
        generate_gramm_rate_file(plain_file_name_buffer, gramm_data_f_name, tmp_arr);
        read_ensemble_from_file(gramm_data_f_name, ensemble);
        calculate_ensemble_entropy(ensemble, entropy);
        printf("Entropy: %0.20f\n", entropy);
        fprintf(entropy_file,"n=%u ; H=%0.12f\n",i ,entropy );
    }
    

    fclose(entropy_file);
    
    return 0;
}

int calculate_text_entropy_for_ngramm_MAP(unsigned int lang, unsigned int n_for_gramm, bool ignoreBlanks)
{
    if (n_for_gramm > 9)
    {
        printf("Error: too long gramm.\n");
        return 1;
    }
    if (!ignoreBlanks) printf("////////////////////////\nAnalysing text including blanks...\n////////////////////////\n");
    else printf("////////////////////////\nAnalysing text NOT including blanks...\n////////////////////////\n");
    char c = '\n';
    printf("Do you want to use custom i/o files(y/n): ");
    while (c == '\n') c = getchar();
    char* raw_file_name_buffer;
    char* plain_file_name_buffer;
    char* entropy_out_f_name;
    if (c == 'y')
    {
        raw_file_name_buffer = new char[fname_buff_size];
        plain_file_name_buffer = new char[fname_buff_size];
        printf("Enter filename|path of text file with raw text: ");
        scanf("%s", raw_file_name_buffer);
        printf("Enter filename|path for plain text file : ");
        scanf("%s", plain_file_name_buffer);

        entropy_out_f_name = new char[fname_buff_size];
        printf("Enter filename|path of text file for entropy output: ");
        scanf("%s", entropy_out_f_name);
    }
    else
    {
        const char* raw_f_name = "raw_text.txt";
        raw_file_name_buffer = new char[strlen(raw_f_name) + 1];
        for (unsigned int i = 0; i < strlen(raw_f_name); i++)
        {
            raw_file_name_buffer[i] = raw_f_name[i];
        }
        raw_file_name_buffer[strlen(raw_f_name)] = '\0';

        const char* plain_f_name = "plain_text.txt";
        plain_file_name_buffer = new char[strlen(plain_f_name) + 1];
        for (unsigned int i = 0; i < strlen(plain_f_name); i++)
        {
            plain_file_name_buffer[i] = plain_f_name[i];
        }
        plain_file_name_buffer[strlen(plain_f_name)] = '\0';

        const char* entropy_o_f_name = "entropy_out.txt";
        entropy_out_f_name = new char[strlen(entropy_o_f_name) + 1];
        for (unsigned int i = 0; i < strlen(entropy_o_f_name); i++)
        {
            entropy_out_f_name[i] = entropy_o_f_name[i];
        }
        entropy_out_f_name[strlen(entropy_o_f_name)] = '\0';
    }
    FILE* entropy_file = fopen(entropy_out_f_name, "a");

    //                                                                                       preparing  for entropy calculation
    printf("Text files: %s, %s, %s\n", raw_file_name_buffer, plain_file_name_buffer, entropy_out_f_name);
    //                                                                                       filtering raw text from ,.? etc..
    if (filter_text(raw_file_name_buffer, plain_file_name_buffer, lang, ignoreBlanks))
    {
        printf("Error filtering raw text.\n");
        return 1;
    }
    unsigned long int plain_text_len = 0, blanks_count = 0;
    //                                                                                       counting total characters count
    if (get_text_length(plain_file_name_buffer, plain_text_len, blanks_count))
    {
        printf("Error calculating plain text length.\n");
        return 1;
    }
    printf("Plain text length: %d\n\n", plain_text_len);
    fprintf(entropy_file, "lang=%u ; N=%u ; n_blanks=%u\n", lang, plain_text_len, blanks_count);
    //                                                                                       n-gramm rate calculation
    unsigned long int tmp_arr[4] = { lang, 1, plain_text_len , blanks_count };
    const char* f_name = "1-gramm_data_____blanks.txt";
    char* gramm_data_f_name = new char[strlen(f_name) + 1];
    for (unsigned int i = 0; i < strlen(f_name); i++)
    {
        gramm_data_f_name[i] = f_name[i];
    }
    gramm_data_f_name[strlen(f_name)] = '\0';
    for (unsigned int i = 1; i <= n_for_gramm; i++)
    {
        map<unsigned long int, float>* ensemble = new map<unsigned long int, float>();
        float entropy = 0.0f;
        tmp_arr[1] = i;
        if (ignoreBlanks)
        {
            gramm_data_f_name[14] = 'n';
            gramm_data_f_name[15] = 'o';
        }
        else
        {
            gramm_data_f_name[13] = 'a';
            gramm_data_f_name[14] = 'n';
            gramm_data_f_name[15] = 'd';
        }
        gramm_data_f_name[0] = (char)(i + (int)'1' - 1);
        generate_gramm_rate_file_MAP(plain_file_name_buffer, gramm_data_f_name, tmp_arr);
        read_ensemble_from_file(gramm_data_f_name, ensemble);
        calculate_ensemble_entropy(ensemble, entropy);
        printf("Entropy: %0.20f\n", entropy);
        fprintf(entropy_file, "n=%u ; H=%0.12f\n", i, entropy);
    }


    fclose(entropy_file);

    return 0;
}