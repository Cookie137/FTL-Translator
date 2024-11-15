#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace elangRPN {

enum class TokenType {
  Operand,
  Operator,
  Function,
  ControlFlow,
  Variable,
};

enum class OperatorType {
  Add,
  Subtract,
  Multiply,
  Divide,
  Modulo,
  Equal,
  NotEqual,
  LessThan,
  GreaterThan,
  LessEqual,
  GreaterEqual,
  LogicalAnd,
  LogicalOr,
  Negate,
  LogicalNot,
  Duplicate,
  Assign,
  AccessProperty,
  FunctionCall,
};

enum class ControlFlowType {
  If,
  Else,
  EndIf,
  While,
  EndWhile,
};

class Expression;
class ElgObject;

class ElgPrimitive {
 public:
  class Null {};
  class Undefined {};
  class Function {
   public:
    std::vector<std::string> parameters;
    std::shared_ptr<Expression> expression;
    std::string name;  // Added to support recursion

    Function(const std::string& funcName,
             const std::vector<std::string>& params,
             std::shared_ptr<Expression> expr)
        : name(funcName), parameters(params), expression(expr) {}
  };

  using ElgPrimitiveValue =
      std::variant<int, float, std::string, Null, Undefined, Function>;

  ElgPrimitive(ElgPrimitiveValue value) : value(value) {}

  ElgPrimitiveValue value;
};

using ElgObjectValue =
    std::variant<std::shared_ptr<ElgPrimitive>,
                 std::unordered_map<std::string, std::shared_ptr<ElgObject>>>;

class ElgObject {
 public:
  ElgObjectValue value;

  ElgObject(ElgObjectValue val) : value(val) {}
  ElgObject() {
    value = std::make_shared<ElgPrimitive>(ElgPrimitive::Undefined());
  }
};

class Token {
 public:
  TokenType type;
  std::variant<ElgObject, OperatorType, ControlFlowType, std::string> value;
};

class Node {
 public:
  virtual ~Node() {}
};

class Expression : public Node {
 public:
  std::vector<Token> tokens;
};

class Context {
 private:
  Context* parentContext_;
  std::unordered_map<std::string, std::shared_ptr<ElgObject>> variables_;
  std::optional<ElgObject> returnValue_;

 public:
  Context(Context* parentContext = nullptr) : parentContext_(parentContext) {}

  void setVariable(const std::string& name,
                   const std::shared_ptr<ElgObject>& object) {
    variables_[name] = object;
  }

  std::shared_ptr<ElgObject> getVariable(const std::string& name) {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
      return it->second;
    } else if (parentContext_) {
      return parentContext_->getVariable(name);
    } else {
      return std::make_shared<ElgObject>(
          std::make_shared<ElgPrimitive>(ElgPrimitive::Undefined()));
    }
  }

  void setReturnValue(const ElgObject& object) { returnValue_ = object; }
  std::optional<ElgObject> getReturnValue() { return returnValue_; }
};

class Interpreter {
 public:
  Interpreter(Context* globalContext) : globalContext_(globalContext) {
    initBuiltInFunctions();
  }

  std::shared_ptr<ElgObject> evaluateExpression(Expression* expr,
                                                Context* context) {
    std::vector<std::shared_ptr<ElgObject>> stack;
    return evaluateExpression(expr, context, stack);
  }

 private:
  Context* globalContext_;
  std::unordered_map<std::string, std::shared_ptr<ElgObject>> builtInFunctions_;

  void initBuiltInFunctions() {
    builtInFunctions_["print"] =
        std::make_shared<ElgObject>(std::make_shared<ElgPrimitive>("print"));
  }

