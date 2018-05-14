
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    pid_t pid = fork();
    if (pid < 0) {
        switch(errno) {
        case EAGAIN:
            fprintf(stderr, "Please try again, a limit was reached.\n");
            break;
        case ENOMEM:
            fprintf(stderr, "Process can't fork due to lack of memory.\n");
            break;
        case ENOSYS:
            fprintf(stderr, "Process forking is not supported on this platform\n");
            break;
        }
    }

    if (pid == 0) {
        fprintf(stdout, "child: running sleep\n", pid);

        freopen("/usr/share/dict/cracklib-small", "r", stdin);
        freopen("words.txt", "w", stdout);

        fprintf(stdout, "Suck it\n");

        execlp("cat", "cat", NULL);
    } else if (pid > 0) {
        fprintf(stdout, "parent: %ld\n", pid);

        int* status;
        waitpid(pid, status, 0);

        if (status != NULL) {
            if (WIFEXITED(*status)) {
                fprintf(stdout, "parent: child exited normally with %d.\n", WEXITSTATUS(*status));
            } else if (WIFSIGNALED(*status)) {
                fprintf(stdout, "parent: child was signalled to stop with %d.\n", WTERMSIG(*status));

#ifdef WCOREDUMP
                if (WCOREDUMP(*status)) {
                    fprintf(stdout, "parent: child also produced a core dump %d.\n");
                }
#endif
            }
        }

        fprintf(stdout, "parent: child has exited\n");
        return 0;
    }

    return 0;
}
