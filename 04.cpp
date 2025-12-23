//思路：
//========================== 头文件与全局变量 ==========================
#include<cstdio>
#include<cmath>
#include<vector>
#include<algorithm>

#define pi 3.14159265358979323846
#define X(i) (xmin + (i)*hx)
#define Y(j) (ymin + (j)*hy)
#define T(n) ((n)*ht)
#define F(x,y,t) (-pi*((x)*(x)+(y)*(y))*sin(pi*(t))-2*cos(pi*(t)))
#define U0(x,y) ((x)*(x)+(y)*(y))
#define G(x,y,t) (cos(pi*(t))*((x)*(x)+(y)*(y)))

using namespace std;

// ======================= 稀疏矩阵 Triplet =======================
struct MyTriplet {
    int r, c;
    double v;
    MyTriplet(int r_, int c_, double v_) : r(r_), c(c_), v(v_) {}
  };
  
  // ======================= CSR 稀疏矩阵 =======================
  struct MySparseMatrix {
    int n;
    vector<int> row_ptr, col;
    vector<double> val;
  
    MySparseMatrix(int n_ = 0) : n(n_) {}
  
    void setFromTriplets(const vector<MyTriplet> &tri_src) {
      vector<MyTriplet> tri = tri_src;
  
      // 显式类型比较器
      sort(tri.begin(), tri.end(), [](const MyTriplet &a, const MyTriplet &b) {
        return (a.r < b.r) || (a.r == b.r && a.c < b.c);
      });
  
      row_ptr.assign(n + 1, 0);
  
      // 不用 auto
      for (size_t i = 0; i < tri.size(); i++) {
        row_ptr[tri[i].r + 1]++;
      }
  
      for (int i = 0; i < n; i++)
        row_ptr[i + 1] += row_ptr[i];
  
      int nnz = tri.size();
      col.resize(nnz);
      val.resize(nnz);
  
      vector<int> cur = row_ptr;
  
      for (size_t i = 0; i < tri.size(); i++) {
        const MyTriplet &t = tri[i];
        int pos = cur[t.r]++;
        col[pos] = t.c;
        val[pos] = t.v;
      }
    }
  
    vector<double> mul(const vector<double> &x) const {
      vector<double> y(n, 0.0);
      for (int i = 0; i < n; i++) {
        double s = 0;
        for (int k = row_ptr[i]; k < row_ptr[i + 1]; k++)
          s += val[k] * x[col[k]];
        y[i] = s;
      }
      return y;
    }
  };

  // ======================= CG 迭代求解器 =======================
struct MyCGSolver {
    const MySparseMatrix *A = nullptr;
    int maxIter = 2000;
    double tol = 1e-12;
  
    void compute(const MySparseMatrix &A_) { A = &A_; }
  
    vector<double> solve(const vector<double> &b) const {
      int n = A->n;
  
      vector<double> x(n, 0.0);
      vector<double> r = b;
      vector<double> p = b;
  
      auto dot = [&](const vector<double> &a, const vector<double> &b) {
        double s = 0;
        for (int i = 0; i < n; i++)
          s += a[i] * b[i];
        return s;
      };
  
      double rsold = dot(r, r);
  
      for (int it = 0; it < maxIter; it++) {
        vector<double> Ap = A->mul(p);
        double pAp = dot(p, Ap);
        double alpha = rsold / (pAp == 0 ? 1e-30 : pAp);
  
        for (int i = 0; i < n; i++) {
          x[i] += alpha * p[i];
          r[i] -= alpha * Ap[i];
        }
        double rsnew = dot(r, r);
  
        if (sqrt(rsnew) < tol)
          break;
  
        double beta = rsnew / rsold;
        for (int i = 0; i < n; i++)
          p[i] = r[i] + beta * p[i];
  
        rsold = rsnew;
      }
      return x;
    }
  };
  