  std::shared_ptr<ElgObject> evaluateExpression(
      Expression* expr, Context* context,
      std::vector<std::shared_ptr<ElgObject>>& stack) {
    size_t i = 0;
    std::vector<size_t> loopStack;

    while (i < expr->tokens.size()) {
      const Token& token = expr->tokens[i];
      switch (token.type) {
        case TokenType::Operand: {
          ElgObject operand = std::get<ElgObject>(token.value);
          stack.push_back(std::make_shared<ElgObject>(operand));
          break;
        }
        case TokenType::Variable: {
          std::string varName = std::get<std::string>(token.value);
          auto varObj = context->getVariable(varName);
          stack.push_back(varObj);
          break;
        }
        case TokenType::Operator: {
          OperatorType opType = std::get<OperatorType>(token.value);
          if (opType == OperatorType::Assign) {
            if (stack.size() < 2) {
              std::cerr << "Not enough operands for assignment." << std::endl;
              return nullptr;
            }
            auto valueObj = stack.back();
            stack.pop_back();
            auto varNameObj = stack.back();
            stack.pop_back();

            auto varNamePrimitive =
                std::get<std::shared_ptr<ElgPrimitive>>(varNameObj->value);
            if (auto varNameStr =
                    std::get_if<std::string>(&varNamePrimitive->value)) {
              context->setVariable(*varNameStr, valueObj);
            } else {
              std::cerr << "Invalid variable name for assignment." << std::endl;
              return nullptr;
            }
          } else if (opType == OperatorType::AccessProperty) {
            if (stack.size() < 2) {
              std::cerr << "Not enough operands for property access."
                        << std::endl;
              return nullptr;
            }
            auto propertyNameObj = stack.back();
            stack.pop_back();
            auto objectObj = stack.back();
            stack.pop_back();

            auto propertyNamePrimitive =
                std::get<std::shared_ptr<ElgPrimitive>>(propertyNameObj->value);
            if (auto propertyNameStr =
                    std::get_if<std::string>(&propertyNamePrimitive->value)) {
              auto objectValue = objectObj->value;
              if (auto objectMap = std::get_if<std::unordered_map<
                      std::string, std::shared_ptr<ElgObject>>>(&objectValue)) {
                auto it = objectMap->find(*propertyNameStr);
                if (it != objectMap->end()) {
                  stack.push_back(it->second);
                } else {
                  // Return undefined object when property not found
                  stack.push_back(std::make_shared<ElgObject>(
                      std::make_shared<ElgPrimitive>(
                          ElgPrimitive::Undefined())));
                }
              } else {
                // Return undefined object when accessing property of non-object
                stack.push_back(std::make_shared<ElgObject>(
                    std::make_shared<ElgPrimitive>(ElgPrimitive::Undefined())));
              }
            } else {
              std::cerr << "Invalid property name for access." << std::endl;
              return nullptr;
            }
          } else if (opType == OperatorType::FunctionCall) {
            if (stack.empty()) {
              std::cerr << "Stack underflow: no function to call." << std::endl;
              return nullptr;
            }
            auto functionObj = stack.back();
            stack.pop_back();

            auto functionPrimitive =
                std::get<std::shared_ptr<ElgPrimitive>>(functionObj->value);

            if (std::holds_alternative<ElgPrimitive::Function>(
                    functionPrimitive->value)) {
              auto function =
                  std::get<ElgPrimitive::Function>(functionPrimitive->value);
              auto result = callFunction(function, stack, context);
              if (result) {
                stack.push_back(result);
              }
            } else if (auto funcName = std::get_if<std::string>(
                           &functionPrimitive->value)) {
              // Handle built-in functions
              if (*funcName == "print") {
                if (stack.empty()) {
                  std::cerr << "Stack underflow: no value to print."
                            << std::endl;
                  return nullptr;
                }
                auto valueObj = stack.back();
                stack.pop_back();
                printValue(valueObj);
                // Optionally push undefined or the printed value onto the stack
                stack.push_back(std::make_shared<ElgObject>(
                    std::make_shared<ElgPrimitive>(ElgPrimitive::Undefined())));
              } else {
                // Try to get function from context (for recursion)
                auto funcObj = context->getVariable(*funcName);
                auto funcPrimitive =
                    std::get<std::shared_ptr<ElgPrimitive>>(funcObj->value);
                if (std::holds_alternative<ElgPrimitive::Function>(
                        funcPrimitive->value)) {
                  // Push back function and call it
                  stack.push_back(funcObj);
                  // Since we already popped arguments earlier, we need to
                  // adjust the stack
                  auto result = callFunction(
                      std::get<ElgPrimitive::Function>(funcPrimitive->value),
                      stack, context);
                  if (result) {
                    stack.push_back(result);
                  }
                } else {
                  std::cerr << "Unknown function: " << *funcName << std::endl;
                  return nullptr;
                }
              }
            } else {
              // Return undefined if trying to call a non-function
              stack.push_back(std::make_shared<ElgObject>(
                  std::make_shared<ElgPrimitive>(ElgPrimitive::Undefined())));
            }
          } else {
            auto result = applyOperator(opType, stack);
            if (result) {
              stack.push_back(result);
            } else {
              // If operation fails due to Undefined operands, push Undefined
              stack.push_back(std::make_shared<ElgObject>(
                  std::make_shared<ElgPrimitive>(ElgPrimitive::Undefined())));
            }
          }
          break;
        }
        case TokenType::Function: {
          // This case is no longer used
          break;
        }
        case TokenType::ControlFlow: {
          // Control flow handling remains the same
          ControlFlowType cfType = std::get<ControlFlowType>(token.value);
          if (cfType == ControlFlowType::While) {
            // Push current position onto loop stack
            loopStack.push_back(i);
          } else if (cfType == ControlFlowType::EndWhile) {
            if (loopStack.empty()) {
              std::cerr << "EndWhile without matching While." << std::endl;
              return nullptr;
            }
            size_t loopStart = loopStack.back();
            // Evaluate condition
            if (stack.empty()) {
              std::cerr << "Stack underflow in While condition." << std::endl;
              return nullptr;
            }
            auto conditionObj = stack.back();
            stack.pop_back();
            bool condition = isTruthy(conditionObj);

            if (condition) {
              // Jump back to the start of the loop
              i = loopStart;
            } else {
              // Exit loop
              loopStack.pop_back();
            }
          } else if (cfType == ControlFlowType::If) {
            // Pop condition from the stack
            if (stack.empty()) {
              std::cerr << "Stack underflow in If condition." << std::endl;
              return nullptr;
            }
            auto conditionObj = stack.back();
            stack.pop_back();
            bool condition = isTruthy(conditionObj);

            if (!condition) {
              // Skip to Else or EndIf
              size_t depth = 1;
              while (depth > 0 && ++i < expr->tokens.size()) {
                const Token& nextToken = expr->tokens[i];
                if (nextToken.type == TokenType::ControlFlow) {
                  ControlFlowType nextCfType =
                      std::get<ControlFlowType>(nextToken.value);
                  if (nextCfType == ControlFlowType::If) {
                    depth++;
                  } else if (nextCfType == ControlFlowType::EndIf) {
                    depth--;
                  } else if (nextCfType == ControlFlowType::Else &&
                             depth == 1) {
                    depth--;
                    break;
                  }
                }
              }
            }
          } else if (cfType == ControlFlowType::Else) {
            // Skip to EndIf
            size_t depth = 1;
            while (depth > 0 && ++i < expr->tokens.size()) {
              const Token& nextToken = expr->tokens[i];
              if (nextToken.type == TokenType::ControlFlow) {
                ControlFlowType nextCfType =
                    std::get<ControlFlowType>(nextToken.value);
                if (nextCfType == ControlFlowType::If) {
                  depth++;
                } else if (nextCfType == ControlFlowType::EndIf) {
                  depth--;
                }
              }
            }
          } else if (cfType == ControlFlowType::EndIf) {
            // Do nothing
          }
          break;
        }
        default:
          std::cerr << "Unknown token type." << std::endl;
          return nullptr;
      }
      i++;
    }

    if (!stack.empty()) {
      return stack.back();
    }
    return nullptr;
  }

