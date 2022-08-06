#include <stdio.h>

int
main()
{
    FILE* fp = fopen("src/ee.txt", "r");
    if (fp != NULL) {
        char str[100];
        fgets(str, 100, fp);
        printf("%s\n", str);
    }
}
