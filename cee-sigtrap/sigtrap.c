#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

static void SignalHandler(int);
void PrintUsage(char*);
char *StripPath(char*);

#define DEFAULT_TIME 20
#define DEFAULT_OP   stdout
#define DEFAULT_NAME "sigtrap"

// ANSI escape for foreground
#define BLACK      "\033[30m"
#define RED        "\033[31m"
#define GREEN      "\033[32m"
#define YELLOW     "\033[33m"
#define BLUE       "\033[34m"
#define MAGENTA    "\033[35m"
#define CYAN       "\033[36m"
#define WHITE      "\033[37m"

// ANSI escape for background
#define ON_BLACK   "\033[40m"
#define ON_RED     "\033[41m"
#define ON_GREEN   "\033[42m"
#define ON_YELLOW  "\033[43m"
#define ON_BLUE    "\033[44m"
#define ON_MAGENTA "\033[45m"
#define ON_CYAN    "\033[46m"
#define ON_WHITE   "\033[47m"

// ANSI escape for normal text
#define NORMAL	   "\033[0m"

char *colours [] = {
    BLACK ON_WHITE,
    BLACK ON_YELLOW,
    BLACK ON_CYAN,
    BLACK ON_GREEN,
    WHITE ON_RED,
    WHITE ON_BLACK,
    WHITE ON_MAGENTA,
    WHITE ON_BLUE,
    MAGENTA ON_YELLOW,
    MAGENTA ON_GREEN,
    MAGENTA ON_WHITE,
    MAGENTA ON_CYAN
    CYAN ON_RED,
    CYAN ON_MAGENTA,
    CYAN ON_BLACK,
    CYAN ON_BLUE,
    BLUE ON_CYAN,
    BLUE ON_YELLOW,
    BLUE ON_GREEN,
    BLUE ON_WHITE,
    GREEN ON_BLUE,
    GREEN ON_MAGENTA,
    GREEN ON_RED,
    GREEN ON_BLACK,
    RED ON_CYAN,
    RED ON_YELLOW,
    RED ON_WHITE,
    RED ON_GREEN,
    YELLOW ON_BLACK,
    YELLOW ON_BLUE,
    YELLOW ON_MAGENTA,
    YELLOW ON_RED,
};

#define N_COLOUR 32

FILE *output = NULL;

static int signal_SIGINT = false;
static int signal_SIGQUIT = false;
static int signal_SIGHUP = false;
static int signal_SIGTERM = false;
static int signal_SIGABRT = false;
static int signal_SIGCONT = false;
static int signal_SIGTSTP = false;

