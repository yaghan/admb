#include <gtest/gtest.h>
#include <fvar.hpp>

class test_grad_stack_entry: public ::testing::Test {};

TEST_F(test_grad_stack_entry, default)
{
  grad_stack_entry gse;

  ASSERT_TRUE(gse.f == nullptr);
  ASSERT_TRUE(gse.func == nullptr);
  ASSERT_TRUE(gse.dep_addr == nullptr);
  ASSERT_TRUE(gse.ind_addr1 == nullptr);
  ASSERT_DOUBLE_EQ(gse.mult1, 0);
  ASSERT_TRUE(gse.ind_addr2 == nullptr);
  ASSERT_DOUBLE_EQ(gse.mult2, 0);
}
TEST_F(test_grad_stack_entry, grad_stack_gse_null)
{
  grad_stack gstack;

  grad_stack_entry* gse = gstack.ptr; 

  void (*f)(grad_stack_entry* gse) = nullptr;
  gstack.set_gradient_stack0(f, nullptr);

  ASSERT_TRUE(gse->f == nullptr);
  ASSERT_TRUE(gse->func == nullptr);
  ASSERT_TRUE(gse->dep_addr == nullptr);
  ASSERT_TRUE(gse->ind_addr1 == nullptr);
  ASSERT_DOUBLE_EQ(gse->mult1, 0);
  ASSERT_TRUE(gse->ind_addr2 == nullptr);
  ASSERT_DOUBLE_EQ(gse->mult2, 0);
}
TEST_F(test_grad_stack_entry, grad_stack_void_null)
{
  grad_stack gstack;

  grad_stack_entry* gse = gstack.ptr; 

  void (*f)(void) = nullptr;
  gstack.set_gradient_stack0(f, nullptr);

  ASSERT_TRUE(gse->f == nullptr);
  ASSERT_TRUE(gse->func == nullptr);
  ASSERT_TRUE(gse->dep_addr == nullptr);
  ASSERT_TRUE(gse->ind_addr1 == nullptr);
  ASSERT_DOUBLE_EQ(gse->mult1, 0);
  ASSERT_TRUE(gse->ind_addr2 == nullptr);
  ASSERT_DOUBLE_EQ(gse->mult2, 0);
}
void fgse(grad_stack_entry* gse) {}
TEST_F(test_grad_stack_entry, grad_stack_gse)
{
  grad_stack gstack;

  grad_stack_entry* gse = gstack.ptr; 

  void (*f)(grad_stack_entry* gse) = &fgse;
  gstack.set_gradient_stack0(f, nullptr);

  ASSERT_TRUE(gse->f == &fgse);
  ASSERT_TRUE(gse->func == nullptr);
  ASSERT_TRUE(gse->dep_addr == nullptr);
  ASSERT_TRUE(gse->ind_addr1 == nullptr);
  ASSERT_DOUBLE_EQ(gse->mult1, 0);
  ASSERT_TRUE(gse->ind_addr2 == nullptr);
  ASSERT_DOUBLE_EQ(gse->mult2, 0);
}
void funcvoid() {}
TEST_F(test_grad_stack_entry, grad_stack_void)
{
  grad_stack gstack;

  grad_stack_entry* gse = gstack.ptr; 

  void (*func)(void) = &funcvoid;
  gstack.set_gradient_stack0(func, nullptr);

  ASSERT_TRUE(gse->f == nullptr);
  ASSERT_TRUE(gse->func == &funcvoid);
  ASSERT_TRUE(gse->dep_addr == nullptr);
  ASSERT_TRUE(gse->ind_addr1 == nullptr);
  ASSERT_DOUBLE_EQ(gse->mult1, 0);
  ASSERT_TRUE(gse->ind_addr2 == nullptr);
  ASSERT_DOUBLE_EQ(gse->mult2, 0);
}
