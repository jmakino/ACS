// -*- C++ -*-                                                                 |
//-----------------------------------------------------------------------------+
//                                                                             |
// tmeta.h                                                                     |
//                                                                             |
// C++ code                                                                    |
//                                                                             |
// Copyright Walter Dehnen, 1996-2004                                          |
// e-mail:   walter.dehnen@astro.le.ac.uk                                      |
// address:  Department of Physics and Astronomy, University of Leicester      |
//           University Road, Leicester LE1 7RH, United Kingdom                |
//                                                                             |
//                                                                             |
//-----------------------------------------------------------------------------+
#ifndef included_tmeta_h
#define included_tmeta_h

#include <iostream>
#include <cmath>

namespace WD {
  //////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  // auxiliary classes needed in tupel.h                                      //
  //                                                                          //
  // in order to insulate the implementation details, we put them in a        //
  // separate namespace                                                       //
  //                                                                          //
  //////////////////////////////////////////////////////////////////////////////
  namespace meta {
    ////////////////////////////////////////////////////////////////////////////
    // times<K>(x) returns K*x;                                                 
    //    optimized at compile time for K=-2,-1,0,1,2                           
    //    template struct times__<> to implement template method times<> below. 
    ////////////////////////////////////////////////////////////////////////////
    template<int N, typename X>
    struct times__ {
      static X act(X const&x) {
	return N * x;
      }
    };
    template<typename X>
    struct times__<2,X> {
      static X act(X const&x) {
	return x+x;
      }
    };
    template<typename X>
    struct times__<1,X> {
      static X act(X const&x) {
	return x;
      }
    };
    template<typename X>
    struct times__<0,X> {
      static X act(X const&x) {
	return X(0);
      }
    };
    template<typename X>
    struct times__<-1,X> {
      static X act(X const&x) {
	return -x;
      }
    };
    template<typename X>
    struct times__<-2,X> {
      static X act(X const&x) {
	return -x-x;
      }
    };

    //    implement times<>() using struct times__<>
    template<int N, typename X> inline
    X times(X const&x) { 
      return times__<N,X>::act(x);
    }

    ////////////////////////////////////////////////////////////////////////////
    // template method square(x) returns x*x (better than macro)                
    ////////////////////////////////////////////////////////////////////////////
    template<typename X> inline
    X square(X const&x) {
      return x*x;
    }

    ////////////////////////////////////////////////////////////////////////////
    // abs<X>(x) to return |x|, optimized for float and double at compile       
    //      template struct abs__ to implement abs<>()                          
    ////////////////////////////////////////////////////////////////////////////
    template<typename X> struct abs__ {
      X act(X const&x) {
	return x < X(0) ? -x : x;
      }
    };
    template<> struct abs__<float> {
      float act(float const&x) {
	return fabs(x);
      }
    };
    template<> struct abs__<double> {
      double act(double const&x) {
	return fabs(x);
      }
    };
    
