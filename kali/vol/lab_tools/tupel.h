// -*- C++ -*-                                                                 |
//-----------------------------------------------------------------------------+
//                                                                             |
// tupel.h                                                                     |
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
//                                                                             |
//                                                                             |
//-----------------------------------------------------------------------------+
#ifndef included_tupel_h
#define included_tupel_h

#include "tmeta.h"

namespace WD {

  //////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  // class tupel<int N,typename X>                                            //
  //                                                                          //
  // is essentially a convenient wrapper around X[N];                         //
  //                                                                          //
  // implementing as member and non-members many operators between tupels as  //
  // well as formated I/O etc.                                                //
  //////////////////////////////////////////////////////////////////////////////
  template<int N, typename X>
  class tupel {

  public:
    // 0   static members and types
    typedef X element_type;
    static const int NDAT = N;
    static int size() { return N; }

  private:
    //    data: array of N elements
    //    essentially, tupel<N,X> is just a (very efficient) wrapper of this
    element_type a[N];

    //    typedef to meta::aux_tupel<N-1,X> used to implement members
    typedef meta::aux_tupel<N-1,X> Meta;

  public:
    // 1   constructors
    // 1.0 construction from nothing: do not initialize
    tupel() {}
    // 1.1 construction from single argument (scalar, array, or tupel)
    tupel(element_type const&x) { Meta::s_as(a,x); }
    tupel(const element_type*x) { Meta::v_as(a,x); }
    tupel(tupel        const&v) { Meta::v_as(a,v.a); }
#ifdef TUPEL_CROSS_TYPE
    template<typename scalar_type>
    tupel(tupel<N,scalar_type> const&v) {
      Meta::v_as(a,static_cast<const scalar_type*>(v));
    }
#endif
    // 1.2 construction from list of elements (up to 10)
    tupel(element_type const&x0,
	  element_type const&x1) {
      a[0]=x0;
      a[1]=x1;
    }
    tupel(element_type const&x0,
	  element_type const&x1,
	  element_type const&x2) {
      a[0]=x0;
      a[1]=x1;
      a[2]=x2;
    }
    tupel(element_type const&x0,
	  element_type const&x1,
	  element_type const&x2,
	  element_type const&x3) {
      a[0]=x0;
      a[1]=x1;
      a[2]=x2;
      a[3]=x3;
    }
    tupel(element_type const&x0,
	  element_type const&x1,
	  element_type const&x2,
	  element_type const&x3,
	  element_type const&x4) {
      a[0]=x0;
      a[1]=x1;
      a[2]=x2;
      a[3]=x3;
      a[4]=x4;
    }
    tupel(element_type const&x0,
	  element_type const&x1,
	  element_type const&x2,
	  element_type const&x3,
	  element_type const&x4,
	  element_type const&x5) { 
      a[0]=x0;
      a[1]=x1;
      a[2]=x2;
      a[3]=x3;
      a[4]=x4;
      a[5]=x5;
    }
    tupel(element_type const&x0,
	  element_type const&x1,
	  element_type const&x2,
	  element_type const&x3,
	  element_type const&x4,
	  element_type const&x5,
	  element_type const&x6) {
      a[0]=x0;
      a[1]=x1;
      a[2]=x2;
      a[3]=x3;
      a[4]=x4;
      a[5]=x5;
      a[6]=x6;
    }
    tupel(element_type const&x0,
	  element_type const&x1,
	  element_type const&x2,
	  element_type const&x3,
	  element_type const&x4,
	  element_type const&x5,
	  element_type const&x6,
	  element_type const&x7) {
      a[0]=x0;
      a[1]=x1;
      a[2]=x2;
      a[3]=x3;
      a[4]=x4;
      a[5]=x5;
      a[6]=x6;
      a[7]=x7;
    }
    tupel(element_type const&x0,
	  element_type const&x1,
	  element_type const&x2,
	  element_type const&x3,
	  element_type const&x4,
	  element_type const&x5,
	  element_type const&x6,
	  element_type const&x7,
	  element_type const&x8) {
      a[0]=x0;
      a[1]=x1;
      a[2]=x2;
      a[3]=x3;
      a[4]=x4;
      a[5]=x5;
      a[6]=x6;
      a[7]=x7;
      a[8]=x8;
    }
    tupel(element_type const&x0,
	  element_type const&x1,
	  element_type const&x2,
	  element_type const&x3,
	  element_type const&x4,
	  element_type const&x5,
	  element_type const&x6,
	  element_type const&x7,
	  element_type const&x8,
	  element_type const&x9) {
      a[0]=x0;
      a[1]=x1;
      a[2]=x2;
      a[3]=x3;
      a[4]=x4;
      a[5]=x5;
      a[6]=x6;
      a[7]=x7;
      a[8]=x8;
      a[9]=x9;
    } 

