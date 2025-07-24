#include "execute.h"
#include <sys/stat.h>
#include <libgen.h>
#include <errno.h>

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

// Helper: get parent directory (returns malloc'd string, or NULL)
static char *get_parent_dir(const char *path)
{
    int len = ft_strlen(path);
    if (len == 0)
        return NULL;
    int i = len - 1;
    // Skip trailing slashes
    while (i > 0 && path[i] == '/')
        i--;
    // Find previous slash
    while (i > 0 && path[i] != '/')
        i--;
    if (i == 0)
        return ft_strdup("/");
    return ft_substr(path, 0, i);
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
    struct stat st;

    if (!cmd || !*cmd)
        return (NULL);

    // Absolute or relative path
    if (ft_strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
                return (char *)-2; // Directory, permission denied
            if (access(cmd, X_OK) == 0)
                return (ft_strdup(cmd));
            if (access(cmd, F_OK) == 0)
                return (char *)-1; // Permission denied
        }
        // stat failed: check parent directory for EACCES
        char *parent = get_parent_dir(cmd);
        if (parent)
        {
            if (access(parent, X_OK) == -1 && errno == EACCES)
            {
                free(parent);
                return (char *)-2; // Permission denied on parent dir
            }
            free(parent);
        }
        return NULL; // Not found
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
        if (full_path && stat(full_path, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                free(full_path);
                ft_free_split(paths);
                return (char *)-2; // Directory, permission denied
            }
            if (access(full_path, X_OK) == 0)
                break;
            if (access(full_path, F_OK) == 0)
            {
                free(full_path);
                ft_free_split(paths);
                return (char *)-1; // Permission denied
            }
        }
        free(full_path);
        full_path = NULL;
        i++;
    }

    ft_free_split(paths);
    return (full_path); // NULL if not found
}