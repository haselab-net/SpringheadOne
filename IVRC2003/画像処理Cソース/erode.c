/* 8‹ß–Tûkˆ—‚ÌƒvƒƒOƒ‰ƒ€ erode.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

void erosion( )
/* ‚Q’l‰æ‘œ‚Ì‚W‹ß–Tûkˆ—                             */
/* Œ´‰æ‘œ image1[y][x] ===> •ÏŠ·Œã‚Ì‰æ‘œ image2[y][x]   */
{
    int repetition;     /* ûkˆ—‚Ì‰ñ” */
    int counter;        /* ‹ß–T’†‚Ì”wŒi‰æ‘f‚Ì” */
    int p_x, p_y;       /* ‹ß–T‚Ì(x,y)À•W */
    int i, x, y, m, n;  /* §Œä•Ï” */

    x_size2 = x_size1;
    y_size2 = y_size1;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = image1[y][x];
    printf("‚Q’l‰æ‘œ‚Ì‚W‹ß–Tûkˆ—‚ğs‚¢‚Ü‚·D\n");
    printf("ûkˆ—‚ğs‚¤‰ñ” = ");
    scanf("%d", &repetition);
    for ( i = 0; i < repetition; i ++ ){
        for ( y = 0; y < y_size1; y ++ ){
            for ( x = 0; x < x_size1; x ++ ){
                if ( image1[y][x] == MAX_BRIGHTNESS ){
                    counter = 0;
                    for ( m = -1; m < 2; m ++ ){
                        for ( n = -1; n < 2; n ++ ){
                            p_x = x + n;
                            p_y = y + m;
                            if ( p_x > 0 && p_x < x_size1 &&
                                 p_y > 0 && p_y < y_size1 &&
                                 image1[p_y][p_x] == 0 )
                                     counter ++;
                        }
                    }
                    if ( counter > 0 ) image2[y][x] = 0;
                }
            }
        }
        for ( y = 0; y < y_size2; y ++ )
            for ( x = 0; x < x_size2; x ++ )
                image1[y][x] = image2[y][x];
    }
}

main( )
{
    load_image_data( ); /* ‰æ‘œ‚ğ“Ç‚İ‚ñ‚Å image1 ‚Ö */
    erosion( );        /* 8‹ß–Tûkˆ—‚µ‚Ä image2 ‚Ö */
    save_image_data( ); /* image2 ‚ğ•Û‘¶‚·‚é */
    return 0;
}
