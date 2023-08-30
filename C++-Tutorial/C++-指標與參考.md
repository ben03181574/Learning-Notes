# C++ 指標與參考
## **目錄**
---
1. [C++ 指標位置與運算](#1-c-指標位置與運算)
2. [C++ 雙重指標](#2-c-雙重指標)
3. [C++ 指標與陣列](#3-c-指標與陣列)
4. [C++ 指標與字元陣列/字串陣列](#4-c-指標與字元陣列字串陣列)
5. [C++ 動態記憶體配置](#5-c-動態記憶體配置)
6. [C++ 命令列引數](#6-c-命令列引數)
7. [C++ 參考](#7-c-參考)
---  
<br/>  

## **1. C++ 指標位置與運算**  
可以使用 & 取址運算子（Address-of operator），來得知變數的位址，也可以透過指標變數來直接儲存特定的記憶體位址，其利用提取 （Dereference）運算子 * 來提取指標儲存位址處的物件。
> 如果宣告指標變數時沒有宣告出值，導致存取未知的記憶體內容，會造成不可預知的結果，因此建議即使沒有初值，也須設定成0或者`nullptr`。
```c++
int main() { 
    int n = 10; 
    int *p = &n;

    cout << "指標 p 儲存的位址：" << p << endl
         << "提取 p 儲存位址處的物件：" << *p << endl;
    *p = 20; 
    cout << "n = " << n << endl // n = 20
         << "*p = " << *p << endl; // *p = 20

    return 0; 
    
}
```

當然如果變數是以 const 宣告，其指標也需是 const 型態指標，當然，其兩者皆不能異動位址處的資料。
> 注意！const int *p 宣告的 p 並不是常數，可以儲存不同的位址。  
```c++
int main() { 
    const int n = 10;
    const int m = 20;

    const int *p = &n;
    cout << p << endl;

    p = &m;
    cout << p << endl;

    return 0; 
}
```
如果想令指標儲存的值無法變動，必須建立指標常數：
```c++
int n = 10;
int m = 20;

int* const p = &n;
p = &m;  //  error: assignment of read-only variable 'p'
```

指標運算上加 1 ，表示前進一個資料型態的記憶體長度，例如若 int 長度為 4 個位元組，在 int* 型態的指標上加 1，表示在位址上前進 4 個位元組的長度：
```c++
int *p = 0;

    cout << "p 位址：" << p << endl // p 位址：0
         << "p + 1："  << p + 1 << endl // p + 1：0x4
         << "p + 2："  << p + 2 << endl; // p + 2：0x8

    return 0; 
``` 
<hr/>

## **2. C++ 雙重指標**  
```c++
int main() {
    // n 儲存了 10，n 的位址 0x61feb8，指定給 p1 儲存，而 p1 的位址是 0x61feb4，指定給 p2 儲存。
    int n = 10; 
    int *p1 = &n; 
    int **p2 = &p1;

    cout << "n 位址：" << p1 << endl // n 位址：0x61feb8
         << "p1 位址：" << p2 << endl;  // p1 位址：0x61feb4

    return 0; 
} 
```
<hr/>

## **3. C++ 指標與陣列** 
宣告陣列之後，使用到陣列變數時，會取得首元素的位址。而陣列索引其實是相對於首元素位址的位移量，這也就代表也可以利用指標運算來取出陣列的元素值。
```c++
char str[]="How are you?";
cout << str[0] << endl;    //H    　 (【索引值0號】內的值)
cout << str[1] << endl;    //o    　 (【索引值1號】內的值)
cout << str[2] << endl;    //w    　 (【索引值2號】內的值)
cout << str[3] << endl;    //(空白)　 (【索引值3號】內的值)
cout << str[4] << endl;    //a    　 (【索引值4號】內的值)

char* ptr = str;      //  宣告一個指向字元型態陣列的指標 ptr, ptr內容為str的位址
// 只有寫陣列名稱時，其代表陣列中的第一個元素的位址，亦即 char* ptr = &str[0];

cout << *ptr << endl;    //H    　 (【索引值0號】內的值)
cout << *(ptr+1) << endl;    //o    　 (【索引值1號】內的值)
cout << *(ptr+2) << endl;    //w    　 (【索引值2號】內的值)
cout << *(ptr+3) << endl;    //(空白)　 (【索引值3號】內的值)
cout << *(ptr+4) << endl;   
```

可以利用指標結合 sizeof 來計算陣列長度，或者也可以利用 begin() 與 end() 來計算，或者利用指標運算求得。
> 每個陣列元素的位址型態是 int*，這表示對它進行運算時，是以 int 長度為單位，而 arr 變數的位址處就是陣列資料的開端，&arr 型態會是int (\*)[LENGTH]，對它進行運算時，是以 LENGTH 個 int 長度為單位，因此 &arr + 1 的結果，會是陣列使用的空間後之位址，而 \*(&arr + 1) 的值型態會回到 int*，也就是最後一個元素後之位址，這時就可以與 int* 的 arr 進行相減，也就是與第一個元素之位址相減，就可以得到陣列長度了。    

> 重點是：對於同一個位址，指標的型態決定了該怎麼看得相對應相加、相減計算。如果將陣列變數指定給指標 p，就只是取得首元素位址並儲存在 p，這會令 sizeof、begin()、end()、以及透過指標運算來計算長度的方式失效。
```c++
int main() {
    constexpr int LENGTH = 5;
    int arr[LENGTH] = {10, 20, 30, 40, 50}; 

    cout << sizeof(arr)/sizeof(*arr) << endl;  // 顯示 20/4
    cout << end(arr) - begin(arr) << endl;     // 顯示 5
    cout << *(&arr + 1) - arr; <<endl // 顯示 5

    return 0; 
}
```
由上述可知，一維陣列變數的元素位址可以儲至 int* 指標，那當然二維陣列則可以看成數段一維陣列構成，不過必須宣告每段一維陣列的長度：
```c++
#include <typeinfo>
#include <iostream>
using namespace std;
int main() {
    int arr[2][3] = {{10, 20, 30}, {40, 50, 60}}; 
    int *p[2] = {arr[0],arr[1]};
    int (*p2)[3] = arr;// 此處 arr 又可代表 &arr[0]，其為陣列{10,20,30}的位址（型態為 int(*)[3]）

    cout << *(*(p+1)+1) <<endl; // 50
    cout << p2 <<endl;
    cout << *(*(p2+1)+1) <<endl; // 50

    //使用 auto 更為方便
    int arr2[1][2][3] = {{{10, 20, 30}, {40, 50, 60}}}; 
    auto p2 = arr2;
}
```
> __注意！__ *p[2] 是指有一長度為 2 的 __指標陣列__，而 (*p)[3] 則是指有一指標指向長度為 3 的整數陣列。

透過指標結合 begin(), end() 函式來尋訪（多維）陣列：
```c++
int maze[2][3] ={
                    {1, 2, 3},
                    {4, 5, 6}
                };
// for(auto row : maze) {
for(int (*it)[3] = begin(maze); it < end(maze); it++) {
    int *row = *it;
    for(int i = 0; i < 3; i++) {
        cout << row[i] << "\t"; 
    }
    cout << endl; 
} 
```
<hr/>

## **4. C++ 指標與字元陣列/字串陣列**
> C++ 中使用 C 風格字串是比較麻煩的，可以的話建議使用 string。

當嘗試直接印出字元或字串的位址時，最終會因出整串字串，原因在於[編譯器中的判斷](http://feislee.blogspot.com/2013/12/cout.html)。解決辦法即是將其指標轉換成 void *（強制編譯器執行參數為 void * 的 func）。
```c++
char name1[]="pckukcp";
cout << name1 << endl;    
// pckukcp
cout << (void *)name1 << endl;  
// 0x5ffe98 (強制印出位址)
cout << &name1[0] << endl;
// pckukcp
cout << &name1[3] << endl;
// ukcp
char d = 'A';
char *p = &d;
cout << p << endl;
// A (後接亂碼)
cout << (void *)p << endl;
// 0x5ffe97 (強制印出位址)

```

C 風格字串本質上就是個字元陣列，而陣列名稱具有指標性質，然而字串常量建立的內容是唯讀的，因此須於前方加入 const 才不會有警示：
```c++
const char *text = "hello";
```
上方結果與以下不同的，底下建立的 text 內容並不是唯讀的，因為 text 是個陣列，text 是將 "hello" 複製至各索引處：
```c++
char text[] = "hello";
```
無論是哪個形式，兩種都可以傳遞位址：
```c++
char text1[] = "hello";
const char *text2 = "hello";
const char *text = text1; // OK
text = text2;             // OK

//不過，底下不行：
char text1[] = "hello";
const char *text2 = "hello";
char *text = text1; // OK
text = text2;       // error: invalid conversion from 'const char*' to 'char*'

//錯誤該行如果真的想通過編譯，就必須明確告訴編譯器，你要去除 const 修飾，也就是使用 const_cast：
char text1[] = "hello";
const char *text2 = "hello";
char *text = text1;               // OK
text = const_cast<char*>(text2);  // OK
```
字串指標陣列建立：
```c++
int main() {
    const char *names[] = {"Justin", "Monica", "Irene"};

    for(auto name : names) {
        cout << name << endl;
    }

    return 0; 
}
```
注意，下方兩者建立意義不同，name1 的每個元素，儲存了各個字串常量的位址值；然而，name2 是有三個長度為 10 的 char 陣列，並複製了各個字串常量的 char。
```c++
const char *names1[] = {"Justin", "Monica", "Irene"};
char names2[][10] = {"Justin", "Monica", "Irene"}; 
```
<hr/>

## **5. C++ 動態記憶體配置** 
目前為止，變數建立後會配置記憶體空間，這類資源是配置在記憶體的堆疊區（Stack），生命週期侷限於函式執行期間。若要將函式執行結果傳回，不能直接傳回這類被自動配置空間的位址，因為函式執行過後，該空間就會釋出。

因此透過開發者自行管理記憶體資源，也就是自行配置記憶體，這些記憶體會被配置在堆積區（Heap），不會自動清除，開發者得在不使用資源時自行刪除記憶體。

```c++
int *p = new int; // 動態配置一個 int 型態大小的記憶體，且其值為初始化
int *p = new int(100); // 初始化其值為 100

int *p2 = new int[1000]; // 動態配置了 1000 個 int 大小的空間，並傳回空間的第一個位址
int *p2 = new int[3]{10, 20, 30};//初始化動態配置之三個值
int *p2 = new int[3](); // 初始化其值為 0 

delete p1;
delete [] p2; // 不使用時要使用 delete[] 歸還給記憶體，表示歸還的是整個連續空間
```

使用 new 動態配置的空間，在程式結束前不會自動歸還，必須使用 delete 將空間歸還，而如果一直沒有歸還，最後可能導致整個記憶體空間用盡。

動態配置也可以用來解決陣列需要事先知道長度的事實，透過動態地配置連續空間，並當成陣列來操作：
```c++
int main() {
    int size = 0; 

    cout << "輸入長度："; 
    cin >> size; 
    int *arr = new int[size]{0}; 

    cout << "指定元素：" << endl; 
    for(int i = 0; i < size; i++) { 
        cout << "arr[" << i << "] = "; 
        cin >> arr[i]; 
    } 

    cout << "顯示元素值：" << endl; 
    for(int i = 0; i < size; i++) {
        cout << "arr[" << i << "] = " << arr[i]
             << endl; 
    } 

    delete [] arr; 

    return 0; 
}
```

若要動態配置連續空間，並當成二維陣列來操作，其實就是將二維陣列想成多段一維陣列。
> 需要動態建立長度不定的容器時，建議考慮使用 vector 之類的容器。
```c++
int **arr = new int*[2];
for(int i = 0; i < 2; i++) {
    arr[i] = new int[3]{0};
}
// 刪除動態記憶體
for(int i = 0; i < 2; i++) {
    delete [] arr[i];
}
delete [] arr; 
```
<hr/>

## **6. C++ 命令列引數**
若要使用命令列引數，可以在括號中填寫如下。參數 argc 是一個整數，記錄命令列引數的個數（以空白作區隔），而 argv 分別儲存了引數的字串。
> 命令列引數的使用中，若輸入是一個包括空白的字串，就使用 "" 來包括。
```c++
int main(int argc, char *argv[]) { 
    for(int i = 1; i < argc; i++) {
        cout << argv[i] << endl;
    }

    return 0; 
}
```
<hr/>

## **7. C++ 參考**  
C++ 參考是指物件的別名，其不單單只是該物件的實例，而是記憶體中的資料。  

定義參考，是在型態關鍵字後加上 & 運算子，且參考一定要初始。
```c++
int n = 10;
int &r = n;

cout << "n：" << n << endl// n：10
     << "r：" << r << endl;// r：10

r = 20;

cout << "n：" << n << endl// n：20
     << "r：" << r << endl;// r：20
```
參考的物件也可以是個指標，或者是陣列，不過陣列需要指定長度。
```c++
// 宣告成指標
int n = 10;
int *p = &n;
int *&r = p; // 等同於 int* &r = p;
// 宣告成陣列
int arr[] = {1, 2};
int (&r)[2] = arr;
```

常量是記憶體中暫時的資料，因此無法透過 & 取址。不過加上 const 後不會報錯，因為編譯器會做下列轉換：
> 參考常量通常會與函式呼叫有關。
```c++
// 編譯器轉換
const int _n = 10;
const int &r = _n;
// 實際
const int &r = 10;
```

此外，如下方之運算式，因為也是將運算結果暫存在記憶體致無法取址，也可透過加入 const 來參考，或者也可以使用 && （rvalue reference）來參考運算結果。
> 粗略的判別方式，是看看 & 可否對運算式取址，若可以的話，運算式是 lvalue，否則是個 rvalue；lvalue 運算式的結果會是個有名稱的物件，例如 a，rvalue 的結果是暫時性存在於記憶體，例如 a + b。
```c++
int a = 10;
int b = 20;
string s1 = "hello";
string s2 = " pcku";
// 透過 const 來參考
const int &r = 10; 
const int &r2 = a + b; 
// 透過 &&（rvalue reference）參考：
int &&rr3 = 50;
int &&rr4 = a + b;
string &&result = s1 + s2;
// lvale 可直接參考：
int i = 10;
int &r = ++i; // ++i 運算結果是遞增後的 i，也就是 ++i 運算結果是個有名稱的物件
// rvalue 可利用 && 參考：
int i = 10;
int &&rr = i++; // i++ 運算結果是遞增前的 i，暫時性存在於記憶體，若不指定給變數的話就不見了
```

上方談到 [C++ 指標與陣列](#3-c-指標與陣列) 時有結合指標與 begin(), end() 來尋訪多維陣列元素，然而在內層的 for 迴圈只能透過指標的索引（位置偏移）來取得值，但是如果利用參考來參考指標就可以在內層迴圈也使用 begin(), end() 函式。
```c++
// for(auto &row : maze) { // 參考
for(int (*it)[3] = begin(maze); it < end(maze); it++) {
    int (&row)[3] = *it;   // 參考
    for(auto offset = begin(row); offset < end(row); offset++) {
        int n = *offset;
        cout << n << "\t"; 
    }
    cout << endl; 
} 
```

　