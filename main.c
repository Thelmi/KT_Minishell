/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:58:22 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/18 21:29:44 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *concat_var_value(const char *variable, const char *value) {
    size_t var_len = ft_strlen(variable);
    size_t val_len = ft_strlen(value);
    char *result = malloc(var_len + val_len + 2); // +2 for '=' and '\0'
    if (result == NULL) {
        perror("malloc");
        return NULL;
    }
    
    // Copy the variable name
    ft_strlcpy(result, variable, var_len + 1); // var_len + 1 for null termination
    // Append '='
    result[var_len] = '=';
    // Append the value
    ft_strlcpy(result + var_len + 1, value, val_len + 1); // val_len + 1 for null termination
    
    return result;
}

void fill_env(t_env **env, char **ev)
{
    t_env *tmp;

    tmp = *(env);
    while (tmp)
    {
        tmp->ev = ev;
        tmp = tmp->next;
    }
}

static char **convert_env(t_env **env) {
    int count = 0;
    t_env *temp = *env;  // Dereference double pointer

    // Count the number of environment variables
    while (temp) {
        count++;
        temp = temp->next;
    }

    // Allocate memory for the array of strings
    char **env_array = malloc((count + 1) * sizeof(char *));
    if (env_array == NULL) {
        perror("malloc");
        return NULL;
    }

    // Populate the array
    temp = *env;  // Dereference double pointer
    int i = 0;
    while (temp) {
        env_array[i] = concat_var_value(temp->variable, temp->value);
        if (env_array[i] == NULL) {
            // Free previously allocated memory in case of error
            int j = 0;
            while (j < i) {
                free(env_array[j]);
                j++;
            }
            free(env_array);
            return NULL;
        }
        i++;
        temp = temp->next;
    }

    env_array[count] = NULL; // Null-terminate the array
    return env_array;
}


static void	initialize_shell(char **ev, t_env **envir, t_export **exp,
		t_context *context)
{
	context->last_exit_status = 0;
	configure_terminal_behavior();
	*envir = storing_env(ev);
	*exp = storing_export(ev);
}

static void	command_loop(char **ev, t_env **envir, t_export **exp,
		t_context *context)
{
	char	*command;
	(void)convert_env; //remove any (void)
    (void)ev;
    (void)context;
	while (1)
	{
		setup_signals();
		// command = readline("minishell$ ");
        write(1, "minishell$ ", ft_strlen("minishell$ ")); //
        command = get_next_line(0); //
		if (command == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
        char *tmp;//
        tmp = ft_substr(command, 0, ft_strlen(command) - 1);//
        free (command);//
        command = tmp;//
		if (*command)
		{
			(*envir)->ev = convert_env(envir);
            fill_env(envir, (*envir)->ev);
			// add_history(command);
            t_main x = parsecmd(command, &context->last_exit_status);//
            x.command = command;
            // x.cmd = expand_tree(x.cmd, *envir, &context->last_exit_status);
            runcmd(x, ev, envir, exp, &context->last_exit_status);
            if (x.cmd)
                freecmd(x.cmd, 0);
            if (envir && *envir && (*envir)->ev)
                free_double_pointer((*envir)->ev);
			// runcmd(parsecmd(command, &context->last_exit_status), ev, envir,
			// 	exp, &context->last_exit_status);
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
