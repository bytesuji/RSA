#ifndef RSA_H
#define RSA_H

#include <cassert>
#include <climits>
#include <random>

#include <mpreal.h>
#include <gmp.h>
#include <sys/random.h>

#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

using mpfr::mpreal;
using std::stringstream;
using std::setprecision;
using std::string;
using std::vector;

struct key_set {
	mpreal n;
	mpreal e;
	mpreal d;
};

typedef mpreal public_key; 
typedef mpreal private_key;

struct public_pair {
	public_key n;
	public_key e;
};

struct private_pair {
	public_key n;
	private_key d;
};

long system_rand() {
	long buf;
	int gen_bytes = getrandom(&buf, sizeof(long), GRND_RANDOM);
	return buf;
}

unsigned bit_count(const mpreal& n) {
	mpreal cnt = mpfr::floor(mpfr::log2(n)) + 1;
	return static_cast<unsigned>(cnt);
}

mpreal large_rand(unsigned bits) {
	mpfr::mpreal::set_default_prec(bits + 10); // 10 extra bits just in case
	mpreal n = mpfr::random(rand());
	// n <<= bits; 

	while (bit_count(n) < bits) {
		std::cout << bit_count(n) << '\n';
		n <<= 1;
	}

	n = mpfr::floor(n);
	return n;
}

string mpreal_to_str(const mpreal& n, unsigned prec = 4096) {
	stringstream ss;
	ss << std::setprecision(prec) << n;

	return ss.str();
}

bool is_prime(const mpreal& n, unsigned reps = 100) {
	mpz_t gmp_n; 
	mpz_init(gmp_n);

	string str_n = mpreal_to_str(n);
	const char* cc_n = str_n.c_str();
	mpz_set_str(gmp_n, cc_n, 10);

	int res = mpz_probab_prime_p(gmp_n, reps);
	return res > 0;
}

mpreal mod_pow(const mpreal& b, const mpreal& e, const mpreal& m) {
	mpz_t gmp_b;
	mpz_t gmp_e;
	mpz_t gmp_m;
	mpz_t gmp_result;
	mpz_init_set_str(gmp_b, mpreal_to_str(b).c_str(), 10);
	mpz_init_set_str(gmp_e, mpreal_to_str(e).c_str(), 10);
	mpz_init_set_str(gmp_m, mpreal_to_str(m).c_str(), 10);
	mpz_init(gmp_result);

	mpz_powm(gmp_result, gmp_b, gmp_e, gmp_m);
	return static_cast<mpreal>(gmp_result);	
}

mpreal mod_inv(const mpreal& a, const mpreal& b) {
	mpz_t gmp_a;
	mpz_t gmp_b;
	mpz_t gmp_result;
	mpz_init_set_str(gmp_a, mpreal_to_str(a).c_str(), 10);
	mpz_init_set_str(gmp_b, mpreal_to_str(b).c_str(), 10);
	mpz_init(gmp_result);

	mpz_invert(gmp_result, gmp_a, gmp_b);
	return static_cast<mpreal>(gmp_result);	
}

mpreal random_prime(unsigned bits) {
	while (true) {
		mpreal p = large_rand(bits);
		if (is_prime(p)) 
			return p;
    }
}

mpreal gcd(mpreal a, mpreal b) { // not by reference because it would require a copy anyway
	while (b != 0) {
		mpreal t = b;
		b = mpfr::mod(a, b);
		a = t;
    }

	return a;
}

mpreal lcm(mpreal a, mpreal b) {
	return mpfr::abs(a * b) / gcd(a, b);
}

namespace mpfr {
	mpreal div(const mpreal& a, const mpreal& b) {
		return mpfr::floor(a / b);
	}
}

namespace RSA {
	key_set generate(unsigned bits) {
		mpfr::mpreal::set_default_prec(bits + 10);
		mpreal e = 65537;
		mpreal p, q, lambda;

		do {
			p = random_prime(bits / 2);
			q = random_prime(bits / 2);
			lambda = lcm(p - 1, q - 1);
        } while (gcd(e, lambda) != 1 || 
			mpfr::abs(p - q) >> (bits / 2 - 100) == 0);
		
		return key_set {
			p * q, 				// pub key 1	(n)
			e,					// pub key 2 	(e) 
			mod_inv(e, lambda) 	// private key	(d)
        };
    }

	mpreal encrypt(const mpreal& message, const public_pair& kp) {
		mpreal ctext = mod_pow(message, kp.e, kp.n);
		return ctext;
    }

	mpreal encrypt(const mpreal& message, const key_set& kp) {
		mpreal ctext = mod_pow(message, kp.e, kp.n);
		return ctext;
    }

	mpreal decrypt(const mpreal& ctext, const private_pair& kp) {
		mpreal message = mod_pow(ctext, kp.d, kp.n);
		return message;
    }

	mpreal decrypt(const mpreal& ctext, const key_set& kp) {
		mpreal message = mod_pow(ctext, kp.d, kp.n);
		return message;
    }

	public_pair to_public(const key_set& ks) {
		return public_pair {
			ks.n,
			ks.e
        };
    }

	private_pair to_private(const key_set& ks) {
		return private_pair {
			ks.n,
			ks.d
        };
    }

	void seed() {
		srand(system_rand());
    }
}

#endif // RSA_H
