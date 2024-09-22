/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:04:39 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/18 14:37:19 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int new_len(char *arg, t_env *envir, int *last_exit_status){
    int len = 0;
    int i = 0; 
    char var_name[256];
    int var_len;
    while (arg[i]) {
		if(arg[i] == '$' && arg[i+1]=='?'){
            len = ft_strlen(ft_itoa(*last_exit_status));
            i+=2;
        }else if (arg[i] == '$' && ft_isalnum(arg[i+1])){
            var_len = 0;
            i++;
            while (arg[i] && (ft_isalnum(arg[i]) || arg[i]=='_')) {
                var_name[var_len++] = arg[i++];
            }
            var_name[var_len] = '\0';
            if (getcopyenv(var_name, &envir)) {
                len += ft_strlen(getcopyenv(var_name, &envir));
            }
        } else{
            len++;
            i++;
        }
    }
    return len;
}

static char *no_quotes(char *arg, t_env *envir, int *last_exit_status){
    int i;
    int j;
    int var_len;
    char *res;
    char var_name[256]; 
    i = 0;
    j = 0;
    res = malloc(sizeof(char)*(new_len(arg, envir) + 1));
    if (!res) {
        return NULL;
    }
    while (arg[i]) {
        if(arg[i] == '$' && arg[i+1]=='?'){
            ft_strcat(res, ft_itoa(*last_exit_status));
            j = ft_strlen(res);
            i+=2;
        }else if (arg[i] == '$' && (ft_isalnum(arg[i+1])|| arg[i+1]=='_')){
            var_len = 0;
            i++;
            while (arg[i] && (ft_isalnum(arg[i]) || arg[i]=='_')) {
                var_name[var_len++] = arg[i++];
            }
            var_name[var_len] = '\0';
            if (getcopyenv(var_name, &envir)) {
                ft_strcat(res, getcopyenv(var_name, &envir));
                j = ft_strlen(res); 
            }
        } else {
            res[j++] = arg[i++];
        }
    }
    res[j] = '\0'; 
    return res;
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
	res = malloc(sizeof(char) * (len - 2  + 1));
	if (!res)
		return (NULL);
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

	i = 1;
	while (args[i])
	{
		len = ft_strlen(args[i]);
		if (args[i][0] == '\'' && args[i][len - 1] == '\'')
			tmp = single_quotes(args[i]);
		else if (args[i][0] == '"' && args[i][len - 1] == '"')
			tmp = double_quotes(args[i], envir, last_exit_status);
		else
			tmp = no_quotes(args[i], envir, last_exit_status);
		if (args[i])
			free(args[i]);
		args[i] = tmp;
		i++;
	}
}


struct cmd* expand_tree(struct cmd *cmd, t_env *envir, int *last_exit_status)
{
  struct execcmd *ecmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if (cmd == 0)
    return 0;
  if (cmd->type == EXEC)
  {
    ecmd = (struct execcmd*)cmd;
    modify_args(ecmd->argv, envir, last_exit_status);
  }
  else if (cmd->type == REDIR) {
    rcmd = (struct redircmd*)cmd;
    expand_tree(rcmd->cmd, envir, last_exit_status);
  }
  else if (cmd->type == PIPE)
  {
    pcmd = (struct pipecmd*)cmd;
    expand_tree(pcmd->left, envir, last_exit_status);
    expand_tree(pcmd->right, envir, last_exit_status);
  }
  return cmd;
}