  bool isTruthy(const std::shared_ptr<ElgObject>& obj) {
    auto primitive = std::get<std::shared_ptr<ElgPrimitive>>(obj->value);
    if (auto intVal = std::get_if<int>(&primitive->value)) {
      return (*intVal != 0);
    } else if (auto floatVal = std::get_if<float>(&primitive->value)) {
      return (*floatVal != 0.0f);
    } else if (auto strVal = std::get_if<std::string>(&primitive->value)) {
      return (!strVal->empty());
    } else if (std::holds_alternative<ElgPrimitive::Null>(primitive->value)) {
      return false;
    } else if (std::holds_alternative<ElgPrimitive::Undefined>(
                   primitive->value)) {
      return false;
    } else {
      return false;
    }
  }

  std::shared_ptr<ElgObject> applyOperator(
      OperatorType opType, std::vector<std::shared_ptr<ElgObject>>& stack) {
    if (opType == OperatorType::Negate || opType == OperatorType::LogicalNot ||
        opType == OperatorType::Duplicate) {
      if (stack.empty()) {
        std::cerr << "Not enough operands on the stack for unary operator."
                  << std::endl;
        return nullptr;
      }
      auto operand = stack.back();
      if (opType == OperatorType::Duplicate) {
        return operand;  // Return the same object to duplicate it on the stack
      }
      stack.pop_back();
      auto operandPrimitive =
          std::get<std::shared_ptr<ElgPrimitive>>(operand->value);

      if (auto intVal = std::get_if<int>(&operandPrimitive->value)) {
        int resultValue;
        if (opType == OperatorType::Negate) {
          resultValue = -(*intVal);
        } else if (opType == OperatorType::LogicalNot) {
          resultValue = !(*intVal);
        }
        return std::make_shared<ElgObject>(
            std::make_shared<ElgPrimitive>(resultValue));
      } else if (std::holds_alternative<ElgPrimitive::Undefined>(
                     operandPrimitive->value)) {
        // Return Undefined if operand is Undefined
        return std::make_shared<ElgObject>(operandPrimitive);
      }
      // Handle other types...
      std::cerr << "Unsupported operand type for unary operator." << std::endl;
      return nullptr;
    } else {
      if (stack.size() < 2) {
        std::cerr << "Not enough operands on the stack." << std::endl;
        return nullptr;
      }

      auto rhs = stack.back();
      stack.pop_back();
      auto lhs = stack.back();
      stack.pop_back();

      auto lhsPrimitive = std::get<std::shared_ptr<ElgPrimitive>>(lhs->value);
      auto rhsPrimitive = std::get<std::shared_ptr<ElgPrimitive>>(rhs->value);

      // Check if either operand is Undefined
      if (std::holds_alternative<ElgPrimitive::Undefined>(
              lhsPrimitive->value) ||
          std::holds_alternative<ElgPrimitive::Undefined>(
              rhsPrimitive->value)) {
        // Return Undefined if any operand is Undefined
        return std::make_shared<ElgObject>(
            std::make_shared<ElgPrimitive>(ElgPrimitive::Undefined()));
      }

      if (auto lhsInt = std::get_if<int>(&lhsPrimitive->value)) {
        if (auto rhsInt = std::get_if<int>(&rhsPrimitive->value)) {
          int resultValue;
          switch (opType) {
            case OperatorType::Add:
              resultValue = *lhsInt + *rhsInt;
              break;
            case OperatorType::Subtract:
              resultValue = *lhsInt - *rhsInt;
              break;
            case OperatorType::Multiply:
              resultValue = *lhsInt * *rhsInt;
              break;
            case OperatorType::Divide:
              if (*rhsInt == 0) {
                std::cerr << "Division by zero." << std::endl;
                return nullptr;
              }
              resultValue = *lhsInt / *rhsInt;
              break;
            case OperatorType::GreaterThan:
              resultValue = *lhsInt > *rhsInt;
              break;
            case OperatorType::LessThan:
              resultValue = *lhsInt < *rhsInt;
              break;
            case OperatorType::Equal:
              resultValue = *lhsInt == *rhsInt;
              break;
            case OperatorType::NotEqual:
              resultValue = *lhsInt != *rhsInt;
              break;
            case OperatorType::LessEqual:
              resultValue = *lhsInt <= *rhsInt;
              break;
            case OperatorType::GreaterEqual:
              resultValue = *lhsInt >= *rhsInt;
              break;
            default:
              std::cerr << "Unsupported operator." << std::endl;
              return nullptr;
          }
          return std::make_shared<ElgObject>(
              std::make_shared<ElgPrimitive>(resultValue));
        }
      }
      if (auto lhsStr = std::get_if<std::string>(&lhsPrimitive->value)) {
        if (auto rhsStr = std::get_if<std::string>(&rhsPrimitive->value)) {
          std::string resultValue;
          if (opType == OperatorType::Add) {
            resultValue = *lhsStr + *rhsStr;
            return std::make_shared<ElgObject>(
                std::make_shared<ElgPrimitive>(resultValue));
          }
          // Handle other string operations...
        }
      }

      // Handle other types (float) similarly...
      std::cerr << "Type mismatch or unsupported types." << std::endl;
      return nullptr;
    }
  }

