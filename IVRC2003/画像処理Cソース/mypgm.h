/* pgm �t�@�C����ǂݏ������邽�߂̃w�b�_�t�@�C�� mypgm.h */

/* �萔�錾 */
#define MAX_IMAGESIZE  1024 /* �z�肷��c�E���̍ő��f�� */
#define MAX_BRIGHTNESS  255 /* �z�肷��ő�K���l */
#define GRAYLEVEL       256 /* �z�肷��K����(=�ő�K���l+1) */
#define MAX_FILENAME    256 /* �z�肷��t�@�C�����̍ő咷 */
#define MAX_BUFFERSIZE  256 /* ���p����o�b�t�@�ő咷 */

/* ���ϐ��̐錾 */
/* �摜�p�z��P�C�摜�p�z��Q */
unsigned char image1[MAX_IMAGESIZE][MAX_IMAGESIZE],
              image2[MAX_IMAGESIZE][MAX_IMAGESIZE];
int x_size1, y_size1, /* image1 �̉���f���C�c��f�� */
    x_size2, y_size2; /* image2 �̉���f���C�c��f�� */

/* �֐��̃v���g�^�C�v�錾 */
void load_image_data( ); /* �摜�ǂݍ��ݗp�֐� */
void save_image_data( ); /* �摜�������ݗp�֐� */


/* �ȉ��͊֐��̖{�� */

void load_image_data( )
/* pgm �摜�C����f���C�c��f���̃f�[�^���t�@�C������ǂݍ��݁C*/
/* image1[ ][ ]�Cx_size1�Cy_size1 �ɂ��ꂼ��������D         */
{
    char file_name[MAX_FILENAME]; /* �t�@�C�����p�̕����z�� */
    char buffer[MAX_BUFFERSIZE];  /* �f�[�^�ǂݍ��ݗp��ƕϐ� */
    FILE *fp; /* �t�@�C���|�C���^ */
    int max_gray; /* �ő�K���l */
    int x, y; /* ���[�v�ϐ� */

    /* ���̓t�@�C���̃I�[�v�� */
    printf("-----------------------------------------------------\n");
    printf("  ���m�N���K���摜���̓��[�`��\n");
    printf("-----------------------------------------------------\n");
    printf("�t�@�C���`���� pgm, �o�C�i���`���Ƃ��܂��D\n");
    printf("���̓t�@�C���� (*.pgm) : ");
    scanf("%s",file_name);
    fp = fopen( file_name, "rb" );
    if ( NULL == fp ){
        printf("���̖��O�̃t�@�C���͑��݂��܂���D\n");
        exit(1);
    }
    /* �t�@�C���^�C�v(=P5)�̊m�F */
    fgets( buffer, MAX_BUFFERSIZE, fp );
    if ( buffer[0] != 'P' || buffer[1] != '5' ){
        printf("�t�@�C���̃t�H�[�}�b�g�� P5 �Ƃ͈قȂ�܂��D\n");
        exit(1);
    }
    /* x_size1, y_size1 �̑���i#����n�܂�R�����g�͓ǂݔ�΂��j */
    x_size1 = 0;
    y_size1 = 0;
    while ( x_size1 == 0 || y_size1 == 0 ){
        fgets( buffer, MAX_BUFFERSIZE, fp );
        if ( buffer[0] != '#' ){
            sscanf( buffer, "%d %d", &x_size1, &y_size1 );
        }
    }
    /* max_gray �̑���i#����n�܂�R�����g�͓ǂݔ�΂��j */
    max_gray = 0;
    while ( max_gray == 0 ){
        fgets( buffer, MAX_BUFFERSIZE, fp );
        if ( buffer[0] != '#' ){
            sscanf( buffer, "%d", &max_gray );
        }
    }
    /* �p�����[�^�̉�ʂւ̕\�� */
    printf("���̉�f�� = %d, �c�̉�f�� = %d\n", x_size1, y_size1 );
    printf("�ő�K���l = %d\n",max_gray);
    if ( x_size1 > MAX_IMAGESIZE || y_size1 > MAX_IMAGESIZE ){
        printf("�z��l %d x %d �𒴂��Ă��܂��D\n", 
            MAX_IMAGESIZE, MAX_IMAGESIZE);
        printf("�������������ȉ摜���g���ĉ������D\n");
        exit(1);
    }
    if ( max_gray != MAX_BRIGHTNESS ){
        printf("�ő�K���l���s�K�؂ł��D\n");
        exit(1);
    }
    /* �摜�f�[�^��ǂݍ���ŉ摜�p�z��ɑ������ */
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            image1[y][x] = (unsigned char)fgetc( fp );
        }
    }
    printf("�f�[�^�͐������ǂݍ��܂�܂����D\n");
    printf("-----------------------------------------------------\n");
    fclose(fp);
}

void save_image_data()
/* image2[ ][ ], x_size2, y_size2 �̃f�[�^���C���ꂼ�� pgm �摜�C*/
/* ����f���C�c��f���Ƃ��ăt�@�C���ɕۑ�����D                  */
{
    char file_name[MAX_FILENAME]; /* �t�@�C�����p�̕����z�� */
    FILE *fp; /* �t�@�C���|�C���^ */
    int x, y; /* ���[�v�ϐ� */

    /* �o�̓t�@�C���̃I�[�v�� */
    printf("-----------------------------------------------------\n");
    printf("  ���m�N���K���摜�ipgm�`���j�o�̓��[�`��\n");
    printf("-----------------------------------------------------\n");
    printf("�o�̓t�@�C���� (*.pgm) : ");
    scanf("%s",file_name);
    fp = fopen(file_name, "wb");
    /* �t�@�C�����ʎq "P5" ��擪�ɏo�͂��� */
    fputs( "P5\n", fp );
    /* # �Ŏn�܂�R�����g�s�i�ȗ��\�j */
    fputs( "# Created by Image Processing\n", fp );
    /* �摜�̉����C�c���̏o�� */
    fprintf( fp, "%d %d\n", x_size2, y_size2 );
    /* �ő�K���l�̏o�� */
    fprintf( fp, "%d\n", MAX_BRIGHTNESS );
    /* �摜�f�[�^�̏o�� */
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            fputc( image2[y][x], fp );
        }
    }
    printf("�f�[�^�͐������o�͂���܂����D\n");
    printf("-----------------------------------------------------\n");
    fclose(fp);
}
