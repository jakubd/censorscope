#ifndef CENSORSCOPE_REGISTER_H
#define CENSORSCOPE_REGISTER_H

#include "options.h"
#include "sandbox.h"

/* Call this function to register a set of C functions with the provided
 * sandbox. The primitives API needs these functions to run.
 *
 * You should run this function after creating a sandbox and before
 * running any scripts in it.
 *
 */
int register_functions(censorscope_options_t *options, sandbox_t *sandbox);

#endif
