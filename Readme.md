#EmergenC
##Author
Sean Sweet
##Overview
EmegenC is a C/Rust based general purpose language that was designed with a more simple syntax and less code.
Like C++, EmergenC is a statically typed language.  Unlike C++, EmergenC does not have different number types.
Instead, all numbers are the same type.  This is done to make the language more simple and have fewer type
conversions.  In the simple interpreter I created, classes and arrays are not supported.  If I had managed my time better,
it would have been possible, but I was unable to get it working.  For this language, I kept the brackets
because without them, the code is harder to read, and trying to copy and paste code can lead to things not being
indented correctly, completely breaking the code.  The semicolons were kept mainly to make parsing easier.
#Grammar
##Keywords
I tried to have as few keywords as possible.  I used the same keywords as C++, but I made some changes.
The keywords are:
* num
* bool
* string
* class (not actually implemented yet)
* if
* else
* while
* return
* break
* continue
* print
* var
* def
* true
* false
##Variable, Function, and class Declarations
Variable: 

`var name: type = value;`

Function: 

```
def name(args) returnType
{
    body
}
```
where args is a comma separated list of arguments with the syntax `name: type`

Class: `class name { body }`
where body is a list of function and variable declarations (not including the assignment part for the variables).
for class functions, the syntax is `def name(args): { body }` for static functions and `def name(self, args): { body }` for
instance functions.
##Left-Hand Side Operations
These are built in features that must appear on the left most side of an expression (if at all) such as declaring and assigning variables, printing, and returning.
It also includes if statements, while loops, and break and continue statements.
Lastly, it includes function and class declarations.
##Expressions
Expressions are the main part of the language.  They are made up of variables separated by operators.  The variables
can be literals, stored variables, or function calls.  The order of operations in this language is:
1. Function calls and Parenthesis
2. Multiplication and Division
3. Addition and Subtraction
4. Comparison
where each stage is performed right to left.
#Examples of Syntax
This is a simple recursive function that returns the nth fibonacci number.  Afterwards, there is a function call
to print the 10th fibonacci number.
```
def fib(n: num) num
{
    if(n < 1) { return 0; }
    if(n < 2) { return 1; }
    return fib(n - 1) + fib(n - 2);
}

print fib(10);
```
This is an example of function overloading within the language.
```
def foo(x: num) num
{
    return fib(x);
}

def foo(x: string) string
{
    return x + x;
}
```
If `foo(5)` is called, the result will be 5, whereas if `foo("5")` is called, the result will be "55".

This language also has a scope system.
```
var x: num = 5;
{
    print x;
}
```
Is valid, whereas
```
{
    var x: num = 5;
}
print x;
```
Is not valid.
#Code Examples
##Example 1
```
def fib(x: num) num
{
    if(x < 1) { return 0; }
    if(x < 2) { return 1; }
    return fib(x - 1) + fib(x - 2);
}

var i: num = 0;
while(i <= 10)
{
    print fib(i);
    i = i + 1;
}
```
###Output
```
0
1
1.000000
2.000000
3.000000
5.000000
8.000000
13.000000
21.000000
34.000000
55.000000
```
##Example 2
```
def foo(x: num, y: num) num
{
    return x + y;
}

def foo(x: num, y: num, z:num) num
{
    return x + y + z;
}

def foo(x: string, y: string) string
{
    return x + y;
}

def foo(x: string, y: string, z:string) string
{
    return x + y + z;
}

print foo(1, 2);
print foo(1, 2, 3);
print foo("a", "b");
print foo("a", "b", "c");

var foo: num = 1;
print foo(foo, foo);
```
###Output
```
3.000000
6.000000
"ab"
"abc"
2.000000
```
This is actually valid code, as depending on the context, a name will be treated as a function or a variable.
as the parameters in the last print statement do not have parameter lists, they are treated as variables.
##Example 3
```
def foo(x: bool) bool
{
    return !x;
}

def bar(x: bool, y: bool) bool
{
    return (x && y) || ((!x) && (!y));
}

print foo(true);
print foo(false);
print bar(true, true);
print bar(true, false);
print bar(false, true);
print bar(false, false);
```
###Output
```
false
true
true
false
false
true
```
##Example 4
```
{
    def foo(x: num) num
    {
        return x;
    }
    print foo(1);
}

print foo(1);
```
###Output
```
1.000000
Variable Error: function call not found: foo
```
This occurs as the scope that foo was defined in has been closed, so it cannot be called.
(the Variable part in the error is for me to tell which source file the error occurred in,
this one was printed by Variable.cpp)