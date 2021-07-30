/*
 * Copyright 2020 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SKSL_DSL_CORE
#define SKSL_DSL_CORE

#include "include/private/SkSLProgramKind.h"
#include "include/private/SkTArray.h"
#include "include/sksl/DSLBlock.h"
#include "include/sksl/DSLCase.h"
#include "include/sksl/DSLErrorHandling.h"
#include "include/sksl/DSLExpression.h"
#include "include/sksl/DSLFunction.h"
#include "include/sksl/DSLStatement.h"
#include "include/sksl/DSLType.h"
#include "include/sksl/DSLVar.h"
#include "include/sksl/DSLWrapper.h"

namespace SkSL {

class Compiler;
struct Program;
struct ProgramSettings;

namespace dsl {

// When users import the DSL namespace via `using namespace SkSL::dsl`, we want the SwizzleComponent
// Type enum to come into scope as well, so `Swizzle(var, X, Y, ONE)` can work as expected.
// `namespace SkSL::SwizzleComponent` contains only an `enum Type`; this `using namespace` directive
// shouldn't pollute the SkSL::dsl namespace with anything else.
using namespace SkSL::SwizzleComponent;

/**
 * Starts DSL output on the current thread using the specified compiler. This must be called
 * prior to any other DSL functions.
 */
void Start(SkSL::Compiler* compiler, SkSL::ProgramKind kind = SkSL::ProgramKind::kFragment);

void Start(SkSL::Compiler* compiler, SkSL::ProgramKind kind, const SkSL::ProgramSettings& settings);

/**
 * Signals the end of DSL output. This must be called sometime between a call to Start() and the
 * termination of the thread.
 */
void End();

/**
 * Returns all global elements (functions and global variables) as a self-contained Program.
 */
std::unique_ptr<SkSL::Program> ReleaseProgram();

/**
 * Installs an ErrorHandler which will be notified of any errors that occur during DSL calls. If
 * no ErrorHandler is installed, any errors will be fatal.
 */
void SetErrorHandler(ErrorHandler* errorHandler);

DSLVar sk_FragColor();

DSLVar sk_FragCoord();

/**
 * break;
 */
DSLStatement Break();

/**
 * continue;
 */
DSLStatement Continue();

/**
 * Creates a local variable declaration statement.
 */
DSLStatement Declare(DSLVar& var, PositionInfo pos = PositionInfo());

/**
 * Declares a global variable.
 */
void DeclareGlobal(DSLVar& var, PositionInfo pos = PositionInfo());

/**
 * default: statements
 */
template<class... Statements>
DSLCase Default(Statements... statements) {
    return DSLCase(DSLExpression(), std::move(statements)...);
}

/**
 * discard;
 */
DSLStatement Discard();

/**
 * do stmt; while (test);
 */
DSLStatement Do(DSLStatement stmt, DSLExpression test, PositionInfo pos = PositionInfo());

/**
 * for (initializer; test; next) stmt;
 */
DSLStatement For(DSLStatement initializer, DSLExpression test, DSLExpression next,
                 DSLStatement stmt, PositionInfo pos = PositionInfo());

/**
 * if (test) ifTrue; [else ifFalse;]
 */
DSLStatement If(DSLExpression test, DSLStatement ifTrue, DSLStatement ifFalse = DSLStatement(),
                PositionInfo pos = PositionInfo());

/**
 * return [value];
 */
DSLStatement Return(DSLExpression value = DSLExpression(), PositionInfo pos = PositionInfo());

/**
 * test ? ifTrue : ifFalse
 */
DSLExpression Select(DSLExpression test, DSLExpression ifTrue, DSLExpression ifFalse,
                     PositionInfo info = PositionInfo());

DSLStatement StaticIf(DSLExpression test, DSLStatement ifTrue,
                      DSLStatement ifFalse = DSLStatement(), PositionInfo pos = PositionInfo());

DSLPossibleStatement StaticSwitch(DSLExpression value, SkTArray<DSLCase> cases);

/**
 * @switch (value) { cases }
 */
template<class... Cases>
DSLPossibleStatement StaticSwitch(DSLExpression value, Cases... cases) {
    SkTArray<DSLCase> caseArray;
    caseArray.reserve_back(sizeof...(cases));
    (caseArray.push_back(std::move(cases)), ...);
    return StaticSwitch(std::move(value), std::move(caseArray));
}

