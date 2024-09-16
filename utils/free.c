/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:17:37 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/15 21:58:48 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// our freeing functions
#include "../minishell.h"

void free_env(t_env *env)
{
	t_env *tmp;
	
	tmp = env;
	while (tmp != NULL)
	{
		env = tmp;



		free(env->variable);		free(env->value);		free(env);		tmp = env->next;
	}
}

void free_export(t_export *export)
{
	t_export *tmp;
	
	tmp = export;
	while (tmp != NULL)
	{
		tmp = export->next;
		free(export->variable);
		free(export->value);
		free(export);
		export = tmp;
	}
}

void free_env_node(t_env *node)
{
    if (node)
    {
        free(node->variable);
        free(node->value);
        free(node);
    }
}