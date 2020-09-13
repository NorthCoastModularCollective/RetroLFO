/*
The following is a modification of PeterSchregle fixed implemention on code project... adapted for use in synthesis and re-implemented so I could learn more about it
The meta-programming is pretty sweet and was new to me. Thanks Peter!
*/
/*******************************************************************************/
/*                                                                             */
/*  Copyright (c) 2007-2009: Peter Schregle,                                   */
/*  All rights reserved.                                                       */
/*                                                                             */
/*  This file is part of the Fixed Point Math Library.                         */
/*                                                                             */
/*  Redistribution of the Fixed Point Math Library and use in source and       */
/*  binary forms, with or without modification, are permitted provided that    */
/*  the following conditions are met:                                          */
/*  1. Redistributions of source code must retain the above copyright notice,  */
/*     this list of conditions and the following disclaimer.                   */
/*  2. Redistributions in binary form must reproduce the above copyright       */
/*     notice, this list of conditions and the following disclaimer in the     */
/*     documentation and/or other materials provided with the distribution.    */
/*  3. Neither the name of Peter Schregle nor the names of other contributors  */
/*     may be used to endorse or promote products derived from this software   */
/*     without specific prior written permission.                              */
/*                                                                             */
/*  THIS SOFTWARE IS PROVIDED BY PETER SCHREGLE AND CONTRIBUTORS 'AS IS' AND   */
/*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE      */
/*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE */
/*  ARE DISCLAIMED. IN NO EVENT SHALL PETER SCHREGLE OR CONTRIBUTORS BE LIABLE */
/*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL */
/*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS    */
/*  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)      */
/*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,        */
/*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN   */
/*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE            */
/*  POSSIBILITY OF SUCH DAMAGE.                                                */
/*                                                                             */
/*******************************************************************************/
#ifndef FIXED_H // include guard
#define FIXED_H
#include <math.h>

namespace fixed_point {

    template<int P> struct power2 
    {
        static const long long value = 2 * power2<P-1>::value;
    };

    template <> struct power2<0> 
    {
        static const long long value = 1;
    };

    template<typename T, typename U=void>
    struct promote_type
    {};

    template< typename U>
    /// Promote signed char to signed short.
    struct promote_type<signed char, U>
    {
        typedef signed short type;
    };

    template<
        /// Make gcc happy.
        typename U>
    /// Promote unsigned char to unsigned short.
    struct promote_type<unsigned char, U>
    {
        typedef unsigned short type;
    };

    template<
        /// Make gcc happy.
        typename U>
    /// Promote signed short to signed int.
    struct promote_type<signed short, U>
    {
        typedef signed int type;
    };

    template<
        /// Make gcc happy.
        typename U>
    /// Promote unsigned short to unsigned int.
    struct promote_type<unsigned short, U> 
    {
        typedef unsigned int type;
    };

    template<
        /// Make gcc happy.
        typename U>
    /// Promote signed int to signed long long.
    struct promote_type<signed int, U> 
    {
        typedef signed long long type;
    };

    template<
        /// Make gcc happy.
        typename U>
    /// Promote unsigned int to unsigned long long.
    struct promote_type<unsigned int, U> 
    {
        typedef unsigned long long type;
    };

    template <typename T>
    struct is_signed 
    {
        static const bool value = T(-1)< T(0);
    };
    
    template <typename B, unsigned char IntegerPrecision, unsigned char FractionalPrecision>
    struct fixed {
        // static_assert(std::is_integral<B>::value);
        static_assert(IntegerPrecision+FractionalPrecision+is_signed<B>::value == 8*sizeof(B));
        private:
            B _number;
            
        public:
            template<typename T> 
            fixed(T _value);

            fixed(){};

            fixed(float _value):_number((B)(_value*power2<FractionalPrecision>::value)){};
            fixed(double _value):_number((B)(_value*power2<FractionalPrecision>::value)){};
    
            fixed(int _value): _number((B)_value << FractionalPrecision){};

            /// Copy constructor.
            fixed(fixed const& rhs): _number(rhs._number){ }