// =========================== 主函数 ===========================
int main()
{
    //========================== 读取输入参数与初始化 ==========================
    int nspace, ntime;
    double c,total_time,xmin,xmax,ymin,ymax;
    double x_d,y_d;
    scanf("%lf %lf %d %d %lf %lf %lf %lf",&c,&total_time,&nspace,&ntime,&xmin,&xmax,&ymin,&ymax);
    scanf("%lf %lf",&x_d,&y_d);

    double hx=(xmax - xmin)/(nspace);
    double hy=(ymax - ymin)/(nspace);
    double ht=total_time/(ntime);
    double co_x=c*ht/(2*hx*hx);
    double co_y=c*ht/(2*hy*hy);

    vector<double> u((nspace-1)*(nspace-1));
    vector<double> b((nspace-1)*(nspace-1));
    MySparseMatrix A((nspace-1)*(nspace-1));
    MySparseMatrix B((nspace-1)*(nspace-1));
    vector<MyTriplet> tripletA, tripletB;

    //========================== 组装矩阵 A 和 B，并初始化 u ==========================
    for(int j=0;j<nspace-1;j++)//j是列
    {
        for(int i=0;i<nspace-1;i++)//i是行
        {
            u[i+(nspace-1)*j]=U0(X(i+1),Y(j+1));
            tripletA.push_back(MyTriplet(i+(nspace-1)*j,i+(nspace-1)*j,1+2*co_x+2*co_y));
            tripletB.push_back(MyTriplet(i+(nspace-1)*j,i+(nspace-1)*j,1-2*co_x-2*co_y));
            if(i!=0)
            {
                tripletA.push_back(MyTriplet(i+(nspace-1)*j,i-1+(nspace-1)*j,-co_x));
                tripletB.push_back(MyTriplet(i+(nspace-1)*j,i-1+(nspace-1)*j,co_x));
            }
            if(i!=nspace-2)
            {
                tripletA.push_back(MyTriplet(i+(nspace-1)*j,i+1+(nspace-1)*j,-co_x));
                tripletB.push_back(MyTriplet(i+(nspace-1)*j,i+1+(nspace-1)*j,co_x));
            }
            if(j!=0)
            {
                tripletA.push_back(MyTriplet(i+(nspace-1)*j,i+(nspace-1)*(j-1),-co_y));
                tripletB.push_back(MyTriplet(i+(nspace-1)*j,i+(nspace-1)*(j-1),co_y));
            }
            if(j!=nspace-2)
            {
                tripletA.push_back(MyTriplet(i+(nspace-1)*j,i+(nspace-1)*(j+1),-co_y));
                tripletB.push_back(MyTriplet(i+(nspace-1)*j,i+(nspace-1)*(j+1),co_y));
            }
        }
    }
    A.setFromTriplets(tripletA);
    B.setFromTriplets(tripletB);
    MyCGSolver solver;
    solver.compute(A);

    //========================== 求解 ==========================
    for(int k=0;k<ntime;k++)
    {
        //========================== 组装 b ==========================
        for(int j=0;j<nspace-1;j++)//j是列
        {
            for(int i=0;i<nspace-1;i++)//i是行
            {
                b[i+(nspace-1)*j]=F(X(i+1),Y(j+1),(T(k)+T(k+1))/2)*ht;
                if(i==0)
                {
                    b[i+(nspace-1)*j]+=co_x*G(X(0),Y(j+1),T(k))+co_x*G(X(0),Y(j+1),T(k+1));
                }
                if(i==nspace-2)
                {
                    b[i+(nspace-1)*j]+=co_x*G(X(nspace),Y(j+1),T(k))+co_x*G(X(nspace),Y(j+1),T(k+1));
                }
                if(j==0)
                {    
                    b[i+(nspace-1)*j]+=co_y*G(X(i+1),Y(0),T(k))+co_y*G(X(i+1),Y(0),T(k+1));
                }
                if(j==nspace-2)
                {    
                    b[i+(nspace-1)*j]+=co_y*G(X(i+1),Y(nspace),T(k))+co_y*G(X(i+1),Y(nspace),T(k+1));
                }
            }
        }
        vector<double> Bu=B.mul(u);
        for(int i=0;i<(nspace-1)*(nspace-1);i++)
            b[i]+=Bu[i];
        
        //========================== 求解 ==========================
        u=solver.solve(b);
    }

    //========================== 线性插值 ==========================
   // 还原全网格
vector<vector<double>> U_full(nspace + 1, vector<double>(nspace + 1));
for(int j=0; j<=nspace; j++) {
    for(int i=0; i<=nspace; i++) {
        if(i==0 || i==nspace || j==0 || j==nspace)
            U_full[i][j] = G(X(i), Y(j), total_time); // 边界
        else
            U_full[i][j] = u[(i-1) + (nspace-1)*(j-1)]; // 内部
    }
}

//  双线性插值
int i = (int)floor((x_d - xmin) / hx+1e-12);
int j = (int)floor((y_d - ymin) / hy+1e-12);

double x0 = X(i), x1 = X(i+1);
double y0 = Y(j), y1 = Y(j+1);

double v00 = U_full[i][j];
double v10 = U_full[i+1][j];
double v01 = U_full[i][j+1];
double v11 = U_full[i+1][j+1];

double tx = (x_d - x0) / hx;
double ty = (y_d - y0) / hy;

double res = (1-tx)*(1-ty)*v00 + tx*(1-ty)*v10 + (1-tx)*ty*v01 + tx*ty*v11;
printf("%lf\n", res);
return 0;
}