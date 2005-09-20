/* ‰æ‘œ‚ğ”½“]‚³‚¹‚éƒvƒƒOƒ‰ƒ€ inverse.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

void make_inverse_image( )
/* Œ´‰æ‘œ image1 ‚ğ”½“]‚³‚¹‚Ä image2 ‚É‘ã“ü‚·‚é */
{
    int x, y;    /* §Œä•Ï” */

    printf("”½“]‰æ‘œ‚ğì¬‚µ‚Ü‚·D\n");
    x_size2 = x_size1;  /* ‰¡‰æ‘f” */
    y_size2 = y_size1;  /* c‰æ‘f” */
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            image2[y][x] = (unsigned char)
                           (MAX_BRIGHTNESS - image1[y][x]);
        }
    }
}

main( )
{
    load_image_data( );    /* ‰æ‘œƒf[ƒ^‚ğ image1 ‚É“Ç‚İ‚Ş */
    make_inverse_image( ); /* image1 ‚ğ”½“]‚³‚¹ image2 ‚Ö */
    save_image_data( );    /* image2 ‚ğ•Û‘¶‚·‚é */
    return 0;
}
