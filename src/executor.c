/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/23 22:57:09 by gmckinle         ###   ########.fr       */
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

	while (i < 7)
	{
		if (ft_strcmp(((t_llist *)elems->key)->head->val, info->reserved_words[i]) == 0) //does weird shit - trash in reserved words
		{
			printf("BUILTIN [%s]\n", ((t_llist *)elems->key)->head->val);
			printf("RESERVED [%s]\n", info->reserved_words[i]);
			printf("ok builtin\n");
			return (i);
		}
		i++;
	}
	return (i);
}

char	*ft_scroll_envp(t_info *info, int i)
{
	t_info	*elems;
	char	*tmp;

	elems = info;
	while ((t_llist *)elems->envp_list->head->val)
	{
		if (ft_strncmp((t_llist *)elems->envp_list->head->val, "PATH", 4) == 0)
			break ;
		tmp = (t_llist *)elems->envp_list->head->val;
		elems = elems->envp_list->head->next;
	}
	return (tmp);
}

char	*ft_finding_path(t_group *cmds, t_info *info)
{
	char	*en_var;
	char	**path;
	char	*to_free;
	int		i;
	t_ll_elem	*elems;

	i = ft_scroll_envp(info->envp_list, 0);
	en_var = ft_substr(info->envp[i], 5, 166);
	printf("EN_VAR [%s]\n", en_var);
	printf("!!![%s]!!!\n", info->envp[i]);
	path = ft_strsplit(en_var, ":");
	elems = cmds->cmds->head;
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
	for (int j = 0; path[j] != NULL; j++) {
		free(path[j]);
	}
	printf("!! !! !! \n");
	free(path);
	return (NULL);
}

int	ft_common(t_group *cmds, t_info *info)
{
	t_ll_elem	*elems;
	char		*path;

	(void)info;
	printf("COMMON!!!!!!!\n");
	elems = cmds->cmds->head;
	path = ft_finding_path(cmds, info);
	// printf("PATH !!![%s]!!!\n", path);
	// printf("ARG in execve !!![%s]!!!\n", ((t_llist *)elems->key)->head->val);
	// execve(path, ((t_llist *)elems->key)->head->val, info->envp);
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

	printf("!!!  !!!!\n");
	builtin_index = check_if_builtins(cmds, info);
	printf("---[%d]---\n", builtin_index);
	if (PRS_PIPELINE & cmds->type) // проверка на пайп ВСЕ ОК
		return (pipeline(cmds, info));
	if (((t_cmd_info *)cmds->cmds->head->val)->flags & CMD_SUBSHELL) // проверка на сабшелл ВСЕ ОК
		return (ft_subshell(cmds, info));
	if (builtin_index < 7)
		return ((*info->f_ptrs[builtin_index])(cmds->cmds->head->key, info)); // проверка на билтин ВСЕ ОК
	// ft_common(cmds, info);
	return (ft_common(cmds, info));
}
