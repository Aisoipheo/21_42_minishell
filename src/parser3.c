/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 13:23:43 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 19:30:17 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "minishell.h"
#include "utils.h"

// static const char	*_lx_get_name(int type)
// {
// 	static const char *names[LX_NCONST] =
// 	{
// 		[LX_SEP] = "SEP",
// 		[LX_WORD] = "WORD",
// 		[LX_IF_AND] = "&&",
// 		[LX_IF_OR] = "||",
// 		[LX_REDIR_APPEND] = ">>",
// 		[LX_REDIR_IN] = "<",
// 		[LX_REDIR_OUT] = ">",
// 		[LX_REDIR_SOURCE] = "<<",
// 		[LX_EXP_FIELD] = "QUOTES \"",
// 		[LX_FIELD] = "QUOTES \'",
// 		[LX_PARN_L] = "(",
// 		[LX_PARN_R] = ")",
// 		[LX_PIPE] = "PIPE"
// 	};
// 	return (names[type]);
// }

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
			return (ft_error(1, "minishell: error near `<' or `<<'", 0, 258));
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
			return (ft_error(1, "minishell: error near `>' or `>>'", 0, 258));
		info->out_file = (*c)->val;
		return (0);
	}
	return (0);
}

static int	_prs_handle_token(t_ll_elem **c, t_cmd_info *info, t_llist *args)
{
	if (NULL == (*c))
		return (ft_error(1, "ur bad >:(", 0, 999));
	if ((int)(*c)->key == LX_PARN_L || (int)(*c)->key == LX_PARN_R)
	{
		info->_shlvl += ((int)(*c)->key == LX_PARN_L)
			+ (-1) * ((int)(*c)->key == LX_PARN_R);
		info->flags |= CMD_SUBSHELL;
		if ((info->_shlvl == 1 && (int)(*c)->key == LX_PARN_L)
			|| (info->_shlvl == 0 && (int)(*c)->key == LX_PARN_R))
			return (0);
	}
	if ((int)(*c)->key == LX_SEP && info->_shlvl == 0)
		return (0);
	if ((info->flags & CMD_SUBSHELL)
		&& info->_shlvl == 0 && (int)(*c)->key == LX_WORD)
		return (ft_error(1,
				"minishell: unexpected token after `(subshell)'", 0, 258));
	if (((int)(*c)->key == LX_WORD
		|| info->_shlvl > 0) && llist_push(args, (*c)->key, (*c)->val))
		return (ft_error(1,
				"minishell: _prs_handle_token", 1, 0));
	if (info->_shlvl > 0)
		return (0);
	return (_prs_handle_token2(c, info));
}

int	_prs_group_cmd(t_ll_elem *h, t_llist *cmds)
{
	t_cmd_info	*info;
	t_llist		*args;

	_prs_alloc_cmd_info(&info);
	args = llist_new(NULL, NULL, NULL);
	if (!info || !args)
		return (ft_error(1, "minishell: _prs_group_cmd", 1, 0));
	while (h && ((int)h->key != LX_PIPE || info->_shlvl))
	{
		if (_prs_handle_token(&h, info, args))
			return (_prs_group_cmd_ret1(info, args));
		h = h->next;
	}
	if ((args->size == 0 && info->out_file == NULL && info->in_file == NULL))
		return (_prs_group_cmd_ret2(info, args));
	if (h == NULL || (int)h->key == LX_PIPE)
	{
		if (llist_push(cmds, args, info))
			return (_prs_group_cmd_ret3(info, args));
		return (0);
	}
	llist_free(args);
	free(info);
	return (1);
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

pid_t	_prs_handle_group(int type, t_llist *group, t_info *info)
{
	t_llist	*expanded;
	t_group	*cmds;
	pid_t	pid;

	pid = -1;
	expanded = _prs_expand(group, info);
	if (expanded)
	{
		cmds = ft_group_new(type);
		if (NULL == cmds)
			return (-1);
		if (_prs_prepare_group(expanded, cmds))
			return (_prs_handle_dstr(cmds, expanded));
		if (_prs_handle_heredoc(cmds))
			return (_prs_handle_dstr(cmds, expanded));
		pid = executor(cmds, info);
		llist_free(expanded);
		ft_group_free(cmds);
	}
	return (pid);
}

int	_prs_logexec(t_llist *groups, t_info *info)
{
	t_ll_elem	*ptr;
	int			expect;
	int			sig;
	pid_t		pid;

	expect = 0;
	ptr = groups->head;
	while (NULL != ptr)
	{
		if (expect == 0)
		{
			pid = _prs_handle_group((int)ptr->key, ptr->val, info);
			if (pid < 0)
				return (1);
			if (pid > 0)
				_prs_waitall(pid, &sig);
			expect = 1;
		}
		else if (_prs_logexec_bonus(ptr, &expect))
			return (0);
		ptr = ptr->next;
	}
	return (0);
}
