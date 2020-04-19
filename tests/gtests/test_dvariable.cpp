#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <fvar.hpp>

extern "C"
{
  void test_ad_exit(const int exit_code);
}

class test_dvariable: public ::testing::Test {};

TEST_F(test_dvariable, default_constructor)
{
  gradient_structure gs;
  dvariable d;
  ASSERT_TRUE(d.v != NULL);
}
TEST_F(test_dvariable, fmax)
{
  gradient_structure gs;

  dvariable fmax(double x,double* y);
  double x = 10;
  double y = 20;
  dvariable d = fmax(x, &y);
  ASSERT_EQ(y, value(d));
  d = fmax(y, &x);
  ASSERT_EQ(y, value(d));
  d = fmax(x, &x);
  ASSERT_EQ(x, value(d));
}
TEST_F(test_dvariable, det)
{
  gradient_structure gs;

  dvar_matrix vm(1, 2, 1, 2);
  vm(1, 1) = 3;
  vm(1, 2) = 8;
  vm(2, 1) = 4;
  vm(2, 2) = 6;

  dvariable vdet = det(vm);
  ASSERT_DOUBLE_EQ(-14, value(vdet));

  dmatrix m(1, 2, 1, 2);
  m(1, 1) = 3;
  m(1, 2) = 8;
  m(2, 1) = 4;
  m(2, 2) = 6;
  double cdet = det(m);
  ASSERT_DOUBLE_EQ(-14, cdet);
}
TEST_F(test_dvariable, exp_1)
{
  gradient_structure gs;
  
  dvariable v = 1;
  try
  {
    double expected = ::exp(1);
    dvariable result = exp(v);
    ASSERT_DOUBLE_EQ(expected, value(result));
  }
  catch (const int exit_code)          
  {   
    const int expected_exit_code = 1;
    if (exit_code == expected_exit_code)
    {
      FAIL();
    }
  }
}
#ifdef DIAG
  ///\todo Must remove macros below once support for MSVC++11 and Solaris Studio 12.3 are removed.
  #if !defined(__SUNPRO_CC) && !(defined(_MSC_VER) && (_MSC_VER <= 1700))
TEST_F(test_dvariable, exp_max)
{
  gradient_structure gs;
  
  dvariable v = std::numeric_limits< double >::max();
  try
  {
    exp(v);
  }
  catch (const int exit_code)          
  {   
    const int expected_exit_code = 1;
    if (exit_code == expected_exit_code)
    {
      SUCCEED();
      return;
    }
  }
  FAIL();
}
  #endif
