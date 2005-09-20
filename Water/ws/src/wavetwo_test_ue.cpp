
#define NX MX-2 //計算格子(x方向)
#define NY MY-2 //計算格子(y方向)
#define DX MX//ダミーセル含む(x)
#define DY MY//ダミーセル含む(y)


//初期条件
void initial(Treald height[DX][DY],Treald wh1[DX][DY],Treald u[DX][DY],Treald u1[DX][DY]
             ,Treald v[DX][DY],Treald v1[DX][DY]);
//波高・速度計算
void solver(Treald height[DX][DY],Treald wh1[DX][DY],Treald u[DX][DY],Treald u1[DX][DY]
            ,Treald v[DX][DY],Treald v1[DX][DY]);
//波高出力
void output(Treald  f[DX][DY],FILE *fp);
//波高・x方向速度出力
void output_u(Treald height[DX][DY],Treald u[DX][DY],FILE *fp);
//波高・y方向速度出力
void output_v(Treald height[DX][DY],Treald v[DX][DY],FILE *fp);
//境界条件
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

    dt=0.001; //時間刻み
    gravity=9.8; //重力加速度
    depth=0.1; //水底までの深さ
    dh=0.005; //格子幅
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
        for(i=0;i<DX;i++){//自由端
            f[i][0]=f[i][1];
            f[i][DY-1]=f[i][DY-2];
        }
    }else if(flag==2){//v
        for(j=0;j<DY;j++){//自由端
            f[0][j]=f[1][j];
            f[DX-1][j]=f[DX-2][j];
        }
        for(i=0;i<DX;i++){
            f[i][0]=0.0;
            f[i][DY-2]=0.0;
        }
    }else if(flag==3){//height
        //固定端
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