DSLPossibleStatement Switch(DSLExpression value, SkTArray<DSLCase> cases);

/**
 * switch (value) { cases }
 */
template<class... Cases>
DSLPossibleStatement Switch(DSLExpression value, Cases... cases) {
    SkTArray<DSLCase> caseArray;
    caseArray.reserve_back(sizeof...(cases));
    (caseArray.push_back(std::move(cases)), ...);
    return Switch(std::move(value), std::move(caseArray));
}

/**
 * while (test) stmt;
 */
DSLStatement While(DSLExpression test, DSLStatement stmt, PositionInfo info = PositionInfo());

/**
 * expression.xyz1
 */
DSLExpression Swizzle(DSLExpression base,
                      SkSL::SwizzleComponent::Type a,
                      PositionInfo pos = PositionInfo());

DSLExpression Swizzle(DSLExpression base,
                      SkSL::SwizzleComponent::Type a,
                      SkSL::SwizzleComponent::Type b,
                      PositionInfo pos = PositionInfo());

DSLExpression Swizzle(DSLExpression base,
                      SkSL::SwizzleComponent::Type a,
                      SkSL::SwizzleComponent::Type b,
                      SkSL::SwizzleComponent::Type c,
                      PositionInfo pos = PositionInfo());

DSLExpression Swizzle(DSLExpression base,
                      SkSL::SwizzleComponent::Type a,
                      SkSL::SwizzleComponent::Type b,
                      SkSL::SwizzleComponent::Type c,
                      SkSL::SwizzleComponent::Type d,
                      PositionInfo pos = PositionInfo());

/**
 * Returns the absolute value of x. If x is a vector, operates componentwise.
 */