    //   implement abs<>(x) using struct abs__<>
    template<typename X> inline
    X abs(X const&x) {
      return abs__<X>::act(x);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    // min and max from abritrary type allowing for operators < and >           
    ////////////////////////////////////////////////////////////////////////////
    template<typename X> inline
    X min(X const&x,
	  X const&y) {
      return x<y? x : y;
    }

    template<typename X> inline
    X max(X const&x,
	  X const&y) {
      return x>y? x : y;
    }
    ////////////////////////////////////////////////////////////////////////////
    //                                                                        //
    // struct aux_tupel<N,X,I>                                                //
    //      its static methods will be used below to implement the member     //
    //      methods of class tupel<N,X>                                       //
    //                                                                        //
    ////////////////////////////////////////////////////////////////////////////
    template<int N, typename X, int I=0> 
    struct aux_tupel {
      typedef X element_type;

      // typedef next to be the aux_tupel for the next element
      typedef aux_tupel<N,element_type,I+1> next;

      // s_as():  assign array elements to scalar
      template<typename scalar_type>
      static void s_as(element_type     *a,
		       const scalar_type&x) {
	a[I]=x;
	next::s_as(a,x);
      }

      // s_ml():  multiply array elements with scalar
      template<typename scalar_type>
      static void s_ml(element_type     *a,
		       const scalar_type&x) {
	a[I]*=x;
	next::s_ml(a,x);
      }

      // v_as():  assign array elements to array elements
      template<typename scalar_type>
      static void v_as(element_type     *a,
		       const scalar_type*b) {
	a[I]=b[I];
	next::v_as(a,b);
      }

      // v_ad():  add array elements to array elements
      template<typename scalar_type>
      static void v_ad(element_type     *a,
		       const scalar_type*b) {
	a[I]+=b[I];
	next::v_ad(a,b);
      }

      // v_su():  subtract array elements from array elements
      template<typename scalar_type>
      static void v_su(element_type     *a,
		       const scalar_type*b) {
	a[I]-=b[I];
	next::v_su(a,b);
      }

      // v_ast():  assign array elements to x times another array's elements
      template<typename scalar_type>
      static void v_ast(element_type      *a,
			const element_type*b,
			scalar_type  const&x) {
	a[I]=x*b[I];
	next::v_ast(a,b,x);
      }

      // v_adt():  add to array elements x times another array's elements
      template<typename scalar_type>
      static void v_adt(element_type      *a,
			const element_type*b,
		        scalar_type  const&x) {
	a[I]+=x*b[I];
	next::v_adt(a,b,x);
      }

      // v_adit():  add to array elements i times another array's elements
      template<int K>
      static void v_adit(element_type      *a,
			 const element_type*b) {
	a[I]+=times<K>(b[I]);
	next:: template v_adit<K>(a,b,x);
      }

      // v_sut():  subtract from array elements x times another array's elements
      template<typename scalar_type> 
      static void v_sut (element_type      *a,
			 const element_type*b,
			 scalar_type  const&x) {
	a[I]-=x*b[I];
	next::v_sut(a,b,x);
      }

      // v_suit(): subtract from array elements i times another array's elements
      template<int K> 
      static void v_suit (element_type      *a,
			  const element_type*b) {
	a[I]-=times<K>(b[I]);
	next:: template v_suit<K>(a,b,x);
      }

      // v_neg():  negate array elements
      static void v_neg (element_type*a) {
	a[I]=-a[I];
	next::v_neg(a);
      }
      
      // v_nega():  set array elements to minus another array's elements
      template<typename scalar_type>
      static void v_nega(element_type     *a,
			 const scalar_type*b) {
	a[I]=-b[I];
	next::v_nega(a,b);
      }

      // v_sum():  set array elements to sum of two other array's elements
      template<typename scalar_type> 
      static void v_sum(element_type      *a,
			const element_type*b,
			const scalar_type *c) {
	a[I]=b[I]+c[I];
	next::v_sum(a,b,c);
      }
      
      // v_diff():  set array elements to difference between
      //            two other array's elements
      template<typename scalar_type> 
      static void v_dif (element_type      *a,
			 const element_type*b,
			 const scalar_type *c) {
	a[I]=b[I]-c[I];
	next::v_dif(a,b,c);
      }

      // s_eq():  compare array elements with a scalar for equality
      static bool s_eq(const element_type*a,
		       const element_type&b) { 
	return a[I]==b && next::s_eq(a,b);
      }

      // s_neq():  compare array elements with a scalar for inequality
      static bool s_neq(const element_type*a,
			const element_type&b) { 
	return a[I]!=b || next::s_neq(a,b);
      }

      // v_eq():  compare two array's elements for equality
      static bool v_eq(const element_type*a,
		       const element_type*b) { 
	return a[I]==b[I] && next::v_eq(a,b);
      }

      // v_neq():  compare two array's elements for equality
      static bool v_neq(const element_type*a,
			const element_type*b) { 
	return a[I]!=b[I] || next::v_neq(a,b);
      }

      // v_min(): return mininum of elements of an array
      static element_type v_min(const element_type*a) {
	return min( a[I], next::v_min(a) );
      }

      // v_max(): return maxinum of elements of an array
      static element_type v_max(const element_type*a) {
	return max( a[I], next::v_max(a) );
      }

      // v_uma(): update maximum: a[I] = max(a[i],b[i])
      static void v_uma (element_type      *a,
			 const element_type*b) {
	if(b[I]>a[I]) a[I] = b[I];
	next::v_uma(a,b);
      }

      // v_umi(): update minimum: a[I] = min(a[i],b[i])
      static void v_umi (element_type      *a,
			 const element_type*b) {
	if(b[I]<a[I]) a[I] = b[I];
	next::v_umi(a,b);
      }

      // methods to be used by non-members of tupel<>
      // v_norm(): add squares of elements of array
      static element_type v_norm(const element_type*a) {
	return a[I]*a[I] + next::v_norm(a);
      }

      // v_amax(): return maxinum of absolute value of elements of an array
      static element_type v_amax(const element_type*a) {
	return max( abs(a[I]), next::v_amax(a) );
      }

      // v_dot(): add products of elements of two arrays
      template<typename scalar_type>
      static element_type v_dot(const element_type*a,
				const scalar_type *b) {
	return a[I]*b[I] + next::v_dot(a,b);
      }
      
      // v_diq(): add square of differences between elements of two arrays
      template<typename scalar_type>
      static element_type v_diq(const element_type*a,
				const scalar_type *b) {
	return square(a[I]-b[I]) + next::v_diq(a,b);
      }

      // v_suq(): add square of sum of elements of two arrays
      template<typename scalar_type>
      static element_type v_suq(const element_type*a,
				const scalar_type *b) {
	return square(a[I]+b[I]) + next::v_suq(a,b);
      }

      // v_out(): formatted output of elements of array
      static void v_out(std::ostream      &o,
			const element_type*a) {
	o<<a[I]<<' ';
	next::v_out(o,a);
      }

      // v_in(): ascii input of elements of array
      static void v_in(std::istream&i,
		       element_type*a) {
	i>>a[I];
	next::v_in(i,a);
      }

    }; // class aux_tupel<N,X,I>

    ////////////////////////////////////////////////////////////////////////////
    //                                                                        //
    // struct aux_tupel<N,X,I=N>                                              //
    //    special case of I=N: last element                                   //
    //                                                                        //
    ////////////////////////////////////////////////////////////////////////////
    template<int I, typename X> struct aux_tupel<I,X,I> {
      typedef X element_type;

      // s_as():  assign array elements to scalar
      template<typename scalar_type>
      static void s_as(element_type     *a,
	const scalar_type&x) {
	a[I]=x;
      }

      // s_ml():  multiply array elements with scalar
      template<typename scalar_type>
      static void s_ml(element_type     *a,
		       const scalar_type&x) {
	a[I]*=x;
      }

      // v_as():  assign array elements to array elements
      template<typename scalar_type>
      static void v_as(element_type     *a,
		       const scalar_type*b) {
	a[I]=b[I];
      }

      // v_ad():  add array elements to array elements
      template<typename scalar_type>
      static void v_ad(element_type     *a,
		       const scalar_type*b) {
	a[I]+=b[I];
      }

      // v_su():  subtract array elements from array elements
      template<typename scalar_type>
      static void v_su(element_type     *a,
		       const scalar_type*b) {
	a[I]-=b[I];
      }

      // v_ast():  assign array elements to x times another array's elements
      template<typename scalar_type>
      static void v_ast(element_type      *a,
			const element_type*b,
			scalar_type  const&x) {
	a[I]=x*b[I];
      }

      // v_adt():  add to array elements x times another array's elements
      template<typename scalar_type>
      static void v_adt(element_type      *a,
			const element_type*b,
		        scalar_type  const&x) {
	a[I]+=x*b[I];
      }

      // v_adit():  add to array elements i times another array's elements
      template<int K>
      static void v_adit(element_type      *a,
			 const element_type*b) {
	a[I]+=times<K>(b[I]);
      }

      // v_sut():  subtract from array elements x times another array's elements
      template<typename scalar_type> 
      static void v_sut (element_type      *a,
			 const element_type*b,
			 scalar_type  const&x) {
	a[I]-=x*b[I];
      }

      // v_suit(): subtract from array elements i times another array's elements
      template<int K> 
      static void v_suit (element_type      *a,
			  const element_type*b) {
	a[I]-=times<K>(b[I]);
      }

      // v_neg():  negate array elements
      static void v_neg (element_type*a) {
	a[I]=-a[I];
      }
      
      // v_nega():  set array elements to minus another array's elements
      template<typename scalar_type>
      static void v_nega(element_type     *a,
			 const scalar_type*b) {
	a[I]=-b[I];
      }

      // v_sum():  set array elements to sum of two other array's elements
      template<typename scalar_type> 
      static void v_sum(element_type      *a,
			const element_type*b,
			const scalar_type *c) {
	a[I]=b[I]+c[I];
      }
      
      // v_diff():  set array elements to difference between
      //            two other array's elements
      template<typename scalar_type> 
      static void v_dif (element_type      *a,
			 const element_type*b,
			 const scalar_type *c) {
	a[I]=b[I]-c[I];
      }

      // s_eq():  compare array elements with a scalar for equality
      static bool s_eq(const element_type*a,
		       const element_type&b) { 
	return a[I]==b;
      }

      // s_neq():  compare array elements with a scalar for inequality
      static bool s_neq(const element_type*a,
			const element_type&b) { 
	return a[I]!=b;
      }

      // v_eq():  compare two array's elements for equality
      static bool v_eq(const element_type*a,
		       const element_type*b) { 
	return a[I]==b[I];
      }

      // v_neq():  compare two array's elements for equality
      static bool v_neq(const element_type*a,
			const element_type*b) { 
	return a[I]!=b[I];
      }

      // v_min(): return mininum of elements of an array
      static element_type v_min(const element_type*a) {
	return a[I];
      }

      // v_max(): return maxinum of elements of an array
      static element_type v_max(const element_type*a) {
	return a[I];
      }

      // v_uma(): update maximum: a[I] = max(a[i],b[i])
      static void v_uma (element_type      *a,
			 const element_type*b) {
	if(b[I]>a[I]) a[I] = b[I];
      }

      // v_umi(): update minimum: a[I] = min(a[i],b[i])
      static void v_umi (element_type      *a,
			 const element_type*b) {
	if(b[I]<a[I]) a[I] = b[I];
      }

      // methods to be used by non-members of tupel<>
      // v_amax(): return maxinum of absolute value of elements of an array
      static element_type v_amax(const element_type*a) {
	return abs(a[I]);
      }

      // v_norm(): add squares of elements of array
      static element_type v_norm(const element_type*a) {
	return a[I]*a[I];
      }

      // v_dot(): add products of elements of two arrays
      template<typename scalar_type>
      static element_type v_dot(const element_type*a,
				const scalar_type *b) {
	return a[I]*b[I];
      }
      
      // v_diq(): add square of differences between elements of two arrays
      template<typename scalar_type>
      static element_type v_diq(const element_type*a,
				const scalar_type *b) {
	return square(a[I]-b[I]);
      }


      // v_suq(): add square of sum of elements of two arrays
      template<typename scalar_type>
      static element_type v_suq(const element_type*a,
				const scalar_type *b) {
	return square(a[I]+b[I]);
      }

      // v_out(): formatted output of elements of array
      static void v_out(std::ostream      &o,
			const element_type*a) {
	o<<a[I];
      }

      // v_in(): ascii input of elements of array
      static void v_in(std::istream&i,
		       element_type*a) {
	i>>a[I];
      }

    }; // class aux_tupel<I,X,I>

  } // namespace meta
} // namespace WD
#endif // included_tmeta_h
