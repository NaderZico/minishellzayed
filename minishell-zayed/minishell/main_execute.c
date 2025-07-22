#include <minishell.h> 
#define true    1
#define false   0
/* execution steps 
0. check if it built-in ? execute : pass to step n2
1. check if there is a pipe if there is dup2 to the next command
2. check if there is redirctions > < >> << make sure that u can open the file 
3. start executing and loop again
4.  
// */
// typedef struct s_flags
// {
//     int piped;
//     int built_in; // if it built in and piped it will be runed in child process
// } t_flags;

// int    built_in(t_command data)
// {
//     char *cmp;
//     int i;
//     int j;

//     i = 0;
//     j = 0;
//     cmp = data.args[0];
//     char *built[8] = {"echo","cd","pwd","export","unset","env","exit", NULL};
//     while (built[i])
//     {
//         while(built[i][j] ==cmp[j])
//         {
//             j++;
//             if (built[i][j] == '\0' && cmp[j] == '\0')
//                 return (true);
//         }
//         j = 0;
//         i++;
//     }
//     return false;
// }


// void echo(t_command data)
// {
//     int i = 1;
//     int newline = 1;

//     // check for -n flag
//     if (data.args[i] && ft_strncmp(data.args[i], "-n", 3) == 0)
//     {
//         newline = 0;
//         i++;
//     }

//     // print arguments
//     while (data.args[i])
//     {
//         printf("%s", data.args[i]);
//         if (data.args[i + 1])
//             printf(" ");
//         i++;
//     }

//     if (newline)
//         printf("\n");
// }

// void    pwd()
// {
//     char    *path;
//     path = getcwd(NULL, 0);// function allowed its allocating and printing out the curreent working dir
//     printf("%s\n", path);
//     free(path);
// }

// void    free_envp(char  **env)
// {
//     int i;

//     i = 0;
//     while (env[i])
//     {
//         free(env[i]);
//         i++;
//     }
//     free(env);
//     env = NULL;
// }

// void    export(t_command   command, t_data *da)
// {
//     int i;
//     int j;
//     char    **new;
//     int nnword;

//     i = 0;
//     j = 0;
//     nnword = 0;
//     while (command.args[nnword + 1])// first word dont count
//         nnword++;
//     if (command.args[1] == NULL)
//         {
//             while (da->env[i] != NULL)
//             {
//                 printf("%s\n", da->env[i]);
//                 i++;
//             }
//         }
//     else
//     {
//         while (da->env[i] != NULL)
//             i++;
//         new = malloc(sizeof(char *) * (i + nnword + 1));
//         new[i + nnword] = NULL;
//         i = 0;
//         while (da->env[i])// copying all past words
//         {
//             new[i] = ft_strdup(da->env[i]);   
//             i++;
//         }
//         //copying last word
//         j = 1;
//         while (command.args[j])
//         {
//             new[i] = ft_strdup(command.args[j]);
//             i++;
//             j++;
//         }
//             free_envp(da->env);
//             da->env = new;
//     }
// }

// // void    unset(t_command   data)
// // {
    
// // }

// // void    env(t_command   data)
// // {
    
// // }


// void    cd(t_command   data)
// {
//         char    *path;

//         path = data.args[1];

//         if (!path)
//             path = getenv("HOME");

//         if (chdir(path))
//         {
//             perror("chdir");
//         }
// }

// void    exit_builtin(t_command command, t_data  *data)
// {
//     int digit;
//     int i;

//     digit = 10;
//     i = 0;
//     if (command.args[1])
//     {
//         while (command.args[1][i])
//         {
//             digit = ft_isdigit(command.args[1][i]);
//             if (!digit)
//                 break;
//             i++;
//         }
//         if (!digit)
//         {
//             free_envp(data->env);
//             printf ("exit: %s: numeric argument required\n", command.args[1]);
//             printf("exit\n");
//             exit(255);
//         }

//         else if (command.args[2])
//         {
//             printf("exit: too many arguments\n");
//             data->last_status = 1;
//             return ;
//         }
//         else
//         {
//             free_envp(data->env);
//             printf("exit\n");
//             exit(ft_atoi(command.args[1]));
//         }
//     }
//     else
//     {
//         free_envp(data->env);
//         printf("exit\n");
//         exit (0);
//     }
// }


// void    execute_built_in(t_command data, t_data *da)
// {
//     char *built[8] = {"echo","cd","pwd","export","unset","env","exit", NULL};
//     int i;
//     i = 0;
//     while (ft_strncmp(built[i], data.args[0], ft_strlen(built[i]))!= 0)
//         i++;
//     if (i == 0)
//         echo(data);// 
//     else if (i == 1)
//         cd(data);
//     else if (i == 2)
//         pwd();
//     else if (i == 3)
//         export(data, da);
//     // else if (i == 4)
//     //     unset();
//     // else if (i == 5)
//     //     env();
//     else if (i == 6)
//         exit_builtin(data, da);
//     else
//         printf("command is not exist");
// }

// void    execute_external(t_data *data)
// {
//     int prev_fd;
//     int fd[2];
//     pid_t *pid;
//     int i;
    
//     i = 0;
//     pid = (int *)malloc(sizeof(int) * data->cmd_count);
//     prev_fd = -1;
//     while(i < data->cmd_count)
//     {
//         if (i + 1 < data->cmd_count)
//             pipe(fd);
//         pid[i] = fork();
//         if (pid[i] == 0)
//         {
//             if (prev_fd != -1)
//                 dup2(prev_fd, STDIN_FILENO);// if there is previous file then it will start read from it

//             if (i + 1 < data->cmd_count)
//                  dup2(fd[1], STDOUT_FILENO);// making the write end of the tube is the output becouse this command will be piped
//             if (built_in(data->commands[i]) == true)
//                 execute_built_in(data->commands[i], data);
//             else                
//                 execvp(data->commands[i].args[0], data->commands[i].args);
//             exit(1);
//         }
//         if (prev_fd != -1)
//             close(prev_fd);/*its important becouse the pipe if not closed in the parent the parent
//                             will blocked the child from reading from the file becouse it will wait for EOF
//                             so he can start reading that will never be reached becouse of the parent
//                            */
//         if (i + 1 < data->cmd_count)
//             prev_fd = fd[0];
//         else
//             prev_fd = -1;
//         i++;
//     }

//     for (int j = 0; j < data->cmd_count; j++)
//     {
//        waitpid(pid[j], NULL, 0);
//     }
//         free(pid);
// }

// void    execute_commands(t_data *data)
// {
//     t_flags flags; // i think i need only on file descriptor to initiliza and then re use it the same one multiple time 
//     flags.built_in = false;
//     flags.piped = false;
//         // i will check if the command is need pip

//     if (data->cmd_count != 1)
//         flags.piped = true; 
//         // i will check if the command is built_in
//     flags.built_in = built_in(data->commands[0]);
//     if (flags.built_in == true && flags.piped == false)
//     {
//         //in this case it will be executed dirctly in the parent process
//         execute_built_in(data->commands[0], data);
//     }
//     else
//     {
//         // executed on child process
//         execute_external(data);
//     }
// }
/*
1. finish built_in command
2. hundel redierctors correctly
3. check all momery leaks and reaces and chiled process
4. check if the signals work corrictly
5. hundle parcing problems
*/



