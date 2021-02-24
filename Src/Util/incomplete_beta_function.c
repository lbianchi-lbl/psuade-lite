////////////////////////////////////////////////////////////////////////////////
// File: incomplete_beta_function.c (from mymathlib)                          //
// Routine(s):                                                                //
//    Incomplete_Beta_Function                                                //
//    xIncomplete_Beta_Function                                               //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Description:                                                              //
//     The incomplete beta function is the integral from 0 to x of            //
//                    t^(a-1) (1-t)^(b-1) dt,                                 //
//     where 0 <= x <= 1, a > 0 and b > 0.                                    //
////////////////////////////////////////////////////////////////////////////////
#include <math.h>                    // required for powl(), fabsl(), expl(),
                                     // and logl().
#include <float.h>                   // required for DBL_MAX and
                                     // LDBL_EPSILON.

//                         Externally Defined Routines                        //

extern long double xBeta_Function(long double a, long double b);

//                         Internally Defined Routines                        //

double Incomplete_Beta_Function(double x, double a,double b);
long double xIncomplete_Beta_Function(long double x, long double a,
                                                               long double b);

static long double Beta_Continued_Fraction( long double x, long double a,
                                                               long double b);

////////////////////////////////////////////////////////////////////////////////
// double Incomplete_Beta_Function( double x, double a, double b )            //
//                                                                            //
//  Description:                                                              //
//     The incomplete beta function is the integral from 0 to x of            //
//                    t^(a-1) (1-t)^(b-1) dt,                                 //
//     where 0 <= x <= 1, a > 0 and b > 0.                                    //
//                                                                            //
//     The procedure for evaluating the incomplete beta function uses the     //
//     continued fraction expansion for the incomplete beta function:         //
//        beta(x,a,b) = x^a * (1-x)^b / a * ( (1/1+)(d[1]/1+)(d[2]/1+)...)    //
//     where d[2m+1] = - (a+m)(a+b+m)x/((a+2m)(a+2m+1))                       //
//           d[2m] = m(b-m)x/((a+2m)(a+2m-1)),                                //
//     the symmetry relation:                                                 //
//           beta(x,a,b) = B(a,b) - beta(1-x,b,a)                             //
//     where B(a,b) is the complete beta function,                            //
//     the recurrence relations:                                              //
//           beta(x,a+1,b) = a/b beta(x,a,b+1) - x^a (1-x)^b / b              //
//           beta(x,a,b+1) = b/a beta(x,a+1,b) + x^a (1-x)^b / a,             //
//     and the interrelationship:                                             //
//           beta(x,a,b) = beta(x,a+1,b) + beta(x,a,b+1).                     //
//                                                                            //
//     If both a > 1 and b > 1, then                                          //
//        if x <= (a-1) / ( a+b-2), then                                      //
//           use the continued fraction expansion                             //
//        otherwise                                                           //
//           use the symmetry relation and use the continued fraction         //
//           expansion to evaluate beta(1-x,b,a).                             //
//                                                                            //
//     If a < 1 and b > 1, then                                               //
//        use the interrelationship equation together with the recurrence     //
//        relation to evaluate                                                //
//           beta(x,a,b) = [(a+b) beta(x,a+1,b) + x^a (1-x)^b]/a.             //
//                                                                            //
//     If a > 1 and b < 1, then                                               //
//        use the interrelationship equation together with the recurrence     //
//        relation to evaluate                                                //
//           beta(x,a,b) = [(a+b) beta(x,a,b+1) - x^a (1-x)^b]/b.             //
//                                                                            //
//     If a < 1 and b < 1, then                                               //
//        use the interrelationship equation to evaluate                      //
//           beta(x,a,b) = beta(x,a+1,b) + beta(x,a,b+1)                      //
//        in terms of beta functions which now have one shape parameter > 1.  //
//                                                                            //
//     If a == 1, then evaluate the integral explicitly,                      //
//           beta(x,a,b) = [1 - (1-x)^b]/b.                                   //
//                                                                            //
//     If b == 1, then evaluate the integral explicitly,                      //
//           beta(x,a,b) = x^a / a.                                           //
//                                                                            //
//                                                                            //
//  Arguments:                                                                //
//     double x   Upper limit of the incomplete beta function integral, x must//
//                be in the closed interval [0,1].                            //
//     double a   A positive shape parameter of the incomplete beta function, //
//                a - 1 is the exponent of the factor x in the integrand.     //
//     double b   A positive Shape parameter of the incomplete beta function, //
//                b - 1 is the exponent of the factor (1-x) in the integrand. //
//                                                                            //
//  Return Values:                                                            //
//     If beta(x,a,b) exceeds DBL_MAX then DBL_MAX is returned otherwise      //
//     beta(x,a,b) is returned.                                               //
//                                                                            //
//  Example:                                                                  //
//     double a, b, beta, x;                                                  //
//                                                                            //
//     beta = Incomplete_Beta_Function(x, a, b);                              //
////////////////////////////////////////////////////////////////////////////////

