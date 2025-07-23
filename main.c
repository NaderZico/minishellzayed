/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:53:55 by nakhalil          #+#    #+#             */
/*   Updated: 2025/07/22 12:28:02 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Add missing prototype
int ft_strcmp(char *s1, char *s2);

// static void debug_print_tokens(t_data *data)
// {
//     printf("-----------------------------------------------------------------\n");
//     printf("→ [debug] token_count = %d\n", data->token_count);
//     for (int i = 0; i < data->token_count; i++)
//     {
//         t_token *tok = &data->tokens[i];
//         printf("   token[%d]: type=%d, quote=%d, value=\"%s\"\n",
//                i, tok->type, tok->quote, tok->value ? tok->value : "NULL");
//     }
// }

// static void debug_print_commands(t_data *data)
// {
//     printf("→ [debug] cmd_count   = %d\n", data->cmd_count);
//     for (int c = 0; c < data->cmd_count; c++)
//     {
//         t_command *cmd = &data->commands[c];
//         printf("   command[%d]:\n", c);

//         if (cmd->args)
//         {
//             for (int j = 0; cmd->args[j]; j++)
//                 printf("      arg[%d] = \"%s\"\n", j, cmd->args[j]);
//         }
//         else
//             printf("      (no args)\n");

//         for (int r = 0; r < cmd->redir_count; r++)
//         {
//             printf("      redir[%d]: type=%d, file=\"%s\"\n",
//                    r, cmd->redirs[r].type, cmd->redirs[r].file);
//         }
//     }
//     printf("-----------------------------------------------------------------\n");
// }

static void	cleanup_and_exit(t_data *data, int exit_code)
{
	free_data(data);
	clear_history();
	exit(exit_code);
}

char	**copy_envp(char **copy)
{
	int	i;
	char	**new;

	i = 0;
	while (copy[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	new[i] = NULL;
	if (!new)
		return (NULL);
	i = 0;
	while(copy[i])
	{
		new[i] = ft_strdup(copy[i]);
		i++;
	}
	new[i] = NULL;
	return(new);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	char	*input;
	t_error	err;

	(void)argc;
	(void)argv;
	ft_bzero(&data, sizeof(data));
	data.env = copy_envp(envp);
	setup_signals();

	while (1)
	{
		data.syntax_error = 0;

		if (g_signal)
		{
			data.last_status = g_signal;
			g_signal = 0;
			continue;
		}

		// Only print prompt if running interactively and output is a terminal
		if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
			input = readline("minishell$ ");
		else
			input = readline(NULL);

		if (!input)
		{
			if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
				write(1, "exit\n", 5);
			break;
		}

		if (*input)
		{
			add_history(input);

			err = tokenize_input(input, &data);
			if (err != SUCCESS)
				handle_error(err, &data, "tokenization");
			else if ((err = validate_syntax(&data)) != SUCCESS)
				handle_error(err, &data, NULL);
			else if ((err = expand_tokens(&data)) != SUCCESS)
				handle_error(err, &data, "expand");
			else if ((err = parse_tokens(&data)) != SUCCESS)
				handle_error(err, &data, "parse");
			else
				execute_commands(&data);

			// Restore signals after execution
			setup_signals();

			// Exit after non-interactive command (e.g., redirected input)
			if (!isatty(STDIN_FILENO))
			{
				free(input);
				break;
			}
			// Exit if the command is 'exit'
			if (data.cmd_count == 1 && data.commands[0].args &&
				data.commands[0].args[0] &&
				ft_strcmp(data.commands[0].args[0], "exit") == 0)
			{
				free(input);
				break;
			}
		}

		free(input);
	}

	cleanup_and_exit(&data, data.last_status);
}
