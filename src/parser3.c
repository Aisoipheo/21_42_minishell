/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 13:23:43 by rdrizzle          #+#    #+#             */
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
			return (ft_error(1, "minishell: parse error near token `<' or `<<'", 0));
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
			return (ft_error(1, "minishell: parse error near token `>' or `>>'", 0)); //parse error near token `>' or `>>'
		info->out_file = (*c)->val;
		return (0);
	}
	return (0);
}

static int	_prs_handle_token(t_ll_elem **c, t_cmd_info *info, t_llist *args)
{
	if (NULL == (*c))
		return (ft_error(1, "ur bad >:(", 0));
	if ((int)(*c)->key == LX_PARN_L || (int)(*c)->key == LX_PARN_R)
	{
		info->_shlvl += ((int)(*c)->key == LX_PARN_L) + (-1) * ((int)(*c)->key == LX_PARN_R);
		info->flags |= CMD_SUBSHELL;
		if ((info->_shlvl == 1 && (int)(*c)->key == LX_PARN_L)
			|| (info->_shlvl == 0 && (int)(*c)->key == LX_PARN_R))
			return (0); //consume first and last ()
	}
	//if (info->_shlvl > 0 && llist_push(args, (*c)->key, (*c)->val))
	//	return (ft_error(1, "minishell: _prs_handle_token", 1)); //malloc err or smth
	if ((int)(*c)->key == LX_SEP)
		return (0); //skip spaces
	if ((info->flags & CMD_SUBSHELL) && info->_shlvl == 0 && (int)(*c)->key == LX_WORD)
		return (ft_error(1, "minishell: unexpected token after subshell command", 0)); //unexpected token
	if (((int)(*c)->key == LX_WORD || info->_shlvl > 0) && llist_push(args, (*c)->key, (*c)->val))
		return (ft_error(1, "minishell: _prs_handle_token", 1)); //malloc err or smthing
	if (info->_shlvl > 0)
		return (0);
	return (_prs_handle_token2(c, info));
}

int	_prs_group_cmd(t_ll_elem *h, t_llist *cmds)
{
	t_cmd_info	*info;
	t_llist		*args;

	info = malloc(sizeof(t_cmd_info));
	args = llist_new(NULL, NULL, NULL);
	if (!info || !args)
		return (1); // malloc error
	info->in_file = NULL;
	info->out_file = NULL;
	info->flags = 0;
	info->_shlvl = 0;
	while (h && (int)h->key != LX_PIPE)
	{
		printf("[parser3.c] PRS_GROUP_CMD TRY HANDLE `%s | %s'\n", _lx_get_name((int)h->key), h->val);
		if (_prs_handle_token(&h, info, args))
			break ;
		h = _prs_next_token(h);
		printf("[parser3.c] PRS_GROUP_CMD NEXT TOKEN IS %p\n", h);
	}
	printf("[parser3.c] PRS_GROUP_CMD TRY PUSH\n");
	if ((h == NULL || (int)h->key == LX_PIPE)
		&& (llist_push(cmds, args, info) == 0))
		return (0);
	printf("[parser3.c] PRS_GROUP_CMD free %p %p\n", args, info);
	llist_free(args);
	free(info);
	return (1);
}

int	_prs_group_pipe(t_llist *expanded, t_llist *cmds)
{
	t_ll_elem	*curr;

	curr = expanded->head;
	while (curr != NULL)
	{
		if (_prs_group_cmd(curr, cmds))
			return (1);
		while (curr != NULL && (int)curr->key != LX_PIPE)
			curr = curr->next;
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

pid_t	_prs_handle_group(int type, t_llist *group, t_info *info)
{
	t_llist	*expanded;
	t_group *cmds;

	expanded = _prs_expand(group, info);
	if (expanded)
	{
		printf("[parser3.c] EXPAND OK\n");
		for (t_ll_elem *h = expanded->head; h != NULL; h = h->next)
			printf("%10s | %s\n", _lx_get_name((int)h->key) , h->val);
		cmds = ft_group_new(type);
		if (NULL == cmds)
			return (-1);
		if (_prs_prepare_group(expanded, cmds))
			return (-1);
		printf("[parser3.c] GROUP READY\n");
		executor(cmds, info);
		//exept of this block of code
		for (t_ll_elem *h = cmds->cmds->head; h != NULL; h = h->next)
		{
			t_cmd_info *cmd_info = (t_cmd_info *)h->val;
			printf(" ======c> llist at %p\n", h);
			printf(" *** INFO ***\n");
			convert((int)cmd_info->flags);
			printf("flags: %.5d\n", cmd_info->flags/* */);
			printf("in: %s\n", cmd_info->in_file);
			printf("out: %s\n", cmd_info->out_file);
			printf("\n *** ARGS ***\n");
			for (t_ll_elem *arg = ((t_llist *)h->key)->head; arg != NULL; arg = arg->next)
				printf("  + %10s | %s\n", _lx_get_name((int)arg->key), arg->val);
		}
		//
		printf("[parser3.c] PRS_HANDLE_GROUP free %p\n", expanded);
		llist_free(expanded);
	}

	return (0);
}

int	_prs_logexec(t_llist *groups, t_info *info)
{
	t_ll_elem	*ptr;
	int			expect;
	int			mode;
	int			sig;
	pid_t		pid;

	expect = 0;
	ptr = groups->head;
	while(NULL != ptr)
	{
		printf("[parser3.c] PRS_LOGEXEC EXPECT %d\n", expect);
		if (expect == 0)
		{
			pid = _prs_handle_group((int)ptr->key, ptr->val, info);
			printf("[parser3.c] PRS_LOGEXEC HANDLE GROUP OK\n");
			if (pid < 0)
				return (1);
			expect = 1;
		}
		else
		{
			mode = (int)(ptr->key);
			if (pid > 0)
				waitpid(pid, &sig, 0);
			if (pid > 0 && ((WEXITSTATUS(sig) == 0 && mode == LX_IF_OR)
				|| (WEXITSTATUS(sig) != 0 && mode == LX_IF_AND)))
				return (0);
			expect = 0;
		}
		ptr = ptr->next;
	}
	return (0);
}
