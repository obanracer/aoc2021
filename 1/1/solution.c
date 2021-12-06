#include <stdio.h>

int main(void)
{
    FILE* input = fopen("../input.txt", "r");

    int p_measurement = -1;
    int n_measurement = 0;
    int counter = 0;
    while (!feof(input))
    {
        fscanf(input, "%d", &n_measurement); // read new measurement

        if (p_measurement != -1 && n_measurement > p_measurement) counter++;

        p_measurement = n_measurement;
    }
    
    printf("%d larger measurements.\n", counter);
    return fclose(input);
}
