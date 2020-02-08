/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smhah <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/01 20:03:25 by smhah             #+#    #+#             */
/*   Updated: 2019/11/01 22:40:47 by smhah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup(char *str)
{
	size_t			leng;
	unsigned int	i;
	char			*dup;

	i = 0;
	leng = ft_strlen((char *)str);
	dup = (char *)malloc((sizeof(char) * (leng + 1)));
	if (dup == 0)
		return (NULL);
	while (str[i] != '\0')
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = str[i];
	return (dup);
}

int		ft_lastline(char **p, char **line, char *buffer)
{
	*line = ft_strdup(*p);
	if (*p)
	{
		free(*p);
		*p = NULL;
		free(buffer);
	}
	return (0);
}

int		ft_freestatic(char **p)
{
	free(*p);
	return (-1);
}

int		ft_boucle(char **p, char **buffer, char **line, int fd)
{
	char	*pp;
	int		j;

	while (1)
	{
		if (!(*buffer = malloc(BUFFER_SIZE + 1)) || fd < 0 ||
				BUFFER_SIZE < 1 || BUFFER_SIZE > 2147483647 || !line)
			return (ft_freestatic(p));
		j = read(fd, *buffer, BUFFER_SIZE);
		if (j == -1)
			return (ft_freestatic(p));
		if (j == 0 || *(*buffer) == '\0')
			return (ft_lastline(p, line, *buffer));
		*(*buffer + j) = '\0';
		if (ft_strchr(*buffer, '\n'))
			return (1);
		pp = *p;
		*buffer = ft_strjoin(*p, *buffer);
		free(pp);
		*p = ft_strdup(*buffer);
		free(*buffer);
	}
	return (0);
}

int		get_next_line(int fd, char **line)
{
	static char	*p;
	char		*buffer;
	int			j;
	char		*pp;

	buffer = NULL;
	if (!p)
		p = ft_strdup("");
	if (ft_strchr(p, '\n') == 0)
		if ((j = ft_boucle(&p, &buffer, line, fd)) <= 0)
			return (j);
	j = 0;
	pp = p;
	if (!buffer)
		buffer = ft_strjoin(p, "");
	else
		buffer = ft_strjoin(p, buffer);
	free(pp);
	while (ft_strchr("\n\0", buffer[j]) == 0)
		j++;
	p = ft_substr(buffer, j + 1, ft_strlen(buffer) - j + 1);
	line[0] = ft_substr(buffer, 0, j);
	free(buffer);
	return (1);
}
