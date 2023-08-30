# C++ 函式
## **目錄**
---
1. [C++ 函式基礎](#1-c-函式基礎)
2. [C++ 引數與參數](#2-c-引數與參數)
3. [C++ 不定長度引數](#3-c-不定長度引數)
4. [C++ 傳回值](#4-c-傳回值)
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
<hr/>  

## **2. C++ 引數與參數**  
提供給函式的資料稱為引數（argument），接受引數的稱為參數（parameter）：
```c++
int increment(int n) {
    // n 為參數
    n = n + 1;
    return n;
}

int main() {
    int x = 10;
    // x 為引數
    cout << increment(x) << endl;
    cout << x << endl;

    return 0;
}
```

C++ 預設引數可以在上方宣告函式原型時事先定義：
> 預設引數一旦出現在參數列，右邊的參數也必須設定預設引數，在使用預設引數時，必須注意配置順序；允許多次定義預設引數。
```c++
double area(double, double pi = 3.14159); 

int main() { 
    double r, pi; 

    cout << "輸入半徑與 PI: "; 
    cin >> r >> pi; 

    cout << "面積（自訂 PI）：" << area(r, pi) << endl
         << "面積（default）：" << area(r) << endl;

    return 0; 
} 

double area(double r, double pi) { 
    return r*r*pi; 
}
```

函式呼叫主要可分為傳值、傳址以及傳參考。
1. 傳值是指新增一塊全新記憶體位置給帶入引數值的參數，無關原本引數的記憶體位置。
    ```c++
    void swap(int x,int y) {
        int temp; 
        temp = x ; 
        x = y ; 
        y = temp ; 
    }
    ```
2. 傳址是指傳遞引數的記憶體位置，使函式內部可以修改引數的值。
    ```c++
    void swap(int *x,int y) {
        int temp; 
        temp = *x ;
        *x = *y ;
        y = temp ; 
    }
    ```
3. 傳參考與傳址在傳遞記憶體位址很類似，不過比較節省一點記憶體空間。
    ```c++
    void swap (int &x , int &y){ 
    　　　int temp = x; 
    　　　x = y;
    　　　y = temp;
    }
    ```
> 1. C++ 支援三種，C 和 Java 支援前兩種。  
> 2. 雖然JAVA把其中一種資料型態系統的名稱命名為reference type，但事實上它卻是使用pass by address。  
> 3. 因為JAVA的開發者們認為把操作記憶體的方式分成3種太過複雜了，而且在JAVA開發的年代記憶體已經愈來愈便宜，硬體效能愈來愈強，所以JAVA的開發者們覺得不支援pass by reference並不會造成什麼問題，而且他們認為pass by address的觀念反而更像真正的pass by reference
<hr/>  

## **3. C++ 不定長度引數**  
C++ 可以使用 C 風格的不定長度引數（但不建議），引入cstdarg 標頭檔
```c++
#include <iostream> 
#include <cstdarg> 
using namespace std; 

void foo(int, ...); 

int main() { 
    foo(3, 1.1, 2.2, 3.3);

    return 0; 
} 

void foo(int size, ...) { 
    va_list args; // 一個特殊的型態（type），在 va_start、 va_arg 與 va_end 三個巨集（macro）中當作參數使用。
    va_start(args, size); // 啟始不定長度引數的巨集，第一個引數是 va_list，第二個引數是最後一個具名參數。

    for(int i = 0; i < size; i++) {
        cout << va_arg(args, double) << endl; // 讀取不定長度引數的巨集。
    }

    va_end(args); // 終止不定長度引數的巨集。
}
```

當然也可以透過 vector 將參數 push 到 vector 後在傳入函式中。
```c++
#include <iostream>
#include <vector> 
using namespace std; 

void foo(vector<double>); 

int main() { 
    vector<double> args;

    args.push_back(1.1);
    args.push_back(2.2);
    args.push_back(3.3);

    foo(args);

    return 0; 
} 

void foo(vector<double> args) { 
    for(auto arg : args) {
       cout << arg << endl; 
    } 
}
```

C++ 中也透過可變參數模版（variadic template）來定義，initializer_list 定義於 initializer_list 標頭檔。
> 相較於 vector ，清單初始化 {} 會建立 initializer_list，而 vector 不過就是有個建構式，可以接受 initializer_list，才令 vector 也可以使用清單初始化。簡單來說，只是想定義不定長度引數時，initializer_list 就可以了，不過它包含的方法比較少。
```c++
#include <iostream>
#include <initializer_list> 
using namespace std; 

void foo(initializer_list<double>); 

int main() { 
    foo({1.1, 2.2, 3.3});

    return 0; 
} 

void foo(initializer_list<double> args) { 
    for(auto arg : args) {
       cout << arg << endl; 
    } 
}
```

<hr/>  

## **4. C++ 傳回值**  
定義函式時，必須定義傳回值型態，如果函式不傳回值，使用 void 表示不傳回任何數值；若定義了傳回值型態不為 void，函式最後要使用 return 傳回數值，否則編譯器失敗。

如果傳回位址，那麼傳回值型態可定義為指標型態，這代表著記憶體位址在函式執行完畢後，必須仍是有效的，也就是說這通常代表著，函式內動態配置記憶體。 

> 如果不是動態配置記憶體，代表傳回指標隨著函式結束而結束，傳回的指標沒有意義，可能導致存取錯誤。
```c++
// 傳回指標
int* makeArray(int m, int initial = 0) { 
    int *a = new int[m]; 
    for(int i = 0; i < m; i++) {
        a[i] = initial; 
    }
    return a; 
}
```
同樣，不應將區域變數以 lvalue 參考（&）傳回，或者將常量以 rvalue 參考（&&）傳回，因為函式執行完畢後，區域變數、常量的記憶體就會被回收了。 
```c++
// 錯誤示範-傳回區域參考
int& showError() {
    int a = 5;
    int &r = a;
    return r;
}
```
下列為傳回 lvalue 或 rvalue 參考程式碼：
> 儘管第二個函式中，lhs 型態是右值引用，但在函數内部右值引用仍會被當作左值引用来對待，因此需要使用 std::move 来將他轉換為右值引用。

```C++
// 傳回 lvalue 參考 
// 此處如果傳回不用參考，接收回傳值時需要額外使用變數去接（產生 string 內容不必要的複製）、或者rvalue reference去接。因為回傳的string 是暫存的、不是有名稱的物件）
// 例如：string &&s3 = longerStr(s1, s2); <-不使用參考的話
string& longerStr(string &s1, string &s2) {
    return s1.length() > s2.length() ? s1 : s2;
}

// 傳回 rvalue 參考 
string&& concat(string &&lhs, string &rhs) {
    lhs += rhs;
    return std::move(lhs);
}
```

