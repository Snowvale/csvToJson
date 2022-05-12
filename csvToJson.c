#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])  {
    if (argc != 3)  {
        printf("usage: ./a input.csv output.json\n");
        return 1;
    }
    FILE *csv = fopen(argv[1], "r");
    if (!csv)   {
        printf("could not open input file %s\n", argv[1]);
        return 1;
    }

    FILE *json = fopen(argv[2], "w");
    if (!json)  {
        printf("could not open output file %s\n", argv[2]);
        return 1;
    }
    fprintf(json, "{\n\"table\":[\n");
    
    char row[500];
    char word[20];
    fgets(row, 500, csv);
    while (1)  {
        if (feof(csv)) break;
        int nl = 0;
        int len = strlen(row);
        // skips if there is just a space and new line
        if (len < 3)   {
            fgets(row, 500, csv);
            continue;
        }
        fprintf(json, "\t[");
        int w = 0;
        // parses each line
        for (int i = 0; i < len; i++)   {
            if (row[i] != '\n' && isspace(row[i])) continue;
            // cell seperator
            if (row[i] == ';')  {
                word[w] = '\0';
                fprintf(json, "\"%s\", ", word);
                w = 0;
                continue;
            }
            // line seperator
            if (row[i] == '\n') {
                word[w-1] = '\0';
                nl = 1;
                continue;
            }
            word[w] = row[i];
            w++;
        }
        fgets(row, 500, csv);
        // determines how to end the line
        if (feof(csv))
            fprintf(json, "\"%s\"]\n", word);
        else if (nl == 1)    {
            fprintf(json, "\"%s\"],\n", word);
        }
    }
    fprintf(json, "\t]\n}\n");

    fclose(json);
    fclose(csv);

    return 0;
}