    // 2   element access
    element_type      &operator[]  (int i)       { return a[i]; }
    element_type const&operator[]  (int i) const { return a[i]; }

    // 3   unitary operators
    // 3.1 negation and unatary minus
    tupel&negate() { Meta::v_neg(a); return*this; }
    tupel operator-() const {
      tupel x;
      Meta::v_nega(x.a,a);
      return x;
    }
    // 3.2 minimum and maximum element
    element_type min() const { return Meta::v_min(a); }
    element_type max() const { return Meta::v_max(a); }
    // 3.3 type conversion to pointer to element_type
    operator element_type      * ()       { return a; }
    operator const element_type* () const { return a; }

    // 4   binary operators with scalar
    // 4.1 with scalar of element_type
    tupel&operator=  (element_type const&x) {
      Meta::s_as(a,x);
      return*this;
    }
    tupel&operator*= (element_type const&x) {
      Meta::s_ml(a,x);
      return*this;
    }
    tupel operator*  (element_type const&x) const {
      tupel y;
      Meta::v_ast(y.a,a,x);
      return y;
    }
    tupel&operator/= (element_type const&x)       { return operator*=(X(1)/x); }
    tupel operator/  (element_type const&x) const { return operator*(X(1)/x); }
    bool  operator== (element_type const&x) const { return Meta::s_eq(a,x); }
    bool  operator!= (element_type const&x) const { return Meta::s_neq(a,x); }
#ifdef TUPEL_CROSS_TYPE
    // 4.2 with scalar of type different from element_type
    template<typename scalar_type>
    tupel&operator= (scalar_type const&x) {
      Meta::s_as(a,x);
      return*this;
    }
    template<typename scalar_type>
    tupel&operator*=(scalar_type const&x) {
      Meta::s_ml(a,x);
      return*this;
    }
    template<typename scalar_type>
    tupel operator* (scalar_type const&x) const {
      tupel y;
      Meta::v_ast(y.a,a,x);
      return y;
    }
    template<typename scalar_type>
    tupel&operator/=(scalar_type const&x) { return operator*=(X(1)/x); }
    template<typename scalar_type>
    tupel operator/ (scalar_type const&x) const { return operator*(X(1)/x); }
#endif

