# Gaussian
Projects employing Gaussian Elimination

Balancer.c++
- Enter a chemical equation lacking coefficients into the command line  
! Currently lacks error checking and support for parentheses  
Ex.  
input: SiO2 + HF > SiF4 + H2O  
output: 1SiO2 + 4HF > SiF4 + H2O  

// Problem obtained from https://www.northallegheny.org/cms/lib/PA01001119/Centricity/Domain/1083/balancingpractice.pdf

System Solver
- Enter a simple linear system of equations that equal a constant to obtain the number of solutions

Ex1.  
input:  
2x + 3y = 12  
x + y = 5  
output:  
One Solution as Follows:  
x = 3  
y = 2  

Ex2.  
input:  
2x + 3y = 12  
4x + 6y = 24  
output:  
Many Solutions. One Possible Solution:  
x = 4.5  
y = 1  

Ex3.  
input:  
2x + 3y = 12   
2x + 3y = 24  
output:  
No Solutions; this system of equations is inconsistent  