            template<
                typename B2,
                /// The other integer part bit count.
                unsigned char I2,
                /// The other fractional part bit count.
                unsigned char F2>
            /// Converting copy constructor.
            fixed(
                /// The right hand side.
                fixed<B2, I2, F2> const& rhs)
                : _number(rhs.get())
            { 
                // if (IntegerPrecision-I2 > 0)
                //     _number >>= IntegerPrecision-I2;
                // if (I2-IntegerPrecision > 0)
                //     _number <<= I2-IntegerPrecision;
            }

            // /// Copy assignment operator.
            // fixed & operator =(
            //     /// The right hand side.
            //     fixed const& rhs)
            // {
            //     fixed temp(rhs);
            //     swap(temp);
            //     return *this;
            // }

            // template<
            //     /// The other integer part bit count.
            //     unsigned char I2,
            //     /// The other fractional part bit count.
            //     unsigned char F2>
            // /// Converting copy assignment operator.
            // fixed<B,IntegerPrecision,FractionalPrecision> & operator =(
            //     /// The right hand side.
            //     fixed<B, I2, F2> const& rhs)
            // {
            //     fixed<B,IntegerPrecision,FractionalPrecision> temp(rhs);
            //     swap(temp);
            //     return *this;
            // }

            // /// Exchanges the elements of two fixed_point objects.
            // void swap(
            //     /// The right hand side.
            //     fixed & rhs)
            // {
            //     std::swap(_number, rhs._number);
            // }

            operator float() const {
                return (float) _number / power2<FractionalPrecision>::value;
            }

            operator double() const {
                return (double) _number / power2<FractionalPrecision>::value;
            }

            operator int() const
            {
                // static_assert(std::is_integral<T>::value);
                return _number >> FractionalPrecision;
            }

            bool operator ==(fixed rhs) const {
                return _number == rhs._number;
            }
            bool operator !=(fixed rhs) const {
                return _number != rhs._number;
            }
            bool operator >(fixed rhs) const {
                return _number > rhs._number;
            }
            bool operator >=(fixed rhs) const {
                return _number >= rhs._number;
            }
            bool operator <(fixed rhs) const {
                return _number < rhs._number;
            }
            bool operator <=(fixed rhs) const {
                return _number <= rhs._number;
            }

            fixed operator -(){
                fixed inverse;
                inverse._number = -_number;
                return inverse;
            }

            fixed operator +=(fixed summand){
                _number += summand._number;
                return *this;
            }

            fixed operator -=(fixed summand){
                _number -= summand._number;
                return *this;
            }

            fixed operator +(fixed summand) const{
                fixed sum;
                sum._number = summand._number+_number;
                return sum;
            }

            fixed operator -(fixed summand) const{
                return (*this)+(-summand);
            }

            fixed operator *=(fixed multiplier){
                _number = (static_cast<typename promote_type<B>::type>(_number)*multiplier._number)>>FractionalPrecision;
                return *this;
            }

            fixed operator /=(fixed divisor){
                _number = (static_cast<typename promote_type<B>::type>(_number)<<FractionalPrecision)/divisor._number;
                return *this;
            }

            fixed operator *(fixed multiplier) const{
                fixed result;
                result._number = _number;
                result*= multiplier;
                return result;
            }

            fixed operator /(fixed multiplier) const{
                fixed result;
                result._number = _number;
                result /= multiplier;
                return result;
            }

            B get() const{
                return _number;
            }

            // fixed<B, IntegerPrecision, FractionalPrecision> & operator = (fixed<B, IntegerPrecision, FractionalPrecision> const& rhs)
            // {
            //     fixed<B, IntegerPrecision, FractionalPrecision> temp(rhs);
            //     swap(temp);
            //     return *this;
            // }

            // void swap(fixed<B, IntegerPrecision, FractionalPrecision> & rhs)
            // { 
            //     std::swap(_number, rhs._number); 
            // }
            #ifdef abs
            friend fixed fabs(fixed x) const
            {
                return x < fixed(0) ? -x : x;
            }
            #else
            friend fixed abs(fixed x) 
            {
                return x < fixed(0) ? -x : x;
            }
            #endif

