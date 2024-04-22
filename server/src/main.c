#include "server.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static void close_parent_process(int pid) {
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
}

static void daemonize_process() {
    pid_t pid = fork();

    close_parent_process(pid);
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    close_parent_process(pid);
    umask(0);
    printf("Process daemonized. PID: %d\n", getpid());
    printf("To stop: kill %d\n", getpid());

    int fd = open("/dev/null", O_RDWR);

    if (fd < 0) {
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
}

int main(int argc, char *argv[]) {
    t_env_params *env = mx_create_env();
    mx_env_init(env, argc, argv);

    t_string port = mx_itoa(env->port);
    t_string addr = mx_strjoin("http://localhost:", port);

    mx_run_migrations(env->db_connection, env);
    daemonize_process();
    mx_run_server(env, addr);

    mx_strdel(&addr);
    mx_strdel(&port);
    mx_env_close(env);

    return EXIT_SUCCESS;
}
