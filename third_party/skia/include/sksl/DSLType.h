/*
 * Copyright 2020 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SKSL_DSL_TYPE
#define SKSL_DSL_TYPE

#include "include/sksl/DSLExpression.h"
#include "include/sksl/DSLModifiers.h"

#include <cstdint>

namespace SkSL {

class Type;

namespace dsl {

class DSLExpression;
class DSLField;

enum TypeConstant : uint8_t {
    kBool_Type,
    kBool2_Type,
    kBool3_Type,
    kBool4_Type,
    kHalf_Type,
    kHalf2_Type,
    kHalf3_Type,
    kHalf4_Type,
    kHalf2x2_Type,
    kHalf3x2_Type,
    kHalf4x2_Type,
    kHalf2x3_Type,
    kHalf3x3_Type,
    kHalf4x3_Type,
    kHalf2x4_Type,
    kHalf3x4_Type,
    kHalf4x4_Type,
    kFloat_Type,
    kFloat2_Type,
    kFloat3_Type,
    kFloat4_Type,
    kFragmentProcessor_Type,
    kFloat2x2_Type,
    kFloat3x2_Type,
    kFloat4x2_Type,
    kFloat2x3_Type,
    kFloat3x3_Type,
    kFloat4x3_Type,
    kFloat2x4_Type,
    kFloat3x4_Type,
    kFloat4x4_Type,
    kInt_Type,
    kInt2_Type,
    kInt3_Type,
    kInt4_Type,
    kShader_Type,
    kShort_Type,
    kShort2_Type,
    kShort3_Type,
    kShort4_Type,
    kUInt_Type,
    kUInt2_Type,
    kUInt3_Type,
    kUInt4_Type,
    kUShort_Type,
    kUShort2_Type,
    kUShort3_Type,
    kUShort4_Type,
    kVoid_Type,
};

class DSLType {
public:
    DSLType(TypeConstant tc)
        : fTypeConstant(tc) {}

    DSLType(const SkSL::Type* type)
        : fSkSLType(type) {}

    DSLType(skstd::string_view name);

    /**
     * Returns true if this type is a bool.
     */
    bool isBoolean() const;

    /**
     * Returns true if this is a numeric scalar type.
     */
    bool isNumber() const;

    /**
     * Returns true if this is a floating-point scalar type (float or half).
     */
    bool isFloat() const;

    /**
     * Returns true if this is a signed scalar type (int or short).
     */
    bool isSigned() const;

    /**
     * Returns true if this is an unsigned scalar type (uint or ushort).
     */
    bool isUnsigned() const;

    /**
     * Returns true if this is a signed or unsigned integer.
     */
    bool isInteger() const;

    /**
     * Returns true if this is a scalar type.
     */
    bool isScalar() const;

    /**
     * Returns true if this is a vector type.
     */
    bool isVector() const;

    /**
     * Returns true if this is a matrix type.
     */
    bool isMatrix() const;

    /**
     * Returns true if this is a array type.
     */
    bool isArray() const;

    /**
     * Returns true if this is a struct type.
     */
    bool isStruct() const;

    template<typename... Args>
    static DSLExpression Construct(DSLType type, Args&&... args) {
        SkTArray<DSLExpression> argArray;
        argArray.reserve_back(sizeof...(args));
        CollectArgs(argArray, std::forward<Args>(args)...);
        return Construct(type, std::move(argArray));
    }

    static DSLExpression Construct(DSLType type, SkTArray<DSLExpression> argArray);

private:
    const SkSL::Type& skslType() const;

    const SkSL::Type* fSkSLType = nullptr;

    static void CollectArgs(SkTArray<DSLExpression>& args) {}

    template<class... RemainingArgs>
    static void CollectArgs(SkTArray<DSLExpression>& args, DSLVar& var,
                            RemainingArgs&&... remaining) {
        args.push_back(var);
        CollectArgs(args, std::forward<RemainingArgs>(remaining)...);
    }

    template<class... RemainingArgs>
    static void CollectArgs(SkTArray<DSLExpression>& args, DSLExpression expr,
                            RemainingArgs&&... remaining) {
        args.push_back(std::move(expr));
        CollectArgs(args, std::forward<RemainingArgs>(remaining)...);
    }

    TypeConstant fTypeConstant;

    friend DSLType Array(const DSLType& base, int count);
    friend DSLType Struct(skstd::string_view name, SkTArray<DSLField> fields);
    friend class DSLFunction;
    friend class DSLVar;
    friend class DSLWriter;
};

#define TYPE(T)                                                                                    \
    template<typename... Args>                                                                     \
    DSLExpression T(Args&&... args) {                                                              \
        return DSLType::Construct(k ## T ## _Type, std::forward<Args>(args)...);                   \
    }

#define VECTOR_TYPE(T)                                                                             \
    TYPE(T)                                                                                        \
    TYPE(T ## 2)                                                                                   \
    TYPE(T ## 3)                                                                                   \
    TYPE(T ## 4)

#define MATRIX_TYPE(T)                                                                             \
    TYPE(T ## 2x2)                                                                                 \
    TYPE(T ## 3x2)                                                                                 \
    TYPE(T ## 4x2)                                                                                 \
    TYPE(T ## 2x3)                                                                                 \
    TYPE(T ## 3x3)                                                                                 \
    TYPE(T ## 4x3)                                                                                 \
    TYPE(T ## 2x4)                                                                                 \
    TYPE(T ## 3x4)                                                                                 \
    TYPE(T ## 4x4)

VECTOR_TYPE(Bool)
VECTOR_TYPE(Float)
VECTOR_TYPE(Half)
VECTOR_TYPE(Int)
VECTOR_TYPE(UInt)
VECTOR_TYPE(Short)
VECTOR_TYPE(UShort)

MATRIX_TYPE(Float)
MATRIX_TYPE(Half)

#undef TYPE
#undef VECTOR_TYPE
#undef MATRIX_TYPE

DSLType Array(const DSLType& base, int count);

class DSLField {
public:
    DSLField(const DSLType type, skstd::string_view name)
        : DSLField(DSLModifiers(), type, name) {}

private:
    DSLField(DSLModifiers modifiers, const DSLType type, skstd::string_view name)
        : fModifiers(modifiers)
        , fType(type)
        , fName(name) {}

    DSLModifiers fModifiers;
    const DSLType fType;
    skstd::string_view fName;

    friend DSLType Struct(skstd::string_view name, SkTArray<DSLField> fields);
};

DSLType Struct(skstd::string_view name, SkTArray<DSLField> fields);

template<typename... Field>
DSLType Struct(skstd::string_view name, Field... fields) {
    SkTArray<DSLField> fieldTypes;
    fieldTypes.reserve_back(sizeof...(fields));
    // in C++17, we could just do:
    // (fieldTypes.push_back(std::move(fields)), ...);
    int unused[] = {0, (fieldTypes.push_back(std::move(fields)), 0)...};
    static_cast<void>(unused);

    return Struct(name, std::move(fieldTypes));
}

} // namespace dsl

} // namespace SkSL

#endif