double Incomplete_Beta_Function(double x, double a, double b)
{
   long double beta = xIncomplete_Beta_Function( (long double) x,
                                             (long double) a, (long double) b);
   return (beta > DBL_MAX) ? DBL_MAX : (double) beta; 
}


////////////////////////////////////////////////////////////////////////////////
// long double xIncomplete_Beta_Function( long double x, long double a,       //
//                                                            long double b ) //
//                                                                            //
//  Description:                                                              //
//     The incomplete beta function is the integral from 0 to x of            //
//                    t^(a-1) (1-t)^(b-1) dt,                                 //
//     where 0 <= x <= 1, a > 0 and b > 0.                                    //
//                                                                            //
//     The procedure for evaluating the incomplete beta function uses the     //
//     continued fraction expansion for the incomplete beta function:         //
//        beta(x,a,b) = x^a * (1-x)^b / a * ( (1/1+)(d[1]/1+)(d[2]/1+)...)    //
//     where d[2m+1] = - (a+m)(a+b+m)x/((a+2m)(a+2m+1))                       //
//           d[2m] = m(b-m)x/((a+2m)(a+2m-1)),                                //
//     the symmetry relation:                                                 //
//           beta(x,a,b) = B(a,b) - beta(1-x,b,a)                             //
//     where B(a,b) is the complete beta function,                            //
//     the recurrence relations:                                              //
//           beta(x,a+1,b) = a/b beta(x,a,b+1) - x^a (1-x)^b / b              //
//           beta(x,a,b+1) = b/a beta(x,a+1,b) + x^a (1-x)^b / a,             //
//     and the interrelationship:                                             //
//           beta(x,a,b) = beta(x,a+1,b) + beta(x,a,b+1).                     //
//                                                                            //
//     If both a > 1 and b > 1, then                                          //
//        if x <= (a-1) / ( a+b-2), then                                      //
//           use the continued fraction expansion                             //
//        otherwise                                                           //
//           use the symmetry relation and use the continued fraction         //
//           expansion to evaluate beta(1-x,b,a).                             //
//                                                                            //
//     If a < 1 and b > 1, then                                               //
//        use the interrelationship equation together with the recurrence     //
//        relation to evaluate                                                //
//           beta(x,a,b) = [(a+b) beta(x,a+1,b) + x^a (1-x)^b]/a.             //
//                                                                            //
//     If a > 1 and b < 1, then                                               //
//        use the interrelationship equation together with the recurrence     //
//        relation to evaluate                                                //
//           beta(x,a,b) = [(a+b) beta(x,a,b+1) - x^a (1-x)^b]/b.             //
//                                                                            //
//     If a < 1 and b < 1, then                                               //
//        use the interrelationship equation to evaluate                      //
//           beta(x,a,b) = beta(x,a+1,b) + beta(x,a,b+1)                      //
//        in terms of beta functions which now have one shape parameter > 1.  //
//                                                                            //
//     If a == 1, then evaluate the integral explicitly,                      //
//           beta(x,a,b) = [1 - (1-x)^b]/b.                                   //
//                                                                            //
//     If b == 1, then evaluate the integral explicitly,                      //
//           beta(x,a,b) = x^a / a.                                           //
//                                                                            //
//                                                                            //
//  Arguments:                                                                //
//     long double x   Upper limit of the incomplete beta function integral,  //
//                     x must be in the closed interval [0,1].                //
//     long double a   Shape parameter of the incomplete beta function, a - 1 //
//                     is the exponent of the factor x in the integrand.      //
//     long double b   Shape parameter of the incomplete beta function, b - 1 //
//                     is the exponent of the factor (1-x) in the integrand.  //
//                                                                            //
//  Return Values:                                                            //
//     beta(x,a,b)                                                            //
//                                                                            //
//  Example:                                                                  //
//     long double a, b, beta, x;                                             //
//                                                                            //
//     beta = xIncomplete_Beta_Function(x, a, b);                             //
////////////////////////////////////////////////////////////////////////////////
long double xIncomplete_Beta_Function(long double x, long double a,
                                                               long double b) 
{
           /* Both shape parameters are strictly greater than 1. */

   if ( a > 1.0L && b > 1.0L )
   {
      if ( x <= (a - 1.0L) / ( a + b - 2.0L ) )
         return Beta_Continued_Fraction(x, a, b);
      else
         return xBeta_Function( (double) a, (double) b )
                                   - Beta_Continued_Fraction( 1.0L - x, b, a );
   }
  
             /* Both shape parameters are strictly less than 1. */

    if ( a < 1.0L && b < 1.0L )  
      return xIncomplete_Beta_Function(x, a + 1.0L, b) 
                                  + xIncomplete_Beta_Function(x, a, b + 1.0L); 
   
              /* One of the shape parameters exactly equals 1. */

   if ( a == 1.0L ) return ( 1.0L - powl(1.0L - x, b) ) / b;

   if ( b == 1.0L ) return powl(x, a) / a;

      /* Exactly one of the shape parameters is strictly less than 1. */

   if ( a < 1.0L )  
      return ( (a + b) * xIncomplete_Beta_Function(x, a + 1.0L, b)
                                       + powl(x, a) * powl(1.0L - x, b) ) / a;
 
                   /* The remaining condition is b < 1.0 */

   return ( (a + b) * xIncomplete_Beta_Function(x, a, b + 1.0L)
                                       - powl(x, a) * powl(1.0L - x, b) ) / b;
}


