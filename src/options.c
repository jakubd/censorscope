#include "options.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void print_usage(const char *program) {
    const char *usage_string =
        "Usage: %s [options]\n"
        "  -h --help\n"
        "  -i --max-instructions <instructions> (default: 0)\n"
        "  -l --luasrc-dir <path> (default: \"luasrc\")\n"
        "  -m --max-memory <bytes> (default: 0)\n"
        "  -s --sandbox-dir <path> (default: \"sandbox\")\n";
    fprintf(stderr, usage_string, program);
}

int censorscope_options_init(censorscope_options_t *options,
                             int argc,
                             char **argv) {
    options->sandbox_dir = strdup("sandbox");
    if (!options->sandbox_dir) {
        perror("stdrup");
        return -1;
    }
    options->luasrc_dir = strdup("luasrc");
    if (!options->luasrc_dir) {
        free(options->sandbox_dir);
        perror("stdrup");
        return -1;
    }
    options->max_memory = 0;
    options->max_instructions = 0;

    const char *short_options = "hi:l:m:s:";
    const struct option long_options[] = {
        {"help", 0, NULL, 'h'},
        {"max-instructions", 1, NULL, 'i'},
        {"luasrc-dir", 1, NULL, 'l'},
        {"max-memory", 1, NULL, 'm'},
        {"sandbox-dir", 1, NULL, 's'},
        {0, 0, 0, 0}
    };
    for (;;) {
        int option_index = 0;
        int c = getopt_long(argc,
                            argv,
                            short_options,
                            long_options,
                            &option_index);
        if (c == -1) {
            break;
        }

        char *first_invalid;
        switch (c) {
        case 'h':
            print_usage(argv[0]);
            censorscope_options_destroy(options);
            exit(EXIT_SUCCESS);

        case 'i':
            errno = 0;
            options->max_instructions = strtol(optarg, &first_invalid, 10);
            if (errno) {
                perror("strtol");
                censorscope_options_destroy(options);
                return -1;
            }
            if (first_invalid[0] != '\0') {
                fprintf(stderr, "Invalid instruction count.\n");
                return -1;
            }
            break;

        case 'l':
            free(options->luasrc_dir);
            options->luasrc_dir = strdup(optarg);
            if (!options->luasrc_dir) {
                perror("strdup");
                censorscope_options_destroy(options);
                return -1;
            }
            break;

        case 'm':
            errno = 0;
            options->max_memory = strtol(optarg, &first_invalid, 10);
            if (errno) {
                perror("strtol");
                censorscope_options_destroy(options);
                return -1;
            }
            if (first_invalid[0] != '\0') {
                fprintf(stderr, "Invalid max memory.\n");
                censorscope_options_destroy(options);
                return -1;
            }
            break;

        case 's':
            free(options->sandbox_dir);
            options->sandbox_dir = strdup(optarg);
            if (!options->sandbox_dir) {
                perror("strdup");
                censorscope_options_destroy(options);
                return -1;
            }
            break;

        default:
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int censorscope_options_destroy(censorscope_options_t *options) {
    free(options->luasrc_dir);
    free(options->sandbox_dir);
    return 0;
}