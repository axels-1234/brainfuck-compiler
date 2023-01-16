#include <stdio.h>
#include <time.h>
#define TRUE 1
#define FALSE 0
#define MEMORY_SIZE 30001

unsigned long long int brainfuck(unsigned char *command)
{
    unsigned long long steps = 0;
    unsigned char *command_placement = command;
    unsigned char memory[MEMORY_SIZE];
    unsigned char *memory_placement = memory;

    while (*command_placement != 0)
    {
        steps++;
        switch (*command_placement)
        {
        case '+':
            (*memory_placement)++;
            break;
        case '-':
            (*memory_placement)--;
            break;
        case '>':
            if (memory_placement == memory + MEMORY_SIZE)
                return -1;
            memory_placement++;
            break;
        case '<':
            if (memory_placement == memory)
                return -1;
            memory_placement--;
            break;
        case '.':
            printf("%c", *memory_placement);
            break;
        case ',':
        {
            char buf[MEMORY_SIZE];
            scanf("%s", buf);
            *memory_placement = buf[0];
            break;
        }
        case '[':
            if ((*memory_placement) <= 0)
            {
                int brackets = 0;
                do
                {
                    if (*command_placement == '[')
                        brackets++;
                    if (*command_placement == ']')
                        brackets--;
                    command_placement++;
                } while (brackets > 0);
                command_placement--;
            }
            break;
        case ']':
        {
            int brackets = 0;
            do
            {
                if (*command_placement == ']')
                    brackets++;
                if (*command_placement == '[')
                    brackets--;
                command_placement--;
            } while (brackets > 0);
            break;
        }
        default:
            steps--;
            break;
        }
        command_placement++;
    }

    return steps;
}

int main(int argc, char **argv)
{
    time_t start_t, end_t;
    double diff_t;
    unsigned long long int success = 1;
    if (argc < 2)
        printf("No file provided");
    else if (argc == 2)
    {
        FILE *fp;
        fp = fopen(argv[1], "r");
        if (fp == NULL)
            printf("Could not open file \"%s\"\n", argv[1]);
        else
        {
            time(&start_t);
            fseek(fp, 0L, SEEK_END);
            long int res = ftell(fp);
            fclose(fp);
            fp = fopen(argv[1], "r");
            unsigned char code[res + 1];
            char buf[1024];
            int brackets = 0;
            int i = 0;
            for (; i < res; i++)
            {
                buf[0] = fgetc(fp);
                if (buf != 0)
                    code[i] = buf[0];
                if (buf[0] == '[')
                    brackets++;
                else if (buf[0] == ']')
                    brackets--;
            }
            int cont = (brackets == 0);
            while (!cont)
            {
                printf("Unmatched brackets, do you want to continue? [y/n]\n");
                scanf("%s", buf);
                if (buf[0] >= 'A' && buf[0] <= 'Z')
                    buf[0] += 'a' - 'A';
                if (buf[0] == 'y')
                {
                    cont = TRUE;
                    printf("Executing program");
                    break;
                }
                else if (buf[0] == 'n')
                {
                    cont = FALSE;
                    printf("Exiting program");
                    break;
                }
            }
            if (cont)
            {
                code[i + 1] = 0;
                fclose(fp);
                int brackets = 0;
                success = brainfuck(code);
                time(&end_t);
                diff_t = difftime(end_t, start_t);
                if (success != -1)
                {
                    printf("\nProgram terminated successfully after %lld steps and %f seconds", success, (int)diff_t);
                    return 0;
                }
                else
                {
                    printf("Something didn't work, exited program after %d seconds", (int)diff_t);
                    return 1;
                }
            }
        }
    }
    else
        printf("Too many arguments provided");

    return 0;
}
