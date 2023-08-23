# C++ 指標
## **目錄**
---
1. [C++ 指標位置與運算](#1-c-指標位置與運算)
2. [C++ 雙重指標](#2-c-雙重指標)
3. [C++ 指標陣列](#3-c-指標陣列)
4. [C++ 字元陣列與字串陣列](#4-c-字元陣列與字串陣列)
5. [C++ 動態記憶體配置](#5-c-動態記憶體配置)
6. [C++ 命令列引數](#6-c-命令列引數)
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

## **3. C++ 指標陣列** 
宣告陣列之後，使用到陣列變數時，會取得首元素的位址。而陣列索引其實是相對於首元素位址的位移量，這也就代表也可以利用指標運算來取出陣列的元素值。

可以利用指標結合 sizeof 來計算陣列長度，或者也可以利用 begin() 與 end() 來計算，或者利用指標運算求得。
> 每個陣列元素的位址型態是 int*，這表示對它進行運算時，是以 int 長度為單位，而 arr 變數的位址處就是陣列資料的開端，&arr 型態會是int (*)[LENGTH]，對它進行運算時，是以 LENGTH 個 int 長度為單位，因此 &arr + 1 的結果，會是陣列使用的空間後之位址，而 *(&arr + 1) 的值型態會回到 int*，也就是最後一個元素後之位址，這時就可以與 int* 的 arr 進行相減，也就是與第一個元素之位址相減，就可以得到陣列長度了。    

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
> 不過直接使用 auto 更為方便
```c++
int arr[2][3] = {{10, 20, 30}, {40, 50, 60}}; 
int (*p)[3] = arr;
//使用 auto
int arr2[1][2][3] = {{{10, 20, 30}, {40, 50, 60}}}; 
auto p2 = arr2;
```

<hr/>

## **4. C++ 字元陣列與字串陣列**
> C++ 中使用 C 風格字串是比較麻煩的，可以的話建議使用 string。

C 風格字串本質上就是個字元陣列，而陣列名稱具有指標性質，然而字串常量建立的內容是唯讀的，因此須臾前方加入 const 才不會有警示：
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
字串陣列建立：
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