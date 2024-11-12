# FTL-Translator

**Грамматика v2.0**
```
Program = {Statement}

Statement = FunctionDeclaration
          | VariableDeclaration
          | StructDeclaration
          | ExceptionDeclaration
          | MiddlewareDeclaration
          | EndpointDeclaration
          | ApplyMiddleware
          | ExpressionStatement
          | ThrowStatement
          | ForLoop
          | WhileLoop

FunctionDeclaration = ["async"] "function" Identifier "(" [Parameters] ")" [":" Type] "{" {Statement} "}"

Parameters = Parameter {"," Parameter}
Parameter = Identifier ":" Type

VariableDeclaration = ("let" | "var") Identifier "=" Expression

StructDeclaration = "struct" Identifier "{" {StructField} "}"
StructField = Identifier ":" Type [","]

ExceptionDeclaration = "exception" Identifier "{" {ExceptionField} "}"
ExceptionField = Identifier ":" Type [","]

MiddlewareDeclaration = "middleware" Identifier "=" "(" [MiddlewareParameters] ")" "=>" "{" {Statement} "}"

MiddlewareParameters = Identifier {"," Identifier}

EndpointDeclaration = HTTPMethod Identifier "(" [MiddlewareList] ")" [EndpointParams] "{" 
                        [ResponseTypes] 
                      "}" "{" {Statement} "}"

HTTPMethod = "GET" | "POST" | "PUT" | "DELETE"

MiddlewareList = Identifier {"," Identifier}

EndpointParams = "[" ParameterList "]"
ParameterList = Parameter {"," Parameter}

ResponseTypes = {StatusCode ":" ResponseBody}
StatusCode = Integer
ResponseBody = "{" {ResponseField} "}"
ResponseField = Identifier ":" Type [","]

ApplyMiddleware = "apply" Identifier "->" EndpointPattern
EndpointPattern = Identifier ("." Identifier)* {"," Identifier ("." Identifier)*}

ExpressionStatement = Expression

ThrowStatement = "throw" Expression

ForLoop = "for" "(" Identifier "in" Expression ")" "{" {Statement} "}"

WhileLoop = "while" "(" Expression ")" "{" {Statement} "}"

Expression = Assignment
           | LogicalOr

Assignment = Identifier "=" Expression

LogicalOr = LogicalAnd {"||" LogicalAnd}

LogicalAnd = Equality {"&&" Equality}

Equality = Relational {("==" | "!=") Relational}

Relational = Additive {("<" | ">" | "<=" | ">=") Additive}

Additive = Multiplicative {("+" | "-") Multiplicative}

Multiplicative = Unary {("*" | "/" | "%") Unary}

Unary = ("!" | "-" ) Unary
      | Primary

Primary = Literal
        | Identifier
        | "(" Expression ")"
        | FunctionExpression
        | LambdaExpression
        | MatchExpression
        | CallExpression
        | AwaitExpression

FunctionExpression = "function" "(" [Parameters] ")" "{" {Statement} "}"

LambdaExpression = "(" [Parameters] ")" "=>" Expression

MatchExpression = "match" Expression "{" {MatchCase} "}"

MatchCase = Pattern "=>" Expression

CallExpression = Identifier "(" [Arguments] ")"

Arguments = Expression {"," Expression}

Pattern = Literal | Identifier | "_"

Literal = Integer | String | Boolean | ListLiteral | ObjectLiteral

ListLiteral = "[" [ExpressionList] "]"
ExpressionList = Expression {"," Expression}

ObjectLiteral = "{" [ObjectFieldList] "}"
ObjectFieldList = ObjectField {"," ObjectField}
ObjectField = Identifier ":" Expression

Type = "number" | "string" | "boolean" | "list" "<" Type ">"
     | Identifier
     | FunctionType

FunctionType = "(" [TypeList] ")" "->" Type
TypeList = Type {"," Type}

Identifier = /[a-zA-Z_][a-zA-Z0-9_]*/

Integer = /[0-9]+/

String = /"([^"\\]|\\.)*"/

Boolean = "true" | "false"
```
