/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "michael",              /* Team name */

    "Michael Wang",     /* First member full name */
    "michael@nowhere.edu",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
        // rotate = transpose + Exchange row
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */


char unroll2_rotate_descr[] = "unroll2_rotate: ratate with up rolling with factor 2";

void unroll2_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim-1; i+=2)
	for (j = 0; j < dim-1; j+=2)
        // rotate = transpose + Exchange row
	    {dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
        dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
        dst[RIDX(dim-1-j-1, i, dim)] = src[RIDX(i, j+1, dim)];
        dst[RIDX(dim-1-j-1, i+1, dim)] = src[RIDX(i+1, j+1, dim)];
        }

    // for the remaining indexes
    // for ( ; i < dim; i++)
	// for ( ; j < dim; j++)
    //     // rotate = transpose + Exchange row
	//     dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}


char symmetric_rotate_descr[] = "symmetric_rotate: ratate with symmetric";
void symmetric_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i <dim; i++)
	for (j = 0; j <= i; j++)
    {
        // rotate = transpose + Exchange row
        // the pixels at (i,j)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
        // the pixels at (j,i)
        dst[RIDX(dim-1-i, j, dim)] = src[RIDX(j, i, dim)];
    }
}

char unroll2s_rotate_descr[] = "unroll2s_rotate: ratate with up rolling with factor 2 and symmetric";

void unroll2s_rotate(int dim, pixel *src, pixel *dst) 
{
    int i=0, j=0;

    for (i = 0; i < dim-1; i+=2)
	for (j = 0; j <=i; j+=2)
        // rotate = transpose + Exchange row
	    {
        // the pixels at (i,j)
        dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
        dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
        dst[RIDX(dim-1-j-1, i, dim)] = src[RIDX(i, j+1, dim)];
        dst[RIDX(dim-1-j-1, i+1, dim)] = src[RIDX(i+1, j+1, dim)];
        
         // the pixels at (j,i)
        dst[RIDX(dim-1-i,   j,   dim)] = src[RIDX(j,   i, dim)];
        dst[RIDX(dim-1-i,   j+1, dim)] = src[RIDX(j+1, i, dim)];
        dst[RIDX(dim-1-i-1, j,   dim)] = src[RIDX(j,   i+1, dim)];
        dst[RIDX(dim-1-i-1, j+1, dim)] = src[RIDX(j+1, i+1, dim)];

        }

    // for the remaining indexes
    // for ( ; i < dim; i++)
	// for ( ; j <= i; j++)
    // {
    //     // rotate = transpose + Exchange row
	//     dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
    //     // the pixels at (j,i)
    //     dst[RIDX(dim-1-i, j, dim)] = src[RIDX(j, i, dim)];
    // }
}

char unroll8_rotate_descr[] = "unroll8_rotate: ratate with up rolling with factor 8";

void unroll8_rotate(int dim, pixel *src, pixel *dst) 
{
    int i=0, j=0;

    for (i = 0; i < dim; i+=8)
	for (j = 0; j < dim; j+=8)
        // rotate = transpose + Exchange row
        for(int k=i; k < i+8; k++)
	    {
            // the pixels at (i,j)
            dst[RIDX(dim-1-j, k, dim)] = src[RIDX(k, j, dim)];
            dst[RIDX(dim-2-j, k, dim)] = src[RIDX(k, j+1, dim)];
            dst[RIDX(dim-3-j, k, dim)] = src[RIDX(k, j+2, dim)];
            dst[RIDX(dim-4-j, k, dim)] = src[RIDX(k, j+3, dim)];
            dst[RIDX(dim-5-j, k, dim)] = src[RIDX(k, j+4, dim)];
            dst[RIDX(dim-6-j, k, dim)] = src[RIDX(k, j+5, dim)];
            dst[RIDX(dim-7-j, k, dim)] = src[RIDX(k, j+6, dim)];
            dst[RIDX(dim-8-j, k, dim)] = src[RIDX(k, j+7, dim)];        

        }

    // for the remaining indexes
    // for ( ; i < dim; i++)
	// for ( ; j <= i; j++)
    // {
    //     // rotate = transpose + Exchange row
	//     dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
    //     // the pixels at (j,i)
    //     dst[RIDX(dim-1-i, j, dim)] = src[RIDX(j, i, dim)];
    // }
}

// char rotate_descr[] ;
// rotate_descr = unroll2_rotate_descr;

