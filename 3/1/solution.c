#include <stdio.h>
#include <math.h>

int main(void)
{
    const char lf = 0x0A;
    const char cr = 0x0D;
    const char zero = 0x30;

    FILE* input = fopen("../input.txt", "rb");

    // first, find input width (amount of bits per line)
    char c;
    unsigned int width = 0;
    while (1)
    {   
        c = fgetc(input);
        if (c == EOF || c == lf || c == cr) break;
        else width++;
    }
    printf("input width: %d\n", width);


    // now read the input
    unsigned int input_size = 0; // amount of numbers in input
    unsigned int counters[width];
    for (size_t i = 0; i < width; i++) counters[i] = 0;
    
    fpos_t pos = 0;
    fsetpos(input, &pos); // go back to the beginning
    
    unsigned int i = 0;
    int is_newline = 1;
    while ((c = fgetc(input)) != EOF)
    {
        if (c < zero || c > zero + 1)
        {
            is_newline = 1;
            i = 0;
        }
        else
        {
            if (is_newline) input_size++;
            is_newline = 0;

            if (c != zero) counters[i]++;
            i++;
        }
    }

    printf("input size: %d\n", input_size);
    printf("counters: ");
    for (size_t i = 0; i < width; i++) 
    {
        printf("%d ", counters[i]);
    }
    printf("\n");
    
    unsigned int gamma = 0;
    unsigned int epsilon = 0;
    printf("most common bits: ");
    for (size_t i = 0; i < width; i++) 
    {
        if (counters[i] < input_size/2) counters[i] = 0;
        else counters[i] = 1;

        gamma += counters[i] * pow(2, width - 1 - i);
        epsilon += !counters[i] * pow(2, width - 1 - i);

        printf("%d ", counters[i]);
    }
    printf("\n");

    printf("gamma: %d, epsilon: %d, gamma * epsilon: %d\n", gamma, epsilon, gamma * epsilon);

    return 0;
}