  std::shared_ptr<ElgObject> callFunction(
      const ElgPrimitive::Function& function,
      std::vector<std::shared_ptr<ElgObject>>& stack, Context* parentContext) {
    // Create a new context for the function, with the parent context as the
    // base
    Context functionContext(parentContext);

    // Collect arguments from the stack and bind them to parameters
    if (stack.size() < function.parameters.size()) {
      std::cerr << "Not enough arguments for function call." << std::endl;
      return nullptr;
    }

    // Arguments are in reverse order on the stack
    std::vector<std::shared_ptr<ElgObject>> args(function.parameters.size());
    for (int i = function.parameters.size() - 1; i >= 0; --i) {
      args[i] = stack.back();
      stack.pop_back();
    }

    // Set parameters in the function context
    for (size_t i = 0; i < function.parameters.size(); ++i) {
      functionContext.setVariable(function.parameters[i], args[i]);
    }

    // Store a reference to the function itself in the context (for recursion)
    functionContext.setVariable(
        function.name,
        std::make_shared<ElgObject>(std::make_shared<ElgPrimitive>(function)));

    // Evaluate the function's expression in the new context
    auto result =
        evaluateExpression(function.expression.get(), &functionContext);

    return result ? result
                  : std::make_shared<ElgObject>(std::make_shared<ElgPrimitive>(
                        ElgPrimitive::Undefined()));
  }

