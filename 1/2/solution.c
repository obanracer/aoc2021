#include <stdio.h>

int main(void)
{
    FILE* input = fopen("../input.txt", "r");

    int counter = 0;

    int first_measurement = 0;
    int second_measurement = 0;
    fpos_t first_next_pos;
    fpos_t second_next_pos;

    // SETUP ============================================================================

    fscanf(input, "%d", &first_measurement); // get first value
    fgetpos(input, &first_next_pos); // store next first pos

    // move the file pointer to second measurement
    fscanf(input, "%d", &second_measurement); // i dont actually care about this measurement yet
    fscanf(input, "%d", &second_measurement); // i dont actually care about this measurement yet

    fscanf(input, "%d", &second_measurement); // get second value
    fgetpos(input, &second_next_pos); // store next second pos

    if (second_measurement > first_measurement) counter++;
    // ==================================================================================
    
    while (!feof(input))
    {
        fsetpos(input, &first_next_pos); // move to the first new measurement
        fscanf(input, "%d", &first_measurement); // get first value
        fgetpos(input, &first_next_pos); // store next first pos
        
        fsetpos(input, &second_next_pos); // move to the second new measurement
        fscanf(input, "%d", &second_measurement); // get second value
        fgetpos(input, &second_next_pos); // store next second pos

        if (second_measurement > first_measurement) counter++;
    }
    
    printf("%d larger sums of measurements.\n", counter);
    return fclose(input);
}

