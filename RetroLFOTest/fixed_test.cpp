// tests.cpp
#include "../RetroLFO/fixed.h"
#include "gtest/gtest.h"
 
using namespace fixed_point; 

TEST(FixedTest, CoerceFloat) { 
    fixed<uint16_t,2,14> num = fixed<uint16_t,2,14>((float)3.141592);
    ASSERT_FLOAT_EQ(3.1415405, (float) num);

    fixed<uint8_t,2,6> num2 = fixed<uint8_t,2,6>((float)3.141592);
    ASSERT_FLOAT_EQ(3.140625, (float) num2);

    fixed<int8_t,2,5> num3 = fixed<int8_t,2,5>((float)3.141592);
    ASSERT_FLOAT_EQ(3.125, (float) num3);

    fixed<uint16_t,1,15> overflow = fixed<uint16_t,1,15>((float)3.141592);
    ASSERT_FLOAT_EQ(1.141571, (float) overflow);

}

TEST(FixedTest, CoerceDouble) { 
    fixed<uint16_t,2,14> num = fixed<uint16_t,2,14>((double)3.141592);
    ASSERT_FLOAT_EQ(3.1415405, (double) num);

    fixed<uint8_t,2,6> num2 = fixed<uint8_t,2,6>((double)3.141592);
    ASSERT_FLOAT_EQ(3.140625, (double) num2);

    fixed<int8_t,2,5> num3 = fixed<int8_t,2,5>((double)3.141592);
    ASSERT_FLOAT_EQ(3.125, (double) num3);

    fixed<uint16_t,1,15> overflow = fixed<uint16_t,1,15>((double)3.141592);
    ASSERT_FLOAT_EQ(1.141571, (double) overflow);

}

TEST(FixedTest, CoerceInt){
    fixed<int16_t, 3, 12> num = fixed<int16_t, 3, 12> (5);
   
    ASSERT_FLOAT_EQ(5, (int) num);

    fixed<uint16_t, 8, 8> num1 = fixed<uint16_t, 8, 8> (50);
    
    ASSERT_FLOAT_EQ(50, (int) num1);
}

TEST(FixedTest, Assignment){
    fixed<uint16_t,4,12> num;
    num = 6.283;
    ASSERT_FLOAT_EQ(6.282959, (float) num);

    num = 3.141592;
    ASSERT_FLOAT_EQ(3.1413574, (float) num);

}

TEST(FixedTest, DownSample){
    fixed<int16_t, 3, 12> num1 = fixed<int16_t, 3, 12> (5.23334334);
    fixed<int32_t, 3, 28> num2 = fixed<int32_t, 3, 28> (num1);
   
    // ASSERT_EQ((fixed<int8_t, 3, 4> (5)), num2);
    ASSERT_FLOAT_EQ(5.0, (float) num2);

}

TEST(FixedTest, Comparison){
    ASSERT_TRUE((fixed<uint16_t,8,8>(2.14) == fixed<uint16_t,8,8>(2.14)));
    ASSERT_FALSE((fixed<uint16_t,8,8>(2.15) == fixed<uint16_t,8,8>(2.14)));

    ASSERT_TRUE((fixed<uint16_t,8,8>(2.14) < fixed<uint16_t,8,8>(2.15)));
    ASSERT_FALSE((fixed<uint16_t,8,8>(2.15) < fixed<uint16_t,8,8>(2.15)));
    ASSERT_FALSE((fixed<uint16_t,8,8>(2.16) < fixed<uint16_t,8,8>(2.15)));

    ASSERT_TRUE((fixed<uint16_t,8,8>(2.14) <= fixed<uint16_t,8,8>(2.15)));
    ASSERT_TRUE((fixed<uint16_t,8,8>(2.16) <= fixed<uint16_t,8,8>(2.16)));
    ASSERT_FALSE((fixed<uint16_t,8,8>(2.18) <= fixed<uint16_t,8,8>(2.16)));

    ASSERT_TRUE((fixed<uint16_t,8,8>(2.16) > fixed<uint16_t,8,8>(2.15)));
    ASSERT_FALSE((fixed<uint16_t,8,8>(2.15) > fixed<uint16_t,8,8>(2.15)));
    ASSERT_FALSE((fixed<uint16_t,8,8>(2.14) > fixed<uint16_t,8,8>(2.15)));

    ASSERT_TRUE((fixed<uint16_t,8,8>(2.16) >= fixed<uint16_t,8,8>(2.15)));
    ASSERT_TRUE((fixed<uint16_t,8,8>(2.15) >= fixed<uint16_t,8,8>(2.15)));
    ASSERT_FALSE((fixed<uint16_t,8,8>(2.14) >= fixed<uint16_t,8,8>(2.15)));

    ASSERT_TRUE((fixed<uint16_t,8,8>(2.14) != fixed<uint16_t,8,8>(2.15)));
    ASSERT_FALSE((fixed<uint16_t,8,8>(2.15) != fixed<uint16_t,8,8>(2.15)));
}

