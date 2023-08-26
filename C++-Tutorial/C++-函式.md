# C++ 函式
## **目錄**
---
1. [C++ 函式基礎](#1-c-函式基礎)

---  
<br/>  

## **1. C++ 函式基礎**  
函式的組成主要包括四個部份：返回值、函式名稱、參數列與函式主體
> C++ 中規定，如果函式是在 main 之後實作，必須在 main 之前進行宣告，否則會出現編譯錯誤。

此外，函式也可以透過建立標頭檔（math.h）、實作（math.cpp）來引入（main.cpp）函數，程式碼如下：
```c++
// math.h
int pow2(int); 
int pow(int, int);

// math.cpp
#include "math.h"
int pow2(int num) { 
    return num * num; 
} 
int pow(int n, int p) { 
    int r = 1; 
    for(int i = 0; i < p; i++) {
        r *= n;
    }
    return r; 
}

// main.cpp
#include <iostream>
#include "math.h"
using namespace std;
int main() {
    int num = 0;
    int power = 0; 
    cout << "輸入數值："; 
    cin >> num; 
    cout << "輸入次方："; 
    cin >> power; 
    cout << num << " 平方：" << pow2(num) << endl
         << num << " 的 " << power << " 次方：" 
         << pow(num, power) 
         << endl; 
    return 0;
}
```
> 由於 Visual Studio Code 的 Code Runner 預設只編譯執行一個程式檔案，因此切換到 TERMINAL ，接著將 math.cpp 與 main.cpp 分別編譯成中間過程檔（.o），最後再將所有中間過程檔連結成執行檔。  
>    `g++ -c math.cpp`  
>    `g++ -c main.cpp`  
>    `g++ -o main main.o math.o`  
>    `./main.exe`  
> 
>![image](./cpp-compile.png)

由於透過上方呼叫函式時會造成額外的資源負擔，因此可以「建議」編譯器設為 __inline__ 行內，若建議被採納，該函式會自動在呼叫點展現為程式碼（inline 函式通常直接在標頭檔中實作），而不是待編譯器需要其實作時才展開函式呼叫。
```c++
// math.h
inline int pow2(int num) { 
    return num * num; 
}
int pow(int, int);
```
> inline 函式只能建議編譯器，建議不一定被採納，例如遞迴函式無法在呼叫點展開，數千行的函式也不適合在呼叫點展開，如果編譯器拒絕將函式展開，會視為一般函式進行編譯，inline 的建議會被忽略。

而當如果函式夠簡單，簡單到編譯器可以推斷出傳回值，可以使用 __constexpr__ 修飾，呼叫這類函式時，若能推斷出值，編譯器就會用值來取代呼叫。
```c++
// math.h
constexpr int addOne(int n) {
    return n + 1;
}

// 編譯器直接推斷的 101 會取代 addOne 呼叫，也就是 constexpr int r = 101 來進行編譯
constexpr int r = addOne(100);
```
> 使用 constexpr 修飾的函式，通常寫在標頭檔中。

__函式重載（Overload）__，為類似功能的函式提供了統一名稱，透過參數列個數或型態的不同，由編譯器選擇要呼叫的函式。注意！傳回型態不能作為重載的依據。


```c++
int main() { 
    foo(10); 
    foo(20, 30); 

    return 0; 
} 

void foo(int x) { 
    cout << "引數：" << x << endl; 
} 

void foo(int x, int y) { 
    cout << "引數：" << x << " " << y << endl; 
}
```

__遞迴（Recursion）__ 是在函式中呼叫自身，呼叫者會先置入記憶體堆疊，被呼叫者執行完後，再從堆疊取出被置入的函式繼續執行。

> 開發者很容易在迴圈執行過多的任務，令迴圈難以閱讀、理解與維護，特別是令那些本質上可以分而治之的任務，難以抽取、平行化，或者令原始碼本質上其實重複的流程，難以辨識出來，進而使用迴圈來避免這些問題。
> 然而，遞迴函式會在記憶體中堆疊，語言會有堆疊的數量限制（可能導致 stack overflow），若遞迴最佳化能展開遞迴，轉為迴圈形式，可以避開這類限制。