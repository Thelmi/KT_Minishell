/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:04:39 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/23 16:47:49 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*no_quotes(char *arg, t_env *envir, int *last_exit_status)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = allocate_result(arg, envir, last_exit_status);
	if (!res)
		return (NULL);
	// res = NULL; //?
	while (arg[i])
	{
		if (arg[i] == '$' && arg[i + 1] == '?')
		{
			j = handle_exit_status(res, j, last_exit_status);
			i += 2;
		}
		else if (arg[i] == '$' && ft_isalnum(arg[i + 1]))
		{
			// printf("$ %c\n", arg[i + 1]);
			// printf("res before: |%s|\n", res);
			j = handle_var_expansion(res, arg, &i, envir);
			// printf("res after: |%s|\n", res);
			// printf("j = %d\n", j);
		}
		else
			res[j++] = arg[i++];
	}
	res[j] = '\0';
	// printf("res |%s|\n", res);
	return (res);
}

static char	*double_quotes(char *arg, t_env *envir,
		int *last_exit_status)
{
	char	*res;
	int		len;

	len = ft_strlen(arg);
	arg[len - 1] = '\0';
	arg = arg + 1;
	res = no_quotes(arg, envir, last_exit_status);
	return (res);
}

static char	*single_quotes(char *arg)
{
	int		i;
	int		len;
	char	*res;

	i = 0;
	len = ft_strlen(arg);
	res = malloc(sizeof(char) * (len + 1 - 2));
	if (!res)
	{
		perror("malloc");
		return (NULL);
	}
	while (i < len - 2)
	{
		res[i] = arg[i + 1];
		i++;
	}
	res[i] = '\0';
	return (res);
}

void	modify_args(char **args, t_env *envir, int *last_exit_status)
{
	int		i;
	char	*tmp;
	int		len;

	i = 0; // was i = 1

	while (args[i])
	{
		len = ft_strlen(args[i]);
		if (args[i][0] == '\'' && args[i][len - 1] == '\''){
			tmp = single_quotes(args[i]);
		}
		else if (args[i][0] == '"' && args[i][len - 1] == '"')
		{
			tmp = double_quotes(args[i], envir,
					last_exit_status);
		}
		else
		{
			tmp = no_quotes(args[i], envir, last_exit_status);
		
		}
		if (args[i])
			free(args[i]);
		args[i] = tmp;
		// printf("x%s\n", tmp);
		i++;
	}
}