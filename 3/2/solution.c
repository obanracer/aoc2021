#include <stdio.h>

void swap(unsigned int* a, unsigned int* b) {
    unsigned int aux = *a;
    *a = *b;
    *b = aux;
}

unsigned int find_oxygen_rating(unsigned int* report, unsigned int size, unsigned int bit, unsigned int start, unsigned int end);
unsigned int find_co2_rating(unsigned int* report, unsigned int size, unsigned int bit, unsigned int start, unsigned int end);

int main(void) {
    
    const char lf = 0x0A;
    const char cr = 0x0D;
    const char zero = 0x30;

    FILE* input = fopen("../input.txt", "rb");
    int c;
    fpos_t pos = 0;

    // get input width
    unsigned int input_width = 0;
    while ((c = fgetc(input)) != EOF)
    {
        if (c < zero || c > zero + 1) break;
        else input_width++;
    }
    fsetpos(input, &pos); // we are done, go back to the beginning
    
    // get input size
    unsigned int input_size = 0;
    while ((c = fgetc(input)) != EOF)
    {
        if (c == lf) input_size++;
    }
    fsetpos(input, &pos); // we are done, go back to the beginning

    // parse the input
    unsigned int report[input_size];
    unsigned int entry_index = -1;
    unsigned int entry = 0;
    unsigned int bit = input_width - 1;
    int is_newline = 0;
    while ((c = fgetc(input)) != EOF)
    {
        if (c == zero || c == zero + 1) 
        {
            if (!is_newline) { is_newline = 1; entry = 0; entry_index++; }
            entry += (c - zero) << bit--;
        }
        else 
        {
            report[entry_index] = entry;
            bit = input_width - 1;
            is_newline = 0;
        }
    }
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    printf("original hex report: \t");
    for (size_t i = 0; i < input_size; i++) printf("%#x\t", report[i]);
    printf("\n");
    printf("original dec report: \t");
    for (size_t i = 0; i < input_size; i++) printf("%d\t", report[i]);
    printf("\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    
    

    // split 0s and 1s
    unsigned int end_0 = -1;
    unsigned int start_1 = input_size;
    unsigned int mask = 1 << (input_width - 1);
    while (end_0 + 1 != start_1)
    {
        if (report[end_0 + 1] & mask) swap(&report[end_0 + 1], &report[--start_1]);
        else end_0++;
    }
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    printf("split hex report: \t");
    for (size_t i = 0; i < input_size; i++) printf("%#x\t", report[i]);
    printf("\n");
    printf("split dec report: \t");
    for (size_t i = 0; i < input_size; i++) printf("%d\t", report[i]);
    printf("\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");

    printf("end_0: %d\n", end_0);
    printf("start_1: %d\n", start_1);

    // determine most and least common values
    unsigned int zeroes = 0 + end_0 + 1;
    unsigned int ones = input_size - zeroes;
    printf("zeroes: %d\n", zeroes);
    printf("ones: %d\n", ones);

    unsigned int oxygen_rating = 0;
    unsigned int co2_rating = 0;

    if (zeroes > ones)
    {
        // 0 are most common
        oxygen_rating = find_oxygen_rating(report, input_size, mask >> 1, 0, end_0);
        co2_rating = find_co2_rating(report, input_size, mask >> 1, start_1, input_size - 1);
    }
    else 
    {
        // 1 are most common or equal
        oxygen_rating = find_oxygen_rating(report, input_size, mask >> 1, start_1, input_size - 1);
        co2_rating = find_co2_rating(report, input_size, mask >> 1, 0, end_0);
    }
    printf("oxygen_rating: %d\n", oxygen_rating);
    printf("co2_rating: %d\n", co2_rating);

    printf("oxygen * co2: %d\n", oxygen_rating * co2_rating);

    fclose(input);
    return 0;
}

unsigned int find_oxygen_rating(unsigned int* report, unsigned int size, unsigned int bit, unsigned int start, unsigned int end) 
{
    printf("+== FINDING OXYGEN RATING =====================================================================================================+\n");

    if (start == end) 
    {
        printf("+==============================================================================================================================+\n");
        return report[start];
    }

    // split 0s and 1s
    unsigned int end_0 = start - 1;
    unsigned int start_1 = end + 1;
    while (end_0 + 1 != start_1)
    {
        if (report[end_0 + 1] & bit) swap(&report[end_0 + 1], &report[--start_1]);
        else end_0++;
    }
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    printf("split hex report: \t");
    for (size_t i = start; i < end + 1; i++) printf("%#x\t", report[i]);
    printf("\n");
    printf("split dec report: \t");
    for (size_t i = start; i < end + 1; i++) printf("%d\t", report[i]);
    printf("\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");

    printf("end_0: %d\n", end_0);
    printf("start_1: %d\n", start_1);

    // determine most and least common values
    unsigned int zeroes = end_0 - start + 1;
    unsigned int ones = end - start + 1 - zeroes;
    printf("zeroes: %d\n", zeroes);
    printf("ones: %d\n", ones);

    if (zeroes > ones) return find_oxygen_rating(report, size, bit >> 1, start, end_0);
    else return find_oxygen_rating(report, size, bit >> 1, start_1, end);

    printf("+==============================================================================================================================+\n");
}

unsigned int find_co2_rating(unsigned int* report, unsigned int size, unsigned int bit, unsigned int start, unsigned int end) 
{
    printf("+== FINDING CO2 RATING ========================================================================================================+\n");

    if (start == end)
    {
        printf("+==============================================================================================================================+\n");
        return report[start];
    }

    // split 0s and 1s
    unsigned int end_0 = start - 1;
    unsigned int start_1 = end + 1;
    while (end_0 + 1 != start_1)
    {
        if (report[end_0 + 1] & bit) swap(&report[end_0 + 1], &report[--start_1]);
        else end_0++;
    }
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    printf("split hex report: \t");
    for (size_t i = start; i < end + 1; i++) printf("%#x\t", report[i]);
    printf("\n");
    printf("split dec report: \t");
    for (size_t i = start; i < end + 1; i++) printf("%d\t", report[i]);
    printf("\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");

    printf("end_0: %d\n", end_0);
    printf("start_1: %d\n", start_1);

    // determine most and least common values
    unsigned int zeroes = end_0 - start + 1;
    unsigned int ones = end - start + 1 - zeroes;
    printf("zeroes: %d\n", zeroes);
    printf("ones: %d\n", ones);

    if (zeroes <= ones) return find_co2_rating(report, size, bit >> 1, start, end_0);
    else return find_co2_rating(report, size, bit >> 1, start_1, end);

    printf("+==============================================================================================================================+\n");
}