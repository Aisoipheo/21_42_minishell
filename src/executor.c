/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/27 20:02:24 by gmckinle         ###   ########.fr       */
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
	filepaths = ft_strsplit(path, ":");
	if (!filepaths)
		ft_error(1, "malloc error for strsplit", 1);
	elems = cmds->cmds->head;
	while (filepaths[i] != NULL)
	{
		to_free = filepaths[i];
		filepaths[i] = ft_strjoin2(filepaths[i], ((t_llist *)elems->key)->head->val, '/', 1);
		free(to_free);
		if (!filepaths[i])
			return(ft_error(1, "malloc error for paths[i]", 1));
		if ((access(filepaths[i], X_OK)) == 0)
		{
			*filepath = filepaths[i];
		//free
			return (0);
		}
		i++;
	}
	ft_free_char2dem(filepaths, i);
	*filepath = NULL;
	return (0);
}

int	get_in_fd(t_cmd_info *c_info)
{
	if (!c_info->in_file)
		return (0);
	if(c_info->flags & CMD_INSOURCE)
		return (create_heredoc(c_info));
	return (open(c_info->in_file, O_RDONLY));
}

int	get_out_fd(t_cmd_info *c_info)
{
	if (!c_info->out_file)
		return (1);
	if (CMD_APPEND & c_info->flags)
		return (open(c_info->out_file, O_CREAT | O_WRONLY | O_APPEND));
	return (open(c_info->out_file, O_CREAT | O_WRONLY));
}

// int search_val(char *ret) 0 success 1 error

int	create_argv(t_group *cmds, char ***args)
{
	t_ll_elem	*ptr;
	t_ll_elem	*elems;
	int			i;

	i = 0;
	elems = cmds->cmds->head;
	*args = malloc(sizeof(char *) * ((t_llist *)elems->key)->size);
	if (!(*args))
		ft_error(1, "malloc error for args", 1);
	ptr = ((t_llist *)elems->key)->head->next;
	while (ptr)
	{
		*args[i] = ft_strcpy(ptr->val);
		// printf("--- args[i] = [%s]\n", args[i]);
		ptr = ptr->next;
		i++;
	}
	*args[i] = NULL;
	return (0);
}

int	remap_fds(int in, int out)
{
	if (in != 0)
	{
		if (dup2(in, 0) == -1)
			ft_error(1, "dup2: mapping to (stdin)", 1);
		if (close(in) == -1)
			ft_error(1, "close: mapping to (stdin)", 1);
	}
	if (out != 1)
	{
		if (dup2(out, 1) == -1)
			ft_error(1, "dup2: mapping to (stdin)", 1);
		if (close(out) == -1)
			ft_error(1, "close: mapping to (stdin)", 1);
	}
	return (0);
}

//fork then ft_common where ft_execve
int	ft_execve(t_group *cmds, t_info *info, int in, int out)
{
	t_ll_elem	*elems;
	char		*path;
	char		*filepath = NULL;
	char		**args;

	pid_t pid = fork();
	if (pid == -1)
		ft_error(-1, "fork error", 1);
	if (pid > 0) {
		waitpid(pid, NULL, 0);
		return (pid);
	}
	remap_fds(in, out);
	if (remap_fds(in, out))
		return (-1);
	path = llist_getval(info->envp_list, "PATH");
	if (!path)
		ft_error(-1, "PATH not set", 1);
	elems = cmds->cmds->head;
	if (ft_acces(cmds, info, path, &filepath))
		ft_error(-1, "acces error", 1);
	if (info->envp_f) {
		//free
		printf("before ENVP\n");
		info->envp = ft_compose_envp(info->envp_list);
		printf("after ENVP\n");
		info->envp_f = 0;
	}
	if (create_argv(cmds, &args))
		return (-1);
	printf("execve\n");
	return (execve(filepath, args, info->envp));
}

int	ft_common(t_group *cmds, t_info *info)
{
	int		in_fd;
	int		out_fd;

	in_fd = get_in_fd(cmds->cmds->head->val);
	if (in_fd == -1)
		return (ft_error(-1, "in_fd error", 1));
	out_fd = get_out_fd(cmds->cmds->head->val);
	if (out_fd == -1)
		return (ft_error(-1, "out_fd error", 1));
	printf("ft_execve\n");
	return(ft_execve(cmds, info, in_fd, out_fd)); //fd
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
	int	builtin_index = 0;

	// printf("!!!  !!!!\n");
	builtin_index = check_if_builtins(cmds, info);
	printf("---[%d]---\n", builtin_index);
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
