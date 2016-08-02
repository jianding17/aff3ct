#include <cassert>
#include <cmath>
#include <complex>
#include <limits>

#include "Modulator_PAM.hpp"

/*
 * Constructor / Destructor
 */
template <typename B, typename R, proto_max<R> MAX>
Modulator_PAM<B,R,MAX>
::Modulator_PAM(const int bits_per_symbol, const R sigma)
: bits_per_symbol(bits_per_symbol),
  nbr_symbols    (1 << bits_per_symbol),
  sigma          (sigma),
  sqrt_es        (std::sqrt((this->nbr_symbols * this->nbr_symbols - 1.0) / 3.0)),
  constellation  (nbr_symbols)
{
	mipp::vector<B> bits(this->bits_per_symbol);

	for (auto j = 0; j < this->nbr_symbols; j++)
	{
		for (auto l = 0; l < this->bits_per_symbol; l++)
			bits[l] = (j >> l) & 1;

		this->constellation[j] = this->bits_to_symbol(&bits[0]);
	}
}

template <typename B, typename R, proto_max<R> MAX>
Modulator_PAM<B,R,MAX>
::~Modulator_PAM()
{
}

/*
 * int get_buffer_size(const int N)
 * N = number of input bits
 * returns number of output symbols
 */
template <typename B, typename R, proto_max<R> MAX>
int Modulator_PAM<B,R,MAX>
::get_buffer_size(const int N)
{
	return std::ceil((float)N / (float)this->bits_per_symbol);
}

/*
 * Mapping function
 */
template <typename B, typename R, proto_max<R> MAX>
R Modulator_PAM<B,R,MAX>
::bits_to_symbol(const B* bits) const
 {
	auto bps = this->bits_per_symbol;

	auto symbol = (R)1.0 - ((R)bits[0] + (R)bits[0]);
	for (auto j = 1; j < bps; j++)
		symbol = (1.0 - ((R)bits[j] + (R)bits[j])) * ((1 << j) - symbol);

	return symbol / this->sqrt_es;
 }

/*
 * Modulator
 */
template <typename B,typename R, typename Q, proto_max<Q> MAX>
void Modulator_PAM<B,R,Q,MAX>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	auto size_in  = (int)X_N1.size();
	auto size_out = (int)X_N2.size();
	auto bps      = this->bits_per_symbol;

	auto main_loop_size = size_in / bps;
	for (auto i = 0; i < main_loop_size; i++)
	{
		// compute the symbol "on the fly"
		// auto symbol = bits_to_symbol(&X_N1[i*bps]);

		// determine the symbol with a lookup table
		unsigned idx = 0;
		for (auto j = 0; j < bps; j++)
			idx += (1 << j) * X_N1[i * bps +j];
		auto symbol = this->constellation[idx];

		X_N2[i] = symbol;
	}

	// last elements if "size_in" is not a multiple of the number of bits per symbol
	if (main_loop_size * bps < size_in)
	{
		unsigned idx = 0;
		for (auto j = 0; j < size_in - (main_loop_size * bps); j++)
			idx += (1 << j) * X_N1[main_loop_size * bps +j];
		auto symbol = this->constellation[idx];

		X_N2[size_out -1] = symbol;
	}
}

/*
 * Demodulator
 */
template <typename B,typename R, typename Q, proto_max<Q> MAX>
void Modulator_PAM<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2)
{
	auto size   = (int)Y_N2.size();
	auto sigma2 = this->sigma * this->sigma;

	for (auto n = 0; n < size; n++)// Boucle sur les LLRs
	{
		auto L0 = -std::numeric_limits<R>::infinity();
		auto L1 = -std::numeric_limits<R>::infinity();
		auto b  = n % this->bits_per_symbol; // position du bit
		auto k  = n / this->bits_per_symbol; // position du symbole

		for (auto j = 0; j < this->nbr_symbols; j++)
			if ((j & (1 << b)) == 0)
				L0 = MAX(L0, -(Y_N1[k] - this->constellation[j]) * (Y_N1[k] - this->constellation[j]) / sigma2);
			else
				L1 = MAX(L1, -(Y_N1[k] - this->constellation[j]) * (Y_N1[k] - this->constellation[j]) / sigma2);

		Y_N2[n] = L0 - L1;
	}
}