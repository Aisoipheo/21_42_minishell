/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:55:48 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 20:12:59 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

int	_prs_handle_subsh_token(t_ll_elem *ptr, t_llist *expanded)
{
	if (_prs_expandable(ptr->key))
		return (llist_push(expanded, ptr->key, ft_strcpy((char *)ptr->val)));
	return (llist_push(expanded, ptr->key, ptr->val));
}

int	_prs_field_expansion_free(t_llist *chunks, char **word)
{
	if (chunks)
		llist_free(chunks);
	if (*word)
		free(*word);
	return (1);
}

int	_prs_field_expansion_copy(t_llist *chunks, char *word)
{
	t_ll_elem		*h;
	unsigned int	i;
	unsigned int	_i;

	h = chunks->head;
	i = 0;
	while (h != NULL)
	{
		_i = ((t_chunk_info *)h->key)->s;
		while (_i < ((t_chunk_info *)h->key)->e)
			word[i++] = ((char *)h->val)[_i++];
		h = h->next;
	}
	return (0);
}

void	_prs_pattern_match_while(const char *pattern,
	const char *str, char **dp)
{
	int		i;
	int		j;
	int		strl;
	int		ptrl;

	i = 1;
	ptrl = ft_strlen(pattern);
	strl = ft_strlen(str);
	while (i < ptrl + 1)
	{
		j = 1;
		while (j < strl + 1)
		{
			if (pattern[i - 1] == '*')
				dp[i][j] = (dp[i - 1][j] || dp[i][j - 1]);
			else if (str[j - 1] == pattern[i - 1])
				dp[i][j] = dp[i - 1][j - 1];
			else
				dp[i][j] = 0;
			++j;
		}
		++i;
	}
}

int	_prs_aep_bool(struct dirent *dirf, const char *word)
{
	return (((word[0] == '.' && dirf->d_name[0] == '.')
			|| (word[0] != '.' && dirf->d_name[0] != '.'))
		&& _prs_asterisk_pattern_matches(word, dirf->d_name));
}
