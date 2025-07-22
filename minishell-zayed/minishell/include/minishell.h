#ifndef MINISHELL_H
#define MINISHELL_H

// ...existing includes and declarations...

void handle_heredoc(const char *eof, int pipe_fd[2]);
void execute_with_heredoc(const char *eof, char **cmd, int is_pipe);

#endif
