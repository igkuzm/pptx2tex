
/**
 * File              : main.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 06.09.2024
 * Last Modified Date: 07.09.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/**
 * src/main.c
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
#include <zip.h>
#include "ezxml.h"
#include "presentation.h"
#include "relationship.h"
#include "slide.h"

#define ezxml_for_each(xml, name, element)\
	for (element = ezxml_child(xml, name); \
			 element; \
			 element = element->next)

FILE *fp;

static void print_usage(char **argv)
{
	fprintf(stderr, 
			"Usage: %s FROM.pptx TO.tex\n", argv[0]);
}

int parse_slide_sp(ezxml_t sp)
{

	// text
	ezxml_t txBody = ezxml_child(sp, "p:txBody");
	ezxml_t p;
	ezxml_for_each(txBody, "a:p", p){

		ezxml_t r;
		ezxml_for_each(p, "a:r", r){
			ezxml_t t = ezxml_child(r, "a:t");
			if (t){
				printf("%s\n", ezxml_txt(t));
			}
		}	
	}

	return 0;
}

int parse_slide(zip_t *zip, const char *target)
{
	ezxml_t s, r;
	if (read_slide(zip, target, &s, &r))	
		return -1;

	// slide content
	ezxml_t cSld = ezxml_child(s, "p:cSld");
	if (!cSld){
		fprintf(stderr,
				"No content in slide: %s\n", target);
		return -1;
	}
	ezxml_t spTree;
	ezxml_for_each(cSld, "p:spTree", spTree)
	{
		// traverse content
		ezxml_t sp;
		ezxml_for_each(spTree, "p:sp", sp)
		{
			if (parse_slide_sp(sp)){

				continue;
			}
		}
	}
	
	return 0;
}

int parse_presentation(zip_t *zip)
{
	ezxml_t p, r;
	if (read_presentation(zip, &p, &r))	
		return -1;

	ezxml_t sldIdLst = 
		ezxml_child(p, "p:sldIdLst");	
	if (!sldIdLst){
		fprintf(stderr,
				"No slides in presentation\n");
		return -1;
	}

	// traverse slides
	int i = 0;
	ezxml_t sldId;
	for(sldId = ezxml_child(sldIdLst, "p:sldId");
			sldId; 
			sldId = sldId->next, i++)
	{	
		const char *id = 
			ezxml_attr(sldId, "r:id");
		if (!id){
			fprintf(stderr,
					"No id for slide: %d\n", i);
			continue;
		}
		// get relationship target
		const char *t = 
			get_relationship_target(r, id);
		if (!t){
			fprintf(stderr,
					"No relationship target for slide: %d\n", i);
			continue;
		}
		parse_slide(zip, t);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 2){
		print_usage(argv);
		return 0;
	}

	// open out file
	fp = fopen(argv[2], "w");
	if (!fp){
		perror("can't open file to write");
		return -1;
	}

	int err = 0;
	zip_file_t *f; 
	
	// try to open zip file
	zip_t *zip = zip_open(argv[1], ZIP_RDONLY, &err);
	if (err){
		perror("fail extracting zip archive");
		return -1;
	}

	if (parse_presentation(zip)){
		perror("fail parse presentation");
		return -1;
	}

	// close files
	fclose(fp);
	zip_close(zip);
	
	return 0;
}
