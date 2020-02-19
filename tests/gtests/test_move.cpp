#include <gtest/gtest.h>
#include <fvar.hpp>
#include <future>

class test_move: public ::testing::Test {};

double_and_int* gradnew();

TEST_F(test_move, gradnew)
{
  gradient_structure gs;

  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  double_and_int* p = gradnew();

  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());
}
TEST_F(test_move, amovea)
{
  gradient_structure gs;

  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvariable a;

  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());

  dvariable a(a);
}
TEST_F(test_move, amove)
{
  gradient_structure gs;

  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvariable a;

  double_and_int* av = a.get_v();

  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());

  dvariable b(a);
  ASSERT_TRUE(b.get_v() != av);

  ASSERT_EQ(1752, gradient_structure::GRAD_LIST->total_addresses());

  dvariable c(std::move(a));

  ASSERT_EQ(1752, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_TRUE(a.get_v() == NULL);
  ASSERT_TRUE(c.get_v() == av);
}
TEST_F(test_move, follow_gradient_structure)
{
  ASSERT_TRUE(gradient_structure::GRAD_STACK1 == NULL);
  ASSERT_TRUE(gradient_structure::GRAD_LIST == NULL);

  gradient_structure gs;

  ASSERT_TRUE(gradient_structure::GRAD_STACK1 != NULL);
  ASSERT_TRUE(gradient_structure::GRAD_LIST != NULL);

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  grad_stack_entry* ptr = gradient_structure::GRAD_STACK1->ptr;
  ASSERT_TRUE(ptr != NULL);

  ASSERT_TRUE(ptr->func == NULL);
  ASSERT_TRUE(ptr->dep_addr == NULL);
  ASSERT_TRUE(ptr->ind_addr1 == NULL);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  dvariable a(1.5);

  ASSERT_EQ(1, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_TRUE(ptr->func == &default_evaluation0);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_TRUE(ptr->ind_addr1 == NULL);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  ptr++;

  ASSERT_TRUE(ptr->func == NULL);
  ASSERT_TRUE(ptr->dep_addr == NULL);
  ASSERT_TRUE(ptr->ind_addr1 == NULL);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  dvariable b(3.5);

  ASSERT_EQ(2, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1752, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_TRUE(ptr->func == &default_evaluation0);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1751));
  ASSERT_TRUE(ptr->ind_addr1 == NULL);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  ptr++;

  ASSERT_TRUE(ptr->func == NULL);
  ASSERT_TRUE(ptr->dep_addr == NULL);
  ASSERT_TRUE(ptr->ind_addr1 == NULL);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  dvariable result = a + b;

  ASSERT_EQ(4, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1753, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_TRUE(ptr->func == &default_evaluation4);
  ASSERT_TRUE(ptr->dep_addr == &(gradient_structure::RETURN_PTR->v->x));
  ASSERT_TRUE(ptr->ind_addr1 == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_TRUE(ptr->ind_addr2 == gradient_structure::GRAD_LIST->get(1751));
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  ptr++;

  ASSERT_TRUE(ptr->func == &default_evaluation1);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1752));
  ASSERT_TRUE(ptr->ind_addr1 == &(gradient_structure::RETURN_PTR->v->x));
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  ptr++;

  ASSERT_TRUE(ptr->func == NULL);
  ASSERT_TRUE(ptr->dep_addr == NULL);
  ASSERT_TRUE(ptr->ind_addr1 == NULL);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  ASSERT_DOUBLE_EQ(value(result), 5.0);
}
TEST_F(test_move, temporary)
{
  gradient_structure gs;

  auto sum = [](const double lhs, const double rhs)
  {
    dvariable a(lhs);
    dvariable b(rhs);
    return a + b;
  };

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());
  dvariable result = sum(1.5, 3.5);
  ASSERT_EQ(4, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1752, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(result), 5.0);

  grad_stack_entry* ptr = gradient_structure::GRAD_STACK1->ptr;
  ASSERT_TRUE(ptr->func == NULL);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation1);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_TRUE(ptr->ind_addr1 == &(gradient_structure::RETURN_PTR->v->x));
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation4);
  ASSERT_TRUE(ptr->dep_addr == &(gradient_structure::RETURN_PTR->v->x));
  ASSERT_TRUE(ptr->ind_addr1 == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_TRUE(ptr->ind_addr2 == gradient_structure::GRAD_LIST->get(1751));
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation0);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1751));
  ASSERT_TRUE(ptr->ind_addr1 == NULL);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation0);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_TRUE(ptr->ind_addr1 == NULL);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);
}
TEST_F(test_move, independents2)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables]()
  {
    dvariable a(variables(1));
    dvariable b(variables(2));
    return a + b;
  };

  dvariable result = sum();
  ASSERT_EQ(4, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1752, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(result), 5.0);

  grad_stack_entry* ptr = gradient_structure::GRAD_STACK1->ptr;
  ASSERT_TRUE(ptr->func == NULL);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation1);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_DOUBLE_EQ(*ptr->dep_addr, 5.0);
  ASSERT_TRUE(ptr->ind_addr1 == &(gradient_structure::RETURN_PTR->v->x));
  ASSERT_DOUBLE_EQ(*ptr->ind_addr1, 5.0);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation4);
  ASSERT_TRUE(ptr->dep_addr == &(gradient_structure::RETURN_PTR->v->x));
  ASSERT_DOUBLE_EQ(*ptr->dep_addr, 5.0);
  ASSERT_TRUE(ptr->ind_addr1 == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_DOUBLE_EQ(*ptr->ind_addr1, 5.0);
  ASSERT_TRUE(ptr->ind_addr2 == gradient_structure::GRAD_LIST->get(1751));
  ASSERT_DOUBLE_EQ(*ptr->ind_addr2, 3.5);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation1);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1751));
  ASSERT_DOUBLE_EQ(*ptr->dep_addr, 3.5);
  ASSERT_TRUE(ptr->ind_addr1 == &(variables(2).v->x));
  ASSERT_DOUBLE_EQ(*ptr->ind_addr1, 3.5);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation1);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_DOUBLE_EQ(*ptr->dep_addr, 5.0);
  ASSERT_TRUE(ptr->ind_addr1 == &(variables(1).v->x));
  ASSERT_DOUBLE_EQ(*ptr->ind_addr1, 1.5);
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);
}
TEST_F(test_move, independents)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  dvariable a(variables(1));
  dvariable b(variables(2));
  auto sum = [&a, &b]()
  {
    return a + b;
  };

  dvariable result = sum();
  ASSERT_EQ(4, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1753, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(result), 5.0);

  grad_stack_entry* ptr = gradient_structure::GRAD_STACK1->ptr;
  ASSERT_TRUE(ptr->func == NULL);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation1);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1752));
  ASSERT_TRUE(ptr->ind_addr1 == &(gradient_structure::RETURN_PTR->v->x));
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation4);
  ASSERT_TRUE(ptr->dep_addr == &(gradient_structure::RETURN_PTR->v->x));
  ASSERT_TRUE(ptr->ind_addr1 == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_TRUE(ptr->ind_addr2 == gradient_structure::GRAD_LIST->get(1751));
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation1);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1751));
  ASSERT_TRUE(ptr->ind_addr1 == &(variables(2).v->x));
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);

  --ptr;
  ASSERT_TRUE(ptr->func == &default_evaluation1);
  ASSERT_TRUE(ptr->dep_addr == gradient_structure::GRAD_LIST->get(1750));
  ASSERT_TRUE(ptr->ind_addr1 == &(variables(1).v->x));
  ASSERT_TRUE(ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(ptr->mult2, 0.0);
}
TEST_F(test_move, independents_gradcalc)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables]()
  {
    dvariable a(variables(1));
    dvariable b(variables(2));
    return a + b;
  };

  dvariable result = sum();
  double v = value(result);

  ASSERT_DOUBLE_EQ(value(variables(1)), 1.5);
  ASSERT_DOUBLE_EQ(value(variables(2)), 3.5);

  dvector g(1, 2);
  gradcalc(2, g);

  ASSERT_DOUBLE_EQ(g(1), 1.0);
  ASSERT_DOUBLE_EQ(g(2), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(1)), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(2)), 1.0);
}
TEST_F(test_move, independents_gradmanual)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables]()
  {
    dvariable a(variables(1));
    dvariable b(variables(2));
    return a + b;
  };

  dvariable result = sum();
  ASSERT_EQ(4, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1752, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(result), 5.0);

  --gradient_structure::GRAD_STACK1->ptr;

  double_and_int* ptr = (double_and_int*)gradient_structure::get_ARRAY_MEMBLOCK_BASE();
  unsigned long int imax = gradient_structure::ARR_LIST1->get_max_last_offset() / sizeof(double_and_int);
  for (unsigned int i = 0; i < imax; ++i)
  {
     ptr->x = 0.0;
     ++ptr;
  }
  gradient_structure::GRAD_LIST->initialize();
  *gradient_structure::GRAD_STACK1->ptr->dep_addr = 1.0;

  //Sum
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->dep_addr), 1.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr1), 0.0);
  (*(gradient_structure::GRAD_STACK1->ptr->func))();
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->dep_addr), 0.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr1), 1.0);
  ASSERT_TRUE(gradient_structure::GRAD_STACK1->ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(gradient_structure::GRAD_STACK1->ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(gradient_structure::GRAD_STACK1->ptr->mult2, 0.0);

  //Addition
  --gradient_structure::GRAD_STACK1->ptr;
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->dep_addr), 1.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr1), 0.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr2), 0.0);
  (*(gradient_structure::GRAD_STACK1->ptr->func))();
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->dep_addr), 0.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr1), 1.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr2), 1.0);
  ASSERT_DOUBLE_EQ(gradient_structure::GRAD_STACK1->ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(gradient_structure::GRAD_STACK1->ptr->mult2, 0.0);

  //Constructor
  --gradient_structure::GRAD_STACK1->ptr;
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->dep_addr), 1.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr1), 0.0);
  (*(gradient_structure::GRAD_STACK1->ptr->func))();
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->dep_addr), 0.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr1), 1.0);
  ASSERT_TRUE(gradient_structure::GRAD_STACK1->ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(gradient_structure::GRAD_STACK1->ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(gradient_structure::GRAD_STACK1->ptr->mult2, 0.0);

  //Constructor
  --gradient_structure::GRAD_STACK1->ptr;
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->dep_addr), 1.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr1), 0.0);
  (*(gradient_structure::GRAD_STACK1->ptr->func))();
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->dep_addr), 0.0);
  ASSERT_DOUBLE_EQ(*(gradient_structure::GRAD_STACK1->ptr->ind_addr1), 1.0);
  ASSERT_TRUE(gradient_structure::GRAD_STACK1->ptr->ind_addr2 == NULL);
  ASSERT_DOUBLE_EQ(gradient_structure::GRAD_STACK1->ptr->mult1, 0.0);
  ASSERT_DOUBLE_EQ(gradient_structure::GRAD_STACK1->ptr->mult2, 0.0);

  //ASSERT_DOUBLE_EQ(*gradient_structure::INDVAR_LIST->get_address(1), 0.0);
  //ASSERT_DOUBLE_EQ(*gradient_structure::INDVAR_LIST->get_address(2), 0.0);
  ASSERT_DOUBLE_EQ(value(variables(1)), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(2)), 1.0);
}
TEST_F(test_move, independents2x_gradcalc)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables](const double lhs, const double rhs)
  {
    dvariable a(variables(1));
    dvariable b(variables(2));
    return dvariable(lhs * a + rhs * b);
  };

  dvariable a = sum(2, -5);
  dvariable b = sum(-4, 6);
  dvariable result = a + b; 

  ASSERT_EQ(14, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1754, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(result), 0.5);
}
/*
TEST_F(test_move, independents2x_async_gradcalc)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables](const double lhs, const double rhs)
  {
    dvariable a(variables(1));
    dvariable b(variables(2));
    return dvariable(lhs * a + rhs * b);
  };

  std::future<dvariable> a(std::async(sum, 2, -5));
  std::future<dvariable> b(std::async(sum, -4, 6));

  dvariable result = a.get() + b.get(); 

  ASSERT_EQ(18, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1754, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(result), 0.5);
}
*/
dvariable vec2()
{
  cout << gradient_structure::GRAD_STACK1->total() << endl;
  dvar_vector vec(1, 3);
  cout << gradient_structure::GRAD_STACK1->total() << endl;
  cout << gradient_structure::ARR_LIST1->number_arr_links << endl;
  return vec(2);
}
TEST_F(test_move, return_vec2)
{
  gradient_structure gs;

  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);

  dvariable result;

  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);

  result = vec2();

  ASSERT_EQ(1752, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(2, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);
}
TEST_F(test_move, return_vec2_null)
{
  gradient_structure gs;

  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);

  dvariable result(nullptr);

  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);

  result = vec2();

  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(1, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);
}
dvariable var()
{
  dvariable var;
  return var;
}
TEST_F(test_move, return_var)
{
  gradient_structure gs;

  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);

  dvariable result;

  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);

  result = var();

  ASSERT_EQ(1752, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(1, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);
}
TEST_F(test_move, return_var_null)
{
  gradient_structure gs;

  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);

  dvariable result(nullptr);

  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);

  result = var();

  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(0, gradient_structure::ARR_LIST1->number_arr_links);
}
TEST_F(test_move, independents_gradcalc_with_null)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables]()
  {
    dvariable a(variables(1));
    dvariable b(variables(2));
    return a + b;
  };

  dvariable result(nullptr);
  result = sum();
  double v = value(result);

  ASSERT_EQ(4, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1752, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(variables(1)), 1.5);
  ASSERT_DOUBLE_EQ(value(variables(2)), 3.5);

  dvector g(1, 2);
  gradcalc(2, g);

  ASSERT_DOUBLE_EQ(g(1), 1.0);
  ASSERT_DOUBLE_EQ(g(2), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(1)), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(2)), 1.0);
}
TEST_F(test_move, independents_gradcalc_with_nonull)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables]()
  {
    dvariable a(variables(1));
    dvariable b(variables(2));
    return a + b;
  };

  dvariable result;
  result = sum();
  double v = value(result);

  ASSERT_EQ(4, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1753, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(variables(1)), 1.5);
  ASSERT_DOUBLE_EQ(value(variables(2)), 3.5);

  dvector g(1, 2);
  gradcalc(2, g);

  ASSERT_DOUBLE_EQ(g(1), 1.0);
  ASSERT_DOUBLE_EQ(g(2), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(1)), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(2)), 1.0);
}
TEST_F(test_move, independents_gradcalc_with_nonull_result)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables]()
  {
    dvariable result;
    dvariable a(variables(1));
    dvariable b(variables(2));
    result = a + b;
    return result;
  };

  dvariable result;
  result = sum();
  double v = value(result);

  ASSERT_EQ(5, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1754, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(variables(1)), 1.5);
  ASSERT_DOUBLE_EQ(value(variables(2)), 3.5);

  dvector g(1, 2);
  gradcalc(2, g);

  ASSERT_DOUBLE_EQ(g(1), 1.0);
  ASSERT_DOUBLE_EQ(g(2), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(1)), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(2)), 1.0);
}
TEST_F(test_move, independents_gradcalc_with_nonull_move)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables]()
  {
    dvariable result;
    dvariable a(variables(1));
    dvariable b(variables(2));
    result = a + b;
    return std::move(result);
  };

  dvariable result;
  result = sum();
  double v = value(result);

  ASSERT_EQ(5, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1754, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(variables(1)), 1.5);
  ASSERT_DOUBLE_EQ(value(variables(2)), 3.5);

  dvector g(1, 2);
  gradcalc(2, g);

  ASSERT_DOUBLE_EQ(g(1), 1.0);
  ASSERT_DOUBLE_EQ(g(2), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(1)), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(2)), 1.0);
}
TEST_F(test_move, independents_gradcalc_with_null_move)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables]()
  {
    dvariable result;
    dvariable a(variables(1));
    dvariable b(variables(2));
    result = a + b;
    return std::move(result);
  };

  dvariable result(nullptr);
  result = sum();
  double v = value(result);

  ASSERT_EQ(4, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1753, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(variables(1)), 1.5);
  ASSERT_DOUBLE_EQ(value(variables(2)), 3.5);

  dvector g(1, 2);
  gradcalc(2, g);

  ASSERT_DOUBLE_EQ(g(1), 1.0);
  ASSERT_DOUBLE_EQ(g(2), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(1)), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(2)), 1.0);
}
TEST_F(test_move, dvar_vector_to_variable)
{
  independent_variables independents(1, 2);
  independents(1) = 1.5;
  independents(2) = 3.5;

  gradient_structure gs;

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1750, gradient_structure::GRAD_LIST->total_addresses());

  dvar_vector variables(independents);

  auto sum = [&variables]()
  {
    return variables(1) + variables(2);
  };
  dvariable result;
  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  result = sum();
  ASSERT_EQ(2, gradient_structure::GRAD_STACK1->total());
  double v = value(result);

  ASSERT_EQ(2, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());

  ASSERT_DOUBLE_EQ(value(variables(1)), 1.5);
  ASSERT_DOUBLE_EQ(value(variables(2)), 3.5);

  dvector g(1, 2);
  gradcalc(2, g);

  ASSERT_DOUBLE_EQ(g(1), 1.0);
  ASSERT_DOUBLE_EQ(g(2), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(1)), 1.0);
  ASSERT_DOUBLE_EQ(value(variables(2)), 1.0);
}
TEST_F(test_move, same)
{
  gradient_structure gs;

  dvariable a;
 
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());
  a = std::move(a);
  ASSERT_EQ(0, gradient_structure::GRAD_STACK1->total());
  ASSERT_EQ(1751, gradient_structure::GRAD_LIST->total_addresses());
}
