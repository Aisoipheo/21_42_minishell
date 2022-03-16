/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 13:23:43 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 15:52:29 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "minishell.h"
#include "utils.h"

static const char	*_lx_get_name(int type)
{
	static const char *names[LX_NCONST] =
	{
		[LX_SEP] = "SEP",
		[LX_WORD] = "WORD",
		[LX_IF_AND] = "&&",
		[LX_IF_OR] = "||",
		[LX_REDIR_APPEND] = ">>",
		[LX_REDIR_IN] = "<",
		[LX_REDIR_OUT] = ">",
		[LX_REDIR_SOURCE] = "<<",
		[LX_EXP_FIELD] = "QUOTES \"",
		[LX_FIELD] = "QUOTES \'",
		[LX_PARN_L] = "(",
		[LX_PARN_R] = ")",
		[LX_PIPE] = "PIPE"
	};
	return (names[type]);
}

t_ll_elem * _prs_next_token(t_ll_elem *curr)
{
	if (NULL == curr)
		return (NULL);
	curr = curr->next;
	while(curr && curr->key == LX_SEP)
		curr = curr->next;
	return (curr);
}

static int	_prs_handle_token2(t_ll_elem **c, t_cmd_info *info)
{
	if ((int)(*c)->key == LX_REDIR_IN || (int)(*c)->key == LX_REDIR_SOURCE)
	{
		if ((int)(*c)->key == LX_REDIR_SOURCE)
			info->flags |= CMD_INSOURCE;
		else
			info->flags &= ~CMD_INSOURCE;
		*c = _prs_next_token(*c);
		if (NULL == *c || (int)(*c)->key != LX_WORD)
			return (ft_error(1, "minishell: parse error near token `<' or `<<'", 0, 258));
		info->in_file = (*c)->val;
		return (0);
	}
	if ((int)(*c)->key == LX_REDIR_OUT || (int)(*c)->key == LX_REDIR_APPEND)
	{
		if ((int)(*c)->key == LX_REDIR_APPEND)
			info->flags |= CMD_APPEND;
		else
			info->flags &= ~CMD_APPEND;
		*c = _prs_next_token(*c);
		if (NULL == *c || (int)(*c)->key != LX_WORD)
			return (ft_error(1, "minishell: parse error near token `>' or `>>'", 0, 258));
		info->out_file = (*c)->val;
		return (0);
	}
	return (0);
}

static int	_prs_handle_token(t_ll_elem **c, t_cmd_info *info, t_llist *args)
{
	// debug_log("_prs_handle_token _shlvl == %d\n", info->_shlvl);
	if (NULL == (*c)) //might delete later
		return (ft_error(1, "ur bad >:(", 0, 999));
	if ((int)(*c)->key == LX_PARN_L || (int)(*c)->key == LX_PARN_R)
	{
		info->_shlvl += ((int)(*c)->key == LX_PARN_L) + (-1) * ((int)(*c)->key == LX_PARN_R);
		info->flags |= CMD_SUBSHELL;
		if ((info->_shlvl == 1 && (int)(*c)->key == LX_PARN_L)
			|| (info->_shlvl == 0 && (int)(*c)->key == LX_PARN_R))
			return (0);
	}
	// if (info->_shlvl > 0 && llist_push(args, (*c)->key, (*c)->val))
	// 	return (ft_error(1, "minishell: _prs_handle_token", 1, 0)); //malloc err or smth
	if ((int)(*c)->key == LX_SEP && info->_shlvl == 0)
		return (0);
	if ((info->flags & CMD_SUBSHELL) && info->_shlvl == 0 && (int)(*c)->key == LX_WORD)
		return (ft_error(1, "minishell: unexpected token after `(subshell)'", 0, 258));
	if (((int)(*c)->key == LX_WORD || info->_shlvl > 0) && llist_push(args, (*c)->key, (*c)->val))
		return (ft_error(1, "minishell: _prs_handle_token", 1, 0));
	if (info->_shlvl > 0)
		return (0);
	return (_prs_handle_token2(c, info));
}

int	_prs_group_cmd(t_ll_elem *h, t_llist *cmds)
{
	t_cmd_info	*info;
	t_llist		*args;

	// debug_log("--------> CMD\n");
	info = malloc(sizeof(t_cmd_info));
	args = llist_new(NULL, NULL, NULL);
	if (!info || !args)
		return (ft_error(1, "minishell: _prs_group_cmd", 1, 0));
	info->in_file = NULL;
	info->out_file = NULL;
	info->flags = 0;
	info->_shlvl = 0;
	while (h && ((int)h->key != LX_PIPE || info->_shlvl))
	{
		// debug_log("[parser3.c] PRS_GROUP_CMD TRY HANDLE `%s | %s'\n", _lx_get_name((int)h->key), h->val);
		if (_prs_handle_token(&h, info, args))
		{
			llist_free(args);
			free(info);
			return (1);
		}
		h = h->next;
		// debug_log("[parser3.c] PRS_GROUP_CMD NEXT TOKEN IS %p\n", h);
	}
	// debug_log("[parser3.c] PRS_GROUP_CMD TRY PUSH\n");
	if ((args->size == 0 && info->out_file == NULL && info->in_file == NULL))
	{
		llist_free(args);
		free(info);
		return (ft_error(1, "minishell: syntax error", 0, 258));
	}
	if (h == NULL || (int)h->key == LX_PIPE)
	{
		if (llist_push(cmds, args, info))
		{
			llist_free(args);
			free(info);
			return (ft_error(1, "minishell: _prs_group_cmd", 1, 0));
		}
		return (0);
	}
	// debug_log("[parser3.c] PRS_GROUP_CMD free %p %p\n", args, info);
	llist_free(args);
	free(info);
	return (1);
}