void rotate(int dim, pixel *src, pixel *dst) 
{
    naive_rotate(dim, src, dst);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr); 
    add_rotate_function(&unroll2_rotate, unroll2_rotate_descr);  
    add_rotate_function(&symmetric_rotate, symmetric_rotate_descr); 
    add_rotate_function(&unroll2s_rotate, unroll2s_rotate_descr); 
    add_rotate_function(&unroll8_rotate, unroll8_rotate_descr); 

    // add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

// static void accumulate_sum_with_gap(pixel_sum *sum, pixel_with_gap p) 
// {   if (~ p.is_gap)
//         {
//             sum->red += (int) p.red;
//             sum->green += (int) p.green;
//             sum->blue += (int) p.blue;
//             sum->num++;
           
//         }
//      return;
// }

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    naive_smooth(dim, src, dst);
}

char unroll2_smooth_descr[] = "unroll2_smooth: unroll with factor 2";
void unroll2_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim-1; i+=2)
	for (j = 0; j < dim-1; j+=2)
    {
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
        dst[RIDX(i, j+1, dim)] = avg(dim, i, j+1, src);
        dst[RIDX(i+1, j, dim)] = avg(dim, i+1, j, src);
        dst[RIDX(i+1, j+1, dim)] = avg(dim, i+1, j+1, src);
    }


    for (; i < dim; i++)
	for (; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}




void extend_pixel_with_gap(pixel_with_gap *dst,int dim,pixel_with_gap * gap_element)
{   
    int j;
    for(j = 0;j<dim;j++)
        {dst[RIDX(-1, j, dim)] = *gap_element; 
         dst[RIDX(dim, j, dim)]= *gap_element; 
         dst[RIDX(j, -1, dim)] = *gap_element; 
         dst[RIDX(j, dim, dim)] =*gap_element; 
        }
        
}
// void convert_to_pixle_with_map(pixel *src,pixel_with_gap *dst,int dim)
//     {
//         int i, j;
//         for (i = 0; i < dim; i++)
//             for (j = 0; j < dim; j++)
//             {
//                 dst[RIDX(i, j, dim)].red = src[RIDX(i, j, dim)].red;
//                 dst[RIDX(i, j, dim)].green = src[RIDX(i, j, dim)].green;
//                 dst[RIDX(i, j, dim)].blue = src[RIDX(i, j, dim)].blue;
//                 dst[RIDX(i, j, dim)].is_gap = 0;
//             }

//     }


// static void inital_pixel_with_gap(pixel_with_gap* x)
// {
    
//     x->is_gap = 0;
//     x->blue=0;
//     x->green=0;
//     x->red=0;

// }

// char unrollavg_smooth_descr[] = "unrollavg_smooth: smooth with unroll avg";
// void unrollavg_smooth(int dim, pixel *src, pixel *dst) 
// {   
//     // // initialize
//     pixel_with_gap * src_with_gap = (pixel_with_gap*)src;

//     // // add gap around src
//     pixel_with_gap pixel_with_gap_element ={.blue = 0, .green =0, .red =0, .is_gap=1};

//     extend_pixel_with_gap(src_with_gap,dim,&pixel_with_gap_element);


//     int i, j;

//     for (i = 0; i < dim; i++)
// 	for (j = 0; j < dim; j++)
// 	{  
//         // int ii, jj;
//         pixel_sum sum;
//         pixel current_pixel = dst[RIDX(i, j, dim)];

//         initialize_pixel_sum(&sum);
//         int up_row = i-1;
//         int above_row= i+1;
//         int left_column =  j-1;
//         int right_column =j+1;

//         accumulate_sum_with_gap(&sum, src_with_gap[RIDX(up_row, left_column, dim)]);
//         accumulate_sum_with_gap(&sum, src_with_gap[RIDX(up_row, j, dim)]);
//         accumulate_sum_with_gap(&sum, src_with_gap[RIDX(up_row, right_column, dim)]);

//         accumulate_sum_with_gap(&sum, src_with_gap[RIDX(i, left_column, dim)]);
//         accumulate_sum_with_gap(&sum, src_with_gap[RIDX(i, j, dim)]);
//         accumulate_sum_with_gap(&sum, src_with_gap[RIDX(i, right_column, dim)]);

//         accumulate_sum_with_gap(&sum, src_with_gap[RIDX(above_row, left_column, dim)]);
//         accumulate_sum_with_gap(&sum, src_with_gap[RIDX(above_row, j, dim)]);
//         accumulate_sum_with_gap(&sum, src_with_gap[RIDX(above_row, right_column, dim)]);

//         assign_sum_to_pixel(&current_pixel, sum);
 