//==============================================================================
// MAIN FUNCTION
//==============================================================================
int main(int argc, char *argv[]) {
    if (argc > 2 || (argc == 2 && !isdigit((int)argv[1][0]))) {
        PrintUsage(argv[0]);
        exit(EX_USAGE);
    }

    output = DEFAULT_OP;

    pid_t pid = getpid();

    long clktck = sysconf(_SC_CLK_TCK);
    struct tms t;
    sigset_t mask;



    char* colour = colours[pid % N_COLOUR]; // select colour for this process
    fprintf(output,"%s%7d; START" BLACK NORMAL "\n", colour, (int) pid);
    fflush(output);

    // let our signal handler catch the following signals
    signal(SIGINT, SignalHandler);
    signal(SIGQUIT, SignalHandler);
    signal(SIGHUP, SignalHandler);
    signal(SIGTERM, SignalHandler);
    signal(SIGABRT, SignalHandler);
    signal(SIGTSTP, SignalHandler);

    // done intrinsically after return from SIGTSTP, due to Darwin/BSD having
    // inconsistent SIGCONT behaviour
    //signal(SIGCONT, SignalHandler);

    // be as nice as possible
    if (setpriority(PRIO_PROCESS, 0, 20) != 0) {
        perror("Setting niceness level to 20 failed");
        exit(EX_OSERR);
    }

    int cycle = DEFAULT_TIME;

    if (argc == 2 && (cycle = atoi(argv[1])) == 0) {
        fprintf(stderr, "Invalid argument, [seconds] is a number.");
        PrintUsage(argv[0]);
        exit(EX_USAGE);
    }

    unsigned int milliseconds = 1000;

    for (int i = 0; i < cycle;) {
        if (signal_SIGCONT) {
            signal_SIGCONT = false;
            fprintf(output, "%s%7d; SIGCONT" BLACK NORMAL "\n", colour, (int) pid);
            fflush(output);
        }

        // get elapsed time of sleep by recording time before
        clock_t before = times(&t);

#if _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000 * 1000;

        struct timespec rm;
        if (nanosleep(&ts, &rm) != 0) {
            perror("Could not nanosleep");
            exit(EX_TEMPFAIL);
        }
#elif _BSD_SOURCE
        if (usleep(milliseconds * 1000) != 0) {
            perror("Could not usleep");
            exit(EX_TEMPFAIL);
        }
#else
        if (sleep(milliseconds / 1000) != 0) {
            fprintf(stderr, "Could not sleep. Sleep was interrupted.");
            exit(EX_TEMPFAIL);
        }
#endif
        // and recording the time after
        clock_t after = times(&t);

        // if we've slept for 1 second
        if ((after - before) >= clktck) {
            fprintf(
                output,
                "%s%7d; tick %d (%d vs %d)" BLACK NORMAL "\n",
                colour,
                (int) pid,
                ++i,
                (after - before),
                clktck / 2
            );
        }

        if (signal_SIGINT) {
            fprintf(output,"%s%7d; SIGINT" BLACK NORMAL "\n", colour, (int) pid);
            exit(0);
        }

        if (signal_SIGQUIT) {
            fprintf(output,"%s%7d; SIGQUIT" BLACK NORMAL "\n", colour, (int) pid);
            exit(0);
        }

        if (signal_SIGHUP) {
            fprintf(output,"%s%7d; SIGHUP" BLACK NORMAL "\n", colour, (int) pid);
            exit(0);
        }

        if (signal_SIGTSTP) {
            signal_SIGTSTP = false;
            fprintf(output,"%s%7d; SIGTSTP" BLACK NORMAL "\n", colour, (int) pid);
            fflush(output);

            // unblock SIGSTP if necessary
            sigemptyset(&mask);
            sigaddset(&mask, SIGTSTP);
            sigprocmask(SIG_UNBLOCK, &mask, NULL);

            // reset trap to default
            signal(SIGTSTP, SIG_DFL);

            // suspend ourself
            raise (SIGTSTP);

            // reset trap on return from suspension
            signal(SIGTSTP, SignalHandler);

            // set flag here rather than trap signal
            signal_SIGCONT = true;
        }

        if (signal_SIGABRT) {
            fprintf(output,"%s%7d; SIGABRT" BLACK NORMAL "\n", colour, (int) pid);
            fflush(output);
            signal(SIGABRT, SIG_DFL);
            raise(SIGABRT);
        }

        if (signal_SIGTERM) {
            fprintf(output,"%s%7d; SIGTERM" BLACK NORMAL "\n", colour, (int) pid);
            exit(EX_OK);
        }

        fflush(output);
    }

    exit(EX_OK);
}

//==============================================================================
// Signal handler is an interrupt handler. Used to set flags so we can handle
// the interrupt in the main while loop.
//==============================================================================
static void SignalHandler(int sig) {
    switch (sig) {
        case SIGINT:
            signal_SIGINT = true;
            break;
        case SIGQUIT:
            signal_SIGQUIT = true;
            break;
        case SIGHUP:
            signal_SIGHUP = true;
            break;
        case SIGCONT:
            signal_SIGCONT = true;
            break;
	case SIGTSTP:
            signal_SIGTSTP = true;
            break;
        case SIGABRT:
            signal_SIGABRT = true;
            break;
        case SIGTERM:
            signal_SIGTERM = true;
            break;
    }
}

//==============================================================================
// Print the usage of the program. Used when the user incorrectly uses the
// program, e.g. the program arguments (argv) are incorrectly provided
//==============================================================================
void PrintUsage(char *pgmName) {
    char *actualName;

    if (!(actualName = StripPath(pgmName))) {
        actualName = DEFAULT_NAME;
    }

    fprintf(
        stderr,
        "\n"
        "  program: %s - trap and report process control signals\n\n"
        "    usage:\n\n"
        "      %s [seconds]\n\n"
        "      where [seconds] is the lifetime of the program - default = 20s.\n\n"
        "    the program sleeps for a second, reports process id and tick count\n"
        "    before sleeping again. any process control signals: SIGINT, SIGQUIT\n"
        "    SIGHUP, SIGTERM, SIGABRT, SIGCONT, SIGTSTP, are trapped and\n"
        "    reported before being actioned.\n\n",
        actualName,
        actualName
    );
}

//==============================================================================
// Strips the path part of a filename by returning the pointer to the filename
// within a given pathname. NULL returned if the path was a directory (i.e. the
// pathname ended in '/') or if something else was not right.
//==============================================================================
char *StripPath(char *pathname) {

    // if pathname is NULL or NULL terminated
    if (!pathname || !(*pathname)) {
        return NULL;
    }

    char *filename = strrchr(pathname, '/');

    // is there a '/' to even consider?
    if (!filename) {
        return pathname;
    }

    // is this a file, and not a directory?
    if (*(++filename)) {
        return filename;
    }

    return NULL;
}

