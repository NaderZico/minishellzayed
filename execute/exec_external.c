#include "execute.h"

/*
 * exec_external
 *   Search PATH for cmd->args[0], check access(), and execve().
 *   On failure, print perror and set *last_status to 127.
 */
#include "execute.h"

/*
 * exec_external
 *   Search PATH for cmd->args[0], check access(), and execve().
 *   On failure, print perror and set *last_status to 127.
 */

void ft_free_split(char **arr)
{
    int i = 0;
    if (!arr)
        return;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}

static char *join_path_cmd(const char *dir, const char *cmd)
{
    char	*full;
    size_t	dir_len = ft_strlen(dir);
    size_t	cmd_len = ft_strlen(cmd);
    size_t	total_len = dir_len + cmd_len + 2;

    full = malloc(total_len);
    if (!full)
        return (NULL);

    ft_memcpy(full, dir, dir_len);
    full[dir_len] = '/';
    ft_memcpy(full + dir_len + 1, cmd, cmd_len);
    full[dir_len + 1 + cmd_len] = '\0';

    return (full);
}

/**
  - Find the full path of a command by searching PATH in envp
 */
char *find_command_path(const char *cmd, char **envp)
{
    char	*path_env = NULL;
    char	**paths = NULL;
    char	*full_path = NULL;
    int		i = 0;

    if (!cmd || !*cmd)
        return (NULL);

    // Absolute or relative path
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        else
            return (NULL);
    }

    // Find PATH in envp
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
        i++;
    }

    if (!path_env)
        return (NULL);

    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);

    i = 0;
    while (paths[i])
    {
        full_path = join_path_cmd(paths[i], cmd);
        if (full_path && access(full_path, X_OK) == 0)
            break;
        free(full_path);
        full_path = NULL;
        i++;
    }

    // Clean up
    ft_free_split(paths);
    return (full_path);
}