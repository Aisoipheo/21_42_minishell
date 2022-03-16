/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser10.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 19:21:24 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 20:55:47 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

int	_prs_group_cmd_ret1(t_cmd_info *info, t_llist *args)
{
	llist_free(args);
	free(info);
	return (1);
}

int	_prs_group_cmd_ret2(t_cmd_info *info, t_llist *args)
{
	llist_free(args);
	free(info);
	return (ft_error(1, "minishell: syntax error", 0, 258));
}

int	_prs_group_cmd_ret3(t_cmd_info *info, t_llist *args)
{
	llist_free(args);
	free(info);
	return (ft_error(1, "minishell: _prs_group_cmd", 1, 0));
}

int	_prs_exp_field_ret(t_llist *str)
{
	llist_free(str);
	return (1);
}

int	_prs_exp_field_ret2(t_llist *str)
{
	llist_free(str);
	return (0);
}
