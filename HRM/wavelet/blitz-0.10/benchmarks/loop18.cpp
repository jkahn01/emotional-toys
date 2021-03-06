
// loop18 generated by makeloops.py Thu Jun 30 16:44:56 2011

#include <blitz/vector2.h>
#include <blitz/array.h>
#include <random/uniform.h>
#include <blitz/benchext.h>

#ifdef BZ_HAVE_VALARRAY
 #define BENCHMARK_VALARRAY
#endif

#ifdef BENCHMARK_VALARRAY
#include <valarray>
#endif

BZ_NAMESPACE(blitz)
extern void sink();
BZ_NAMESPACE_END

BZ_USING_NAMESPACE(blitz)
BZ_USING_NAMESPACE(std)

#if defined(BZ_FORTRAN_SYMBOLS_WITH_TRAILING_UNDERSCORES)
 #define loop18_f77 loop18_f77_
 #define loop18_f77overhead loop18_f77overhead_
 #define loop18_f90 loop18_f90_
 #define loop18_f90overhead loop18_f90overhead_
#elif defined(BZ_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
 #define loop18_f77 loop18_f77__
 #define loop18_f77overhead loop18_f77overhead__
 #define loop18_f90 loop18_f90__
 #define loop18_f90overhead loop18_f90overhead__
#elif defined(BZ_FORTRAN_SYMBOLS_CAPS)
 #define loop18_f77 LOOP18_F77
 #define loop18_f77overhead LOOP18_F77OVERHEAD
 #define loop18_f90 LOOP18_F90
 #define loop18_f90overhead LOOP18_F90OVERHEAD
#endif

extern "C" {
  void loop18_f77(const int& N, double* x, double* a, double* b, const double& u, const double& v);
  void loop18_f77overhead(const int& N, double* x, double* a, double* b, const double& u, const double& v);
  void loop18_f90(const int& N, double* x, double* a, double* b, const double& u, const double& v);
  void loop18_f90overhead(const int& N, double* x, double* a, double* b, const double& u, const double& v);

}

void VectorVersion(BenchmarkExt<int>& bench, double u, double v);
void ArrayVersion(BenchmarkExt<int>& bench, double u, double v);
void ArrayVersion_unaligned(BenchmarkExt<int>& bench, double u, double v);
void ArrayVersion_misaligned(BenchmarkExt<int>& bench, double u, double v);
void ArrayVersion_index(BenchmarkExt<int>& bench, double u, double v);
void doTinyVectorVersion(BenchmarkExt<int>& bench, double u, double v);
void F77Version(BenchmarkExt<int>& bench, double u, double v);
#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, double u, double v);
#endif
#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, double u, double v);
#endif

const int numSizes = 80;
const bool runvector=false; // no point as long as Vector is Array<1>

int main()
{
    int numBenchmarks = 5;
    if (runvector) numBenchmarks++;
#ifdef BENCHMARK_VALARRAY
    numBenchmarks++;
#endif
#ifdef FORTRAN_90
    numBenchmarks++;
#endif

    BenchmarkExt<int> bench("loop18: $x = (u+$a)*(v+$b)", numBenchmarks);

    bench.setNumParameters(numSizes);

    Array<int,1> parameters(numSizes);
    Array<long,1> iters(numSizes);
    Array<double,1> flops(numSizes);

    parameters=pow(pow(2.,0.25),tensor::i)+tensor::i;
    flops = 3 * parameters;
    iters = 100000000L / flops;
    iters = where(iters<2, 2, iters);
    cout << iters << endl;
    
    bench.setParameterVector(parameters);
    bench.setIterations(iters);
    bench.setOpsPerIteration(flops);
    bench.setDependentVariable("flops");
    bench.beginBenchmarking();

    double u = 0.39123982498157938742;
    double v = 0.39123982498157938742;


    ArrayVersion(bench, u, v);
    ArrayVersion_unaligned(bench, u, v);
    ArrayVersion_misaligned(bench, u, v);
    ArrayVersion_index(bench, u, v);
    //doTinyVectorVersion(bench, u, v);
    F77Version(bench, u, v);
#ifdef FORTRAN_90
    F90Version(bench, u, v);
#endif
#ifdef BENCHMARK_VALARRAY
    ValarrayVersion(bench, u, v);
#endif

    if(runvector)
      VectorVersion(bench, u, v);

    bench.endBenchmarking();

    bench.saveMatlabGraph("loop18.m");
    return 0;
}

