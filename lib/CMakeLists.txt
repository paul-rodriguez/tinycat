
add_library(argparse)

target_sources(
    argparse
    PRIVATE
    argparse/argparse.c
)

target_include_directories(
    argparse
    PUBLIC
    argparse
)


