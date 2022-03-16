/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:13:25 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 22:13:13 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

static int	_prs_extract_return(char	**envpvar_ptr)
{
	*envpvar_ptr = NULL;
	return (0);
}

int	_prs_extract_var(const char *s, t_info *info,
		char **envpvar_ptr, unsigned int *j)
{
	unsigned int	i;
	char			*name;

	i = 0;
	if (s[i] == '\0')
		return (_prs_extract_return(envpvar_ptr));
	if (s[i] == '?')
	{
		if (_prs_update_g_exit_str(info))
			return (1);
		*envpvar_ptr = info->g_exit_str;
		*j += 1;
		return (0);
	}
	while (ft_isalnum(s[i]) || s[i] == '_')
		++i;
	if (i == 0)
		return (_prs_extract_return(envpvar_ptr));
	name = ft_substr(s, 0, i);
	if (name == NULL)
		return (ft_error(1, "minishell: _prs_extract_var", 1, 0));
	*envpvar_ptr = llist_getval(info->envp_list, name);
	*j += i;
	free(name);
	return (0);
}

static int	_prs_fexcc_handle_env(char *s,
		t_info *info, t_llist *chunks, t_fpack1 *fp)
{
	char			*envpvar_ptr;

	if (_prs_extract_var(s + *(fp->i) + 1, info, &envpvar_ptr, fp->i))
		return (1);
	if (envpvar_ptr == NULL && s[*(fp->i)] == '$')
		*(fp->size) += (++(fp->ci->e) || 1);
	if (envpvar_ptr != NULL)
	{
		if (_prs_alloc_ci(&(fp->ci), 0, ft_strlen(envpvar_ptr)))
			return (1);
		if (llist_push(chunks, fp->ci, envpvar_ptr))
		{
			free(fp->ci);
			return (ft_error(1,
					"minishell: _prs_field_exp_collect_chunks", 1, 0));
		}
		*(fp->size) += (fp->ci->e);
	}
	++(*(fp->i));
	return (0);
}

static void	_prs_prepare_fp1(t_fpack1 *fp,
			t_chunk_info *ci, unsigned int *i, unsigned int *size)
{
	fp->ci = ci;
	fp->i = i;
	fp->size = size;
}

int	_prs_field_exp_collect_chunks(char *s, t_info *info,
		t_llist *chunks, unsigned int *size)
{
	t_chunk_info	*ci;
	t_fpack1		fp;
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	ci = NULL;
	while (s[i])
	{
		while (s[i] && s[i] != '$')
			*size += (++i || 1);
		if (_prs_alloc_ci(&ci, j, i))
			return (1);
		if (llist_push(chunks, ci, s))
			return (_prs_fecc_return(ci));
		_prs_prepare_fp1(&fp, ci, &i, size);
		if (s[i] == '$' && _prs_fexcc_handle_env(s, info, chunks, &fp))
			return (1);
		j = i;
	}
	return (0);
}
