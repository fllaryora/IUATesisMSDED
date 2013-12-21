/*
   
   Random Number Generator is based on the algorithm in a FORTRAN
   version published by George Marsaglia and Arif Zaman, Florida State
   University; ref.: see original comments below.
   At the fhw (Fachhochschule Wiesbaden, W.Germany), Dept. of Computer
   Science, we have written sources in further languages (C, Modula-2
   Turbo-Pascal(3.0, 5.0), Basic and Ada) to get exactly the same test
   results compared with the original FORTRAN version.
   April 1989
   Karl-L. Noell <NOELL@DWIFH1.BITNET>
      and  Helmut  Weber <WEBER@DWIFH1.BITNET>

   This random number generator originally appeared in "Toward a Universal
   Random Number Generator" by George Marsaglia and Arif Zaman.
   Florida State University Report: FSU-SCRI-87-50 (1987)
   It was later modified by F. James and published in "A Review of Pseudo-
   random Number Generators"
   THIS IS THE BEST KNOWN RANDOM NUMBER GENERATOR AVAILABLE.
   (However, a newly discovered technique can yield
   a period of 10^600. But that is still in the development stage.)
   It passes ALL of the tests for random number generators and has a period
   of 2^144, is completely portable (gives bit identical results on all
   machines with at least 24-bit mantissas in the floating point
   representation).
   The algorithm is a combination of a Fibonacci sequence (with lags of 97
   and 33, and operation "subtraction plus one, modulo one") and an
   "arithmetic sequence" (using subtraction).

   Use IJ = 1802 & KL = 9373 to test the random number generator. The
   subroutine RANMAR should be used to generate 20000 random numbers.
   Then display the next six random numbers generated multiplied by 4096*4096
   If the random number generator is working properly, the random numbers
   should be:
           6533892.0  14220222.0  7275067.0
           6172232.0  8354498.0   10633180.0
*/

#include "RNGs.h"

/*
 Retorna una semilla creada por tiempo+pseudoruido
*/
int SeedGenerator(const int maxSeed ){
	int seed = -1;
	FILE* urandom = fopen("/dev/urandom", "r");

	while(seed < 0 || seed > maxSeed) {
		fread(&seed, sizeof(int), 1, urandom);
		seed  ^= (unsigned)time(NULL);
	}
	fclose(urandom);
	return seed;
}

/*
   This is the initialization routine for the random number generator.
   NOTE: The seed variables can have values between:    0 <= IJ <= 31328
                                                        0 <= KL <= 30081
   The random number sequences created by these two seeds are of sufficient
   length to complete an entire calculation with. For example, if sveral
   different groups are working on different parts of the same calculation,
   each group could be assigned its own IJ seed. This would leave each group
   with 30000 choices for the second seed. That is to say, this random
   number generator can create 900 million different subsequences -- with
   each subsequence having a length of approximately 10^30.
*/
void RandomInitialise(RngInstance * rng, int ij,int kl ){
	double s,t;
	int ii,i,j,k,l,jj,m;

   /*
      Handle the seed range errors
         First random number seed must be between 0 and 31328
         Second seed must have a value between 0 and 30081
   */
	if (ij < 0 || ij > 31328 || kl < 0 || kl > 30081) {
		ij = 1802;
		kl = 9373;
	}

	i = (ij / 177) % 177 + 2;
	j = (ij % 177)       + 2;
	k = (kl / 169) % 178 + 1;
	l = (kl % 169);

	for (ii=0; ii<97; ii++) {
		s = 0.0;
		t = 0.5;
		for (jj=0; jj<24; jj++) {
			m = (((i * j) % 179) * k) % 179;
			i = j;
			j = k;
			k = m;
			l = (53 * l + 1) % 169;
			if (((l * m % 64)) >= 32)
				s += t;
			t *= 0.5;
		}
		rng->uList[ii] = s;
	}

	rng->coeficient = 362436.0 / 16777216.0;
	rng->coeficientD = 7654321.0 / 16777216.0;
	rng->coeficientM = 16777213.0 / 16777216.0;
	rng->integer97 = 97;
	rng->jinteger97 = 33;
	rng->isInitialise = TRUE;
}

