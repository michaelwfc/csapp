/*
 * driver.h - Various definitions for the Performance Lab.
 * 
 * DO NOT MODIFY ANYTHING IN THIS FILE
 */
#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdlib.h>

#define RIDX(i,j,n) ((i)*(n)+(j))
/* Get element a[i][j] 
int vec_ele(size_t n, int *a, size_t i, size_t j)
{
  return a[RIDX(n,i,j)];
}

*/
typedef struct {
  char *team;
  char *name1, *email1;
  char *name2, *email2;
} team_t;

extern team_t team;

typedef struct {
   unsigned short red;
   unsigned short green;
   unsigned short blue;
} pixel;

typedef struct {
   unsigned short red;
   unsigned short green;
   unsigned short blue;
   // if the pixel is gap or not
   unsigned short is_gap;

} pixel_with_gap;


typedef void (*lab_test_func) (int, pixel*, pixel*);

void smooth(int, pixel *, pixel *);
void rotate(int, pixel *, pixel *);

void register_rotate_functions(void);
void register_smooth_functions(void);
void add_smooth_function(lab_test_func, char*);
void add_rotate_function(lab_test_func, char*);

#endif /* _DEFS_H_ */