TEST(FixedTest, Inversion){
    ASSERT_FLOAT_EQ((-2.1328125),(-fixed<int16_t,8,7>(2.14)));
}

TEST(FixedTest, AdditionSubtraction){
    ASSERT_FLOAT_EQ(3.359375,(fixed<int16_t,8,7>(2.14)+fixed<int16_t,8,7>(1.23)));
    ASSERT_FLOAT_EQ(0,(fixed<int16_t,8,7>(2.14)-fixed<int16_t,8,7>(2.14)));
}

TEST(FixedTest, AdditionSubtractionOverflow){
    ASSERT_FLOAT_EQ(-1.65625,(fixed<int8_t,1,6>(1.14)+fixed<int8_t,1,6>(1.23)));
    ASSERT_FLOAT_EQ(1.65625,(fixed<int8_t,1,6>(-1.14)-fixed<int8_t,1,6>(1.23)));
}

TEST(FixedTest, AdditionAndAssignment){
    fixed<uint16_t,4,12> num = 3.141592;
    ASSERT_FLOAT_EQ(3.1413574, num);
    num += 1.2;
    ASSERT_FLOAT_EQ(4.3413086, num);
}

TEST(FixedTest, Multiplication){
    fixed<uint16_t,4,12> a = 3.141592;
    fixed<uint16_t,4,12> b = 2.14;

    ASSERT_FLOAT_EQ(6.7219238, a*b);

    fixed<int16_t,4,11> c = -2.33333;
    fixed<int16_t,4,11> d = 1.245;

    ASSERT_FLOAT_EQ(-2.9038086, c*d);

}

TEST(FixedTest, Division){
    fixed<uint16_t,4,12> a = 6.7219238;
    fixed<uint16_t,4,12> b = 2.14;

    ASSERT_FLOAT_EQ(3.1411133, a/b);

    fixed<int16_t,4,11> c = -2.9038086;
    fixed<int16_t,4,11> d = 1.245;

    ASSERT_FLOAT_EQ(-2.3330078, c/d);

}

TEST(FixedTest, SQRT){
    // ASSERT_EQ((fixed<int16_t,7,8>(2)), (fixed<int16_t,7,8>::sqrt(fixed<int16_t,7,8>(4))));
    ASSERT_TRUE(false);

}

TEST(FixedTest, SINE){
    // first 2 cycles within 0.1 of std::sin for 27 bits of precision
    for(int idx=0; idx<200; idx++){
        fixed<int,4,27> phase = fixed<int,4,27>(2*idx*M_PI/100);
        float acceptedValue = sin((float)phase);
        ASSERT_GT(0.1, (abs((float)sin(phase)-acceptedValue)));
    }
}

TEST(FixedTest, COSINE){
    // first 2 cycles within 0.15 of std::cos for 27 bits of precision
    for(int idx=0; idx<200; idx++){
        fixed<int,4,27> phase = fixed<int,4,27>(2*idx*M_PI/100);
        float acceptedValue = cos((float)phase);
        ASSERT_GT(0.1, (abs((float)cos(phase)-acceptedValue)));
    }

}

TEST(FixedTest, EXP){
    ASSERT_TRUE(false);
}

