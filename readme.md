# Summer-of-Bitcoin-Challenge

### **Parsing the File and Toplogical Sort**

1. I started by taking input from the CSV file. I created Two Structures for the data and added some essential Data Structures to store the relevant data which came useful in the later part of the code .

2. To solve the problem , the very first thing after reading the data is to perform Toplogical Sort by an algorithm of your choice ( I used Kahn's Algorithm ) , so that all the transactions are ordered correctly and we can proceed further .

---

### **Approaches to the Solution**

3. A naive approach to solve this problem would be use Recursion , and consider all the possible states
   of taking a particular transaction or not , but this will be of O(2<sup>n</sup>, where n is the
   given the number of transactions) , which is impractical

4. If this was a tradition 0/1 Knapsack problem , then we could have optimized it using Dynamic Programming , but here since we have to check all the visited parents , the best possible optimization would be to use
   DP with Bitmasking where we maintain a mask such that (we can also keep a visited array for the same) to check wether all of our parents are visited , but given the samples , there will be 2<sup>5000</sup> order of unique states , which we can't store in our array .

5. Now , inorder to find the result , we will try a greedy algorithm along with the use of traditional Knapsack <br/>
6. The approach goes something like , first we find the total average ratio (fees/weight) of each Transaction .
7. Then we run a loop from (0.1 to Average_total_ratio) and find at which ratio , we obtain the answer under the given constraints .

8. The only demerit of this approach is that we are rejecting all those cases where we could have better answers with ratio greater than avg_total_ratio .

---

### **Conclusion**

My result :<br/>
Final maximum Fees : 5669422 <br/>
Final Weight to get this Fees : 3880668<br/>
