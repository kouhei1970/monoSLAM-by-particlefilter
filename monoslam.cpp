//Copyright Kouhei Ito 2015.5.
//g++ monoslam.cpp -I/usr/include/eigen3

#include <iostream>
#include <Eigen/Dense>
#include <stdio.h>
#include <cmath>

#define PRINT_MAT(X) cout << #X << ":\n" << X << endl << endl
#define PRINT_MAT2(X,DESC) cout << DESC << ":\n" << X << endl << endl
#define PRINT_FNC    cout << "[" << __func__ << "]" << endl

using namespace std;
using namespace Eigen;

class feature {
		double x,y,z,u,v;
	public:
		void setPos(double x,double y,double z);
		void showPos(void);
		void project(double f);
};

void feature::setPos(double v1,double v2,double v3)
{
	x=v1;
	y=v2;
	z=v3;
}

void feature::showPos(void)
{
	//cout<<x<<","<<y<<","<<z<<"\n";
	printf("%f,%f,%f\n",x,y,z);
}

void feature::project(double f)
{
	u=x*f/(y+f);
	v=z*f/(y+f);
	printf("%f %f\n",u,v);
}

int main()
{
	//特徴点の座標
	double fpos[8][3]={	-1000,     0,	 5000,
									 		 1000,     0,	 5000,
									        0,     0,	 7000,
									        0,  2000, 10000,
										  -1000,  2000, 10000,
									        0,  3000, 10000,
									     1000,  2000, 10000,
											    0,  1000, 10000};

	//特徴点オブジェクト生成
	feature feat[8];

	//特徴点座標設定
	for(int i=0;i<8;i++){
		feat[i].setPos(fpos[i][0],fpos[i][1],fpos[i][2]);
	}

	//回転座標変換行列計算
	Vector3d axisx;
	axisx << 1,0,0;
	Vector3d axisy;
	axisy << 0,1,0;
	Vector3d axisz;
	axisz << 0,0,1;	
	Matrix3d rotx; 
	rotx = AngleAxisd(0.1,axisx);
	Matrix3d roty;
	roty = AngleAxisd(0.0,axisy);
	Matrix3d rotz;
	rotz = AngleAxisd(0.0,axisz);
	Matrix3d rot = rotz*roty*rotx;

	PRINT_MAT(rot);
	double tx=0.0,ty=2000.0,tz=0.0;
	Matrix4d T;
	T<< 0,0,0,-tx,
			0,0,0,-ty,
			0,0,0,-tz,
			0,0,0,1;
	PRINT_MAT(T);

	T.block(0,0,3,3)=rot;
	PRINT_MAT(T);
	
	double f=0.5,ox=320.0,oy=240.0,kx=10.0,ky=10.0;
	Matrix<double,3,4> K;
	K<< f*kx,    0, ox, 0,
			   0, f*ky, oy, 0,
			   0,    0,  1.0, 0;
	PRINT_MAT(K);
	
	Matrix<double,3,4> P;
	P=K*T;

	PRINT_MAT(P);

	Vector4d fp;
	Vector3d img;
	fp<<0.0,2000.0,10000.0,1.0;
	

	img=P*fp;

	PRINT_MAT(img);

  //画面上の座標表示
	printf("%f,%f\n",img(0)/img(2),img(1)/img(2));
	
	
}
