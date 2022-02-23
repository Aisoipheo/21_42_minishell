/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/23 20:43:55 by gmckinle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

/*
◦ echo with option -n
◦ cd with only a relative or absolute path
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options
*/

int	forking();

int	check_if_builtins(t_group *cmds, t_info *info)
{
	t_ll_elem	*elems;
	int			i;

	i = 0;
	(void)info;
	elems = cmds->cmds->head;
	// while (i < 7)
	// {
	// 	if (ft_strcmp(((t_llist *)elems->key)->head->val, info->reserved_words[i]))
	// 		return (i);
	// 	i++;
	// }
	return (i);
}

int	ft_scroll_envp(t_info *info, int i)
{
	while (info->envp[i])
	{
		if (ft_strncmp(info->envp[i], "PATH", 4) == 0)
			break ;
		i++;
	}
	return (i);
}

char	*ft_finding_path(t_group *cmds, t_info *info)
{
	char	*en_var;
	char	**path;
	char	*to_free;
	int		i;
	// int		j;
	t_ll_elem	*elems;

	elems = cmds->cmds->head;
	i = ft_scroll_envp(info, 0);
	en_var = ft_substr(info->envp[i], 5, 166);
	path = ft_strsplit(en_var, ":");
	free(en_var);
	i = 0;
	while (path[i] != NULL)
	{
		to_free = ft_strjoin(path[i], "/");
		free(path[i]);
		path[i] = ft_strjoin(to_free, ((t_llist *)elems->key)->head->val);
		free(to_free);
		if ((access(path[i], X_OK)) == 0)
		{
			for (int j = 0; path[j] != NULL; j++) {
				if (j != i)
					free(path[j]);
			}
			free(path);
			return (path[i]);
		}
		i++;
	}
	// for (int j = 0; path[j] != NULL; j++) {
	// 	free(path[j]);
	// }
	free(path);
	return (NULL);
}

int	ft_common(t_group *cmds, t_info *info)
{
	t_ll_elem	*elems;
	char		*path;

	elems = cmds->cmds->head;
	path = ft_finding_path(cmds, info);
	execve(path, ((t_llist *)elems->key)->head->val, info->envp);
	return (0);
}

//is pipe? execute pipe
//						потому что в функции пайплайна будут вызываться все последующие функции
//is sub? execute sub
//is builtin? execute builtin
//execute default

// expr | expr | expr
// expr может быть или сабшелл или билтин или обычка

//в данной функции вернуть значение меньше нуля если произошла ошибка, иначе
//значение будет восприниматься

//cmd1 | cmd2 << delim | cmd3  
//cmd1 << delim //while 1 and dup2 to .tmp file
pid_t	executor(t_group *cmds, t_info *info)
{
	int	builtin_index = 0;

	// builtin_index = check_if_builtins(cmds, info);
	if (PRS_PIPELINE & cmds->type) // проверка на пайп ВСЕ ОК
		return (pipeline(cmds, info));
	if (((t_cmd_info *)cmds->cmds->head->val)->flags & CMD_SUBSHELL) // проверка на сабшелл ВСЕ ОК
		return (ft_subshell(cmds, info));
	if (builtin_index < 8)
		return ((*info->f_ptrs[builtin_index])(cmds->cmds->head->key, info)); // проверка на билтин ВСЕ ОК
	return (ft_common(cmds, info));
}
