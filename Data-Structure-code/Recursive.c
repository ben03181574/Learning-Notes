#include <stdio.h>

// n factorial - recursive
int factorial(int n){
    if(n == 0){
        return 1;
    }else{
        return n * F(n - 1);
    }
}

// fibonacc - recursive
int fibonacc_r(int n){
    if(n == 0){
        return 0;
    }else if(n == 1){
        return 1;
    }else{
        return F(n-1) + F(n-2);
    }
}
// fibonacc - iteration
int fibonacci_i(int n){
     int prev1 = 1;
     int prev2 = 0;
     int tmp = 0;
     
     if(n == 0){
        return prev2;
     }else if(n == 1){
        return prev1;
     }else{
        for(int i = 2; i < n; i++){
            tmp = prev1;
            prev1 = prev1 + prev2;
            prev2 = tmp;
        }
        return prev1 + prev2;
     }
}

// Binomial Coefficient - recursive
int binomial_r(int n, int m){
   if(n == m){
      return 1;
   }else if(m == 0){
      return 1;
   }else{
      return F(n - 1, m) + F(n - 1, m - 1);
   }
}

// // Binomial Coefficient - iteration
int binomial_i(int n, int m){
   int denominator = 1;
   int numerator_1 = 1;
   int numerator_2 = 1;
   if(n == m || m == 0){
      return 1;
   }else{
      for(int i = 1; i <= n; i++){
         denominator *= i;
         if(i <= m){
            numerator_1 *= i;
         }
         if(i <= n-m){
            numerator_2 *= i;
         }
      }
      return denominator/(numerator_1*numerator_2);
   }
}
// Ackerman's - recursive
int ackerman(int n, int m){
   if(n == 0){
      return m + 1;
   }else if(m == 0){
      return F(n - 1, 1);
   }else{
      return F(n - 1, F(n, m - 1));
   }
}
// GCD - recursive
int gcd_r(int n, int m){

   if(n % m == 0){
      return m;
   }else{
      return F(m, n % m);
   }
}

// GCD - iteration
int gcd_i(int n, int m){
   int large = (n > m) ? n : m;
   int small = (n > m) ? m : n;
   int tmp;

   while(large % small != 0){
      tmp = large;
      large = small;
      small = tmp % small;
   }
   return small;
}

// power - recursive
int power_r(int x, int y){
   if(y == 0){
      return 1;
   }else{
      return x * F(x, y - 1);
   }
}
// power - iteration
int power_i(int x, int y){
   int answer = 1;

   for(int i = 1; i <= y; i++){
      answer *= x;
   }
   return answer;
}
// Hanoi Tower - recursive
void han(int n,char src,char spare, char dest){
   if(n == 1){
      printf("嚙盤嚙締嚙緬 %c 嚙踝蕭嚙踝蕭 %c\n", src, dest);
   }    
   else{
      han(n - 1, src, dest, spare);
      printf("嚙盤嚙締嚙緬 %c 嚙踝蕭嚙踝蕭 %c\n", src, dest);
      han(n - 1, spare, src, dest);
   }    
}
// Permutation - recursive
void perm(char list[], int start, int end){
   if(start == end){
      for(int i = 0; i <= end; i++){
         printf("%c ", list[i]);
      }
      printf("\n");
   }else{
      for(int i = start; i <= end; i++){
         // swapt list[i], list[start]
         int tmp = list[i];
         list[i] = list[start];
         list[start] = tmp;
         // recrusive
         perm(list, start + 1, end);
         // swap back list[start], list[i]
         list[start] = list[i];
         list[i] = tmp;
      }
   }
}

int main(void) {
   // int n = 8;
   // int m = 11;
   // int answer = factorial(n, m);

   // printf("Input: %d, %d\n", n, m);
   // printf("Answer: %d", answer);

   // han(3, 'A', 'B', 'C');
   perm((char[]){'A','B','C','D'}, 0, 3);

   return 0;
}