//     }
// }


void avg_leftup_corner(int dim, pixel *src,pixel *dst) 
{
    // i=0,j=0;
    pixel_sum sum;
    initialize_pixel_sum(&sum);
    accumulate_sum(&sum, src[RIDX(0, 0, dim)]);
    accumulate_sum(&sum, src[RIDX(0, 1, dim)]);
    accumulate_sum(&sum, src[RIDX(1, 0, dim)]);
    accumulate_sum(&sum, src[RIDX(1, 1, dim)]);
    assign_sum_to_pixel(&dst[RIDX(0,0,dim)], sum);
}

void avg_rightup_corner(int dim, pixel *src,pixel *dst) 
{
    pixel_sum sum;
    initialize_pixel_sum(&sum);
    int j= dim-1;
    accumulate_sum(&sum, src[RIDX(0, j, dim)]);
    accumulate_sum(&sum, src[RIDX(0, j-1, dim)]);
    accumulate_sum(&sum, src[RIDX(1, j, dim)]);
    accumulate_sum(&sum, src[RIDX(1, j-1, dim)]);
    assign_sum_to_pixel(&dst[RIDX(0,j,dim)], sum);
}

void avg_leftdown_corner(int dim, pixel *src,pixel *dst) 
{
    int i =dim-1;
    pixel_sum sum;
    initialize_pixel_sum(&sum);

    accumulate_sum(&sum, src[RIDX(i, 0, dim)]);
    accumulate_sum(&sum, src[RIDX(i, 1, dim)]);
    accumulate_sum(&sum, src[RIDX(i-1, 0, dim)]);
    accumulate_sum(&sum, src[RIDX(i-1, 1, dim)]);
    assign_sum_to_pixel(&dst[RIDX(i,0,dim)], sum);
}

void avg_rightdown_corner(int dim, pixel *src,pixel *dst) 
{   
    int i = dim-1;
    int j= dim-1;
    pixel_sum sum;
    initialize_pixel_sum(&sum);
    
    accumulate_sum(&sum, src[RIDX(i, j, dim)]);
    accumulate_sum(&sum, src[RIDX(i, j-1, dim)]);
    accumulate_sum(&sum, src[RIDX(i-1, j, dim)]);
    accumulate_sum(&sum, src[RIDX(i-1, j-1, dim)]);
    assign_sum_to_pixel(&dst[RIDX(i,j,dim)], sum);
}


