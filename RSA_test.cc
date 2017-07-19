#include "RSA.h"
#include <cassert>

#include <iostream>
#include <iomanip>
#include <string>
#include <climits>

using namespace std;

int main() {
	RSA::seed();
	// {
	// 	assert(mod_inv(1234567, 8910) == 7813);
	// 	assert(mod_pow(1234, 5678, 910) == 326);
	// }

	// // manual crypto test 
	// {
	// 	key_set keys = {3233, 17, 413};
	// 	public_pair pub = RSA::to_public(keys);
	// 	private_pair priv = RSA::to_private(keys);

	// 	assert(keys.n == 3233);
	// 	assert(keys.e == 17);
	// 	assert(keys.d == 413);
	// 	assert(pub.n == keys.n);
	// 	assert(pub.e == keys.e);
	// 	assert(priv.n == keys.n);
	// 	assert(priv.d == keys.d);

	// 	mpreal m = 65;
	// 	mpreal c = RSA::encrypt(m, pub);
	// 	mpreal decrypted_m = RSA::decrypt(c, priv);

	// 	assert(c == 2790);
	// 	assert(decrypted_m == m);

	// 	mpreal m2 = 3232;
	// 	mpreal c2 = RSA::encrypt(m2, pub);
	// 	mpreal d2 = RSA::decrypt(c2, priv);

	// 	assert(d2 == m2);
	// }
	// 
	// // misc 
	// {
	// 	mpfr::mpreal::set_default_prec(266);
	// 	mpreal p = random_prime(128);

	// 	assert(is_prime(p));

	// 	mpreal p_str = mpreal_to_str(p);
	// 	mpreal p_cstr = mpreal_to_str(p).c_str();

	// 	assert(p_str == p);
	// 	assert(p_cstr == p);

	// 	mpfr::mpreal::set_default_prec(522);
	// 	mpreal p_512 = random_prime(512);
	// 	mpreal p_512_str = mpreal_to_str(p_512); 
	// 	mpreal p_512_cstr = mpreal_to_str(p_512).c_str();

	// 	assert(is_prime(p_512));
	// 	assert(p_512_str == p_512);
	// 	assert(p_512_cstr == p_512);
	// }

	// // functions test
	// /*{
	// 	assert(bit_count(32000) == 15);
	// 	assert(bit_count(large_rand(128)) == 128);
	// 	assert(bit_count(large_rand(4096)) == 4096);
	// 	assert(is_prime(7));
	// 	assert(mod_pow(1243512345, 678678, 910) == 365);
	// 	assert(mod_inv(234895234, 12893) == 6499);
	// 	assert(is_prime(random_prime(4096)));
	// 	assert(gcd(1234512345, 678678) == 33);
	// 	assert(lcm(12345, 675) == 555525);
	// 	assert(mpfr::div(7, 2) == 3);
	// 	assert(mpfr::div(1234512345, 678) == 1820814);
	// }*/

	// // RSA::generate testing
	// {
	// 	key_set keys = RSA::generate(2048);
	// 	public_pair pub = RSA::to_public(keys);
	// 	private_pair priv = RSA::to_private(keys);

	// 	assert(bit_count(keys.n) <= 2049 && bit_count(keys.n) >= 2047);
	// 	assert(pub.n == keys.n);
	// 	assert(pub.e == keys.e);
	// 	assert(priv.n == pub.n);
	// 	assert(priv.d == keys.d);

	// // RSA::encrypt/decrypt testing
	// 	/*
	// 	mpreal message = 4912;
	// 	mpreal cipher = RSA::encrypt(message, pub);
	// 	mpreal cipher_manual = mod_pow(message, pub.e, pub.n);

	// 	assert(cipher == cipher_manual);

	// 	mpreal dtext = RSA::decrypt(cipher, priv);
	// 	mpreal manual_dtext = mod_pow(cipher, priv.d, priv.n);	

	// 	assert(dtext == manual_dtext);
	// 	assert(dtext == message);
	// 	*/
	// }

	// bit-incremental loop-based testing
	{
		key_set keys = RSA::generate(8);
		mpreal m = 123;
		auto c = RSA::encrypt(m, keys);
		auto d = RSA::decrypt(c, keys);
		assert(d == m);
    }

	return 0;
}
