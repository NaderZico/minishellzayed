#include "minishell.h"
// ...existing includes...

void handle_heredoc(const char *eof, int pipe_fd[2]) {
    char *line;

    close(pipe_fd[0]); // Close the read end of the pipe
    while (1) {
        line = readline("> ");
        if (!line || strcmp(line, eof) == 0) {
            free(line);
            break;
        }
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]); // Close the write end of the pipe
}
