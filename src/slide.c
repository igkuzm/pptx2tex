
/**
 * File              : slide.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 06.09.2024
 * Last Modified Date: 07.09.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/**
 * slide.c
 * Copyright (c) 2024 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include "zip_entry_read.h"
#include "slide.h"

#define STRF(...) ({char _s[BUFSIZ]; sprintf(_s, __VA_ARGS__); _s;})

int read_slide(zip_t *zip, const char *target, 
		ezxml_t *slide, ezxml_t *rels)
{
	size_t size;
	void *slide_buf = NULL;
	if (zip_entry_read(zip, 
				STRF("ppt/%s", target),
				&slide_buf, &size))
	{
		fprintf(stderr, "No slide %s\n", target);
		return -1;
	}

	// parse xml
	if (slide)
		*slide = ezxml_parse_str(slide_buf, size);

	// get slide path
	char *s = strdup(target);
	if(!s)
		return -1;
	char *bn = basename(s);
	char *dn = dirname(s);

	void *rels_buf = NULL;
	if (zip_entry_read(zip, 
				STRF("ppt/%s/_rels/%s.rels", dn, bn),
				&rels_buf, &size))
	{
		fprintf(stderr, "No rels for slide %s\n", target);
	}

	free(s);

	// parse rels
	if (rels)
		if (rels_buf)
			*rels = ezxml_parse_str(rels_buf, size);

	return 0;
}
