# ZINC
ZINC Is a Normal Compiler

## Development Instructions
* Before you start, run `make init` to generate the directories needed. `src` directory should only contain source files while `include` directory should only contain header files. You can make multiple directories with any structure you want.
* `g++` is used in the `Makefile`, along with `gdb` for debugging.
* To compile and run the program, run `./run.sh` for debugging mode, and `./run.sh -r` for release mode. See `./run.sh -h` for more options.

## Coding, syntax and supported commands
### Operators 
The following operators are supported :
 - Greater than or equal `>=`
 - Equal equal `==`
 - Equal `=`
 - Greater than `>`
 - Less than `<`
 - Less than or equal `<=`
 - Not equal `!=`
 - Addition(Plus)  `+`
 - Subtraction(Minus) `-`
 - Division(Slash) `/`
 - Multiplication(Asterisk) `*`
## 
### Comments 
`# write your comment like this :D`   
`# one line after the other`
##
### `PRINT`
for printing strings , literals or variables.

***examples:***
```
PRINT 13
PRINT "HELLO WORLD!"
```
### `LET`
to denote an expression whether it involves defining a variable , assignment or a mathematical expression.
```
LET a = 12
LET b = a + 1
LET c = a + b 
PRINT a
PRINT b
PRINT c
```

> Strings and string assignment are not supported yet    
> Something like `LET A = "Hello there"` will not work

### `INPUT`
accept input from the user into a variable, the `INPUT`command also handles variable definition for you.
```
PRINT "Please enter a number :"
INPUT a
PRINT "the number you entered is : "
PRINT a
```
### `IF` , `THEN` & `ENDIF`
```
#Simple conditional program
PRINT "Please enter a number"
INPUT A 
IF A >= 0 THEN
PRINT "The number you entered is positive"
ENDIF
```
### `ELSE`
```
# with else added
PRINT "Please enter a number"
INPUT A 
IF A > 0 THEN
PRINT "The number you entered is positive"
ELSE
PRINT "The number you entered is negative"
ENDIF
```
### `ELSEIF`
```
# with else if added
PRINT "Please enter a number"
INPUT A 
IF A > 0 THEN
PRINT "The number you entered is positive"
ELSEIF A < 0 THEN
PRINT "The number you entered is negative"
ELSE
PRINT "The number you entered is Zero"
ENDIF
```
### `WHILE` , `REPEAT` & `ENDWHILE`
```
# calculating the sum from 1 till 10
LET A = 1
LET SUM = 0 
WHILE A <= 10 REPEAT
LET SUM = SUM + A 
LET A = A + 1
ENDWHILE
PRINT "The sum is "
PRINT SUM
```
### `GOTO` & `LABEL`
`LABEL` identifies a location within source code that `GOTO` can jump to.
```
# a program that won't print anything when 0 is entered 
PRINT "Please enter any number other than zero"
INPUT num 
IF num == 0 THEN
GOTO END
ENDIF
# If the num is Zero
# this line will be skipped because of GOTO 
PRINT "The number is not Zero"
LABEL END
```

> Please note that you shouldn't have any variable definitions that `GOTO` can jump over, otherwise an error will be raised by the compiler

##
More examples are available in the repository , feel free to play around and give them a try :D.
