#include <stdio.h>

int main(void)
{
    const char f = 0x66;
    const char d = 0x64;
    const char u = 0x75;
    const char lf = 0x0A;
    const char cr = 0X0D;
    const unsigned int FORWARD_UNITS_INDEX = 8;
    const unsigned int DOWN_UNITS_INDEX = 5;
    const unsigned int UP_UNITS_INDEX = 3;

    FILE* input = fopen("../input.txt", "rb");
    
    int x = 0;
    int y = 0;
    int aim = 0;

    char command;
    int units;
    fpos_t pos;
    fgetpos(input, &pos);

    while ((command = fgetc(input)) != EOF)
    {
        if (command != lf && command != cr)
        {
            // move pointer to units
            fgetpos(input, &pos);
            if (command == f) pos += FORWARD_UNITS_INDEX - 1;
            else if (command == d) pos += DOWN_UNITS_INDEX - 1;
            else if (command == u) pos += UP_UNITS_INDEX - 1;
            fsetpos(input, &pos);

            units = fgetc(input) - 0x30; // read units (should only be 1 digit)

            // move
            if (command == f) { x += units; y += units * aim; }
            else if (command == d) aim += units;
            else if (command == u) aim -= units;
        }
    }

    printf("x: %d, y: %d - x*y: %d\n", x, y, x*y);
    fclose(input);
    return 0;
}
