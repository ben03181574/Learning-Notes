# C++ 物件封裝
## **目錄**
---
1. [C++ 類別、物件](#1-c-類別物件)
2. [C++ 建構式、解構式、複製、移動建構式](#2-c-建構式解構式複製移動建構式)
3. [C++ 巢狀、區域、匿名類別](#3-c-巢狀區域匿名類別)
4. [C++ 關鍵字](#4-c-關鍵字)
5. [C++ 運算子重載、自訂型態轉換](#5-c-運算子重載自訂型態轉換)
6. [C++ 類別模板](#6-c-類別模板)
7. [C++ 類別成員指標](#7-c-類別成員指標)
---  
<br/>  

## **1. C++ 類別、物件** 
類別即把相關聯的資料組織在一起，提升其資料本身的可用性或者是程式碼的可讀性。 在 C++ 中，struct 與 class 的差別在於，前者在第一個權限可見的修飾詞出現前，定義的成員預設會是公開可存取，而後者預設會是私有 。

在實作類別建構式或方法（method）時，在 ::前指明實作哪類別之定義，:: 是類別範圍解析（class scope resolution）運算子。在建構式或方法的實作中，若要存取實例本身，可以透過 this，這是個指標，因此要透過箭號運算子來存取值域。

如果使用 new 來動態建構的實例，不需要動態建立的實例時，需要使用 delete 清除，透過指標存取實例成員時，要使用箭號運算子「->」；而一般存取實例的值域時可以使用 dot 運算子「.」。

```c++
//account.h
#include <string>
using namespace std; 

class Account { 
private:
    string id;  
    string name; 
    double balance;

public: 
    Account(string id, string name, double balance);
    void deposit(double amount);
    void withdraw(double amount);
    string to_string();
};
//account.cpp
#include <iostream> 
#include <string>
#include "account.h"
using namespace std;

Account::Account(string id, string name, double balance) {
    this->id = id;
    this->name = name;
    this->balance = balance;
}

string Account::to_string() {
    return string("Account(") + 
           this->id + ", " +
           this->name + ", " +
           std::to_string(this->balance) + ")";
}

void Account::deposit(double amount) {
    if(amount <= 0) {
        cout << "必須存入正數" << endl;
        return;
    }
    this->balance += amount;
}

void Account::withdraw(double amount) {
    if(amount > this->balance) {
        cout << "餘額不足" << endl;
        return;
    }
    this->balance -= amount;
}
//main.cpp
#include <iostream> 
#include <string>
#include "account.h"

int main() { 
    // 隱含地尋找符合初始式清單的建構式來進行實例建構
    Account acct = {"123-456-789", "Justin Lin", 1000};
    // 若account.h定義中改為 
    // explict Account(string id, string name, double balance); 
    // 則會抱錯。除非透過以下明確定義
    // Account acct("123-456-789", "Justin Lin", 1000);
    cout << acct.to_string() << endl;

    acct.deposit(500);
    cout << acct.to_string() << endl;

    acct.withdraw(700);
    cout << acct.to_string() << endl;

    return 0; 
}
```

<hr/>  

## **2. C++ 建構式、解構式、複製、移動建構式** 
### 建構式：
如果沒有定義任何建構式，編譯器會自動產生沒有參數的預設建構式，其會對每個值域進行預設初始化，對基本型態來說，會初始為各型態的零值，就類別型態來說，使用其無參數建構式來初始化，例如 string 來說，會初始為空字串。
> 如果定義了類別內初始式（in-class initializer），那麼預設建構式會使用初始式，例如以下：
```c++
class Account { 
public: 
    string id = "000-000-000";  
    string name = "Anonymous"; 
    double balance;
};
```

但是，若自定義了建構式，就算定義了無參數建構式，也不稱為預設建構式。例如以下：
```c++
class Account { 
    string id;  
    string name; 
    double balance;

public: 
    Account() {
        this->id = "000-000-000";
        this->name = "Anonymous";
        this->balance = 0.0;
    };
};
```

兩者(預設建構式 & 自訂建構式)實際上是有分別的，就以上的類別來說，若實例化 Account，id、name、balance 會進行預設初始化，之後執行建構式，將 "000-000-000"、"Anonymous"、0.0 指定給對應的值域。因此，如果將使用自訂建構式並將成員設成const 則會出錯，如以下：
```c++
class Foo {
    const int wat;
    Foo(int wat) {
        this->wat = wat;
    }
};
```

如果自定義了建構式，也想提供無參建構式，並希望其行為與預設建構式相同，可以加上 default；或者如果想在建構式中想要指定某個值域的值，可以定義初始式清單（constructor initializer list），這麼一來，id 值域就會用參數 id 的值初始化，name 值域就會用參數 name 的值初始化，balance 值域就會用參數 balance 的值初始化（即不會因為成員為 const 而報錯），括號中指定不一定要是參數，也可以是運算式，如果初始式清單省略了某個值域，那就會使用預設初始化。例如以下：
```c++
class Account { 
    string id;  
    string name; 
    double balance;

public: 
    Account() = default; 
    Account(string id, string name, double balance) : 
        id(id), name(name), balance(balance) {};
    Account(string id, string name) : Account(id, name, 0.0) {}
};
```

### 解構式：
若不再使用實例，應該清除這些 new 出來的 Node 實例，這可以藉由定義解析式（destructor）來實作，解析式是由 ~ 開頭，不用指定傳回型態，與類別名稱空間的成員函式，當實例被清除時，就會執行解構式。
> 如果沒有定義解構式，那麼編譯器會自行建立一個本體為空的解構式。
```c++
template <typename T>
class LinkedList {
    class Node {
    public:
        Node(T value, Node *next) : value(value), next(next) {}
        T value;
        Node *next;
    };

    Node *first = nullptr;

public:
    ~LinkedList(); // 解構式
    ...略
};

...略

template <typename T>
LinkedList<T>::~LinkedList() {
    if(this->first == nullptr) {
        return;
    }

    Node *last = this->first;
    do {
        Node *next = last->next;
        delete last;
        last = next;
    } while(last != nullptr);
}
```

### 複製建構式：
若使用一個類別實例來建構另一類別實例，預設會發生值域的複製，複製的行為視型態而定，以指標類型來說，會是複製位址，也就是淺複製（shallow copy）；也可以透過定義複製建構式（copy constructor）來實現自定義的複製行為。
> 跟預設建構式不同的是，無論有沒有定義其他建構式，若沒有定義複製建構式，那編譯器一定會生成一個複製建構式，預設會發生值域的複製，複製的行為視型態而定，基本型態的話就是複製值，指標的話是複製位址值，陣列的話，會逐一複製每個元素，類別型態的話，視各類別定義的複製建構式而定。

> 此外，通常以會一起自定義複製指定運算子（copy assignment operator）：
```c++
template <typename T>
class LinkedList {
    class Node {
    public:
        Node(T value, Node *next) : value(value), next(next) {}
        T value;
        Node *next;
    };

    Node *first = nullptr;

public:
    LinkedList() = default;               // 預設建構式
    LinkedList(const LinkedList<T> &lt);  // 複製建構式
    LinkedList<T>& operator=(const LinkedList<T> &lt);  // 定義複製指定運算子
    ~LinkedList();                        // 解構式
    ...略
};

template <typename T>
void LinkedList<T>::copy(const LinkedList<T> &lt) {
    // 逐一複製 Node 實例（而不是複製位址值）
    if(lt.first != nullptr) {
        this->first = new Node(lt.first->value, nullptr);
    }

    Node *thisLast = this->first;
    Node *srcNext = lt.first->next;
    while(srcNext != nullptr) {
        thisLast->next = new Node(srcNext->value, nullptr);
        thisLast = thisLast->next;
        srcNext = srcNext->next;
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &lt) {
    this->copy(lt);
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> &lt) {
    this->copy(lt);
    return *this;
}
...
```

### 移動建構式：
std::move 用來實現移動語義，而建構式、指定運算子也可以實現移動語義，也就是移動建構式（move constructor）、移動指定運算子（move assignment operator）
```c++
template <typename T>
class LinkedList {
    class Node {
    public:
        Node(T value, Node *next) : value(value), next(next) {}
        T value;
        Node *next;
    };

    Node *first = nullptr;

    void copy(const LinkedList<T> &lt);
    void move(LinkedList<T> &lt);

public:
    LinkedList() = default;
    LinkedList(const LinkedList<T> &lt);                // 複製建構式
    LinkedList(LinkedList<T> &&lt);                     // 移動建構式
    ~LinkedList();                                      // 解構式

    LinkedList<T>& operator=(const LinkedList<T> &lt);  // 複製指定運算子
    LinkedList<T>& operator=(LinkedList<T> &&lt);       // 移動指定運算子

    LinkedList<T>& append(T value);
    T get(int i);
};

template <typename T>
void LinkedList<T>::move(LinkedList<T> &lt) {
    if(lt.first != nullptr) {
        this->first = lt.first;
        lt.first = nullptr;
    }
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList<T> &&lt) {
    this->move(lt);
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T> &&lt) {
    this->move(lt);
    return *this;
}
...

int main() {
    LinkedList<int> lt1;
    lt1.append(1).append(2).append(3);

    LinkedList<int> lt2 = std::move(lt1); // 將 lt1 的資料移動給 lt2
    cout << lt2.get(2) << endl;

    return 0;
}
```

> 具有解構式、複製／移動建構式、複製／移動指定運算子的類別，要全權負責管理自身資源，稱為 Rule of five；具有解構式、複製／複製指定運算子，稱為 Rule of three；至於其他類別，就完全不需要其中之一，這就是 Rule of zero。
<hr/>  

## **3. C++ 巢狀、區域、匿名類別** 

在類別中可以再定義類別，稱為巢狀類別或內部類別，如以下：
```c++
class IntLinkedList {
    class IntNode {
    public:
        IntNode(int value, IntNode *next) : value(value), next(next) {}
        int value;
        IntNode *next;
    };

    IntNode *first = nullptr;

public:
    IntLinkedList& append(int value);
    int get(int i);
};
```
內部類別也可以透過類別解析子定義於外部：
```c++
class IntLinkedList {
    class IntNode {
    public:
        IntNode(int value, IntNode *next) : value(value), next(next) {}
        int value;
        IntNode *next;
    };

    IntNode *first = nullptr;

public:
    IntLinkedList& append(int value);
    int get(int i);
};
```
> IntNode 的值域是 public，這是為了便於給外部類別取用 IntNode 的值域，因為內部類別中若有 private 成員，外部類別預設也是不可存取的。

如果內部類別會是 public，你又不想公開某些值域，又想允許外部類別存取內部類別的 private 值域，可以宣告外部類別是內部類別的朋友，例如以下：
```c++
#include <iostream>
#include <string>
using namespace std;

class Screen {
public:
    class Pixel {
        int x;
        int y;
        friend Screen;  // 朋友類別
    public:
        Pixel(int x, int y) : x(x), y(y) {}
    };

    string info(Pixel px) {
        return "Pixel(" + to_string(px.x) + ", " + to_string(px.y) + ")";
    }
};

int main() {
    Screen screen;
    Screen::Pixel px(10, 10);
    cout << screen.info(px) << endl;

    return 0;
}
```

類別也可以在定義在函式之中，也就是區域類別，主要用來臨時封裝一組資料，然而，不可以存取函式中的區域變數。
```c++
#include <iostream>
using namespace std;

int main() {
    class Point {
    public:
        Point(int x, int y) : x(x), y(y) {}
        int x;
        int y;
    };

    Point p1(10, 10);
    Point p2(20, 20);

    return 0;
}
```

區域類別也可以匿名，也就是匿名類別
```c++
#include <iostream>
using namespace std;

int main() {
    const int dx = 10;
    const int dy = 20;

    class {
    public:
        int x = dx;
        int y = dy;
    } p;

    cout << p.x << endl;

    return 0;
}
```
> 範例中的 const 是必要的，因為類別中出現的 dx、dy 實際上並不是外部的 dx、dy，編譯器在類別中建立了新的 dx、dy，將外部 dx、dy 的值複製，為了避免類別中試圖參考或取址後進行變更，誤以為外部的 dx、dy 取值時也會隨之變化，故要求加上 const，這麼一來類別中試圖參考或取址也得加上 const，這樣就沒有變更的問題了。

<hr/>  

## **4. C++ 關鍵字** 
建立實例時，可以將其設定成const，然而如果呼叫的方法沒有被 const 修飾，就不能通過編譯。
```c++
#include <iostream> 
#include "account.h"
using namespace std;

int main() {
    const Account acct = {"123-456-789", "Justin Lin", 1000};
    cout << acct.to_string() << endl; //  error: passing 'const Account' as 'this' argument discards qualifiers
}
```
需要將其設定成 const，如以下：
```c++
#include <string>
using namespace std; 

class Account { 
private:
    string id;  
    string name; 
    double balance;

public: 
    Account(string id, string name, double balance);
    void deposit(double amount);
    void withdraw(double amount);
    string to_string() const;
};
...
string Account::to_string() const {
    return string("Account(") + 
           this->id + ", " +
           this->name + ", " +
           std::to_string(this->balance) + ")";
}
...
```
> const 的要求很嚴格，不僅要求方法不能變動實例狀態，如果以參考傳回型值域，或者是如上以參考傳回實例本身，也會要求傳回值的狀態不得改變

```c++
#include <iostream> 
#include "account.h"
#include <string>
using namespace std;

class Foo {
public:
    const Foo& doSome() const {
        return *this;
    }
    const Foo& doOther() const {
        return *this;
    }
};

int main() {
    Foo foo;
    foo.doSome().doOther();
}
```

被限定為 const 的方法中，如果想要改變某些值域（這些值域的改變，從使用者來看，並不代表實例狀態的改變），若是有這類需求，值域在宣告時，可以加上 mutable
```c++
class Foo {
    mutable int hash = 0;

public:
    int hashCode() const {
        if(hash == 0) {
            // 計算雜湊值 v
            this->hash = v;
        }

        return this->hash;
    }
};
```

有些資料不用實例各自擁有一份，而可以屬於類別，例如可以定義 Math 類別，它提供了 PI 成員，因為 PI 是個常數，不需要個別實例擁有各自的 PI，可以以下定義：
```c++
class Math {
public:
    constexpr static double PI = 3.14159;
};
```
想在類別內初始 static 資料成員的話，必須是個 constexpr，若否，必須在類別外指定
```c++
class Math {
public:
    static double PI;
};

double Math::PI = 3.14159;
```

static 成員屬於類別，同樣地，可以使用類別名稱加上 :: 解析運算子來呼叫 static 函式成員。
> static 函式中不會有 this 指標，因此 static 函式中不允許使用非 static 成員，因為沒有 this 可以使用。
<hr/>  

## **5. C++ 運算子重載、自訂型態轉換** 
運算子重載是函式重載的延伸應用
> 後置的 int 會傳入 0，實際上沒有作用，只是用來識別前置或後置，通常在重載 ++ 與 -- 運算子時，前置與後置都要重載。

>  .、::、.*、?: 不能重載
```c++
傳回型態 類別名稱::operator#(參數列) {
    // 實作重載內容
}
傳回型態 類別名稱::operator++();    // 前置，例如 ++x
傳回型態 類別名稱::operator++(int); // 後置，例如 x++
傳回型態 類別名稱::operator--();    // 前置，例如 --x
傳回型態 類別名稱::operator--(int); // 後置，例如 x--
```
實作如下：
```c++
#include <iostream>
#include <string>
using namespace std;

class Rational {
    int numer;
    int denom;

public:
    Rational(int numer, int denom) : numer(numer), denom(denom) {}
    Rational operator+(const Rational&);
    Rational operator-(const Rational&);
    Rational operator*(const Rational&);
    Rational operator/(const Rational&);
    Rational& operator++();
    Rational& operator--();
    Rational operator++(int);
    Rational operator--(int);
    string to_string() const;
};

Rational Rational::operator+(const Rational &that) { 
    return Rational(
        this->numer * that.denom + that.numer * this->denom,
        this->denom * that.denom
    ); 
} 

Rational Rational::operator-(const Rational &that) { 
    return Rational(
        this->numer * that.denom - that.numer * this->denom,
        this->denom * that.denom
    ); 
} 

Rational Rational::operator*(const Rational &that) { 
    return Rational(
        this->numer * that.numer,
        this->denom * that.denom
    ); 
} 

Rational Rational::operator/(const Rational &that) { 
    return Rational(
        this->numer * that.denom,
        this->denom * that.numer
    ); 
} 

Rational& Rational::operator++() {
    this->numer = this->numer + this->denom;
    return (*this);
}

Rational& Rational::operator--() {
    this->numer = this->numer - this->denom;
    return (*this);
}

Rational Rational::operator++(int) {
    Rational r = (*this);
    this->numer = this->numer + this->denom;
    return r;
}

Rational Rational::operator--(int) {
    Rational r = (*this);
    this->numer = this->numer - this->denom;
    return r;
}

string Rational::to_string() const { 
    return std::to_string(this->numer) + "/" + std::to_string(this->denom);
} 

int main() {
    Rational a(1, 2);
    Rational b(2, 3);

    cout << (a + b).to_string() << endl;
    cout << (a - b).to_string() << endl;
    cout << (a * b).to_string() << endl;
    cout << (a / b).to_string() << endl;
    cout << (++a).to_string() << endl;
    cout << (--a).to_string() << endl;
    cout << (b++).to_string() << endl;
    cout << (b--).to_string() << endl;

    return 0;
}
```

若運算子涉及不同型態的運算，運算子就得定義為非成員函式
```c++
#include <iostream>
#include <string>
using namespace std;

class Rational {
    int numer;
    int denom;

public:
    Rational(int numer, int denom) : numer(numer), denom(denom) {}
    friend Rational operator+(int, const Rational&);
    friend Rational operator+(const Rational&, int);
    ...略
};

...略

Rational operator+(int lhs, const Rational &rhs) { 
    return Rational(
        lhs * rhs.denom + rhs.numer,
        rhs.denom
    ); 
} 

Rational operator+(const Rational &lhs, int rhs) { 
    return Rational(
        lhs.numer + rhs * lhs.denom,
        lhs.denom
    ); 
} 

...略

int main() {
    Rational a(1, 2);
    Rational b(2, 3);

    ...略

    cout << (1 + a).to_string() << endl;
    cout << (a + 1).to_string() << endl;

    return 0;
}
```

當重載輸出運算子 << 時候，只能選擇實作為非成員函式：
```c++
#include <iostream>
#include <string>
using namespace std;

class Rational {
    ...略
public:
    ...略
    string to_string() const;
};

...略

string Rational::to_string() const { 
    return std::to_string(this->numer) + "/" + std::to_string(this->denom);

} 

ostream& operator<<(ostream &os, const Rational &r) {
    return os << r.to_string();
}

int main() {
    Rational a(1, 2);
    Rational b(2, 3);

    cout << (a + b) << endl;
    cout << (a - b) << endl;
    cout << (a * b) << endl;
    cout << (a / b) << endl;
    cout << (++a) << endl;
    cout << (--a) << endl;
    cout << (b++) << endl;
    cout << (b--) << endl;

    cout << (1 + a) << endl;
    cout << (a + 1) << endl;

    return 0;
}
```

呼叫函式時的 () 是呼叫運算子（call operator），你可以重載呼叫運算子
```c++
#include <iostream> 
using namespace std;

struct IntPlus {
    int operator()(int rhs, int lhs) const {
        return rhs + lhs;
    }
};

int main() { 
    IntPlus plus;
    cout << plus(10, 20) << endl;
    return 0; 
} 
```
相較於 lambda ，編譯器會對函子最佳化，因此比較有效率，不過就目的來說，因為函子是個物件，它就可以攜帶更多的資訊
```c++
#include <iostream> 
using namespace std;

class PrintLine {
    string sep;
public:
    PrintLine(string sep) : sep(sep) {}
    void operator()(string text) const {
        cout << text << sep;
    }
};

int main() { 
    PrintLine printLf("\n");
    PrintLine printCrLf("\r\n");

    printLf("print lf");
    printCrLf("print crlf");

    return 0; 
} 
```

其他的運算需求中，可能會想要 Rational 能轉換為 int、double 或者是其他型態，以便進一步以該型態的其他值進行運算，這可以透過自訂轉換函式來達到，又稱為轉型運算子
```c++
#include <iostream>
#include <string>
using namespace std;

struct Double {
    const double n;
    Double(double n) : n(n) {}
};

class Rational {
    int numer;
    int denom;

public:
    Rational(int numer, int denom) : numer(numer), denom(denom) {}

    operator double() {
        return static_cast<double>(this->numer) / this->denom;
    }

    operator Double() {
        return Double(static_cast<double>(this->numer) / this->denom);
    }
};

void foo(Double d) {
    cout << d.n << endl;
}

int main() {
    Rational a(1, 2);

    // a 隱含地轉換為 double
    cout << a + 0.1 << endl;  
    cout << 0.3 + a << endl;

    // a 隱含地轉換為 Double
    foo(a);

    return 0;
}
```

如果型態轉換必須得明確，可以加上 explicit
```c++
#include <iostream>
#include <string>
using namespace std;

struct Double {
    const double n;
    explicit Double(double n) : n(n) {}
};

class Rational {
    int numer;
    int denom;

public:
    Rational(int numer, int denom) : numer(numer), denom(denom) {}

    explicit operator double() {
        return static_cast<double>(this->numer) / this->denom;
    }

    explicit operator Double() {
        return Double(static_cast<double>(this->numer) / this->denom);
    }
};

void foo(Double d) {
    cout << d.n << endl;
}

int main() {
    Rational a(1, 2);

    cout << static_cast<double>(a) + 0.1 << endl;
    cout << 0.3 + static_cast<double>(a) << endl;

    foo(static_cast<Double>(a));

    return 0;
}
```
<hr/>  

## **6. C++ 類別模板**
類別模版實例化後的是各個不同的類別，更具體來說，是各種不同的型態。

類別模版實例化後，會是不同的類別、不同的型態，因此定義類別模版時，在傳回型態涉及類別模版本身時，必須包含模版參數，在 :: 範圍解析時也必須包含模版參數。
> 對於實作於類別之中的成員函式，不用範圍解析 ::，也不用重複宣告 template 模版參數名稱
```c++
#include <iostream>
using namespace std;

template <typename T>
class LinkedList {
    class Node {
    public:
        Node(T value, Node *next) : value(value), next(next) {}
        T value;
        Node *next;
    };

    Node *first = nullptr;

public:
    LinkedList<T>& append(T value);
    T get(int i);
};

template <typename T>
LinkedList<T>& LinkedList<T>::append(T value) {
    Node *node = new Node(value, nullptr);
    if(first == nullptr) {
        this->first = node; 
    }
    else {
        Node *last = this->first;
        while(last->next != nullptr) {
            last = last->next;
        }
        last->next = node;
    }
    return *this;
}

template <typename T>
T LinkedList<T>::get(int i) {
    Node *last = this->first;
    int count = 0;
    while(true) {
        if(count == i) {
            return last->value;
        }
        last = last->next;
        count++;
    }
}

int main() {
    LinkedList<int> intLt;
    intLt.append(1).append(2).append(3);
    cout << intLt.get(1) << endl;

    LinkedList<char> charLt;
    charLt.append('a').append('b').append('c');
    cout << charLt.get(2) << endl;

    return 0;
}
```

如果 static 資料成員是在類別外指定，記得範圍解析時也得加上型態參數，而使用 static 成員時，必須實例化，即使實例化時指定的型態與 static 無關，也是得實例化。
```c++
#include <iostream>
using namespace std;

template <typename T>
class Foo {
    static int wat;
public:
    static int wat10(); 
};

template <typename T>
int Foo<T>::wat = 10; 

template<typename T>
int Foo<T>::wat10() {
    return wat * 10;
}

int main() {
    cout << Foo<double>::wat10() << endl;
    return 0;
}
```

模版類別中若要宣告 friend 比較麻煩，因為 friend 與類別之間有耦合關係，因此必須事先做好宣告，宣告 friend 時才看得到彼此
```c++
#include <iostream>
using namespace std;

template <typename T>
class Foo;

template <typename T>
void foo(Foo<T> t);

template <typename T>
class Foo {
    T t;
public:
    Foo(T t) :t(t) {}
    friend void foo<T>(Foo<T> t);
};

template <typename T>
void foo(Foo<T> f) {
    cout << f.t << endl;
}

int main() {
    Foo<int> f(10);
    foo(f);

    return 0;
}
```
<hr/>  

## **7. C++ 類別成員指標** 
要宣告成員函式的指標，與非成員函式的指標宣告類似，主要是要以 :: 來指定是哪個類別的成員函式，函式簽署必須符合
```c++
void (Account::*mf1)(double) = nullptr;

mf1 = &Account::deposit;   
mf1 = &Account::withdraw;  

string (Account::*mf2)() = &Account::to_string; 
```

呼叫成員函式時，必須將提供實例的位址給成員函式中的 this 指標
> 如果 self 是個指標，就要透過 ->，
```c++
#include <iostream> 
#include <string>
#include "account.h"

void call(Account &self, void (Account::*member)(double), double param) {
    (self.*member)(param);
}

int main() {
    Account acct = {"123-456-789", "Justin Lin", 1000};

    call(acct, &Account::deposit, 1000);
    call(acct, &Account::withdraw, 500);

    cout << acct.to_string() << endl;

    return 0;
}
```
類似類別的成員函式指標，也可以宣告類別的資料成員指標
```c++
#include <iostream> 
using namespace std;

class Point {
public:
    int x;
    int y;
    Point(int x, int y) : x(x), y(y) {}
};

void printCord(Point &pt, int Point::*cord) {
    cout << pt.*cord << endl;
}

int main() {
    Point pt(10, 20);

    printCord(pt, &Point::x);    
    printCord(pt, &Point::y);    

    return 0;
}
```

static 成員屬於類別，某些程度上，就是將類別當成是一種名稱空間，實際上與一般函式無異
```c++
double (*fn)(double) = Math::toRadian;
```