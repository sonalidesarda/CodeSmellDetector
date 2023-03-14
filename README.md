Code Smell Detector to detect the following code smells:  
1.  Long Method/Function 
    In the context of this homework, we define 15 Lines of Code as the threshold for a long method/function. In other words, if a method/function contains 16 or more lines of code, we will say the code smell Long Method/Function exists. Please notice that blank line is NOT considered as one line of code. 
 
2.  Long Parameter List 
    In the context of this homework, we define 3 Parameters as the threshold for a long parameter list. In other words, if a method/function contains 4 or more parameters, we will say this method/function has a long parameter list. 
 
3.  Duplicated Code 
    As we discussed in this class, there are different types of duplicated code. In the context of this homework, you DO NOT need to consider semantic duplicated code. In other words, one way to achieve this is to convert your two code fragments to strings and compare the similarity between the two strings. When you use similarity metrics, we define 0.75 as the threshold. There are several metrics you can use here, such as Jaccard Similarity.  

I have facilitated the user experience, provided a user interface for the following functions: 
1.  Given a list of all the functions in the test file before conducting code smell detection. 
 
2. Given the user option to choose one of the following actions: 
- Detect whether long method/function exists in ANY of the functions on the function list. If it does, which methods/function? 
- Detect whether Long Parameter List exists in ANY of the functions on the function list. If it does, which methods/function? 
- Detect whether Duplicated Code exists in ANY of the functions on the function list. If it does, which two methods/function are duplicated?
 
Assumptions:
1. no comments provided in the cpp file

Command to compile cpp program in mac:

g++ -std=c++11 CodeSmellDetector.cpp -o CodeSmellDetector

To run the code smell detector program:

./CodeSmellDetector filetotest.cpp