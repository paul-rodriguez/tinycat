#include <stdio.h>

#include "cli.h"
#include "client.h"
#include "server.h"

int main(int const argc, char const * * argv)
{
    Args const args = parse(argc, argv);
    if (args.listen)
    {
        server(&args);
    }
    else
    {
        client(&args);
    }

    return 0;
}

