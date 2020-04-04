#include "platform/exception.h"

#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace location::nearby {

TEST(ExceptionOr, Result_Copy_NonConst) {
  ExceptionOr<std::vector<int>> exception_or_vector({1, 2, 3});
  EXPECT_FALSE(exception_or_vector.result().empty());

  // Expect a copy when not explicitly moving the result.
  std::vector<int> copy = exception_or_vector.result();
  EXPECT_FALSE(copy.empty());
  EXPECT_FALSE(exception_or_vector.result().empty());

  // Modifying |exception_or_vector| should not affect the copy.
  exception_or_vector.result().clear();
  EXPECT_FALSE(copy.empty());
}

TEST(ExceptionOr, Result_Copy_Const) {
  const ExceptionOr<std::vector<int>> exception_or_vector({1, 2, 3});
  EXPECT_FALSE(exception_or_vector.result().empty());

  // Expect a copy when not explicitly moving the result.
  std::vector<int> copy = exception_or_vector.result();
  EXPECT_FALSE(copy.empty());
  EXPECT_FALSE(exception_or_vector.result().empty());
}

TEST(ExceptionOr, Result_Reference_NonConst) {
  ExceptionOr<std::vector<int>> exception_or_vector({1, 2, 3});
  EXPECT_FALSE(exception_or_vector.result().empty());

  // Getting a reference should not modify the source.
  std::vector<int>& reference = exception_or_vector.result();
  EXPECT_FALSE(reference.empty());
  EXPECT_FALSE(exception_or_vector.result().empty());

  // Modifying |exception_or_vector| should reflect in the reference.
  exception_or_vector.result().clear();
  EXPECT_TRUE(reference.empty());
}

TEST(ExceptionOr, Result_Reference_Const) {
  const ExceptionOr<std::vector<int>> exception_or_vector({1, 2, 3});
  EXPECT_FALSE(exception_or_vector.result().empty());

  // Getting a reference should not modify the source.
  const std::vector<int>& reference = exception_or_vector.result();
  EXPECT_FALSE(reference.empty());
  EXPECT_FALSE(exception_or_vector.result().empty());
}

TEST(ExceptionOr, Result_Move_NonConst) {
  ExceptionOr<std::vector<int>> exception_or_vector({1, 2, 3});
  ASSERT_FALSE(exception_or_vector.result().empty());

  // Moving the result should clear the source.
  std::vector<int> moved = std::move(exception_or_vector).result();
  ASSERT_FALSE(moved.empty());
}

TEST(ExceptionOr, Result_Move_Const) {
  const ExceptionOr<std::vector<int>> exception_or_vector({1, 2, 3});
  ASSERT_FALSE(exception_or_vector.result().empty());

  // Moving const rvalue reference will result in a copy.
  std::vector<int> moved = std::move(exception_or_vector).result();
  ASSERT_FALSE(moved.empty());
}

}  // namespace location::nearby
