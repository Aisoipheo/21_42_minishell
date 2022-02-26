/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/26 19:00:34 by gmckinle         ###   ########.fr       */
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
		if (ft_strcmp(((t_llist *)elems->key)->head->val, info->reserved_words[i]) == 0)
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

int	ft_acces(t_group *cmds, t_info *info, char *path, char **filepath)
{
	char	**filepaths;
	char	*to_free;
	t_ll_elem	*elems;

	int	i = 0;
	(void)info;
	// printf("!!!\n");
	filepaths = ft_strsplit(path, ":");
	// printf("filepaths[i] = [%s]\n", filepaths[i]);
	elems = cmds->cmds->head;
	while (filepaths[i] != NULL)
	{
		to_free = filepaths[i];
		filepaths[i] = ft_strjoin2(filepaths[i], ((t_llist *)elems->key)->head->val, '/', 1);
		// printf("filepaths[i] = [%s]\n", filepaths[i]);
		free(to_free);
		if (!(*filepaths))
			return(ft_error(1, "malloc error for paths[i]", 1));
		if ((access(filepaths[i], X_OK)) == 0)
		{
			*filepath = filepaths[i];
			// for (int j = 0; paths[j] != NULL; j++) {
			// 	if (j != i)
			// 		free(paths[j]);
			// }
			// *path = path;
			// free(path);
			return (0);
		}
		i++;
	}
	ft_free_char2dem(filepaths, i);
	return (1);
}

// int search_val(char *ret) 0 success 1 error

int	ft_execve(t_group *cmds, t_info *info)
{
	t_ll_elem	*elems;
	char		*path;
	char		*filepath = NULL;

	(void)info;
	printf("EXECVE!!!!!!!\n");
	path = llist_getval(info->envp_list, "PATH");
	if (!path)
		ft_error(1, "PATH not set", 1);
	elems = cmds->cmds->head;
	ft_acces(cmds, info, path, &filepath);
	printf("PATH !!![%s]!!!\n", path);
	printf("ARG in execve !!![%s]!!!\n", ((t_llist *)elems->key)->head->val);
	printf("FILEPATH in execve !!![%s]!!!\n", filepath);
	if (info->envp_f) {
		
		free(info->envp);
		info->envp = ft_compose_envp(info->envp_list);
		info->envp_f = 0;
	}
	char ** args = malloc(sizeof(char *) * 2);
	args[0] = ".";
	args[1] = NULL;
	//for(char **it = argv; *it != NULL; ++it)
	//	printf("%s\n", *it);
	execve(filepath, args, envparr);
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
	return (ft_execve(cmds, info));
}