    // 5   binary operators with tupel
    // 5.1 with tupel<N,element_type>
    tupel& operator=  (tupel const&v) {
      Meta::v_as(a,v.a);
      return*this;
    }
    tupel& operator+= (tupel const&v) {
      Meta::v_ad(a,v.a);
      return*this;
    }
    tupel& operator-= (tupel const&v) {
      Meta::v_su(a,v.a);
      return*this;
    }
    bool   operator== (tupel const&v) const { return Meta::v_eq(a,v.a); }
    bool   operator!= (tupel const&v) const { return Meta::v_neq(a,v.a); }
    tupel  operator+  (tupel const&v) const {
      tupel y; 
      Meta::v_sum(y.a,a,v.a);
      return y;
    }
    tupel  operator-  (tupel const&v) const {
      tupel y; 
      Meta::v_dif(y.a,a,v.a);
      return y;
    }
    //     add or subtract another tupel integer times
    template<int> tupel& add_int_times(tupel const&v) {
      Meta:: template v_adit<K>(a,v.a);
      return*this;
    }
    template<int> tupel& sub_int_times(tupel const&v) {
      Meta:: template v_suit<K>(a,v.a);
      return*this;
    }
    //     update the minimum/maximum in each element: a[i] = max(a[i],b[i])
    tupel& update_max (tupel const&v) {
      Meta::v_uma(a,v.a);
      return*this;
    }
    tupel& update_min (tupel const&v) {
      Meta::v_umi(a,v.a);
      return*this;
    }
#ifdef TUPEL_CROSS_TYPE
    // 5.2 with tupel<N,scalar_type>
    template<typename scalar_type>
    tupel& operator=  (tupel<N,scalar_type> const&v) {
      Meta::v_as(a,v.a);
      return*this;
    }
    template<typename scalar_type>
    tupel& operator+= (tupel<N,scalar_type> const&v) {
      Meta::v_ad(a,v.a);
      return*this;
    }
    template<typename scalar_type>
    tupel& operator-= (tupel<N,scalar_type> const&v) {
      Meta::v_su(a,v.a);
      return*this;
    }
    template<typename scalar_type>
    tupel  operator+  (tupel<N,scalar_type> const&v) const {
      tupel y; 
      Meta::v_sum(y.a,a,v.a);
      return y;
    }
    template<typename scalar_type>
    tupel  operator-  (tupel<N,scalar_type> const&v) const {
      tupel y; 
      Meta::v_dif(y.a,a,v.a);
      return y;
    }
    //     add or subtract another tupel integer times
    template<int, typename scalar_type>
    tupel& add_int_times(tupel<N,scalar_type> const&v) {
      Meta:: template v_adit<K>(a,v.a);
      return*this;
    }
    template<int, typename scalar_type>
    tupel& sub_int_times(tupel<N,scalar_type> const&v) {
      Meta:: template v_suit<K>(a,v.a);
      return*this;
    }
#endif

    // 6   miscellaneous
    // 6.1 copy from array (for security, operator= is not provided)
    tupel&copy(const element_type*x) {
      Meta::v_as(a,x);
      return*this;
    }
#ifdef TUPEL_CROSS_TYPE
    template<typename scalar_type>
    tupel&copy(const scalar_type*x) {
      Meta::v_as(a,x);
      return*this;
    }
#endif
    // 6.2 assign, add to, or subtract: real times another tupel
    template<typename scalar_type>
    tupel&ass_times(tupel const&v, scalar_type const&x) {
      Meta::v_ast(a,v.a,x);
      return*this;
    }
    template<typename scalar_type>
    tupel&add_times(tupel const&v, scalar_type const&x) {
      Meta::v_adt(a,v.a,x);
      return*this;
    }
    template<typename scalar_type>
    tupel&sub_times(tupel const&v, scalar_type const&x) {
      Meta::v_sut(a,v.a,x);
      return*this;
    }
    
  }; // class tupel<N,X>

  //////////////////////////////////////////////////////////////////////////////
  // 7  non-member methods that use tupel<>s                                  //
  // 7.1  formatted I/O                                                       //
  //////////////////////////////////////////////////////////////////////////////
  template<int N, typename X> inline
  std::ostream&operator<< (std::ostream&s,tupel<N,X> const&v) {
    meta::aux_tupel<N-1,X>::v_out(s,static_cast<const X*>(v));
    return s;
  }
  template<int N, typename X> inline
  std::istream&operator>> (std::istream&s,tupel<N,X>&v) {
    meta::aux_tupel<N-1,X>::v_in(s,static_cast<X*>(v));
    return s;
  }

  // 7.2 norm and absolute value: norm == sum_i x[i]*x[i]
  template<int N, typename X> inline
  X norm(tupel<N,X> const&v) {
    return meta::aux_tupel<N-1,X>::v_norm(static_cast<const X*>(v));
  }
  template<int N, typename X> inline
  X abs (tupel<N,X> const&) {
    return sqrt(norm(v));
  }

  // 7.3 maximum(abs(element))
  template<int N, typename X> inline
  X maxnorm(tupel<N,X> const&v) {
    return meta::aux_tupel<N-1,X>::v_amax(static_cast<const X*>(v));
  }

