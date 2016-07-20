Jithendra Yella

Use the file - "Makefile" present to build the project

Command:

make - to build the project
make cl - to clean the project.

Use the command
perl difftest.pl -1 "./rpal -ast FILE" -2 "./p2 -ast FILE" -t ~/rpal/tests/
to test the project.

Did:
1. AST Generation.
2. Standardized tree is perfect - uncomment the command "printTree(myStack.top());" in the main function after
"subtreeTransformControl();" to view the standardized tree.
3. Control Strcutures are formed correctly - Uncomment the command "printControlStructs();" in the main function
after "formControlStructs();" to print and view the control structures.
4. Only implemented 8 rules in the CSE machine.

Didn't:
1. 9-13 rules in the CSE Machine.
2. Tuple handling.