/* 
   This is the random number generator proposed by George Marsaglia in
   Florida State University Report: FSU-SCRI-87-50
*/
double RandomUniform(RngInstance * rng){
	double uni;

	/* Make sure the initialisation routine has been called */
	if (!rng->isInitialise){
		RandomInitialise(rng, SeedGenerator(31328 ),SeedGenerator(30081));
	}

	uni = rng->uList[rng->integer97-1] - rng->uList[rng->jinteger97-1];
	if (uni <= 0.0)
		uni++;
	rng->uList[rng->integer97-1] = uni;
	(rng->integer97)--;
	if (rng->integer97 == 0)
		rng->integer97 = 97;
	(rng->jinteger97)--;
	if (rng->jinteger97 == 0)
		rng->jinteger97 = 97;
	rng->coeficient -= rng->coeficientD;
	if (rng->coeficient < 0.0)
		rng->coeficient += rng->coeficientM;
	uni -= rng->coeficient;
	if (uni < 0.0)
		uni++;

	return(uni);
}

/*
   Return random integer within a range, lower -> upper INCLUSIVE
*/
int RandomInt(RngInstance * rng, const int lower, const int upper){
	return((int)(RandomUniform(rng) * (upper - lower + 1)) + lower);
}

/*
 * uniforme en modelo precursor
 * "least" "highest"
   Return random float within a range, lower -> upper
*/
double RandomDouble(RngInstance * rng, const double lower, const double upper){
	return((upper - lower) * RandomUniform(rng) + lower);
}

/*
  ALGORITHM 712, COLLECTED ALGORITHMS FROM ACM.
  THIS WORK PUBLISHED IN TRANSACTIONS ON MATHEMATICAL SOFTWARE,
  VOL. 18, NO. 4, DECEMBER, 1992, PP. 434-435.
  The function returns a normally distributed pseudo-random number
  with a given mean and standard devaiation.  Calls are made to a
  function subprogram which must return independent random
  numbers uniform in the interval (0,1).
  The algorithm uses the ratio of uniforms method of A.J. Kinderman
  and J.F. Monahan augmented with quadratic bounding curves.
*/
double RandomGaussian(RngInstance * rng, const double mean, const double stddev){
	double  q,u,v,x,y;
	/*  
		Generate P = (u,v) uniform in rect. enclosing acceptance region 
      Make sure that any random numbers <= 0 are rejected, since
      gaussian() requires uniforms > 0, but RandomUniform() delivers >= 0.
	*/
	do {
		u = RandomUniform(rng);
		v = RandomUniform(rng);
		if (u <= 0.0 || v <= 0.0) {
			u = 1.0;
			v = 1.0;
		}
		v = 1.7156 * (v - 0.5);

		/*  Evaluate the quadratic form */
		x = u - 0.449871;
		y = fabs(v) + 0.386595;
		q = x * x + y * (0.19600 * y - 0.25472 * x);

		/* Accept P if inside inner ellipse */
		if (q < 0.27597)
			break;

		/*  Reject P if outside outer ellipse, or outside acceptance region */
	} while ((q > 0.27846) || (v * v > -4.0 * log(u) * u * u));

	/*  Return ratio of P's coordinates as the normal deviate */
	return (mean + stddev * v / u);
}

double RandomNormal(RngInstance * rng, const double mean, const double variance){
	//conversion varianza por desviacion estandar
	return RandomGaussian(rng, mean, sqrt(variance));
}

/*
  Wikipedia, using inverse formula
*/
double RandomExponential(RngInstance * rng, const double lambda){
	double u = RandomUniform(rng); 
	return -log(u)/lambda;
}