void avg_board(int dim, pixel *src,pixel *dst) 
{
   for(int j=1;j<dim-1;j++)
    {
        pixel_sum sum;
        pixel current_pixel = dst[RIDX(0, j, dim)];

        initialize_pixel_sum(&sum);
        int i =0;
        int up_row = -1;
        int above_row= 1;
        int left_column =  j-1;
        int right_column =j+1;

        // accumulate_sum(&sum, src[RIDX(up_row, left_column, dim)]);
        // accumulate_sum(&sum, src[RIDX(up_row, j, dim)]);
        // accumulate_sum(&sum, src[RIDX(up_row, right_column, dim)]);

        accumulate_sum(&sum, src[RIDX(i, left_column, dim)]);
        accumulate_sum(&sum, src[RIDX(i, j, dim)]);
        accumulate_sum(&sum, src[RIDX(i, right_column, dim)]);

        accumulate_sum(&sum, src[RIDX(above_row, left_column, dim)]);
        accumulate_sum(&sum, src[RIDX(above_row, j, dim)]);
        accumulate_sum(&sum, src[RIDX(above_row, right_column, dim)]);

        assign_sum_to_pixel(&current_pixel, sum);


        // for the down side
        // TODO: Benchmark "unrollavg_smooth_v2: smooth with unroll avg" failed correctness check for dimension 96.
        // You have dst[95][94].{red,green,blue} = {0,0,0}
        // It should be dst[95][94].{red,green,blue} = {32994,31885,46479} 
        i = dim-1;
        current_pixel = dst[RIDX(i, j, dim)];

        initialize_pixel_sum(&sum);
        up_row = dim-2;
        above_row= dim;
        left_column =  j-1;
        right_column =j+1;

        accumulate_sum(&sum, src[RIDX(up_row, left_column, dim)]);
        accumulate_sum(&sum, src[RIDX(up_row, j, dim)]);
        accumulate_sum(&sum, src[RIDX(up_row, right_column, dim)]);

        accumulate_sum(&sum, src[RIDX(i, left_column, dim)]);
        accumulate_sum(&sum, src[RIDX(i, j, dim)]);
        accumulate_sum(&sum, src[RIDX(i, right_column, dim)]);

        // accumulate_sum(&sum, src[RIDX(above_row, left_column, dim)]);
        // accumulate_sum(&sum, src[RIDX(above_row, j, dim)]);
        // accumulate_sum(&sum, src[RIDX(above_row, right_column, dim)]);

        assign_sum_to_pixel(&current_pixel, sum);


        // for the left side
        i = j;
        int jj=0;
        current_pixel = dst[RIDX(i, 0, dim)];

        initialize_pixel_sum(&sum);
        up_row = i-1;
        above_row= i+1;
        left_column =  -1;
        right_column = 1;

        // accumulate_sum(&sum, src[RIDX(up_row, left_column, dim)]);
        // accumulate_sum(&sum, src[RIDX(i, left_column, dim)]);
        // accumulate_sum(&sum, src[RIDX(above_row, left_column, dim)]);

        accumulate_sum(&sum, src[RIDX(up_row, jj, dim)]);
        accumulate_sum(&sum, src[RIDX(i, jj, dim)]);
        accumulate_sum(&sum, src[RIDX(above_row, jj, dim)]);

        accumulate_sum(&sum, src[RIDX(up_row, right_column, dim)]);      
        accumulate_sum(&sum, src[RIDX(i, right_column, dim)]);      
        accumulate_sum(&sum, src[RIDX(above_row, right_column, dim)]);

        assign_sum_to_pixel(&current_pixel, sum);

        // for the right side
        i=j;
        jj=dim-1;
        current_pixel = dst[RIDX(i, jj, dim)];

        initialize_pixel_sum(&sum);
        up_row = i-1;
        above_row= i+1;
        left_column =  dim-2;
        right_column = dim;

        accumulate_sum(&sum, src[RIDX(up_row, left_column, dim)]);
        accumulate_sum(&sum, src[RIDX(i, left_column, dim)]);
        accumulate_sum(&sum, src[RIDX(above_row, left_column, dim)]);

        accumulate_sum(&sum, src[RIDX(up_row,jj , dim)]);
        accumulate_sum(&sum, src[RIDX(i, jj, dim)]);
        accumulate_sum(&sum, src[RIDX(above_row, jj, dim)]);

        // accumulate_sum(&sum, src[RIDX(up_row, right_column, dim)]);      
        // accumulate_sum(&sum, src[RIDX(i, right_column, dim)]);      
        // accumulate_sum(&sum, src[RIDX(above_row, right_column, dim)]);

        assign_sum_to_pixel(&current_pixel, sum);



    }
}



char unrollavg_smooth_v2_descr[] = "unrollavg_smooth_v2: smooth with unroll avg";
void unrollavg_smooth_v2(int dim, pixel *src, pixel *dst) 
{   
    avg_leftup_corner(dim,src,dst);
    avg_rightup_corner(dim,src,dst);
    avg_leftdown_corner(dim,src,dst);
    avg_rightdown_corner(dim,src,dst);
    avg_board(dim,src,dst);
    
    int i=0, j=0;
    for (i = 1; i < dim-1; i++)
	for (j = 1; j < dim-1; j++)
	{  
        pixel_sum sum;
        pixel current_pixel = dst[RIDX(i, j, dim)];

        initialize_pixel_sum(&sum);
        int up_row = i-1;
        int above_row= i+1;
        int left_column =  j-1;
        int right_column =j+1;

        accumulate_sum(&sum, src[RIDX(up_row, left_column, dim)]);
        accumulate_sum(&sum, src[RIDX(up_row, j, dim)]);
        accumulate_sum(&sum, src[RIDX(up_row, right_column, dim)]);

        accumulate_sum(&sum, src[RIDX(i, left_column, dim)]);
        accumulate_sum(&sum, src[RIDX(i, j, dim)]);
        accumulate_sum(&sum, src[RIDX(i, right_column, dim)]);

        accumulate_sum(&sum, src[RIDX(above_row, left_column, dim)]);
        accumulate_sum(&sum, src[RIDX(above_row, j, dim)]);
        accumulate_sum(&sum, src[RIDX(above_row, right_column, dim)]);

        assign_sum_to_pixel(&current_pixel, sum);
 
    }
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    // add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&unroll2_smooth,unroll2_smooth_descr);
    // add_smooth_function(&unrollavg_smooth,unrollavg_smooth_descr);
    add_smooth_function(&unrollavg_smooth_v2,unrollavg_smooth_v2_descr);
    /* ... Register additional test functions here */
}

