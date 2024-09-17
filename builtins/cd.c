/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:00:29 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/17 18:33:29 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd(char **args, t_env **envir, int *last_exit_status)
{
	char	*path;
	char	cwd[256];

	if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			perror("cd: $HOME not set");
			*last_exit_status = 1;
			return ;
		}
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		*last_exit_status = 1;
	}
	else
	{
		update_env_for_cd(envir, "OLDPWD", getcopyenv("PWD", envir));
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			update_env_for_cd(envir, "PWD", cwd);
		else
		{
			perror("cd: Error getting current working directory");
			*last_exit_status = 1;
		}
	}
}
