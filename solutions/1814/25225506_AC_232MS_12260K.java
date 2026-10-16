// POJ 1814 - Polynomial
// Model: gpt-6.1-sol
// Yun's square-free decomposition over the integers. Primitive pseudo-remainders
// compute exact polynomial gcds with BigInteger intermediates. The hint requires
// pairwise coprime groups; only nonconstant groups are printed, largest exponent first.
import java.io.*;
import java.math.BigInteger;
import java.util.*;
public class Main {
    static final BigInteger Z=BigInteger.ZERO, O=BigInteger.ONE;
    static class P {
        BigInteger[] a;
        P(BigInteger[] b) { int n=b.length; while(n>1 && b[n-1].signum()==0)--n; a=new BigInteger[n]; System.arraycopy(b,0,a,0,n); }
        int d(){return a.length-1;}
        boolean zero(){return d()==0 && a[0].signum()==0;}
    }
    static P primitive(P p) {
        if(p.zero())return p;
        BigInteger g=Z; for(int i=0;i<p.a.length;i++)g=g.gcd(p.a[i]);
        if(p.a[p.d()].signum()<0)g=g.negate();
        BigInteger[] b=new BigInteger[p.a.length];for(int i=0;i<b.length;i++)b[i]=p.a[i].divide(g);return new P(b);
    }
    static P remainder(P a,P b) {
        while(!a.zero() && a.d()>=b.d()) {
            int k=a.d()-b.d();BigInteger x=a.a[a.d()],y=b.a[b.d()];
            BigInteger[] c=new BigInteger[a.a.length];for(int i=0;i<c.length;i++)c[i]=a.a[i].multiply(y);
            for(int i=0;i<b.a.length;i++)c[i+k]=c[i+k].subtract(x.multiply(b.a[i]));
            a=primitive(new P(c));
        }return a;
    }
    static P gcd(P a,P b){a=primitive(a);b=primitive(b);while(!b.zero()){P r=remainder(a,b);a=b;b=r;}return primitive(a);}
    static P divide(P a,P b) {
        BigInteger[] r=(BigInteger[])a.a.clone(),q=new BigInteger[a.d()-b.d()+1];Arrays.fill(q,Z);
        for(int k=q.length-1;k>=0;k--){q[k]=r[k+b.d()].divide(b.a[b.d()]);for(int j=0;j<=b.d();j++)r[k+j]=r[k+j].subtract(q[k].multiply(b.a[j]));}
        return new P(q);
    }
    public static void main(String[] args)throws Exception {
        StreamTokenizer s=new StreamTokenizer(new BufferedReader(new InputStreamReader(System.in)));
        s.nextToken();int t=(int)s.nval;StringBuffer out=new StringBuffer();
        while(t-->0){s.nextToken();int n=(int)s.nval;BigInteger[] a=new BigInteger[n+1];
            for(int i=n;i>=0;i--){s.nextToken();a[i]=BigInteger.valueOf((long)s.nval);}
            P f=new P(a);BigInteger[] der=new BigInteger[n];for(int i=1;i<=n;i++)der[i-1]=a[i].multiply(BigInteger.valueOf(i));
            P c=gcd(f,new P(der)),w=divide(f,c);ArrayList<P> groups=new ArrayList<P>();ArrayList<Integer> powers=new ArrayList<Integer>();
            for(int i=1;w.d()>0;i++){P y=gcd(w,c),z=divide(w,y);if(z.d()>0){groups.add(z);powers.add(Integer.valueOf(i));}w=y;c=divide(c,y);}
            out.append(groups.size()).append('\n');for(int j=groups.size()-1;j>=0;j--){P p=groups.get(j);out.append(powers.get(j)).append(' ').append(p.d());for(int k=p.d();k>=0;k--)out.append(' ').append(p.a[k]);out.append('\n');}
        }System.out.print(out);
    }
}