int	_prs_group_pipe(t_llist *expanded, t_llist *cmds)
{
	t_ll_elem	*curr;

	// debug_log("-------> PIPE\n");
	curr = expanded->head;
	while (curr != NULL)
	{
		if (_prs_group_cmd(curr, cmds))
			return (1);
		while (curr != NULL && (int)curr->key != LX_PIPE)
			curr = curr->next;
		if (curr && curr->next == NULL)
			return (ft_error(1, "minishell: parse error near `|'", 0, 258));
		if (curr)
			curr = curr->next;
	}
	return (0);
}

int	_prs_prepare_group(t_llist *expanded, t_group *cmds)
{
	if (cmds->type & PRS_PIPELINE)
		return _prs_group_pipe(expanded, cmds->cmds);
	return (_prs_group_cmd(expanded->head, cmds->cmds));
}

//debug only
// static long long convert(int n) {
//   long long bin = 0;
//   int rem, i = 1;

//   while (n!=0) {
//     rem = n % 2;
//     n /= 2;
//     bin += rem * i;
//     i *= 10;
//   }

//   return bin;
// }

int	_prs_handle_heredoc(t_group *cmds)
{
	t_ll_elem	*cmd;
	t_cmd_info	*ci;
	int			ret;

	cmd = cmds->cmds->head;
	while(cmd)
	{
		ci = (t_cmd_info *)cmd->val;
		ret = 0;
		if (ci->flags & CMD_INSOURCE)
			ret = create_heredoc(ci, cmds->files);
		debug_log("heredoc exit: %d\n", ret);
		if (ret == 2)
		{
			g_exit = 1;
			return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}

int	_prs_handle_dstr(t_group *group, t_llist *expanded)
{
	ft_group_free(group);
	llist_free(expanded);
	return (-1);
}

pid_t	_prs_handle_group(int type, t_llist *group, t_info *info)
{
	t_llist	*expanded;
	t_group *cmds;
	pid_t	pid;

	pid = -1;
	expanded = _prs_expand(group, info);
	if (expanded)
	{
		// debug_log("[parser3.c] EXPAND OK\n");
		// for (t_ll_elem *h = expanded->head; h != NULL; h = h->next)
		// 	debug_log("%10s | %s\n", _lx_get_name((int)h->key) , h->val);
		cmds = ft_group_new(type);
		if (NULL == cmds)
			return (-1);
		if (_prs_prepare_group(expanded, cmds))
			return (_prs_handle_dstr(cmds, expanded));
		if (_prs_handle_heredoc(cmds))
			return (_prs_handle_dstr(cmds, expanded));
		// debug_log("[parser3.c] GROUP READY\n");
		pid = executor(cmds, info);
		// for (t_ll_elem *h = cmds->cmds->head; h != NULL; h = h->next)
		// {
		// 	t_cmd_info *cmd_info = (t_cmd_info *)h->val;
		// 	debug_log(" ======c> llist at %p\n", h);
		// 	// debug_log(" TYPE: %lld\n", convert(type));
		// 	debug_log(" *** INFO ***\n");
		// 	// convert((int)cmd_info->flags);
		// 	debug_log("flags: %.5d\n", cmd_info->flags/* */);
		// 	debug_log("in: %s\n", cmd_info->in_file);
		// 	debug_log("out: %s\n", cmd_info->out_file);
		// 	debug_log("\n *** ARGS ***\n");
		// 	for (t_ll_elem *arg = ((t_llist *)h->key)->head; arg != NULL; arg = arg->next)
		// 		debug_log("  + %10s | %s\n", _lx_get_name((int)arg->key), arg->val);
		// }
		// debug_log("[parser3.c] PRS_HANDLE_GROUP free %p\n", expanded);
		llist_free(expanded);
		ft_group_free(cmds);
	}
	return (pid);
}

void	_prs_waitall(int pid, int *sig)
{
	int		pid2;

	pid2 = 1;
	while (pid2 > 0)
	{
		pid2 = waitpid(0, sig, 0);
		if (pid == pid2)
			g_exit	= WEXITSTATUS(*sig);
	}
}

int	_prs_logexec(t_llist *groups, t_info *info)
{
	t_ll_elem	*ptr;
	int			expect;
	int			sig;
	pid_t		pid;

	expect = 0;
	ptr = groups->head;
	while(NULL != ptr)
	{
		// debug_log("[parser3.c] PRS_LOGEXEC EXPECT %d\n", expect);
		if (expect == 0)
		{
			pid = _prs_handle_group((int)ptr->key, ptr->val, info);
			debug_log("REC PID: %d\n", pid);
			// debug_log("[parser3.c] PRS_LOGEXEC HANDLE GROUP OK\n");
			if (pid < 0)
				return (1);
			if (pid > 0)
				_prs_waitall(pid, &sig);
			expect = 1;
			debug_log("EXIT STATUS: %d\n", g_exit);
		}
		else
		{
			debug_log("PID: %d STATUS: %d MODE:%s \n", pid, g_exit, _lx_get_name((int)ptr->key));
			if ((g_exit == 0 && ((int)ptr->key == LX_IF_OR))
			|| (g_exit != 0 && ((int)ptr->key == LX_IF_AND)))
				return (0);
			expect = 0;
		}
		ptr = ptr->next;
	}
	return (0);
}
