/*!
 * \file
 * \brief Class tools::Interleaver_core_golden.
 */
#ifndef INTERLEAVER_CORE_GOLDEN_HPP
#define INTERLEAVER_CORE_GOLDEN_HPP

#include <cstdint>
#include <random>

#include "Tools/Interleaver/Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_golden : public Interleaver_core<T>
{
private:
	std::mt19937                           gen;
	std::uniform_real_distribution<double> dist;

public:
	Interleaver_core_golden(const int size, const int seed = 0, const bool uniform = false, const int n_frames = 1);
	virtual ~Interleaver_core_golden() = default;

	virtual Interleaver_core_golden<T>* clone() const;

	virtual void set_seed(const int seed);

protected:
	void gen_lut(T *lut, const int frame_id);
};
}
}

#endif  /* INTERLEAVER_CORE_GOLDEN_HPP */