template<class T>
void initializeRandomDouble(T* data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

template<class T>
void initializeRandomDouble(valarray<T>& data, int numElements, int stride = 1)
{
    ranlib::Uniform<T> rnd;

    for (int i=0; i < numElements; ++i)
        data[size_t(i*stride)] = rnd.random();
}

void VectorVersion(BenchmarkExt<int>& bench, double u, double v)
{
    bench.beginImplementation("Vector<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Vector<double> x(N);
        initializeRandomDouble(x.data(), N);
        Vector<double> a(N);
        initializeRandomDouble(a.data(), N);
        Vector<double> b(N);
        initializeRandomDouble(b.data(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion(BenchmarkExt<int>& bench, double u, double v)
{
    bench.beginImplementation("Array<T,1>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<double,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<double,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<double,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}


  void ArrayVersion_index(BenchmarkExt<int>& bench, double u, double v)
{
    bench.beginImplementation("Array<T,1> (indexexpr.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;

        Array<double,1> x(N);
        initializeRandomDouble(x.dataFirst(), N);
        Array<double,1> a(N);
        initializeRandomDouble(a.dataFirst(), N);
        Array<double,1> b(N);
        initializeRandomDouble(b.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a(tensor::i))*(v+b(tensor::i));;
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_unaligned(BenchmarkExt<int>& bench, double u, double v)
{
    bench.beginImplementation("Array<T,1> (unal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<double,1> xfill(N+1);
    Array<double,1> x(xfill(Range(1,N)));
    initializeRandomDouble(x.dataFirst(), N);

    Array<double,1> afill(N+1);
    Array<double,1> a(afill(Range(1,N)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<double,1> bfill(N+1);
    Array<double,1> b(bfill(Range(1,N)));
    initializeRandomDouble(b.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

  void ArrayVersion_misaligned(BenchmarkExt<int>& bench, double u, double v)
{
    bench.beginImplementation("Array<T,1> (misal.)");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        long iters = bench.getIterations();

        cout << bench.currentImplementation() << ": N = " << N << endl;


    Array<double,1> xfill(N+3);
    Array<double,1> x(xfill(Range(0,N+0-1)));
    initializeRandomDouble(x.dataFirst(), N);

    Array<double,1> afill(N+3);
    Array<double,1> a(afill(Range(1,N+1-1)));
    initializeRandomDouble(a.dataFirst(), N);

    Array<double,1> bfill(N+3);
    Array<double,1> b(bfill(Range(2,N+2-1)));
    initializeRandomDouble(b.dataFirst(), N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
            sink();
	}

        bench.stopOverhead();
    }

    bench.endImplementation();
}

#ifdef BENCHMARK_VALARRAY
void ValarrayVersion(BenchmarkExt<int>& bench, double u, double v)
{
    bench.beginImplementation("valarray<T>");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        long iters = bench.getIterations();

        valarray<double> x(N);
        initializeRandomDouble(x, N);
        valarray<double> a(N);
        initializeRandomDouble(a, N);
        valarray<double> b(N);
        initializeRandomDouble(b, N);


        bench.start();
        for (long i=0; i < iters; ++i)
        {
            x = (u+a)*(v+b);
            sink();
        }
        bench.stop();

        bench.startOverhead();
        for (long i=0; i < iters; ++i) {
	  sink();
	}
        bench.stopOverhead();
    }

    bench.endImplementation();
}
#endif

void F77Version(BenchmarkExt<int>& bench, double u, double v)
{
    bench.beginImplementation("Fortran 77");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        double* x = new double[N];
        initializeRandomDouble(x, N);
        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* b = new double[N];
        initializeRandomDouble(b, N);
        

        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop18_f77(N, x, a, b, u, v);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop18_f77overhead(N, x, a, b, u, v);

        bench.stopOverhead();

        delete [] x;
        delete [] a;
        delete [] b;

    }

    bench.endImplementation();
}

#ifdef FORTRAN_90
void F90Version(BenchmarkExt<int>& bench, double u, double v)
{
    bench.beginImplementation("Fortran 90");

    while (!bench.doneImplementationBenchmark())
    {
        int N = bench.getParameter();
        cout << bench.currentImplementation() << ": N = " << N << endl;

        int iters = bench.getIterations();

        double* x = new double[N];
        initializeRandomDouble(x, N);
        double* a = new double[N];
        initializeRandomDouble(a, N);
        double* b = new double[N];
        initializeRandomDouble(b, N);


        bench.start();
        for (int iter=0; iter < iters; ++iter)
            loop18_f90(N, x, a, b, u, v);
        bench.stop();

        bench.startOverhead();
        for (int iter=0; iter < iters; ++iter)
            loop18_f90overhead(N, x, a, b, u, v);

        bench.stopOverhead();
        delete [] x;
        delete [] a;
        delete [] b;

    }

    bench.endImplementation();
}
#endif

