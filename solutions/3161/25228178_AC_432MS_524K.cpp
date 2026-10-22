// POJ 3161 - Numerical Integration
// Model: gpt-6-astra
// Parse with C unary precedence and right-associative exponentiation.
// Globally adaptive Gauss-Kronrod quadrature with Wynn extrapolation.
// Split at detected zeros of potential singular factors. Integrate outward
// from each endpoint with compensated subtraction to retain small distances.
// QUADPACK routines adapted from SciPy's C translation (license below).
/*
 * Copyright (C) 2024 SciPy developers
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * a. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * b. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * c. Names of the SciPy Developers may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;
static const double uflow=2.2250738585072014e-308;
static const double oflow=1.7976931348623157e308;
static const double epmach=2.220446049250313e-16;
void dqelg(int* n, double* epstab, double* result, double* abserr, double* res3la, int* nres)
{
    int i, indx, k1, k2, k3, limexp, n2_rem, newelm, starting_n;
    double delta1, delta2, delta3, epsinf, error, err1, err2, err3;
    double e0, e1, e1abs, e2, e3, res, ss, tol1, tol2, tol3;
    starting_n = *n;
    *nres += 1;
    *abserr = oflow;
    *result = epstab[*n];
    if (*n < 2)
    {
        *abserr = fmax(*abserr, 5.0*epmach*fabs(*result));
        return;
    }
    limexp = 49; 
    epstab[*n + 2] = epstab[*n];
    newelm = *n / 2;
    epstab[*n] = oflow;
    k1 = *n;
    for (i = 0; i < newelm; i++)
    {
        k2 = k1 - 1;
        k3 = k1 - 2;
        res = epstab[k1 + 2];
        e0 = epstab[k3];
        e1 = epstab[k2];
        e2 = res;
        e1abs = fabs(e1);
        delta2 = e2 - e1;
        err2 = fabs(delta2);
        tol2 = fmax(fabs(e2), e1abs)*epmach;
        delta3 = e1 - e0;
        err3 = fabs(delta3);
        tol3 = fmax(e1abs, fabs(e0))*epmach;
        if (!((err2 > tol2) || (err3 > tol3)))
        {
            *result = res;
            *abserr = err2 + err3;
            *abserr = fmax(*abserr, 5.0*epmach*fabs(*result));
            return;
        }
        e3 = epstab[k1];
        epstab[k1] = e1;
        delta1 = e1 - e3;
        err1 = fabs(delta1);
        tol1 = fmax(e1abs, fabs(e3))*epmach;
        if ((err1 <= tol1) || (err2 <= tol2) || (err3 <= tol3))
        {
            *n = i + i;
            break;  
        }
        ss = 1.0/delta1 + 1.0/delta2 - 1.0/delta3;
        epsinf = fabs(ss*e1);
        if (!(epsinf > 1e-4))
        {
            *n = i + i;
            break;  
        }
        res = e1 + 1.0 / ss;
        epstab[k1] = res;
        k1 -= 2;
        error = err2 + fabs(res - e2) + err3;
        if (!(error > *abserr))
        {
            *abserr = error;
            *result = res;
        }
    }
    if (*n == limexp) { *n = 2*(limexp / 2); }
    n2_rem = (starting_n % 2);
    for (i = 0; i <= newelm; i++)
    {
        epstab[2*i + n2_rem] = epstab[2*i + 2 + n2_rem];
    }
    if (*n != starting_n)
    {
        indx = starting_n - *n;
        for (i = 0; i <= *n; i++)
        {
            epstab[i] = epstab[indx];
            indx++;
        }
    }
    if (*nres < 4)
    {
        res3la[*nres - 1] = *result;
        *abserr = oflow;
    } else {
        *abserr = fabs(*result-res3la[2]) + fabs(*result-res3la[1]) + fabs(*result-res3la[0]);
        res3la[0] = res3la[1];
        res3la[1] = res3la[2];
        res3la[2] = *result;
    }
    *abserr = fmax(*abserr, 5.0*epmach*fabs(*result));
    return;
}
void dqk21(double(*fcn)(double* x), const double a, const double b,
      double* result, double* abserr, double* resabs, double* resasc)
{
    int j, jtw, jtwm1;
    double absc, centr, dhlgth, fc, fsum, fval1, fval2, hlgth, resg, resk, reskh;
    double fv1[10], fv2[10];
    double mut_D;
    static const double wg[5] = {
        0.066671344308688137593568809893332,
        0.149451349150580593145776339657697,
        0.219086362515982043995534934228163,
        0.269266719309996355091226921569469,
        0.295524224714752870173892994651338
    };
    static const double xgk[11] = {
        0.995657163025808080735527280689003,
        0.973906528517171720077964012084452,
        0.930157491355708226001207180059508,
        0.865063366688984510732096688423493,
        0.780817726586416897063717578345042,
        0.679409568299024406234327365114874,
        0.562757134668604683339000099272694,
        0.433395394129247190799265943165784,
        0.294392862701460198131126603103866,
        0.148874338981631210884826001129720,
        0.000000000000000000000000000000000
    };
    static const double wgk[11] = {
        0.011694638867371874278064396062192,
        0.032558162307964727478818972459390,
        0.054755896574351996031381300244580,
        0.075039674810919952767043140916190,
        0.093125454583697605535065465083366,
        0.109387158802297641899210590325805,
        0.123491976262065851077958109831074,
        0.134709217311473325928054001771707,
        0.142775938577060080797094273138717,
        0.147739104901338491374841515972068,
        0.149445554002916905664936468389821
    };
    centr = 0.5 * (a + b);
    hlgth = 0.5 * (b - a);
    dhlgth = fabs(hlgth);
    fc=(*fcn)(&centr);
    resg = 0.0;
    resk = wgk[10]*fc;
    *resabs = fabs(resk);
    for (j = 0; j < 5; j++) {
        jtw = 2*j + 1;
        absc = hlgth * xgk[jtw];
        mut_D = centr - absc;
        fval1 = (*fcn)(&mut_D);
        mut_D = centr + absc;
        fval2 = (*fcn)(&mut_D);
        fv1[jtw] = fval1;
        fv2[jtw] = fval2;
        fsum = fval1 + fval2;
        resg = resg + wg[j] * fsum;
        resk = resk + wgk[jtw] * fsum;
        *resabs = *resabs + wgk[jtw] * (fabs(fval1) + fabs(fval2));
    }
    for (j = 0; j < 5; j++) {
        jtwm1 = 2 * j;
        absc = hlgth * xgk[jtwm1];
        mut_D = centr - absc;
        fval1 = (*fcn)(&mut_D);
        mut_D = centr + absc;
        fval2 = (*fcn)(&mut_D);
        fv1[jtwm1] = fval1;
        fv2[jtwm1] = fval2;
        fsum = fval1 + fval2;
        resk = resk + wgk[jtwm1] * fsum;
        *resabs = *resabs + wgk[jtwm1] * (fabs(fval1) + fabs(fval2));
    }
    reskh = resk * 0.5;
    *resasc = wgk[10]*fabs(fc - reskh);
    for (j = 0; j < 10; j++ )
    {
        *resasc = *resasc + wgk[j] * (fabs(fv1[j] - reskh) + fabs(fv2[j] - reskh));
    }
    *result = resk * hlgth;
    *resabs = (*resabs)*dhlgth;
    *resasc = (*resasc)*dhlgth;
    *abserr = fabs((resk - resg) * hlgth);
    if ((*resasc != 0.0) && (*abserr != 0.0))
    {
        *abserr = (*resasc) * fmin(1.0, pow((200.0 * (*abserr)/(*resasc)), 1.5));
    }
    if (*resabs > uflow/(50.0 * epmach))
    {
        *abserr = fmax(epmach * 50.0 * (*resabs),(*abserr));
    }
    return;
}
void dqpsrt(const int limit, const int last, int* maxerr, double* ermax,
       const double* elist, int *iord, int* nrmax)
{
    int i, ibeg, j, jbnd, jupbn, k;
    double errmin, errmax;
    if (last <= 2)
    {
        iord[0] = 0;
        iord[1] = 1;
        *maxerr = iord[*nrmax];
        *ermax = elist[*maxerr];
        return;
    }
    errmax = elist[*maxerr];
    while ((*nrmax > 0) && (errmax > elist[iord[*nrmax-1]]))
    {
        iord[*nrmax] = iord[*nrmax - 1];
        *nrmax -= 1;
    }
    jupbn = (last > limit/2 + 2 ? limit - last + 2 : last - 1);
    errmin = elist[last - 1];
    jbnd = jupbn-1;
    ibeg = *nrmax + 1;
    if (ibeg <= jbnd)
    {
        for (i = ibeg; i <= jbnd; i++)
        {
            if (errmax >= elist[iord[i]]) { goto LINE60; }
            iord[i-1] = iord[i];
        }
    }
    iord[jbnd] = *maxerr;
    iord[jupbn] = last-1;
    *maxerr = iord[*nrmax];
    *ermax = elist[*maxerr];
    return;
LINE60:
    iord[i-1] = *maxerr;
    k = jbnd;
    for (j = i; j <= jbnd; j++)
    {
        if (errmin < elist[iord[k]]) { break; }
        iord[k+1] = iord[k];
        k--;
    }
    if (errmin < elist[iord[k]])
    {
        iord[k+1] = last-1;
    } else {
        iord[i] = last-1;
    }
    *maxerr = iord[*nrmax];
    *ermax = elist[*maxerr];
    return;
}
void dqagse(double(*fcn)(double* x), const double a, const double b,
       const double epsabs, const double epsrel, const int limit, double* result,
       double* abserr, int* neval, int* ier, double* alist, double* blist,
       double* rlist, double* elist, int* iord, int* last)
{
    int extrap, ierror, iroff1, iroff2, iroff3, k, ksgn, ktmin, L, maxerr;
    int noext, nres, nrmax, numrl2;
    double a1, a2, area, area1, area12, area2, b1, b2, defab1, defab2, defabs;
    double dres, erlarg, erlast, errbnd, errmax, error1, error12, error2, errsum;
    double ertest, small, jupbnd, correc, resabs, reseps, abseps;
    double res3la[3], rlist2[52];
    small = 0.0;
    *ier = 0;
    *neval = 0;
    *last = 0;
    alist[0] = a;
    blist[0] = b;
    rlist[0] = 0.0;
    elist[0] = 0.0;
    iord[0] = 0;
    *result = 0.0;
    *abserr = 0.0;
    ierror = 0;
    erlarg = 0.0;
    ertest = 0.0;
    correc = 0.0;
    if ((epsabs <= 0.0) && (epsrel < fmax(50.0*epmach, 0.5e-28)))
    {
        *ier = 6;
        return;
    }
    dqk21(fcn, a, b, result, abserr, &defabs, &resabs);
    dres = fabs(*result);
    errbnd = fmax(epsabs, epsrel*dres);
    *last = 1;
    rlist[0] = *result;
    elist[0] = *abserr;
    iord[0] = 0;
    if ((*abserr <= 100.0*epmach*defabs) && (*abserr > errbnd)) { *ier = 2; }
    if (limit == 1) { *ier = 1; }
    if ((*ier != 0) || ((*abserr <= errbnd) && (*abserr != resabs)) || (*abserr == 0.0)) { goto LINE140; }
    rlist2[0] = *result;
    errmax = *abserr;
    maxerr = 0;
    area = *result;
    errsum = *abserr;
    *abserr = oflow;
    nrmax = 0;
    nres = 0;
    numrl2 = 1;
    ktmin = 0;
    extrap = 0;
    noext = 0;
    iroff1 = 0;
    iroff2 = 0;
    iroff3 = 0;
    ksgn = (dres >= (1.0 - 50.0*epmach)*defabs ? 1 : -1);
    for (L = 1; L < limit; L++)
    {
        *last = L + 1;
        a1 = alist[maxerr];
        b1 = 0.5*(alist[maxerr] + blist[maxerr]);
        a2 = b1;
        b2 = blist[maxerr];
        erlast = errmax;
        dqk21(fcn, a1, b1, &area1, &error1, &resabs, &defab1);
        dqk21(fcn, a2, b2, &area2, &error2, &resabs, &defab2);
        area12 = area1 + area2;
        error12 = error1 + error2;
        errsum = errsum + error12 - errmax;
        area = area + area12 - rlist[maxerr];
        if ((defab1 != error1) && (defab2 != error2))
        {
            if (!((fabs(rlist[maxerr] - area12) > 1.0e-5*fabs(area12)) || (error12 < 0.99*errmax)))
            {
                if (extrap) { iroff2++; } else { iroff1++; }
            }
            if ((L > 9) && (error12 > errmax)) { iroff3++; }
        }
        rlist[maxerr] = area1;
        rlist[L] = area2;
        errbnd = fmax(epsabs, epsrel*fabs(area));
        if (((iroff1 + iroff2) >= 10) || (iroff3 >= 20)) { *ier = 2; }
        if (iroff2 >= 5) { ierror = 3; }
        if (*last == limit) { *ier = 1; }
        if (fmax(fabs(a1), fabs(b2)) <= (1.0 + 100.0*epmach)*(fabs(a2) + 1000.0*uflow))
        {
            *ier = 4;
        }
        if (!(error2 > error1))
        {
            alist[L] = a2;
            blist[maxerr] = b1;
            blist[L] = b2;
            elist[maxerr] = error1;
            elist[L] = error2;
        } else {
            alist[maxerr] = a2;
            alist[L] = a1;
            blist[L] = b1;
            rlist[maxerr] = area2;
            rlist[L] = area1;
            elist[maxerr] = error2;
            elist[L] = error1;
        }
        dqpsrt(limit, *last, &maxerr, &errmax, elist, iord, &nrmax);
        if (errsum <= errbnd) { goto LINE115; }
        if (*ier != 0) { break; }
        if (L == 1) { goto LINE80; }
        if (noext) { continue; }
        erlarg = erlarg - erlast;
        if (fabs(b1 - a1) > small) { erlarg = erlarg + error12; }
        if (!(extrap))
        {
            if (fabs(blist[maxerr] - alist[maxerr]) > small) { continue; }
            extrap = 1;
            nrmax = 1;
        }
        if ((ierror == 3) || (erlarg <= ertest)) { goto LINE60; }
        jupbnd = (*last > 2 + (limit/2) ? limit + 3 - *last : *last);
        for (k = nrmax; k < jupbnd; k++) {
            maxerr = iord[nrmax];
            errmax = elist[maxerr];
            if (fabs(blist[maxerr] - alist[maxerr]) > small) { goto LINE90; } 
            nrmax++;
        }
LINE60:
        numrl2++;
        rlist2[numrl2] = area;
        dqelg(&numrl2, rlist2, &reseps, &abseps, res3la, &nres);
        ktmin += 1;
        if ((ktmin > 5) && (*abserr < 1.0e-3 * errsum)) { *ier = 5; }
        if (abseps >= *abserr) { goto LINE70; }
        ktmin = 0;
        *abserr = abseps;
        *result = reseps;
        correc = erlarg;
        ertest = fmax(epsabs, epsrel*fabs(reseps));
        if (*abserr <= ertest) { break; }
LINE70:
        if (numrl2 == 0) { noext = 1; }
        if (*ier == 5) { break; }
        maxerr = iord[0];
        errmax = elist[maxerr];
        nrmax = 0;
        extrap = 0;
        small = small * 0.5;
        erlarg = errsum;
        continue;
LINE80:
        small = fabs(b - a)*0.375;
        erlarg = errsum;
        ertest = errbnd;
        rlist2[1] = area;
LINE90:
        ; 
    }
    if (*abserr == oflow) { goto LINE115; }
    if ((*ier + ierror) == 0) { goto LINE110; }
    if (ierror == 3) { *abserr = *abserr + correc; }
    if (*ier == 0) { *ier = 3; }
    if ((*result != 0.0) && (area != 0.0)) { goto LINE105; }
    if (*abserr > errsum) { goto LINE115; }
    if (area == 0.0) { goto LINE130; }
    goto LINE110;
LINE105:
    if (*abserr/fabs(*result) > errsum/fabs(area)) { goto LINE115; }
LINE110:
    if ((ksgn == -1) && (fmax(fabs(*result), fabs(area)) <= defabs*0.01)) { goto LINE130; }
    if ((0.01 > (*result/area)) || ((*result/area) > 100.0) || (errsum > fabs(area))) { *ier = 6; }
    goto LINE130;
LINE115:
    *result = 0.0;
    for (k = 0; k <= L; k++)
    {
        *result = *result + rlist[k];
    }
    *abserr = errsum;
LINE130:
    if (*ier > 2) { *ier -= 1; }
LINE140:
    *neval = 42*(*last) - 21;
    return;
}
struct Op { int t; double v; Op(int a,double b=0):t(a),v(b){} };
vector<Op> code; string expr; size_t posi;
void expression();
void atom() {
 char c=expr[posi];
 if(c=='+'||c=='-') { ++posi;atom();if(c=='-')code.push_back(Op(3)); }
 else if(c=='(') { ++posi;expression();++posi; }
 else if(c=='x') { ++posi;code.push_back(Op(1)); }
 else if(isalpha(c)) {
  size_t b=posi;while(isalpha(expr[posi]))++posi;
  string name=expr.substr(b,posi-b);++posi;expression();++posi;
  const char* names[]={"sin","cos","tan","log","exp","asin","acos","atan","abs"};
  for(int i=0;i<9;i++)if(name==names[i])code.push_back(Op(10+i));
 } else { char* e;double v=strtod(expr.c_str()+posi,&e);posi=e-expr.c_str();code.push_back(Op(0,v)); }
}
void power(){atom();if(expr[posi]=='^'){++posi;power();code.push_back(Op(8));}}
void product(){power();while(expr[posi]=='*'||expr[posi]=='/'){char c=expr[posi++];power();code.push_back(Op(c=='*'?6:7));}}
void expression(){product();while(expr[posi]=='+'||expr[posi]=='-'){char c=expr[posi++];product();code.push_back(Op(c=='+'?4:5));}}
vector<double> st, low, values; double xlow=0; vector<int> candidates;
double raw(double x) {
 int n=0;
 for(size_t i=0;i<code.size();i++) {
  int t=code[i].t;
  if(t==0){low[n]=0;st[n++]=code[i].v;}
  else if(t==1){low[n]=xlow;st[n++]=x;}
  else if(t==3){st[n-1]=-st[n-1];low[n-1]=-low[n-1];}
  else if(t<10){double b=st[--n],blow=low[n];double &a=st[n-1],&alow=low[n-1];
   if(t==4||t==5){if(t==5){b=-b;blow=-blow;}double sum=a+b,bb=sum-a;double err=(a-(sum-bb))+(b-bb)+alow+blow;a=sum+err;alow=err-(a-sum);}
   else if(t==6){double v=a*b,e=a*blow+b*alow+alow*blow;a=v+e;alow=e-(a-v);}
   else if(t==7){a=(a+alow)/(b+blow);alow=0;}else{a=pow(a+alow,b+blow);alow=0;}
  }
  else {double &a=st[n-1];a+=low[n-1];low[n-1]=0;switch(t){case 10:a=sin(a);break;case 11:a=cos(a);break;case 12:a=tan(a);break;case 13:a=log(a);break;case 14:a=exp(a);break;case 15:a=asin(a);break;case 16:a=acos(a);break;case 17:a=atan(a);break;case 18:a=fabs(a);}}
  values[i]=st[n-1];
 }
 return st[0];
}
double anchor,scale;
double eval(double* t){
 double d=scale*(*t),x=anchor+d,bb=x-anchor;xlow=(anchor-(x-bb))+(d-bb);
 double v=raw(x);xlow=0;return v*fabs(scale);
}

vector<int> lc,rc;
void mark(int k) {
 if(k<0)return;
 if(find(candidates.begin(),candidates.end(),k)!=candidates.end())return;
 candidates.push_back(k);
 int t=code[k].t;
 if(t==18 || t==3 || t==8)mark(lc[k]);
 if(t==6){mark(lc[k]);mark(rc[k]);}
}
void setup() {
 int n=code.size();lc.assign(n,-1);rc.assign(n,-1);candidates.clear();vector<int> stack;
 for(int i=0;i<n;i++){
  int t=code[i].t;
  if(t>=4&&t<=8){rc[i]=stack.back();stack.pop_back();lc[i]=stack.back();stack.pop_back();}
  else if(t>=3){lc[i]=stack.back();stack.pop_back();}
  stack.push_back(i);
  if(t==7)mark(rc[i]);
  if(t==8||t==13)mark(lc[i]);
 }
}
vector<double> splits(double a,double b) {
 vector<double> result;result.push_back(a);result.push_back(b);
 const int N=257;
 vector<double> old;
 raw(a);old=values;
 for(int i=1;i<=N;i++){
  double lo=a+(b-a)*(i-1)/N,hi=a+(b-a)*i/N;
  raw(hi);vector<double> now=values;
  for(size_t j=0;j<candidates.size();j++){
   int k=candidates[j];double u=old[k],v=now[k];
   if(u==0 && v!=0 && lo>a)result.push_back(lo);
   if(v==0 && u!=0 && hi<b)result.push_back(hi);
   if((u<0&&v>0)||(u>0&&v<0)){
    double l=lo,r=hi;
    for(int q=0;q<60;q++){
     double m=(l+r)*0.5;if(m==l||m==r)break;raw(m);double w=values[k];
     if(w==0){l=r=m;break;}
     if((u<0&&w<0)||(u>0&&w>0)){l=m;u=w;}else r=m;
    }
    double c=(l+r)*0.5;if(c>a&&c<b)result.push_back(c);
   }
  }
  old=now;
 }
 sort(result.begin(),result.end());result.erase(unique(result.begin(),result.end()),result.end());return result;
}
const int LIM=10000;
double al[LIM+1],bl[LIM+1],rl[LIM+1],el[LIM+1];int io[LIM+1];
int main(){
 string line;
 while(getline(cin,line)) {
  istringstream in(line);vector<string> tok;string t;while(in>>t)tok.push_back(t);
  if(tok.size()<3)continue;
  double a=strtod(tok[tok.size()-2].c_str(),0),b=strtod(tok.back().c_str(),0);
  expr="";for(size_t i=0;i+2<tok.size();i++)expr+=tok[i];expr+='\0';posi=0;code.clear();expression();st.resize(code.size()+2);low.resize(code.size()+2);values.resize(code.size());setup();
  double r=0,e;int nv,ier,last;
  double sign=1;if(a>b){swap(a,b);sign=-1;}
  if(a!=b){vector<double> cuts=splits(a,b);for(size_t j=1;j<cuts.size();j++){
   for(int side=0;side<2;side++){
    anchor=side?cuts[j]:cuts[j-1];scale=(side?-1:1)*(cuts[j]-cuts[j-1]);
    double part;dqagse(eval,0,0.5,1e-8,1e-12,LIM,&part,&e,&nv,&ier,al,bl,rl,el,io,&last);r+=part;
   }
  }}r*=sign;
  if(fabs(r)<0.00005)r=0;
  printf("%.4f\n",r);
 }
}