TEST(FixedTest, MOD){
    ASSERT_EQ((fixed<int16_t,7,8>(0)), mod(fixed<int16_t,7,8>(0),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(1)), mod(fixed<int16_t,7,8>(1),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(2)), mod(fixed<int16_t,7,8>(2),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(3)), mod(fixed<int16_t,7,8>(3),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(4)), mod(fixed<int16_t,7,8>(4),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(0)), mod(fixed<int16_t,7,8>(5),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(1)), mod(fixed<int16_t,7,8>(6),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(2)), mod(fixed<int16_t,7,8>(7),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(3)), mod(fixed<int16_t,7,8>(8),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(4)), mod(fixed<int16_t,7,8>(9),fixed<int16_t,7,8>(5)));
    ASSERT_EQ((fixed<int16_t,7,8>(0)), mod(fixed<int16_t,7,8>(10),fixed<int16_t,7,8>(5)));

    ASSERT_EQ((fixed<int16_t,7,8>(0)), mod(fixed<int16_t,7,8>(3.25),fixed<int16_t,7,8>(3.25)));
    ASSERT_EQ((fixed<int16_t,7,8>(2.444)), mod(fixed<int16_t,7,8>(2.444),fixed<int16_t,7,8>(3.25)));

    // is it better to be consistent with how % works for int, or to be mathematically right?
    // ASSERT_EQ(3,(-11)%7);
    // ASSERT_FLOAT_EQ((fixed<int16_t,7,8>(3)), mod(fixed<int16_t,7,8>(-11),fixed<int16_t,7,8>(7)));
    // ASSERT_FLOAT_EQ((fixed<int16_t,7,8>(2.02)), mod(fixed<int16_t,7,8>(-1.233),fixed<int16_t,7,8>(3.25)));
    // ASSERT_FLOAT_EQ((fixed<int16_t,7,8>(-3)), mod(fixed<int16_t,7,8>(11),fixed<int16_t,7,8>(-7)));

}

TEST(FixedTest, ABS){
    ASSERT_EQ((fixed<int16_t,7,8>(1.233)), abs(fixed<int16_t,7,8>(-1.233)));
    ASSERT_EQ((fixed<int16_t,7,8>(1.233)), abs(fixed<int16_t,7,8>(1.233)));
    ASSERT_EQ((fixed<uint16_t,8,8>(4.72)), abs(fixed<uint16_t,8,8>(4.72)));
}

TEST(FixedTest, CEIL){
    ASSERT_EQ((fixed<int16_t,7,8>(-1)), ceil(fixed<int16_t,7,8>(-1.233)));
    ASSERT_EQ((fixed<int16_t,7,8>(2)), ceil(fixed<int16_t,7,8>(1.233)));
    ASSERT_EQ((fixed<int16_t,7,8>(-8)), ceil(fixed<int16_t,7,8>(-8.0001)));
    ASSERT_EQ((fixed<int16_t,7,8>(123)), ceil(fixed<int16_t,7,8>(122.722)));
}

TEST(FixedTest, FLOOR){
    ASSERT_EQ((fixed<int16_t,7,8>(-2)), floor(fixed<int16_t,7,8>(-1.233)));
    ASSERT_EQ((fixed<int16_t,7,8>(0)), floor(fixed<int16_t,7,8>(0.01)));
    ASSERT_EQ((fixed<int16_t,7,8>(99)), floor(fixed<int16_t,7,8>(99.9)));

}

TEST(FixedTest, PROMOTE_TYPE){
    ASSERT_TRUE(false);
}


TEST(FixedTest, Power2) { 
    ASSERT_EQ(1, 1*power2<0>::value);
    ASSERT_EQ(2, 1*power2<1>::value);
    ASSERT_EQ(4, 1*power2<2>::value);
    ASSERT_EQ(8, 1*power2<3>::value);
    
    ASSERT_EQ(16, 1*power2<4>::value);
    ASSERT_EQ(32, 1*power2<5>::value);
    ASSERT_EQ(64, 1*power2<6>::value);
    ASSERT_EQ(32768, 1*power2<15>::value);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}