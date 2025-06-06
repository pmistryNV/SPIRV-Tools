// Copyright (c) 2015-2016 The Khronos Group Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <vector>

#include "test/unit_spirv.h"

namespace spvtools {
namespace {

using ::testing::ValuesIn;

TEST(OperandString, AllAreDefinedExceptVariable) {
  // None has no string, so don't test it.
  EXPECT_EQ(0u, SPV_OPERAND_TYPE_NONE);
  // Start testing at enum with value 1, skipping None.
  for (int i = 1; i < int(SPV_OPERAND_TYPE_NUM_OPERAND_TYPES); i++) {
    const auto type = static_cast<spv_operand_type_t>(i);
    if (spvOperandIsVariable(type)) {
      EXPECT_STREQ("unknown", spvOperandTypeStr(type))
          << " variable type " << i << " has a name '"
          << spvOperandTypeStr(type) << "'when it should not";
    } else {
      EXPECT_STRNE("unknown", spvOperandTypeStr(type))
          << " operand type " << i << " has no name when it should";
    }
  }
}

TEST(OperandIsConcreteMask, Sample) {
  // Check a few operand types preceding the concrete mask types.
  EXPECT_FALSE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_NONE));
  EXPECT_FALSE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_ID));
  EXPECT_FALSE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_LITERAL_INTEGER));
  EXPECT_FALSE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_CAPABILITY));

  // Check all the concrete mask operand types.
  EXPECT_TRUE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_IMAGE));
  EXPECT_TRUE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_FP_FAST_MATH_MODE));
  EXPECT_TRUE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_SELECTION_CONTROL));
  EXPECT_TRUE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_LOOP_CONTROL));
  EXPECT_TRUE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_FUNCTION_CONTROL));
  EXPECT_TRUE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_MEMORY_ACCESS));

  // Check a few operand types after the concrete mask types, including the
  // optional forms for Image and MemoryAccess.
  EXPECT_FALSE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_OPTIONAL_ID));
  EXPECT_FALSE(spvOperandIsConcreteMask(SPV_OPERAND_TYPE_OPTIONAL_IMAGE));
  EXPECT_FALSE(
      spvOperandIsConcreteMask(SPV_OPERAND_TYPE_OPTIONAL_MEMORY_ACCESS));
}

TEST(OperandType, NoneTypeClassification) {
  EXPECT_FALSE(spvOperandIsConcrete(SPV_OPERAND_TYPE_NONE));
  EXPECT_FALSE(spvOperandIsOptional(SPV_OPERAND_TYPE_NONE));
  EXPECT_FALSE(spvOperandIsVariable(SPV_OPERAND_TYPE_NONE));
}

TEST(OperandType, EndSentinelTypeClassification) {
  EXPECT_FALSE(spvOperandIsConcrete(SPV_OPERAND_TYPE_NUM_OPERAND_TYPES));
  EXPECT_FALSE(spvOperandIsOptional(SPV_OPERAND_TYPE_NUM_OPERAND_TYPES));
  EXPECT_FALSE(spvOperandIsVariable(SPV_OPERAND_TYPE_NUM_OPERAND_TYPES));
}

TEST(OperandType, WidthForcingTypeClassification) {
  EXPECT_FALSE(spvOperandIsConcrete(SPV_FORCE_32BIT_spv_operand_type_t));
  EXPECT_FALSE(spvOperandIsOptional(SPV_FORCE_32BIT_spv_operand_type_t));
  EXPECT_FALSE(spvOperandIsVariable(SPV_FORCE_32BIT_spv_operand_type_t));
}

TEST(OperandType, EachTypeIsEitherConcreteOrOptionalNotBoth) {
  EXPECT_EQ(0u, SPV_OPERAND_TYPE_NONE);
  // Start testing at enum with value 1, skipping None.
  for (int i = 1; i < int(SPV_OPERAND_TYPE_NUM_OPERAND_TYPES); i++) {
    const auto type = static_cast<spv_operand_type_t>(i);
    EXPECT_NE(spvOperandIsConcrete(type), spvOperandIsOptional(type))
        << " operand type " << int(type) << " concrete? "
        << int(spvOperandIsConcrete(type)) << " optional? "
        << int(spvOperandIsOptional(type));
  }
}

TEST(OperandType, EachVariableTypeIsOptional) {
  EXPECT_EQ(0u, SPV_OPERAND_TYPE_NONE);
  // Start testing at enum with value 1, skipping None.
  for (int i = 1; i < int(SPV_OPERAND_TYPE_NUM_OPERAND_TYPES); i++) {
    const auto type = static_cast<spv_operand_type_t>(i);
    if (spvOperandIsVariable(type)) {
      EXPECT_TRUE(spvOperandIsOptional(type)) << " variable type " << int(type);
    }
  }
}

}  // namespace
}  // namespace spvtools
