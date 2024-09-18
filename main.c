/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:58:22 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/18 15:52:49 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialize_shell(char **ev, t_env **envir, t_export **exp,
		t_context *context)
{
	context->last_exit_status = 0;
	configure_terminal_behavior();
	setup_signals(context);
	*envir = storing_env(ev);
	*exp = storing_export(ev);
}

static void	command_loop(char **ev, t_env **envir, t_export **exp,
		t_context *context)
{
	char	*command;

	while (1)
	{
		command = readline("minishell$ ");
		if (command == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*command)
		{
			add_history(command);
			runcmd(parsecmd(command, &context->last_exit_status), ev, envir,
				exp, &context->last_exit_status);
		}
		free(command);
	}
	free_env(*envir);
	free_export(*exp);
}

int	main(int ac, char **av, char **ev)
{
	t_env		*envir;
	t_export	*exp;
	t_context	context;

	(void)ac;
	(void)av;
	initialize_shell(ev, &envir, &exp, &context);
	command_loop(ev, &envir, &exp, &context);
	return (context.last_exit_status);
}
