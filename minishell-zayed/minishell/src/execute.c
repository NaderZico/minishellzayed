#include "minishell.h"
// ...existing includes...

void execute_with_heredoc(const char *eof, char **cmd, int is_pipe) {
    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        if (is_pipe) {
            dup2(pipe_fd[0], STDIN_FILENO);
            close(pipe_fd[1]);
        }
        execvp(cmd[0], cmd);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        handle_heredoc(eof, pipe_fd);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        waitpid(pid, NULL, 0);
    }
}
