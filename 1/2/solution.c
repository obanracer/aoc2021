#include <stdio.h>

// returns eof
int get_sum_and_set_next_pos(FILE* input, fpos_t* next_pos, int* sum) {
    *sum = 0;
    int measurement;

    fscanf(input, "%d", &measurement);
    *sum += measurement;

    // save the next position to scan
    fgetpos(input, next_pos);

    fscanf(input, "%d", &measurement);
    *sum += measurement;

    fscanf(input, "%d", &measurement);
    *sum += measurement;

    return feof(input);
}

int main(void)
{
    FILE* input = fopen("input.txt", "r");

    int counter = 0;
    int p_sum = -1;
    int n_sum = 0;
    fpos_t pos;

    int eof = 0;
    while(1) {
        if (eof) break;
        eof = get_sum_and_set_next_pos(input, &pos, &n_sum);
        printf("n_num: %d - new_pos: %d\n", n_sum, pos);
        fsetpos(input, &pos);
        if (p_sum != -1 && n_sum > p_sum) counter++;
        p_sum = n_sum;
    }

    printf("%d larger sum of measurements.\n", counter);
    return fclose(input);
}

