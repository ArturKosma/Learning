#include <iostream>

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
        printf("packager: %s\n", argv[i]);
    }
}