/*
 * RandomTriangular
 * Se usa para: una descripción subjetiva de una población
 * para la que sólo se cuenta con una cantidad limitada de datos
 * muestrales y, especialmente en casos en que la relación entre 
 * variables es conocida pero los datos son escasos (posiblemente 
 * porque es alto el costo de recolectarlos). Está basada en un 
 * conocimiento del mínimo y el máximo y un "pálpito inspirado" como el 
 * del valor modal. Por estos motivos, la Distribución Triangular ha 
 * sido denominada como la de "falta de precisión" o de información.
 * 
 *  min ≤ moda ≤ max
 * 
 * Metodo sacado de: http://www.ntrand.com/triangular-distribution/
 * 
 *  ED : rama ,Simulacion: metodos y aplicaciones __> da malos resultados
 * 
 * Random number x is generated by inverse function method,
 *  which is for uniform random U, 
 * */
double RandomTriangular(RngInstance * rng, const double min, const double max, const double moda){
	double U = RandomUniform(rng); 
	double beta = (moda - min) / (max - min);
	if (U <= beta)
		return min + sqrt(U * (max - min) * (moda - min));
	//ELSE	
	return max - sqrt((1 - U) * (max - min) * (max - moda));
}
	
/*
 * http://en.wikipedia.org/wiki/Log-normal_distribution
 * scale = mu = media
 * shape = stddev = sigma = desviacion estandar
 * sigma cuadrado es la varianza
 * */
double RandomLogNormal(RngInstance * rng, const double scale, const double shape){
	double Z = RandomGaussian(rng, 0.0 , 1.0);
	return exp(scale + shape * Z);
}

/*
 * Generador de Distribucion lognormal. con minimo
 * */
double RandomLogNormalWithMinimun(RngInstance * rng, const double scale, const double shape, const double minimun){
	return RandomLogNormal(rng, scale,  shape) + minimun;
}

/*
 * Computer Generation of Statistical Distributions
 * by Richard Saucier ed. 2000
 * */
double RandomGamma(RngInstance * rng, double a, double b, double c ){
	if ( b <= 0.0 && c <= 0.0 )
		return 0.0;
	const double A = 1.0 / sqrt( 2.0 * c - 1.0 );
	const double B = c - log( 4.0 );
	const double Q = c + 1.0 / A;
	const double T = 4.5;
	const double D = 1.0 + log( T );
	const double C = 1.0 + c / 2.71828182845904523536;
	
	//CASE c < 1
	if ( c < 1.0 ) {
		while ( 1 ) {
			double p = C * RandomUniform(rng);
			if ( p > 1.0 ) {
				double y = -log( ( C - p ) / c );
				if ( RandomUniform(rng) <= pow( y, c - 1.0 ) )
				return a + b * y;
			}
			else {
				double y = pow( p, 1.0 / c );
				if ( RandomUniform(rng) <= exp( -y ) )
					return a + b * y;
			}
		}
	}
	
	//CASE c = 1
	if ( c == 1.0 )
		return ( a - b * log( RandomUniform(rng) ) );
	
	//CASE c > 1	
	while ( 1 ) {
		double p1 = RandomUniform(rng);
		double p2 = RandomUniform(rng);
		double v = A * log( p1 / ( 1. - p1 ) );
		double y = c * exp( v );
		double z = p1 * p1 * p2;
		double w = B + Q * v - y;
		if ( w + D - T * z >= 0.0 || w >= log( z ) )
			return a + b * y;
	}
	
}

/*
* Generador de Distribucion beta. con minimo y maximo
* Computer Generation of Statistical Distributions
* by Richard Saucier 
* alpha = v; beta = w;
*/
double RandomBeta(RngInstance * rng, double shapeAlpha ,double shapeBeta, double minimun ,double maximun  ){
	double X1,X2;
	X1 = RandomGamma(rng, 0.0, 1.0, shapeAlpha);
	X2 = RandomGamma(rng, 0.0, 1.0, shapeBeta );
	
	if (shapeAlpha < shapeBeta) {
		return maximun - (maximun - minimun) * X2/(X1+X2);
	}
	
	return((maximun - minimun) * X1/(X1+X2) + minimun);
}