DSLExpression Abs(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns true if all of the components of boolean vector x are true.
 */
DSLExpression All(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns true if any of the components of boolean vector x are true.
 */
DSLExpression Any(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the arctangent of y over x. Operates componentwise on vectors.
 */
DSLExpression Atan(DSLExpression y_over_x, PositionInfo pos = PositionInfo());
DSLExpression Atan(DSLExpression y, DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns x rounded towards positive infinity. If x is a vector, operates componentwise.
 */
DSLExpression Ceil(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns x clamped to between min and max. If x is a vector, operates componentwise.
 */
DSLExpression Clamp(DSLExpression x, DSLExpression min, DSLExpression max,
                    PositionInfo pos = PositionInfo());

/**
 * Returns the cosine of x. If x is a vector, operates componentwise.
 */
DSLExpression Cos(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the cross product of x and y.
 */
DSLExpression Cross(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns x converted from radians to degrees. If x is a vector, operates componentwise.
 */
DSLExpression Degrees(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the distance between x and y.
 */
DSLExpression Distance(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns the dot product of x and y.
 */
DSLExpression Dot(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns a boolean vector indicating whether components of x are equal to the corresponding
 * components of y.
 */
DSLExpression Equal(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns e^x. If x is a vector, operates componentwise.
 */
DSLExpression Exp(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns 2^x. If x is a vector, operates componentwise.
 */
DSLExpression Exp2(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * If dot(i, nref) >= 0, returns n, otherwise returns -n.
 */
DSLExpression Faceforward(DSLExpression n, DSLExpression i, DSLExpression nref,
                          PositionInfo pos = PositionInfo());

/**
 * Returns x rounded towards negative infinity. If x is a vector, operates componentwise.
 */
DSLExpression Floor(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the fractional part of x. If x is a vector, operates componentwise.
 */
DSLExpression Fract(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns a boolean vector indicating whether components of x are greater than the corresponding
 * components of y.
 */
DSLExpression GreaterThan(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns a boolean vector indicating whether components of x are greater than or equal to the
 * corresponding components of y.
 */
DSLExpression GreaterThanEqual(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns the 1/sqrt(x). If x is a vector, operates componentwise.
 */
DSLExpression Inversesqrt(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the inverse of the matrix x.
 */
DSLExpression Inverse(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the length of the vector x.
 */
DSLExpression Length(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns a boolean vector indicating whether components of x are less than the corresponding
 * components of y.
 */
DSLExpression LessThan(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns a boolean vector indicating whether components of x are less than or equal to the
 * corresponding components of y.
 */
DSLExpression LessThanEqual(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns the log base e of x. If x is a vector, operates componentwise.
 */
DSLExpression Log(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the log base 2 of x. If x is a vector, operates componentwise.
 */
DSLExpression Log2(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the larger (closer to positive infinity) of x and y. If x is a vector, operates
 * componentwise. y may be either a vector of the same dimensions as x, or a scalar.
 */
DSLExpression Max(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns the smaller (closer to negative infinity) of x and y. If x is a vector, operates
 * componentwise. y may be either a vector of the same dimensions as x, or a scalar.
 */
DSLExpression Min(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns a linear intepolation between x and y at position a, where a=0 results in x and a=1
 * results in y. If x and y are vectors, operates componentwise. a may be either a vector of the
 * same dimensions as x and y, or a scalar.
 */
DSLExpression Mix(DSLExpression x, DSLExpression y, DSLExpression a,
                  PositionInfo pos = PositionInfo());

/**
 * Returns x modulo y. If x is a vector, operates componentwise. y may be either a vector of the
 * same dimensions as x, or a scalar.
 */
DSLExpression Mod(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns the vector x normalized to a length of 1.
 */
DSLExpression Normalize(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns a boolean vector indicating whether components of x are not equal to the corresponding
 * components of y.
 */
DSLExpression NotEqual(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns x raised to the power y. If x is a vector, operates componentwise. y may be either a
 * vector of the same dimensions as x, or a scalar.
 */
DSLExpression Pow(DSLExpression x, DSLExpression y, PositionInfo pos = PositionInfo());

/**
 * Returns x converted from degrees to radians. If x is a vector, operates componentwise.
 */
DSLExpression Radians(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns i reflected from a surface with normal n.
 */
DSLExpression Reflect(DSLExpression i, DSLExpression n, PositionInfo pos = PositionInfo());

/**
 * Returns i refracted across a surface with normal n and ratio of indices of refraction eta.
 */
DSLExpression Refract(DSLExpression i, DSLExpression n, DSLExpression eta,
                      PositionInfo pos = PositionInfo());

/**
 * Samples the child processor at the current coordinates.
 */
DSLExpression Sample(DSLExpression fp, PositionInfo pos = PositionInfo());

/**
 * Implements the following functions:
 *     half4 sample(fragmentProcessor fp, float2 coords);
 *     half4 sample(fragmentProcessor fp, half4 input);
 */
DSLExpression Sample(DSLExpression target, DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Implements the following functions:
 *     half4 sample(fragmentProcessor fp, float2 coords, half4 input);
 */
DSLExpression Sample(DSLExpression childProcessor, DSLExpression x, DSLExpression y,
                     PositionInfo pos = PositionInfo());

/**
 * Returns x clamped to the range [0, 1]. If x is a vector, operates componentwise.
 */
DSLExpression Saturate(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns -1, 0, or 1 depending on whether x is negative, zero, or positive, respectively. If x is
 * a vector, operates componentwise.
 */
DSLExpression Sign(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the sine of x. If x is a vector, operates componentwise.
 */
DSLExpression Sin(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns a smooth interpolation between 0 (at x=edge1) and 1 (at x=edge2). If x is a vector,
 * operates componentwise. edge1 and edge2 may either be both vectors of the same dimensions as x or
 * scalars.
 */
DSLExpression Smoothstep(DSLExpression edge1, DSLExpression edge2, DSLExpression x,
                         PositionInfo pos = PositionInfo());

/**
 * Returns the square root of x. If x is a vector, operates componentwise.
 */
DSLExpression Sqrt(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns 0 if x < edge or 1 if x >= edge. If x is a vector, operates componentwise. edge may be
 * either a vector of the same dimensions as x, or a scalar.
 */
DSLExpression Step(DSLExpression edge, DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns the tangent of x. If x is a vector, operates componentwise.
 */
DSLExpression Tan(DSLExpression x, PositionInfo pos = PositionInfo());

/**
 * Returns x converted from premultipled to unpremultiplied alpha.
 */
DSLExpression Unpremul(DSLExpression x, PositionInfo pos = PositionInfo());

} // namespace dsl

} // namespace SkSL

#endif