////////////////////////////////////////////////////////////////////////////////
// long double Beta_Continued_Fraction( long double x, long double a,         //
//                                                            long double b ) //
//                                                                            //
//  Description:                                                              //
//     The continued fraction expansion used to evaluate the incomplete beta  //
//     function is                                                            //
//        beta(x,a,b) = x^a * (1-x)^b / a * ( (1/1+)(d[1]/1+)(d[2]/1+)...)    //
//     where d[2m+1] = - (a+m)(a+b+m)x/((a+2m)(a+2m+1))                       //
//           d[2m] = m(b-m)x/((a+2m)(a+2m-1)).                                //
//                                                                            //
//     where a > 1, b > 1, and x <= (a-1)/(a+b-2).                            //
//                                                                            //
//  Arguments:                                                                //
//     long double x   Upper limit of the incomplete beta function integral,  //
//                     x must be in the closed interval [0,1].                //
//     long double a   Shape parameter of the incomplete beta function, a - 1 //
//                     is the exponent of the factor x in the integrand.      //
//     long double b   Shape parameter of the incomplete beta function, b - 1 //
//                     is the exponent of the factor (1-x) in the integrand.  //
//                                                                            //
//  Return Values:                                                            //
//     beta(x,a,b)                                                            //
//                                                                            //
//  Example:                                                                  //
//     long double a, b, beta, x;                                             //
//                                                                            //
//     beta = Beta_Continued_Fraction(x, a, b);                               //
////////////////////////////////////////////////////////////////////////////////
static long double Beta_Continued_Fraction( long double x, long double a,
                                                                 long double b)
{
   long double Am1 = 1.0L;
   long double A0 = 0.0L;
   long double Bm1 = 0.0L;
   long double B0 = 1.0L;
   long double e = 1.0L;
   long double Ap1 = A0 + e * Am1;
   long double Bp1 = B0 + e * Bm1;
   long double f_less = Ap1 / Bp1;
   long double f_greater = 0.0L;
   long double aj = a;
   long double am = a;
   static long double eps = 10.0L * LDBL_EPSILON;
   int j = 0;
   int m = 0;
   int k = 1;

   if ( x == 0.0L ) return 0.0L;
   
   while ( (2.0L * fabsl(f_greater - f_less) > eps * fabsl(f_greater + f_less)) ) {
      Am1 = A0;
      A0 = Ap1;
      Bm1 = B0;
      B0 = Bp1;
      am = a + m;
      e = - am * (am + b) * x / ( (aj + 1.0L) * aj );
      Ap1 = A0 + e * Am1;
      Bp1 = B0 + e * Bm1;
      k = (k + 1) & 3;
      if (k == 1) f_less = Ap1/Bp1;
      else if (k == 3) f_greater = Ap1/Bp1;
      if ( fabsl(Bp1) > 1.0L) {
         Am1 = A0 / Bp1;
         A0 = Ap1 / Bp1;
         Bm1 = B0 / Bp1;
         B0 = 1.0;
      } else {
         Am1 = A0;
         A0 = Ap1;
         Bm1 = B0;
         B0 = Bp1;
      }
      m++;
      j += 2;
      aj = a + j;
      e = m * ( b - m ) * x / ( ( aj - 1.0L) * aj  );
      Ap1 = A0 + e * Am1;
      Bp1 = B0 + e * Bm1;
      k = (k + 1) & 3;
      if (k == 1) f_less = Ap1/Bp1;
      else if (k == 3) f_greater = Ap1/Bp1;
   }
   return expl( a * logl(x) + b * logl(1.0L - x) + logl(Ap1 / Bp1) ) / a;
}