  void printValue(const std::shared_ptr<ElgObject>& obj) {
    auto primitive = std::get<std::shared_ptr<ElgPrimitive>>(obj->value);
    if (auto intVal = std::get_if<int>(&primitive->value)) {
      std::cout << *intVal << std::endl;
    } else if (auto floatVal = std::get_if<float>(&primitive->value)) {
      std::cout << *floatVal << std::endl;
    } else if (auto strVal = std::get_if<std::string>(&primitive->value)) {
      std::cout << *strVal << std::endl;
    } else if (std::holds_alternative<ElgPrimitive::Null>(primitive->value)) {
      std::cout << "null" << std::endl;
    } else if (std::holds_alternative<ElgPrimitive::Undefined>(
                   primitive->value)) {
      std::cout << "undefined" << std::endl;
    } else {
      std::cout << "Unknown value" << std::endl;
    }
  }
};

// Global context instance
Context globalContext;

// Example usage
void example() {
  // Create an interpreter instance with the global context
  Interpreter interpreter(&globalContext);

  // Function definition: function factorial(n) { if n <= 1 { 1 } else { n *
  // factorial(n - 1) } } Represented in RPN:
  // ["factorial", ["n"], function_body_expression, Assign]

  // Function body expression:
  auto functionBody = std::make_shared<Expression>();
  // n 1 <= If
  functionBody->tokens.push_back(Token{TokenType::Variable, std::string("n")});
  functionBody->tokens.push_back(
      Token{TokenType::Operand, ElgObject(std::make_shared<ElgPrimitive>(1))});
  functionBody->tokens.push_back(
      Token{TokenType::Operator, OperatorType::LessEqual});
  functionBody->tokens.push_back(
      Token{TokenType::ControlFlow, ControlFlowType::If});
  // Then: Push 1
  functionBody->tokens.push_back(
      Token{TokenType::Operand, ElgObject(std::make_shared<ElgPrimitive>(1))});
  functionBody->tokens.push_back(
      Token{TokenType::ControlFlow, ControlFlowType::Else});
  // Else:
  // n n 1 - factorial FunctionCall *
  functionBody->tokens.push_back(Token{TokenType::Variable, std::string("n")});
  functionBody->tokens.push_back(Token{TokenType::Variable, std::string("n")});
  functionBody->tokens.push_back(
      Token{TokenType::Operand, ElgObject(std::make_shared<ElgPrimitive>(1))});
  functionBody->tokens.push_back(
      Token{TokenType::Operator, OperatorType::Subtract});
  functionBody->tokens.push_back(
      Token{TokenType::Variable, std::string("factorial")});
  functionBody->tokens.push_back(
      Token{TokenType::Operator, OperatorType::FunctionCall});
  functionBody->tokens.push_back(
      Token{TokenType::Operator, OperatorType::Multiply});
  functionBody->tokens.push_back(
      Token{TokenType::ControlFlow, ControlFlowType::EndIf});

  // Create function object with parameter ["n"]
  std::vector<std::string> parameters = {"n"};
  auto functionObj = std::make_shared<ElgObject>(std::make_shared<ElgPrimitive>(
      ElgPrimitive::Function("factorial", parameters, functionBody)));

  // Assign function to variable "factorial": "factorial" functionObj Assign
  Expression funcDefExpr;
  funcDefExpr.tokens.push_back(
      Token{TokenType::Operand,
            ElgObject(std::make_shared<ElgPrimitive>("factorial"))});
  funcDefExpr.tokens.push_back(Token{TokenType::Operand, *functionObj});
  funcDefExpr.tokens.push_back(
      Token{TokenType::Operator, OperatorType::Assign});

  // Evaluate the function definition
  interpreter.evaluateExpression(&funcDefExpr, &globalContext);

  // Function call: 5 factorial FunctionCall
  Expression funcCallExpr;
  funcCallExpr.tokens.push_back(
      Token{TokenType::Operand, ElgObject(std::make_shared<ElgPrimitive>(7))});
  funcCallExpr.tokens.push_back(
      Token{TokenType::Variable, std::string("factorial")});
  funcCallExpr.tokens.push_back(
      Token{TokenType::Operator, OperatorType::FunctionCall});

  // Evaluate the function call
  auto funcCallResult =
      interpreter.evaluateExpression(&funcCallExpr, &globalContext);

  // Print the result: print(result)
  if (funcCallResult) {
    // Push 'print' function onto the stack
    Expression printExpr;
    printExpr.tokens.push_back(Token{TokenType::Operand, *funcCallResult});
    printExpr.tokens.push_back(
        Token{TokenType::Operand,
              ElgObject(std::make_shared<ElgPrimitive>("print"))});
    printExpr.tokens.push_back(
        Token{TokenType::Operator, OperatorType::FunctionCall});

    interpreter.evaluateExpression(&printExpr, &globalContext);
    // Expected output: 120
  }
}

}  // namespace elangRPN
