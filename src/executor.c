/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/03/15 19:29:55 by rdrizzle         ###   ########.fr       */
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

int	check_if_builtins(t_ll_elem *cmd, t_info *info)
{
	t_llist		*elems;
	int			i;

	i = 0;
	elems = cmd->key;
	while (i < 7)
	{
		if (ft_strcmp(elems->head->val, info->reserved_words[i]) == 0)
		{
			// debug_log("BUILTIN [%s]\n", elems->head->val);
			// debug_log("RESERVED [%s]\n", info->reserved_words[i]);
			// debug_log("ok builtin\n");
			return (i);
		}
		i++;
	}
	return (i);
}

static int	ft_iterfps(char	**fps, char	**fp, t_llist *elems)
{
	int		i;
	char	*to_free;
	char	*prev;

	i = 0;
	prev = NULL;
	while (fps[i] != NULL)
	{
		to_free = fps[i];
		fps[i] = ft_strjoin2(fps[i], elems->head->val, '/', 1);
		free(to_free);
		if (!fps[i])
			return (ft_error(1, "minishell: join path", 1, 0));
		if (access(fps[i], F_OK) == 0)
			prev = fps[i];
		if (access(fps[i], X_OK) == 0)
		{
			*fp = ft_strcpy(fps[i]);
			ft_free_char2dem(fps, -1);
			return (0);
		}
		i++;
	}
	// debug_log("prev: %s\n", prev);
	return (1 + (prev != NULL));
}

int	ft_acces(t_ll_elem *cmd, char *path, char **filepath)
{
	char		**filepaths;
	t_llist		*elems;
	int			ret;

	elems = cmd->key;
	if (ft_strcontains(elems->head->val, '/') || NULL == path)
	{
		*filepath = ft_strcpy(elems->head->val);
		if (access(*filepath, F_OK) == 0 && access(*filepath, X_OK) == -1)
			return (ft_error(1, "minishell: permission denied", 0, 126));
		return (0);
	}
	filepaths = ft_strsplit(path, ":");
	if (!filepaths)
		ft_error(1, "malloc error for strsplit", 1, 0);
	ret = ft_iterfps(filepaths, filepath, elems);
	if (ret == 0)
		return (0);
	ft_free_char2dem(filepaths, -1);
	*filepath = NULL;
	if (ret == 2)
		return (ft_error(1, "minishell: permission denied", 0, 126));
	return (ft_error(1, "minishell: command not found", 0, 127));
}

int	create_argv(t_ll_elem *cmd, char ***args, char *path)
{
	t_ll_elem	*ptr;
	t_llist		*elems;
	int			i;

	i = 0;
	elems = cmd->key;
	*args = malloc(sizeof(char *) * (elems->size + 1));
	if (!(*args))
		ft_error(1, "minishell: create_argv: malloc argv", 1, 0);
	ptr = elems->head->next;
	(*args)[i++] = ft_strcpy(path);
	while (ptr)
	{
		(*args)[i] = ft_strcpy(ptr->val);
		if ((*args)[i] == NULL && ft_free_char2dem(*args, i) == NULL)
			ft_error(1, "minishell: malloc", 1, 0);
		// debug_log("--- args[i] = [%s]\n", (*args)[i]);
		ptr = ptr->next;
		i++;
	}
	(*args)[i] = NULL;
	return (0);
}

int	ft_common(t_group *cmds, t_info *info)
{
	t_fd		fd;
	t_ll_elem	*cmd;
	int			pid;

	cmd = cmds->cmds->head;
	fd.pfd[0] = -1;
	fd.fds[0] = get_in_fd(cmd->val);
	if (fd.fds[0] == -1)
		return (ft_error(-1, "minishell: get_in_fd", 1, 0));
	fd.fds[1] = get_out_fd(cmd->val);
	if (fd.fds[1] == -1)
		return (ft_error(-1, "minishell: get_out_fd", 1, 0));
	// debug_log("ft_execve\n");
	if (((t_cmd_info *)cmd->val)->flags & CMD_SUBSHELL)
		pid = ft_execsubshell(cmd, info, &fd);
	else
		pid = ft_execcommon(cmd, info, &fd, 0);
	if (fd.fds[0] != STDIN_FILENO)
		close(fd.fds[0]);
	if (fd.fds[1] != STDOUT_FILENO)
		close(fd.fds[1]);
	return (pid);
}

// Случаи Ин и Аут:
//		Ин:
//			cmd1; cmd1 < infile; cmd1 << delimiter;
//		Аут:
//			cmd1; cmd1 > outfile; cmd1 >> outfile;

//is pipe? execute pipe
//	потому что в функции пайплайна будут вызываться все последующие функции
//is sub? execute sub
//is builtin? execute builtin
//execute default

// expr | expr | expr
// expr может быть или сабшелл или билтин или обычка

//в данной функции вернуть значение меньше нуля если произошла ошибка, иначе
//значение будет восприниматься

//cmd1 | cmd2 << delim | cmd3
//cmd1 << delim //while 1 and dup2 to .tmp file
