#pragma once
#ifndef COMMON_INC_APPLY_HPP_
#define COMMON_INC_APPLY_HPP_

#include <cstdlib>
#include <tuple>
#include <utility>

namespace common {

	template < typename TUPLE_TYPE, typename FUNCTOR_TYPE, std::size_t ... INDICES >
	void apply( TUPLE_TYPE&& tuple, FUNCTOR_TYPE&& functor, std::index_sequence< INDICES ... > )
	{
		using swallow = int[];
		(void) swallow {1, ( functor( std::get< INDICES >( std::forward< TUPLE_TYPE >( tuple ) ) ), void(), int {}) ...};
	}

	template < typename TUPLE_TYPE, typename FUNCTOR_TYPE >
	void apply( TUPLE_TYPE&& tuple, FUNCTOR_TYPE&& functor )
	{
		constexpr std::size_t SIZE = std::tuple_size< std::remove_reference_t< TUPLE_TYPE > >::value;
		apply( std::forward< TUPLE_TYPE >( tuple ), std::forward< FUNCTOR_TYPE >( functor ), std::make_index_sequence< SIZE > {});
	}

}

#endif /* COMMON_INC_APPLY_HPP_ */
