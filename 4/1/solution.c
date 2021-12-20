#include <stdio.h>

#define GRID_SIZE 5
#define NUMBER_CAP 128
#define BOARD_CAP 128
#define ROW_IDX 0
#define COL_IDX 1

#define LF 0x0A
#define CR 0x0D
#define ZERO_CHAR 0x30
#define SPACE_CHAR 0x20
#define COMMA_CHAR 0x2C

void parse_number_sequence(FILE* input, unsigned int* array, unsigned int* size);
void parse_boards(FILE* input, unsigned char (*boards)[GRID_SIZE][GRID_SIZE], unsigned int* boards_size);
unsigned int parse_board_number(FILE* input, int first_char);
void print_boards(unsigned char (*boards)[GRID_SIZE][GRID_SIZE], unsigned int size);
int is_marked(unsigned char board_number, unsigned int* n_sequence, size_t winning_n_idx) ;
int sum_of_unmarked_numbers(unsigned char (*boards)[GRID_SIZE][GRID_SIZE], size_t board_idx, 
    unsigned int* n_sequence, size_t winning_n_idx);

int main(void)
{
    FILE* input = fopen("../input.txt", "rb");

    // parse number sequence
    unsigned int numbers[NUMBER_CAP];
    unsigned int numbers_size = 0;
    parse_number_sequence(input, numbers, &numbers_size);

    // parse boards
    unsigned char boards[BOARD_CAP][GRID_SIZE][GRID_SIZE];
    unsigned int boards_size = 0;
    parse_boards(input, boards, &boards_size);


    // we will count how many numbers are marked per row and column on each board
    //  22 13 17 11  0 | 0                  22 13 17 11  0 | 0
    //   8  2 23  4 24 | 0                   8  2 23  4 24 | 1
    //  21  9 14 16  7 | 0  -- 7, 4, 9 -->  21  9 14 16  7 | 2
    //   6 10  3 18  5 | 0                   6 10  3 18  5 | 0
    //   1 12 20 15 19 | 0                   1 12 20 15 19 | 0
    //  --------------+                     --------------+
    //   0  0  0  0  0                       0  1  0  1  1

    unsigned char counters[BOARD_CAP][2][GRID_SIZE];
    size_t counters_size = boards_size; 
    for (size_t i = 0; i < counters_size; i++) 
        for (size_t j = 0; j < GRID_SIZE; j++)
            { counters[i][ROW_IDX][j] = 0; counters[i][COL_IDX][j] = 0; }
        
    
    // find the winning board and calculate
    int winner = 0;
    for (size_t n = 0; n < numbers_size && !winner; n++)
    {
        for (size_t b = 0; b < boards_size && !winner; b++)
        {
            for (size_t r = 0; r < GRID_SIZE && !winner; r++)
            {
                for (size_t c = 0; c < GRID_SIZE && !winner; c++)
                {
                    if (boards[b][r][c] == numbers[n])
                    {
                        counters[b][ROW_IDX][r]++;
                        counters[b][COL_IDX][c]++;

                        if (counters[b][ROW_IDX][r] == 5 || counters[b][COL_IDX][c] == 5) 
                        {
                            winner = 1;
                            printf("winner winner chicken dinner\n");
                            printf("n: %d, b: %d\n", n, b);

                            // calculate sum of unmarked numbers
                            printf("score: %d\n", sum_of_unmarked_numbers(boards, b, numbers, n) * numbers[n]);
                        }
                    }
                }
            }
        }
    }



    fclose(input);
    return 0;
}

void parse_number_sequence(FILE* input, unsigned int* array, unsigned int* size)
{
    int c;
    unsigned int num = 0;
    while (1)
    {
        c = fgetc(input);

        if (c >= ZERO_CHAR && c <= ZERO_CHAR + 9)
        {
            num = num * 10 + c - ZERO_CHAR;
        }
        else if (c == COMMA_CHAR || c == LF || c == CR)
        {
            array[*size] = num;
            *size = *size + 1;
            num = 0;

            if (c != COMMA_CHAR) break;
        }
        else break;
    }
    printf("numbers: ");
    for (size_t i = 0; i < *size; i++) printf("%d ", array[i]);
    printf("\n");
    printf("numbers_size: %d\n", *size);
}

void parse_boards(FILE* input, unsigned char (*boards)[GRID_SIZE][GRID_SIZE], unsigned int* boards_size)
{
    int c;
    unsigned int num = 0;
    unsigned int row_counter = 0;
    unsigned int column_counter = 0;
    while ((c = fgetc(input)) != EOF)
    {
        if (column_counter == GRID_SIZE) 
        {
            column_counter = 0;
            row_counter++;

            if (row_counter == GRID_SIZE)
            {
                row_counter = 0;
                *boards_size = *boards_size + 1;
            }
        }

        if ((c >= ZERO_CHAR && c <= ZERO_CHAR + 9) || c == SPACE_CHAR)
        {
            unsigned int num = parse_board_number(input, c);

            boards[*boards_size][row_counter][column_counter] = num;
            column_counter++;

            fgetc(input); // consume char after board number, should be either space, cr or lf
        }
    }

    printf("boards: \n");
    print_boards(boards, *boards_size);
    printf("boards_size: %d\n", *boards_size);
}

// parses the current char (first_char) and the next char
// (they should represent a number on the board)
unsigned int parse_board_number(FILE* input, int first_char) {
    unsigned int num = 0;
    if (first_char != SPACE_CHAR) num = first_char - ZERO_CHAR;
    return num * 10 + fgetc(input) - ZERO_CHAR;
}

void print_boards(unsigned char (*boards)[GRID_SIZE][GRID_SIZE], unsigned int size)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < GRID_SIZE; j++)
        {
            for (size_t k = 0; k < GRID_SIZE; k++)
            {
                if (boards[i][j][k] > 9) printf("%d ", boards[i][j][k]);
                else printf("%d  ", boards[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int is_marked(unsigned char board_number, unsigned int* n_sequence, size_t winning_n_idx) 
{
    for (size_t i = 0; i <= winning_n_idx; i++)
        if (board_number == n_sequence[i]) return 1;
    
    return 0;
}

int sum_of_unmarked_numbers(
    unsigned char (*boards)[GRID_SIZE][GRID_SIZE], 
    size_t board_idx, 
    unsigned int* n_sequence, 
    size_t winning_n_idx) 
{
    int sum = 0;
    for (size_t r = 0; r < GRID_SIZE; r++)
    {
        for (size_t c = 0; c < GRID_SIZE; c++)
        {
            if (!is_marked(boards[board_idx][r][c], n_sequence, winning_n_idx))
                sum += boards[board_idx][r][c];
        }
    }

    return sum;
}