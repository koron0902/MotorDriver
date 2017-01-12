#include <region.hpp>
namespace common {
template class region<int32_t> ;
template class region<fix32> ;

namespace regions {
const region<fix32> one(fix32::CreateInt(-1), fix32::CreateInt(1));

const region<fix32> percent(fix32::CreateInt(0), fix32::CreateInt(1));
}
}
