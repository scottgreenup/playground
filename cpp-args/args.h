#ifndef _ARGS_H_
#define _ARGS_H_

#include <string>

/**
 * The API should be simple.
 *
 * I want 
 */
class Args {
 public:
    Args(int argc, char* argv[]);
    ~Args();

    struct Rep;

 private:
    Rep* rep;
};

#endif
