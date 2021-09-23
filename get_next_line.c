/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <nic.flenghi@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 13:52:44 by fnichola          #+#    #+#             */
/*   Updated: 2021/05/27 16:07:02 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * Allocate MORE memory for the 'line' variable.
 */

int	expand_line(char **line, size_t add_length)
{
	char	*new_line;
	size_t	i;

	new_line = (char *)malloc(ft_strlen(*line) + add_length + 1);
	if (!new_line)
		return (ERROR);
	i = 0;
	while ((*line)[i])
	{
		new_line[i] = (*line)[i];
		i++;
	}
	new_line[i] = 0;
	free(*line);
	*line = new_line;
	return (SUCCESS);
}

/**
 * Shift the content of buffer to the left.
 * Replace remaining contents with nulls.
 * e.g. if start_index = 5,
 * |0123456789|     |01234 5 6 7 8 9|
 * "HelloWorld" --> "World\0\0\0\0\0"
 */

void	shift_buf(char *buf, const size_t start_index)
{
	size_t	i;
	size_t	j;

	i = start_index;
	j = 0;
	if (i > 0)
	{
		while (i < BUFFER_SIZE && buf[i])
			buf[j++] = buf[i++];
		while (j < BUFFER_SIZE && buf[j])
			buf[j++] = 0;
	}
}

/**
 * Copy contents of buffer (up to newline) to line array.
 * Call expand_line to make space in the array.
 */

int	copy_buf_to_line(char *buf, char **line)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < BUFFER_SIZE && buf[i] && buf[i] != '\n')
		i++;
	if (i < BUFFER_SIZE && buf[i] == '\n')
		i++;
	if (i > 0)
	{
		if (expand_line(line, i) < 0)
			return (ERROR);
		i = 0;
		j = 0;
		while ((*line)[j])
			j++;
		while (i < BUFFER_SIZE && buf[i] && buf[i] != '\n')
			(*line)[j++] = buf[i++];
		if (i < BUFFER_SIZE && buf[i] == '\n')
			(*line)[j++] = '\n';
		(*line)[j] = 0;
	}
	shift_buf(buf, i);
	return (SUCCESS);
}

/**
 * Read a line from buffer/fd and save it.
 * If buf[0] is a newline, then it will first read the
 * remaining contents of the buffer before reading from
 * the file at fd.
 *
 *  * RETURN:
 * 1 : A line has been read
 * 0 : EOF has been reached
 * -1 : An error happened
 */

int	read_and_copy_line(int fd, char *buf, char **line)
{
	if (buf[0] == '\n')
	{
		shift_buf(buf, 1);
		if (copy_buf_to_line(buf, line) < 0)
			return (ERROR);
		if (buf[0] == '\n')
			return (SUCCESS);
	}
	if (read(fd, buf, BUFFER_SIZE) < 0)
		return (ERROR);
	while (buf[0])
	{
		if (copy_buf_to_line(buf, line) < 0)
			return (ERROR);
		if (buf[0] == '\n')
			return (SUCCESS);
		if (read(fd, buf, BUFFER_SIZE) < 0)
			return (ERROR);
	}
	return (EOF);
}

/**
 * Returns a line read from a file descriptor.
 *
 * RETURN:
 * Read line : correct behaviour
 * NULL : nothing else to read or an error occurred
 */

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE];
	int			ret;
	char		*line;

	if (!BUFFER_SIZE)
		return (NULL);
	line = (char *)malloc(sizeof(char));
	if (!line)
		return (NULL);
	line[0] = 0;
	ret = read_and_copy_line(fd, buf, &line);
	if (ret == ERROR || (ret == EOF && ft_strlen(line) == 0))
	{
		free(line);
		line = NULL;
		return (NULL);
	}
	return (line);
}
