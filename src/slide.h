#ifndef SLIDE_H
#define SLIDE_H
#include "ezxml.h"
#include <zip.h>
int read_slide(zip_t *zip, const char *target, 
		ezxml_t *slide, ezxml_t *rels);
#endif /* ifndef SLIDE_H */