#endif
TEST_F(test_dvariable, mfexp)
{
  gradient_structure gs;
  
  {
  double input = 100; 
  double result = mfexp(input);

  dvariable vinput = 100; 
  dvariable vresult = mfexp(vinput);

  ASSERT_DOUBLE_EQ(result, value(vresult));
  ASSERT_TRUE(result > ::exp(60));
  }
  {
  double input = -100; 
  double result = mfexp(input);

  dvariable vinput = -100; 
  dvariable vresult = mfexp(vinput);

  ASSERT_DOUBLE_EQ(result, value(vresult));
  ASSERT_TRUE(result < ::exp(-60));
  }
  {
  double input = 60; 
  double result = mfexp(input);

  dvariable vinput = 60; 
  dvariable vresult = mfexp(vinput);

  ASSERT_DOUBLE_EQ(result, value(vresult));
  ASSERT_DOUBLE_EQ(result, ::exp(60));
  }
  {
  double input = -60; 
  double result = mfexp(input);

  dvariable vinput = -60; 
  dvariable vresult = mfexp(vinput);

  ASSERT_DOUBLE_EQ(result, value(vresult));
  ASSERT_DOUBLE_EQ(result, ::exp(-60));
  }
  {
  double input = 61; 
  double result = mfexp(input);

  dvariable vinput = 61; 
  dvariable vresult = mfexp(vinput);

  ASSERT_DOUBLE_EQ(result, value(vresult));
  ASSERT_TRUE(result > ::exp(60));
  ASSERT_TRUE(result < ::exp(61));
  }
  {
  double input = -61; 
  double result = mfexp(input);

  dvariable vinput = -61; 
  dvariable vresult = mfexp(vinput);

  ASSERT_DOUBLE_EQ(result, value(vresult));
  ASSERT_TRUE(result < ::exp(-60));
  ASSERT_TRUE(result > ::exp(-61));
  }
  {
  double input = 10; 
  double result = mfexp(input);

  dvariable vinput = 10; 
  dvariable vresult = mfexp(vinput);

  ASSERT_DOUBLE_EQ(result, value(vresult));
  ASSERT_DOUBLE_EQ(result, ::exp(10));
  }
  {
  double input = -10; 
  double result = mfexp(input);

  dvariable vinput = -10; 
  dvariable vresult = mfexp(vinput);

  ASSERT_DOUBLE_EQ(result, value(vresult));
  ASSERT_DOUBLE_EQ(result, ::exp(-10));
  }
  {
  double input = 0; 
  double result = mfexp(input);

  dvariable vinput = 0; 
  dvariable vresult = mfexp(vinput);

  ASSERT_DOUBLE_EQ(result, value(vresult));
  ASSERT_DOUBLE_EQ(result, ::exp(0));
  }
}
TEST_F(test_dvariable, copy_constructor)
{
  gradient_structure gs;

  dvariable a(5.2);

  size_t addresses = gradient_structure::GRAD_LIST->total_addresses();
  unsigned long last = gradient_structure::ARR_LIST1->get_max_last_offset();
  size_t total  = gradient_structure::GRAD_STACK1->total();
  dvariable b(a);
  ASSERT_EQ(total + 1, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(last, gradient_structure::ARR_LIST1->get_max_last_offset());
  ASSERT_EQ(addresses + 1, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_EQ(value(a), value(b));
  ASSERT_TRUE(a.get_v() != b.get_v());
}
TEST_F(test_dvariable, move_constructor)
{
  gradient_structure gs;

  dvariable a(5.2);

  size_t addresses = gradient_structure::GRAD_LIST->total_addresses();
  unsigned long last = gradient_structure::ARR_LIST1->get_max_last_offset();
  size_t total  = gradient_structure::GRAD_STACK1->total();
  dvariable b(std::move(a));
  ASSERT_EQ(total + 1, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(last, gradient_structure::ARR_LIST1->get_max_last_offset());
  ASSERT_EQ(addresses + 1, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_EQ(value(a), value(b));
  ASSERT_TRUE(a.get_v() != b.get_v());
}
TEST_F(test_dvariable, assignment)
{
  gradient_structure gs;

  dvariable a(5.2);
  dvariable b;

  size_t addresses = gradient_structure::GRAD_LIST->total_addresses();
  unsigned long last = gradient_structure::ARR_LIST1->get_max_last_offset();
  size_t total  = gradient_structure::GRAD_STACK1->total();
  b = a;
  ASSERT_EQ(total + 1, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(last, gradient_structure::ARR_LIST1->get_max_last_offset());
  ASSERT_EQ(addresses, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_EQ(value(a), value(b));
  ASSERT_TRUE(a.get_v() != b.get_v());
}
TEST_F(test_dvariable, move_assignment)
{
  gradient_structure gs;

  dvariable a(5.2);
  dvariable b;

  size_t addresses = gradient_structure::GRAD_LIST->total_addresses();
  unsigned long last = gradient_structure::ARR_LIST1->get_max_last_offset();
  size_t total  = gradient_structure::GRAD_STACK1->total();
  b = std::move(a);
  ASSERT_EQ(total + 1, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(last, gradient_structure::ARR_LIST1->get_max_last_offset());
  ASSERT_EQ(addresses, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_EQ(value(a), value(b));
  ASSERT_TRUE(a.get_v() != b.get_v());
}
TEST_F(test_dvariable, assignment_double)
{
  gradient_structure gs;

  dvariable a(5.2);

  double b = -2.5;

  a = b;

  ASSERT_EQ(value(a), b);
}
TEST_F(test_dvariable, assignment_prevariable)
{
  gradient_structure gs;

  dvariable a(5.2);

  dvar_vector b("{-1, -2, -3.1, -4}");

  a = b(3);

  ASSERT_EQ(value(a), value(b(3)));
  ASSERT_EQ(value(a), -3.1);
}
TEST_F(test_dvariable, assignment_dvariable)
{
  gradient_structure gs;

  dvariable a(5.2);
  dvariable b;

  b = a;

  ASSERT_EQ(value(a), value(b));
  ASSERT_EQ(5.2, value(b));
}
TEST_F(test_dvariable, constructor_prevariable)
{
  gradient_structure gs;

  dvar_vector b("{-1, -2, -3.1, -4}");

  size_t addresses = gradient_structure::GRAD_LIST->total_addresses();
  dvariable a(b(3));
  ASSERT_EQ(addresses + 1, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_EQ(value(a), value(b(3)));
  ASSERT_EQ(value(a), -3.1);
}
TEST_F(test_dvariable, constructor_dvariable)
{
  gradient_structure gs;

  dvariable a(5.2);
  dvariable b(a);

  ASSERT_EQ(value(a), value(b));
  ASSERT_EQ(5.2, value(b));
}
TEST_F(test_dvariable, prevariable_copy_constructor)
{
  gradient_structure gs;

  dvar_vector b("{-1, -2, -3.1, -4}");

  size_t addresses = gradient_structure::GRAD_LIST->total_addresses();
  prevariable a(b(3));
  ASSERT_EQ(addresses, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_EQ(value(a), value(b(3)));
  ASSERT_EQ(value(a), -3.1);
  ASSERT_TRUE(a.get_v() == b(3).get_v());
}
