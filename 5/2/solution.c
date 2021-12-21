#include <stdio.h>

#define LINE_CAP 512
#define LENGTH_CAP 1024

#define LF 0x0A
#define CR 0x0D
#define ZERO_CHAR 0x30
#define COMMA_CHAR 0x2C
#define SPACE_CHAR 0x20
#define DASH_CHAR 0x2D

typedef struct Vector {
    unsigned int x;
    unsigned int y;
} Vector;

typedef struct Line {
    Vector p1;
    Vector p2;
} Line;

void parse(FILE* input, Line* lines, size_t* lines_size, unsigned int* max_x, unsigned int* max_y);
void parse_line(FILE* input, Line* lines, size_t line_idx, int first_char);
void draw_hoz_line(unsigned char (*grid)[LENGTH_CAP], Line line);
void draw_ver_line(unsigned char (*grid)[LENGTH_CAP], Line line);
void draw_dgn_line(unsigned char (*grid)[LENGTH_CAP], Line line);


int main(void) {
    FILE* input = fopen("../input.txt", "rb");

    // parse input
    Line lines[LINE_CAP];
    size_t lines_size = 0;
    unsigned int max_x = 0;
    unsigned int max_y = 0;
    parse(input, lines, &lines_size, &max_x, &max_y);

    // fill grid
    unsigned char grid[LENGTH_CAP][LENGTH_CAP];
    for (size_t i = 0; i < LENGTH_CAP; i++)
        for (size_t j = 0; j < LENGTH_CAP; j++)
            grid[i][j] = 0;

    for (size_t i = 0; i < lines_size; i++) {
        if (lines[i].p1.y == lines[i].p2.y) draw_hoz_line(grid, lines[i]);
        else if (lines[i].p1.x == lines[i].p2.x) draw_ver_line(grid, lines[i]);
        else draw_dgn_line(grid, lines[i]);
    }

    // printf("grid:\n");
    // for (size_t i = 0; i <= max_y; i++) {
    //     for (size_t j = 0; j <= max_x; j++) {
    //         if (grid[j][i] == 0) printf(". ");
    //         else printf("%d ", grid[j][i]);
    //     }
    //     printf("\n");
    // }

    unsigned int overlaps = 0;
    for (size_t i = 0; i < LENGTH_CAP; i++)
        for (size_t j = 0; j < LENGTH_CAP; j++)
            if (grid[i][j] > 1) overlaps++;
    printf("overlaps: %d\n", overlaps);
    
    fclose(input);
    return 0;
}


void parse(FILE* input, Line* lines, size_t* lines_size, unsigned int* max_x, unsigned int* max_y) {
    int c;
    *lines_size = 0;
    while ((c = fgetc(input)) != EOF) {
        if (c >= ZERO_CHAR && c <= ZERO_CHAR + 9) {
            parse_line(input, lines, *lines_size, c);

            // check if we have reached a new max
            if (lines[*lines_size].p1.x > *max_x) *max_x = lines[*lines_size].p1.x;
            if (lines[*lines_size].p1.y > *max_y) *max_y = lines[*lines_size].p1.y;
            if (lines[*lines_size].p2.x > *max_x) *max_x = lines[*lines_size].p2.x;
            if (lines[*lines_size].p2.y > *max_y) *max_y = lines[*lines_size].p2.y;

            *lines_size = *lines_size + 1;
        }
    }

    printf("lines:\n");
    for (size_t i = 0; i < *lines_size; i++) {
        printf("%d,%d -> %d,%d\n", lines[i].p1.x, lines[i].p1.y, lines[i].p2.x, lines[i].p2.y);
    }
    printf("max x: %d\n", *max_x);
    printf("max y: %d\n", *max_y);
}

void parse_line(FILE* input, Line* lines, size_t line_idx, int first_char) {
    int c;

    // parse x1
    unsigned int x1 = first_char - ZERO_CHAR;
    while (1) {
        c = fgetc(input);
        if (c == EOF || c == CR || c == LF) return;

        if (c >= ZERO_CHAR && c <= ZERO_CHAR + 9) x1 = x1 * 10 + c - ZERO_CHAR;
        else if (c == COMMA_CHAR) break;
    }
    
    // parse y1
    unsigned int y1 = 0;
    while (1) {
        c = fgetc(input);
        if (c == EOF || c == CR || c == LF) return;

        if (c >= ZERO_CHAR && c <= ZERO_CHAR + 9) y1 = y1 * 10 + c - ZERO_CHAR;
        else if (c == SPACE_CHAR) break;
    }

    // parse x2
    unsigned int x2 = 0;
    while (1) {
        c = fgetc(input);
        if (c == EOF || c == CR || c == LF) return;

        if (c >= ZERO_CHAR && c <= ZERO_CHAR + 9) x2 = x2 * 10 + c - ZERO_CHAR;
        else if (c == COMMA_CHAR) break;
    }
    
    // parse y2
    unsigned int y2 = 0;
    while (1) {
        c = fgetc(input);
        if (c == EOF) return;

        if (c >= ZERO_CHAR && c <= ZERO_CHAR + 9) y2 = y2 * 10 + c - ZERO_CHAR;
        else if (c == CR || c == LF) break;
    }

    lines[line_idx].p1.x = x1;
    lines[line_idx].p1.y = y1;
    lines[line_idx].p2.x = x2;
    lines[line_idx].p2.y = y2;
}

void draw_hoz_line(unsigned char (*grid)[LENGTH_CAP], Line line) {
    unsigned int start_x;
    unsigned int end_x;
    if (line.p1.x < line.p2.x) { start_x = line.p1.x; end_x = line.p2.x; }
    else { start_x = line.p2.x; end_x = line.p1.x; }

    while (start_x <= end_x) grid[start_x++][line.p1.y]++;
}

void draw_ver_line(unsigned char (*grid)[LENGTH_CAP], Line line) {
    unsigned int start_y;
    unsigned int end_y;
    if (line.p1.y < line.p2.y) { start_y = line.p1.y; end_y = line.p2.y; }
    else { start_y = line.p2.y; end_y = line.p1.y; }

    while (start_y <= end_y) grid[line.p1.x][start_y++]++;
}

void draw_dgn_line(unsigned char (*grid)[LENGTH_CAP], Line line) {
    int length = line.p1.x - line.p2.x;
    if (length < 0) length *= -1;
    length++;
    for (size_t i = 0; i < length; i++) {
        grid[line.p1.x][line.p1.y]++;

        line.p1.x < line.p2.x ? line.p1.x++ : line.p1.x--;
        line.p1.y < line.p2.y ? line.p1.y++ : line.p1.y--;
    }
}