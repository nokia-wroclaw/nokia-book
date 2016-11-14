// Run-Time environment
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct args {
    int data[16];
};

int __MINIK_read()
{
    char line[32];
    int val = 0;
    fgets(line, 19, stdin);
    line[31] = 0;
    sscanf(line, "%i", &val);
    return val;
}

void __MINIK_write(int x)
{
    printf("%i\n", x);
}

int __MINIK_main();

int main()
{
    int ret = __MINIK_main();
    printf("%d\n", ret);
    return ret;
}
