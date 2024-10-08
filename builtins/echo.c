/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:03:09 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/18 14:28:05 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(char **command, char echar[MAXARGS])
{
	bool	n_flag;
	int	i;
// (void)echar;
	n_flag = false;
	i = 1;
	while (command[i] && command[i][0] == '-' && is_only_n(command[i] + 1))
	{
		n_flag = true;
		i++;
	}
	while (command[i])
	{
		printf("%s", command[i]);
		if (command[i + 1] && echar[i] == ' ')
			printf(" ");
		i++;
	}
	if (n_flag == false)
	{
		printf("\n");
	}
}