            friend fixed ceil(fixed x) 
            {
                fixed result;
                result._number = x._number & ~(power2<FractionalPrecision>::value-1);
                return result + fixed(x._number & (power2<FractionalPrecision>::value-1) ? 1 : 0);
            }

            friend fixed floor(fixed x) 
            {
                fixed result;
                result._number = x._number & ~(power2<FractionalPrecision>::value-1);
                return result;
            }

            friend fixed mod(fixed x, fixed y) 
            {
                fixed result;
                result._number = x._number % y._number;
                return result;
            }

            friend fixed exp(fixed x) 
            {
                // a[x] = exp( (1/2) ^ x ), x: [0 ... 31]
                fixed a[] = {
                    1.64872127070012814684865078781, 
                    1.28402541668774148407342056806, 
                    1.13314845306682631682900722781, 
                    1.06449445891785942956339059464, 
                    1.03174340749910267093874781528, 
                    1.01574770858668574745853507208, 
                    1.00784309720644797769345355976, 
                    1.00391388933834757344360960390, 
                    1.00195503359100281204651889805, 
                    1.00097703949241653524284529261, 
                    1.00048840047869447312617362381, 
                    1.00024417042974785493700523392, 
                    1.00012207776338377107650351967, 
                    1.00006103701893304542177912060, 
                    1.00003051804379102429545128481, 
                    1.00001525890547841394814004262, 
                    1.00000762942363515447174318433, 
                    1.00000381470454159186605078771, 
                    1.00000190735045180306002872525, 
                    1.00000095367477115374544678825, 
                    1.00000047683727188998079165439, 
                    1.00000023841860752327418915867, 
                    1.00000011920929665620888994533, 
                    1.00000005960464655174749969329, 
                    1.00000002980232283178452676169,
                    1.00000001490116130486995926397,
                    1.00000000745058062467940380956,
                    1.00000000372529030540080797502,
                    1.00000000186264515096568050830,
                    1.00000000093132257504915938475,
                    1.00000000046566128741615947508 };

                fixed e(2.718281828459045);

                fixed y(1);
                for (int i=FractionalPrecision-1; i>=0; --i)
                {
                    if (!(x._number & 1<<i))
                        y *= a[FractionalPrecision-i-1];
                }

                int x_int = (int)(floor(x));
                if (x_int<0)
                {
                    for (int i=1; i<=-x_int; ++i)
                        y /= e;
                }
                else
                {
                    for (int i=1; i<=x_int; ++i)
                        y *= e;
                }

                return y;
            }

            friend fixed cos(fixed x) 
            {
                static const fixed PI_OVER_2 = fixed(M_PI/2);
                // the expansion for sin proved to be more accurate
                return sin(x+PI_OVER_2);
            }

            friend fixed sin(fixed x) 
            {
                fixed x_ = 
                    mod(x, fixed(M_PI * 2));
                if (x_ > fixed(M_PI))
                    x_ -= fixed(M_PI * 2);

                fixed xx = x_ * x_;

                fixed y = - xx * fixed(1. / (2 * 3 * 4 * 5 * 6 * 7));
                y += fixed(1. / (2 * 3 * 4 * 5));
                y *= xx;
                y -= fixed(1. / (2 * 3));
                y *= xx;
                y += fixed(1);
                y *= x_;

                return y;
            }

            // friend fixed sqrt(fixed x)
            // {
            //     if (x < fixed(0))
            //     {
            //         //errno = EDOM;
            //         return 0;
            //     }

            //     typename promote_type<B>::type op = static_cast<typename promote_type<B>::type>(x._number) << (IntegerPrecision - 1);
            //     typename promote_type<B>::type res = 0;
            //     typename promote_type<B>::type one = 
            //         (typename promote_type<B>::type)1 << 
            //             (std::numeric_limits<typename promote_type<B>
            //                 ::type>::digits - 1); 

            //     while (one > op)
            //         one >>= 2;

            //     while (one != 0)
            //     {
            //         if (op >= res + one)
            //         {
            //             op = op - (res + one);
            //             res = res + (one << 1);
            //         }
            //         res >>= 1;
            //         one >>= 2;
            //     }

            //     fixed root;
            //     root._number = static_cast<B>(res);
            //     return root;
            // }


            

    };



}
#endif