  // 7.4 binary operator*:  scalar * tupel
  template<int N, typename X> inline
  tupel<N,X> operator*(X const&x, tupel<N,X> const&v) {
    return v*x;
  }
#ifdef TUPEL_CROSS_TYPE
  template<int N, typename X, typename Y> inline
  tupel<N,X> operator*(Y const&x, tupel<N,X> const&v) {
    return v*x;
  }
#endif

  // 7.5 distance, squared distance, and squared sum 
  template<int N, typename X> inline
  X dist_sq (tupel<N,X> const&v, tupel<N,X> const&w) {
    return meta::aux_tupel<N-1,X>::v_diq(static_cast<const X*>(v),
					 static_cast<const X*>(w));
  }
  template<int N, typename X> inline
  X dist    (tupel<N,X> const&v, tupel<N,X> const&w) {
    return sqrt(dist_sq(v,w));
  }
  template<int N, typename X> inline
  X sum_sq  (tupel<N,X> const&v, tupel<N,X> const&w) {
    return meta::aux_tupel<N-1,X>::v_suq(static_cast<const X*>(v),
					 static_cast<const X*>(w));
  }
#ifdef TUPEL_CROSS_TYPE
  template<int N, typename X, typename Y> inline
  X dist_sq (tupel<N,X> const&v, tupel<N,Y> const&w) {
    return meta::aux_tupel<N-1,X>::v_diq(static_cast<const X*>(v),
					 static_cast<const Y*>(w));
  }
  template<int N, typename X, typename Y> inline
  X dist    (tupel<N,X> const&v, tupel<N,Y> const&w) {
    return sqrt(dist_sq(v,w));
  }
  template<int N, typename X, typename Y> inline
  X sum_sq  (tupel<N,X> const&v, tupel<N,Y> const&w) {
    return meta::aux_tupel<N-1,X>::v_suq(static_cast<const X*>(v),
					 static_cast<const Y*>(w));
  }
#endif

  // 7.6 vector dot product
  template<int N, typename X> inline
  X operator* (tupel<N,X> const&v, tupel<N,X> const&w) {
    return meta::aux_tupel<N-1,X>::v_dot(static_cast<const X*>(v),
					 static_cast<const X*>(w));
  }
#ifdef TUPEL_CROSS_TYPE
  template<int N, typename X, typename Y> inline
  X operator* (tupel<N,X> const&v, tupel<N,Y> const&w) {
    return meta::aux_tupel<N-1,X>::v_dot(static_cast<const X*>(v),
					 static_cast<const Y*>(w));
  }
#endif

  // 7.7 vector cross product in 2D and 3D, coded as operator ^
  //     in 2D, we just return the scalar  a[0]*b[1]-a[1]*b[0],
  //     so that X^V gives Lz and (Lx,Ly,Lz) in 2D and 3D, respectively
  template<typename X> inline
  X          operator^ (tupel<2,X> const&v, tupel<2,X> const&w) {
    return v[0]*w[1] - v[1]*w[0];
  }
  template<typename X> inline
  tupel<3,X> operator^ (tupel<3,X> const&v, tupel<3,X> const&w) {
    return tupel<3,X>(v[1]*w[2] - v[2]*w[1],
		      v[2]*w[0] - v[0]*w[2],
		      v[0]*w[1] - v[1]*w[0]);
  }
#ifdef TUPEL_CROSS_TYPE
  template<typename X, typename Y> inline
  X          operator^ (tupel<2,X> const&v, tupel<2,Y> const&w) {
    return v[0]*w[1] - v[1]*w[0];
  }
  template<typename X, typename Y> inline
  tupel<3,X> operator^ (tupel<3,X> const&v, tupel<3,Y> const&w) {
    return tupel<3,X>(v[1]*w[2] - v[2]*w[1],
		      v[2]*w[0] - v[0]*w[2],
		      v[0]*w[1] - v[1]*w[0]);
  }
#endif
  //////////////////////////////////////////////////////////////////////////////
} // namespace WD
////////////////////////////////////////////////////////////////////////////////
#endif // included_tupel_h
