
#define NX MX-2 //�v�Z�i�q(x����)
#define NY MY-2 //�v�Z�i�q(y����)
#define DX MX//�_�~�[�Z���܂�(x)
#define DY MY//�_�~�[�Z���܂�(y)


//��������
void initial(Treald height[DX][DY],Treald wh1[DX][DY],Treald u[DX][DY],Treald u1[DX][DY]
             ,Treald v[DX][DY],Treald v1[DX][DY]);
//�g���E���x�v�Z
void solver(Treald height[DX][DY],Treald wh1[DX][DY],Treald u[DX][DY],Treald u1[DX][DY]
            ,Treald v[DX][DY],Treald v1[DX][DY]);
//�g���o��
void output(Treald  f[DX][DY],FILE *fp);
//�g���Ex�������x�o��
void output_u(Treald height[DX][DY],Treald u[DX][DY],FILE *fp);
//�g���Ey�������x�o��
void output_v(Treald height[DX][DY],Treald v[DX][DY],FILE *fp);
//���E����
void bound(Treald f[DX][DY],int flag);


void simulate(){

    //initial condition
    initial(height,wh1,u,u1,v,v1);

    //boundary condition
    bound(u,1);
    bound(v,2);
    bound(height,3);
    //solve equation
    solver(height,wh1,u,u1,v,v1);
    bound(u,1);
    bound(v,2);
    bound(height,3);
}

void initial(Treald height[DX][DY],Treald wh1[DX][DY],Treald u[DX][DY],Treald u1[DX][DY]
             ,Treald v[DX][DY],Treald v1[DX][DY]){
    int i,j;
    Treald tempx,tempy;

    for(i=0;i<DX;i++){
        for(j=0;j<DY;j++){
            height[i][j]=0.0;
            wh1[i][j]=0.0;
            u[i][j]=0.0;
            u1[i][j]=0.0;
            v[i][j]=0.0;
            v1[i][j]=0.0;
        }
    }

    dt=0.001; //���ԍ���
    gravity=9.8; //�d�͉����x
    depth=0.1; //����܂ł̐[��
    dh=0.005; //�i�q��
}

void solver(Treald height[DX][DY],Treald wh1[DX][DY],Treald u[DX][DY],Treald u1[DX][DY]
            ,Treald v[DX][DY],Treald v1[DX][DY]){
    int i,j;

    for(i=1;i<=NX;i++){
        for(j=1;j<=NY;j++){
            u1[i][j]=u[i][j]-gravity*(dt/dh)*(height[i+1][j]-height[i][j]);
            v1[i][j]=v[i][j]-gravity*(dt/dh)*(height[i][j+1]-height[i][j]);
        }
    }
    for(i=1;i<=NX;i++){
        for(j=1;j<=NY;j++){
            wh1[i][j]=height[i][j]-height*dt*((u1[i][j]-u1[i-1][j])*dh+(v1[i][j]-v1[i][j-1])*dh)/(dh*dh);
        }
    }
    
    for(i=0;i<DX;i++){
        for(j=0;j<DY;j++){
            height[i][j]=wh1[i][j];
            u[i][j]=u1[i][j];
            v[i][j]=v1[i][j];
        }
    }
}
void bound(Treald f[DX][DY],int flag){
    int i,j;
    if(flag==1){
        for(j=0;j<DY;j++){//u
            f[0][j]=0.0;
            f[DX-2][j]=0.0;
        }
        for(i=0;i<DX;i++){//���R�[
            f[i][0]=f[i][1];
            f[i][DY-1]=f[i][DY-2];
        }
    }else if(flag==2){//v
        for(j=0;j<DY;j++){//���R�[
            f[0][j]=f[1][j];
            f[DX-1][j]=f[DX-2][j];
        }
        for(i=0;i<DX;i++){
            f[i][0]=0.0;
            f[i][DY-2]=0.0;
        }
    }else if(flag==3){//height
        //�Œ�[
        for(j=0;j<DY;j++){
            f[0][j]=f[1][j];
            f[DX-1][j]=f[DX-2][j];
        }
        for(i=0;i<DX;i++){
            f[i][0]=f[i][1];
            f[i][DY-1]=f[i][DY-2];
        }
    }
}
