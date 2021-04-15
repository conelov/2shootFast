//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_DEFINE_HPP
#define INC_2SHOOT_DEFINE_HPP

#ifdef TO_LITERAL_STRING
#error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

namespace draw{
constexpr auto paintersCount = 2;
}

#endif // INC_2SHOOT_DEFINE_HPP
