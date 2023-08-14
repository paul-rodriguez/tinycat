#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "argparse.h"
#include "cli.h"

static char const * const usages[] =
{
    "tinycat [-h|--help]",
    "tinycat [-kl] [[ADDRESS] PORT]",
    "tinycat [-kl] [-U SOCKET]",
    NULL,
};

static char const * const description =
    "\nA small drop-in netcat replacement that exclusively deals with"
    "\nTCP and UNIX socket proxying.";

static char const * undefIfNull(char const * const str);
static char const * boolToString(bool const b);

Args parse(int const argc, char const * * argv)
{
    bool loop = false;
    bool listen = false;
    char const * unixSocket = NULL;
    char const * address = "localhost";
    char const * port = NULL;

    struct argparse_option options[] = {
        OPT_GROUP("Options"),
        OPT_HELP(),
        OPT_BOOLEAN(
                'k',
                "loop",
                &loop,
                "When a connection is completed, listen for another one. "
                "Requires -l. ",
                NULL,
                0,
                0
        ),
        OPT_BOOLEAN(
                'l',
                "listen",
                &listen,
                "Listen for an incoming connection rather than initiating "
                "a connection to a remote host. "
                "The port to listen on must be specified.",
                NULL,
                0,
                0
        ),
        OPT_STRING(
                'U',
                "unix",
                &unixSocket,
                "Use a UNIX-domain socket. "
                "The path to the socket must be provided. "
                "If this option is present, ADDRESS and PORT are ignored. ",
                NULL,
                0,
                0
        ),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(
            &argparse,
            description,
            "\nArguments"
            "\n    ADDRESS    The address to connect to (or listen to if -l "
            "is used)."
            "\n               The default is 'localhost'."
            "\n    PORT       The port to connect to (or listen to if -l is "
            "used)");
    int const remainingArgs = argparse_parse(&argparse, argc, argv);

    if (remainingArgs == 1)
    {
        port = argv[0];
    }
    else if (remainingArgs == 2)
    {
        address = argv[0];
        port = argv[1];
    }
    else if (remainingArgs != 0)
    {
        argparse_usage(&argparse);
        exit(1);
    }

    Args const result = {
            .loop = loop,
            .listen = listen,
            .unixSocket = unixSocket,
            .address = address,
            .port = port,
    };
    return result;
}

int toString(char * const buf, int const size, Args const * const args)
{
    int result = snprintf(
            buf,
            size,
            "Args{loop:%s, listen:%s, unixSocket:%s "
            "address:%s, port:%s}",
            boolToString(args->loop),
            boolToString(args->listen),
            undefIfNull(args->unixSocket),
            undefIfNull(args->address),
            undefIfNull(args->port)
    );
    return result;
}

SocketType getSocketType(Args const * const args)
{
    if (args->unixSocket == NULL)
    {
        return TcpSocket;
    }
    else
    {
        return UnixSocket;
    }
}

static char const * undefIfNull(char const * const str)
{
    return str ? str : "<not defined>";
}

static char const * boolToString(bool const b)
{
    return b ? "true" : "false";
}

