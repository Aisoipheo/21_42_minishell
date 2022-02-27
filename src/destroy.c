/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:26:04 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/26 19:12:11 by gmckinle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "utils.h"

int	ft_destroy(t_info *info)
{
	free(info->envp);
	llist_free(info->envp_list);
	rl_clear_history();
	return (0);
}
