#pragma once

#include <cmath>
#include <initializer_list>

namespace ns {

	template<unsigned N, typename T = float>
	struct vec {
		T data[N] {};

		inline T& operator[] (unsigned i) {
			return data[i];
		}

		inline const T& operator[] (unsigned i) const {
			return data[i];
		}

		inline bool operator== (const vec &o) const {
			bool eq = true;
			for (unsigned i=0; i<N; ++i) {
				eq = eq && data[i] == o.data[i];
			}
			return eq;
		}

		inline bool operator!= (const vec &o) const {
			return !(*this == o);
		}

	#define DEFINE_OP(op)\
		inline vec& operator op##= (const vec &o) {\
			for (unsigned i=0; i<N; ++i) {\
				data[i] op##= o.data[i];\
			}\
			return *this;\
		}\
		inline vec operator op(const vec &o) const {\
			vec r = *this;\
			return r op##= o;\
		}

		DEFINE_OP(+)
		DEFINE_OP(-)
		DEFINE_OP(*)
		DEFINE_OP(/)
	#undef DEFINE_OP

		inline vec& operator*= (const T &c) {
			for (unsigned i=0; i<N; ++i) {
				data[i] *= c;
			}
			return *this;
		}

		inline vec operator* (const T &c) const {
			vec r = *this;
			return r *= c;
		}

		inline vec operator- () const {
			vec r = *this;
			for (unsigned i=0; i<N; ++i) {
				r.data[i] = -data[i];
			}
			return r;
		}

		template<typename T2>
		inline vec<N,T2> typecast() const {
			vec<N,T2> r;
			for (unsigned i=0; i<N; ++i) {
				r.data[i] = T2(data[i]);
			}
			return r;
		}
	};

	template<unsigned N, typename T>
	inline vec<N,T> operator* (const T &a, const vec<N,T> &v) {
		return v * a;
	}

	typedef vec<2> vec2;

	template<unsigned N, typename T>
	inline T dot(const vec<N,T> &a, const vec<N,T> &b) {
		T r = 0;
		for (unsigned i=0; i<N; ++i) {
			r += a[i]*b[i];
		}
		return r;
	}

	template<unsigned N, typename T>
	inline T length_sqr(const vec<N,T> &a) {
		return dot(a,a);
	}

	template<unsigned N, typename T>
	inline T length(const vec<N,T> &a) {
		return std::sqrt(length_sqr(a));
	}

	template<unsigned N, typename T>
	inline vec<N,T> normalized(const vec<N,T> &a) {
		return a * (T(1) / length(a));
	}

	template<typename T, typename U>
	inline T lerp(const T &a, const T &b, U t) {
		return (U(1)-t)*a + t*b;
	}

	template<unsigned N, typename T>
	inline vec<N,T> nlerp(const vec<N,T> &a, const vec<N,T> b, const T &t) {
		return normalized(lerp(a, b, t));
	}
}
