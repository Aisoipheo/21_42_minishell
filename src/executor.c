/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/03/09 18:25:14 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
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

int	check_if_builtins(t_group *cmds, t_info *info)
{
	t_ll_elem	*elems;
	int			i;

	i = 0;
	elems = cmds->cmds->head;
	while (i < 7)
	{
		if (ft_strcmp(((t_llist *)elems->key)->head->val, info->reserved_words[i]) == 0)
		{
			debug_log("BUILTIN [%s]\n", ((t_llist *)elems->key)->head->val);
			debug_log("RESERVED [%s]\n", info->reserved_words[i]);
			debug_log("ok builtin\n");
			return (i);
		}
		i++;
	}
	return (i);
}

int	ft_acces(t_group *cmds, char *path, char **filepath)
{
	char		**filepaths;
	char		*to_free;
	t_ll_elem	*elems;

	int	i = 0;
	elems = cmds->cmds->head;
	if (ft_strcontains(((t_llist *)elems->key)->head->val, '/') || NULL == path)
	{
		*filepath = ft_strcpy(((t_llist *)elems->key)->head->val);
		return (0);
	}
	filepaths = ft_strsplit(path, ":");
	if (!filepaths)
		ft_error(1, "malloc error for strsplit", 1);
	while (filepaths[i] != NULL)
	{
		// debug_log("PATH BEFORE: %s\n", filepaths[i]);
		to_free = filepaths[i];
		filepaths[i] = ft_strjoin2(filepaths[i], ((t_llist *)elems->key)->head->val, '/', 1);
		debug_log("PATH AFTER: %s\n", filepaths[i]);
		free(to_free);
		if (!filepaths[i])
			return(ft_error(1, "minishell: join path", 1));
		if ((access(filepaths[i], X_OK)) == 0)
		{
			*filepath = ft_strcpy(filepaths[i]);
			debug_log("FILEPATH: %s\n", *filepath);
			return (0);
		}
		i++;
	}
	ft_free_char2dem(filepaths, i);
	*filepath = NULL;
	return (ft_error(1, "minishell: command not found", 0));
}

// int search_val(char *ret) 0 success 1 error

int	create_argv(t_group *cmds, char ***args, char *path)
{
	t_ll_elem	*ptr;
	t_ll_elem	*elems;
	int			i;

	i = 0;
	elems = cmds->cmds->head;
	*args = malloc(sizeof(char *) * (((t_llist *)elems->key)->size + 1));
	if (!(*args))
		ft_error(1, "minishell: create_argv: malloc argv", 1);
	ptr = ((t_llist *)elems->key)->head->next;
	(*args)[i++] = ft_strcpy(path);
	while (ptr)
	{
		(*args)[i] = ft_strcpy(ptr->val);
		debug_log("--- args[i] = [%s]\n", (*args)[i]);
		ptr = ptr->next;
		i++;
	}
	(*args)[i] = NULL;
	return (0);
}

int	ft_execve(t_group *cmds, t_info *info, int in, int out)
{
	char		*path;
	char		*filepath = NULL;
	char		**args;

	if (info->envp_f)
	{
		ft_free_char2dem(info->envp, -1);
		info->envp = ft_compose_envp(info->envp_list);
		if (NULL == info->envp)
			return (ft_error(-1, "minishell: ft_execve: compose_envp", 1));
		info->envp_f = 0;
	}
	pid_t pid = fork();
	if (pid == -1)
		ft_error(-1, "minishell: ft_execve: fork", 1);
	if (pid > 0)
	{
		debug_log("RET PID: %d\n", pid);
		return (pid);
	}
	debug_log("TRY REMAPFDS\n");
	if (remap_fds(in, out))
		exit(1);
	debug_log("REMAPFDS OK\n");
	path = llist_getval(info->envp_list, "PATH");
	if (ft_acces(cmds, path, &filepath))
		exit(1);
	if (create_argv(cmds, &args, filepath))
		exit(1);
	debug_log("execve\n");
	debug_log("%s\n", filepath);
	if (execve(filepath, args, info->envp) == -1)
		exit(ft_error(1, "minishell: execve", 1));
	return (1);
}

int	ft_common(t_group *cmds, t_info *info)
{
	int		in_fd;
	int		out_fd;
	int		pid;

	in_fd = get_in_fd(cmds->cmds->head->val, cmds->files);
	if (in_fd == -1)
		return (ft_error(-1, "minishell: get_in_fd", 1));
	out_fd = get_out_fd(cmds->cmds->head->val);
	if (out_fd == -1)
		return (ft_error(-1, "minishell: get_out_fd", 1));
	debug_log("ft_execve\n");
	pid = ft_execve(cmds, info, in_fd, out_fd);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	return (pid);
}

// Случаи Ин и Аут:
//		Ин:
//			cmd1; cmd1 < infile; cmd1 << delimiter;
//		Аут:
//			cmd1; cmd1 > outfile; cmd1 >> outfile;
/*ft_common
{
fd_in = STDIN_FILENO;
fd_out = STDOUT_FILENO;
if (file_in)
	fd_in
if (file_out)
	fd_out
ft_exec(cmds, info, fd_in, fd_out);
}
*/

/*ft_pipe
{
	// ebanina c файлами и пайпами
	ft_exec(cmds, info, fd_in, fd_out)
}
*/

pid_t	executor(t_group *cmds, t_info *info)
{
	int	builtin_index;

	builtin_index = check_if_builtins(cmds, info);
	// debug_log("---[%d]---\n", builtin_index);
	if (PRS_PIPELINE & cmds->type) // проверка на пайп ВСЕ ОК
		return (pipeline(cmds, info));
	if (((t_cmd_info *)cmds->cmds->head->val)->flags & CMD_SUBSHELL) // проверка на сабшелл ВСЕ ОК
		return (ft_subshell(cmds, info));
	if (builtin_index < 7)
		return ((*info->f_ptrs[builtin_index])(cmds->cmds->head->key, info)); // проверка на билтин ВСЕ ОК
	return (ft_common(cmds, info));
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
