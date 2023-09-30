# C++ 物件繼承與多型
## **目錄**
---
1. [C++ 繼承](#1-c-繼承)
2. [C++ 覆寫與多型](#2-c-覆寫與多型)
3. [C++ 純虛擬函式](#3-c-純虛擬函式)
4. [C++ 模板與繼承](#4-c-模板與繼承)
5. [C++ 多重繼承](#5-c-多重繼承)
6. [C++ 虛擬繼承](#6-c-虛擬繼承)
7. [C++ 繼承相關問題](#7-c-繼承相關問題)
---  
<br/>  

## **1. C++ 繼承** 

重複在程式設計上，就是不好的訊號。如果要改進，可以把相同的程式碼提昇（Pull up）為父類別：
> : 右邊的 public 表示權限控制最大是 public，也就是 Role 繼承而來的相關成員維持既有的權限控制。

> 在繼承類別時，還可以在 : 右邊指定 protected 或 private，表示繼承而來的 Role 成員權限控制最大是 protected 或 private，例如若 : 右邊指定 private，Role 的 protected 或 public 成員在子類中，權限就會被限縮為 private。
```c++
class Role {
    string name;   // 角色名稱
    int level;     // 角色等級
    int blood;     // 角色血量

public:
    Role(string name, int level, int blood)
     : name(name), level(level), blood(blood) {}

    string to_string() {
        return "(" + 
            this->name + ", " + 
            std::to_string(this->level) + ", " + 
            std::to_string(this->blood) + 
        ")";
    };
};

class SwordsMan : public Role {
public:
    SwordsMan(string name, int level, int blood) : Role(name, level, blood) {}

    void fight() {
        cout << "揮劍攻擊" << endl;
    }
};
```

然而，可以使用 using 指出哪些成員要維持父類中設定之權限
```c++
class D : private P {
public:
    using P::publicMember;    // 維持 public

protected:
    using P::protectedMember; // 維持 protected
};
```

SwordsMan、Magician 中定義了建構式，並呼叫了父類 Role 建構式，實際上建構式本體沒寫什麼，在這種情況下，你可能會想直接繼承 Role 定義的建構流程，這可以透過 using 指定父類名稱來達到
```c++
class SwordsMan : public Role {
public:
    using Role::Role;

    void fight() {
        cout << "揮劍攻擊" << endl;
    }
};

class Magician : public Role {
public:
    using Role::Role;

    void fight() {
        cout << "魔法攻擊" << endl;
    }

    void cure() {
        cout << "魔法治療" << endl;
    }
};
```

在子類別中若要呼叫父類建構式或者是父類方法，在其他語言中，會有 super 之類的關鍵字可以用，然而 C++ 必須使用父類名稱
```c++
class Magician : public Role {
public:
    using Role::Role;

    void fight() {
        cout << "魔法攻擊" << endl;
    }

    void cure() {
        cout << "魔法治療" << endl;
    }

    string to_string() {
        return "Magician" + Role::to_string();
    };
};
```

然而，在更複雜的情況，多個方法都得呼叫父類方法時，寫死一大堆父類名稱，可能就是個問題，如果父類名稱在撰寫時又比較複雜，問題可能就更大。

因此可以透過 using 來緩解：
```c++
class SwordsMan : public Role {
    using super = Role;

public:
    using Role::Role;

    void fight() {
        cout << "揮劍攻擊" << endl;
    }

    string to_string() {
        return "SwordsMan" + super::to_string();
    };
};
```

如果類別不希望有子類，可以定義類別為 final：
```c++
class Foo final {
};
```
如果不希望方法被子類重新定義，可以定義方法為 final 的 virtual，例如：
```c++
class Foo {
public:
    virtual void foo() final {
        cout << "foo" << endl;
    }
};
```
<hr/>  

## **2. C++ 覆寫與多型** 
覆寫父函式，這個需求可以藉由在各自的類別中定義函式來達成。在遮敝同名方法之後，編譯時綁定的版本，就是各自類別中定義的方法。因此如下：
```c++
SwordsMan swordsMan("Justin", 1, 1000);

Role &role = swordsMan;
cout << role.to_string() << endl; // 顯示 (Justin, 1, 1000)
```
雖然 role 實際上參考了 swordsMan，然而編譯時期能綁定的就是 Role 的 to_string 定義，因此執行的結果會是來自 Role 的 to_string 定義，而不是 SwordsMan 的 to_string 定義。

如果想在編譯時期，不管實例實際上是哪種型態，一律視呼叫方法時的變數型態來決定呼叫的版本，會透過模版來達成。例如：
```c++
template <typename T>
void printInfo(T &t) {
    cout << t.to_string() << endl;
}
```
因為是模版，實際上會依呼叫時指定的實例型態，重載出對應型態的版本，該呼叫哪個版本，是編譯時期就決定的事，是一種多型（polymorphism）的實現，此處亦被稱為編譯時期多型（compile-time polymorphism）。

上述皆針對編譯時期，以下針對執行時期

於父類的方法，你預期它的執行時期行為會被重新定義，也就是希望在執行時期，依照實例的型態綁定對應的方法版本，可以在父類定義方法時加上 virtual
> 程式碼中透過父類型態參考或指標操作，會在執行時期才綁定要執行的版本

> 標註 override，編譯器就會檢查，目前定義的方法是否真的是重新定義了父類別的 virtual 方法。
```c++
#include <iostream>
#include <string> 
using namespace std;

class Role {
    ...略

    virtual string to_string() {
        return "(" + 
            this->name + ", " + 
            std::to_string(this->level) + ", " + 
            std::to_string(this->blood) + 
        ")";
    };

    virtual ~Role() = default;
};

class SwordsMan : public Role {
    using super = Role;

    ...略

    string to_string() override {
        return "SwordsMan" + super::to_string();
    };
};

class Magician : public Role {
    using super = Role;

public:

    ...略

    string to_string() override {
        return "Magician" + super::to_string();
    };
};

void printInfo(Role &role) {
    cout << role.to_string() << endl;
}

int main() { 
    SwordsMan swordsMan("Justin", 1, 1000);
    Magician magician("Magician", 1, 800);

    printInfo(swordsMan);
    printInfo(magician);

    return 0;
}
```

在範例中 Role 的解構式也被定義為 virtual 了，這表示執行時期才會決定使用哪個版本的解構器，這影響的會是動態建立 Role 的子類實例後，以 delete 刪除該實例，會執行的是哪個版本的解構式
```c++
Role *role = new SwordsMan("Justin", 1, 1000);
delete role;
```
如果 Role 的解構式不是 virtual，那麼 role 會在編譯時期就綁定 Role 定義的解構式，delete role 執行的就只會是 Role 定義的解構式，這通常不會是你想要的結果，如果 Role 的解構式是 virtual，role 是在執行時期，依實例類型綁定解構式，就這邊就是 SwordsMan 的解構式，因此 delete role 執行的就會是 SwordsMan 定義的解構式，接著是 Role 的解構式。

絕大多數情況下，子類實例解決時，當然也想要執行子類的解構式，解構式預設並不是 virtual，因此若定義的類別，是會被用來繼承的基礎類別，應該定義解構式為 virtual。

<hr/>

注意！若非透過父類型態參考或指標操作，只是透過複製建構式建構了父類實例罷了（這邊的 role 實際上是建立了 Role 實例，而不是參考了 SwordsMan 實例）。例如：
```c++
SwordsMan swordsMan("Justin", 1, 1000);
Role role = swordsMan;
cout << role.to_string() << endl;
``` 

注意！父類型態可以參考子類型態實例，反過來則不行，例如：
```c++
SwordsMan swordsMan("Justin", 1, 1000);
Role &role = swordsMan;
SwordsMan &swordsMan2 = role; // 編譯錯誤
```
不過還是可以明確地轉換型態：
```c++
SwordsMan swordsMan("Justin", 1, 1000);
Role &role = swordsMan;
SwordsMan &swordsMan2 = dynamic_cast<SwordsMan&>(role);
```
<hr/>  

## **3. C++ 純虛擬函式** 
當子類別都有某方法，但卻不知道方法內容該寫什麼（可能因為子類別而異），可以將它設為純虛擬函式（pure virtual function）
```c++
class Role {
    ...略

public:
    Role(string name, int level, int blood)
     : name(name), level(level), blood(blood) {}

    virtual void fight() = 0;

    …略
};
```

被設為 0 的 virtual 函式，沒有任何實作，是個抽象方法，而擁有抽象方法的類別，是個抽象類別（abstract class），不能用來實例化，也不能如下指定：
```c++
Role role("Justin", 1, 1000); // error: cannot declare variable 'role' to be of abstract type 'Role'
Role role = swordsMan;
```

> 在定義類別時，可以完全只有純虛擬函式，完全不提供實作，也沒有任何狀態定義，將類別當成是一種行為規範。
<hr/>  

## **4. C++ 模板與繼承** 
Ball 類別繼承時將模版實例化為 Comparable<Ball>，Ball 只要重新定義 compareTo，就可以使用事先實作的 lessThan 等方法了，在這種情況下，Ball 是一種 Comparable<Ball>，也就是這球是一種可比較的球，關係上也比較合理。
```c++
#include <iostream>
using namespace std;

template <typename T>
class Comparable {
public:
    virtual int compareTo(T that) = 0;

    bool lessThan(T that) {
        return compareTo(that) < 0;
    }

    bool lessOrEquals(T that) {
        return compareTo(that) <= 0;
    }

    bool greaterThan(T that) {
        return compareTo(that) > 0;
    }

    bool greaterOrEquals(T that) {
        return compareTo(that) >= 0;
    }    

    bool equals(T that) {
        return compareTo(that) == 0;
    }

    virtual ~Comparable() = default;
};

class Ball : public Comparable<Ball> {
    double radius;

public:
    Ball(double radius) : radius(radius) {}

    int compareTo(Ball that) override {
        return this->radius - that.radius;   
    }
};

int main() { 
    Ball b1(100);
    Ball b2(50);

    cout << b1.greaterThan(b2) << endl;

    return 0;
}

```
<hr/>  

## **5. C++ 多重繼承** 
繼承本身就具有複雜性，在設計上並不鼓勵，在可以使用其他設計方式替代的場合，例如合成（composite），往往建議別使用繼承

C++ 可以多重繼承，也就是子類別可以同時繼承多個父類，多重繼承更會急劇地增加複雜度。

運用了多重繼承，若要繼承多個父類，可利用逗號區隔
```c++
class FlyingFish : public Fish, public Flyer {
public:    
    using Fish::Fish;

    void swim() override {
        cout << "飛魚 " + this->name + " 游泳" << endl; 
    }

    void fly() override {
        cout << "飛魚 " + this->name + " 飛翔" << endl; 
    }
};

在單一繼承時，情況比較單純，建構子類實例時，會先執行父類建構式，接著是子類建構式，而解構的時候相反，會先執行子類解構式，接著才是父類解構式。

單一繼承時 this 實際位址與父位址一樣
```c++
#include <iostream>
using namespace std;

class P {
    int x;

public:
    P(int x) : x(x) {
        cout << "P:" << this << endl;
    }
};

class C : public P {
public:
    C(int x) : P(x) {
        cout << "C:" << this << endl;
    }
};

int main() { 
    C c(10);

    cout << &c << endl;

    return 0;
}

P:0x61febc
C:0x61febc
0x61febc
```

多重繼承時，建構式的執行順序會與繼承的順序有關（而不是呼叫父類建構式的順序），至於解構式的執行順序，會是與建構式執行相反的順序
```c++
#include <iostream>
using namespace std;

class P1 {
    int x;

public:
    P1(int x) : x(x) {
        cout << "P1:" << this << endl;
    }
};

class P2 {
    int x;

public:
    P2(int x) : x(x) {
        cout << "P2:" << this << endl;
    }
};

class C : public P1, public P2 {
public:
    C(int x) : P1(x), P2(x) {
        cout << "C:" << this << endl;
    }
};

int main() { 
    C c(10);

    cout << &c << endl;

    return 0;
}
P1:0x61feb8
P2:0x61febc
C:0x61feb8
0x61feb8
```
若在個別的類別中寫 this->x，因為 this 位址不同，取得就會是各自不同的 x，因為是各自不同的位址，建構時也是個別地初始化在不同的位址，從執行結果中也可以看到，衍生類別實例的位址會用來初始第一個繼承的父類。

重繼承時，個別類別中的 this 位址不同的事實，也會反應在以父類型態參考子類實例之時：
```c++
...略

int main() { 
    C c(10);

    cout << &c << endl;

    P1 &p1 = c;
    P2 &p2 = c;

    cout << "p1:" << &p1 << endl;
    cout << "p2:" << &p2 << endl;

    return 0;
}
p1:0x61feb8
p2:0x61febc
```
<hr/>  

## **6. C++ 虛擬繼承** 
類別若繼承兩個以上的抽象類別，而兩個抽象類別都定義了相同方法，程式面上來說，並不會有錯誤，照樣通過編譯：
```c++
class Task {
public:
    virtual void execute() = 0;
    virtual void doSome() = 0;
    virtual ~Task() = default;
};

class Command {
public:
    virtual void execute() = 0;
    virtual void doOther() = 0;
    virtual ~Command() = default;
};

class Service : public Task, public Command {
public:
    void execute() override {
        cout << "foo" << endl;
    }

    void doSome() override {
        cout << "some" << endl;
    }

    void doOther() override {
        cout << "other" << endl;
    }
};
```

但如果表示相同的行為，那可以定義一個父類別，在當中定義純虛擬 execute 方法，而 Task 與 Command 繼承該類別，各自定義純虛擬的 doSome 與 doOther 方法，如：
```c++
#include <iostream>
using namespace std;

class Action {
public:
    virtual void execute() = 0;
    virtual ~Action() = default;
};

class Task : public Action {
public:
    virtual void doSome() = 0;
};

class Command : public Action {
public:
    virtual void doOther() = 0;
};

class Service : public Task, public Command {
public:
    void execute() override {
        cout << "service" << endl;
    }

    void doSome() override {
        cout << "some" << endl;
    }

    void doOther() override {
        cout << "other" << endl;
    }
};

int main() { 
    Service service;
    service.execute();
    service.doSome();
    service.doOther();

    Task &task = service;
    task.doSome();

    Command &command = service;
    command.doOther();

    return 0;
}
```
就上例來說，Action 的建構流程會跑兩次，一次是以 task 的位址，一次是以 command 的位址，這意謂著，如果 Action 定義了值域，task 與 command 會各自擁有一份。

目前為止的繼承方式，都是編譯時期就決定了子類從父類繼承而來的定義，例如，單看 Task，編譯時期就決定了從 Action 繼承而來的定義，而單看 Command，編譯時期就決定了從 Action 繼承而來的定義。

結果就是，由於 Task、Command 各自有一份編譯時期繼承而來的 Action 定義，如果 Service 同時繼承了 Task、Command，那它會有兩份 Action 定義，各來自 Task、Command，藉由 this 的實際位址來決定該使用哪個定義。

因此當如果是用 Action 型態來參考 service ，會因為 Service 有兩份 Action 定義，作為父型態的 Action 要參考 service 時，編譯器不知道你想採用哪份 Action 定義而報錯：
```c++
Action &action = service; // error: 'Action' is an ambiguous base of 'Service'
```

如果想在編譯時期就決定這件事，就得明確告訴編譯器：
```c++
Action &action1 = static_cast<Task&>(service);
Action &action2 = static_cast<Command&>(service);

action1.execute();
action2.execute();
```

如果不想使用 static_cast，可以透過虛繼承，也就是在繼承時加上 virtual 關鍵字來達到：
```c++
class Task : public virtual Action {
public:
    virtual void doSome() = 0;
};

class Command : public virtual Action {
public:
    virtual void doOther() = 0;
};

class Service : public Task, public Command {
public:
    void execute() override {
        cout << "service" << endl;
    }

    void doSome() override {
        cout << "some" << endl;
    }

    void doOther() override {
        cout << "other" << endl;
    }
};
```
現在 Task、Command 編譯過後，不會各自包含 Action 的定義了，只會各自有個可用來指向 Action 的指標，在執行時期才指向同一個 Action 類別，因此 Service 繼承而來的 Action 類別也就是 Task、Command 共享的那一個

> 在虛繼承下，Action 的建構式只會以 Service 實例的位址執行一次。

從語義上理解，實際上 Service 才真的實作 execute，Task、Command 不用真的包含 Action 定義，virtual 繼承時，Task、Command 就像是轉接 Action，Service 發現這兩個類別轉接的對象是同一個 Action，最後就會像是 Service 直接繼承 Action，若要做個比喻，就會像 class Service : public Action, public Task, public Command。
<hr/>  

## **7. C++ 繼承相關問題**
如果繼承的父類有實作方法，而另一父類有同名的純 virtual 函式，從父類繼承的實作方法並不被視為實作了純 virtual 函式。
```c++
#include <iostream>
using namespace std;

class P1 {
public:
    void foo() {
        cout << "foo" << endl;
    }
};

class P2 {
public:
    virtual void foo() = 0;
};

class C : public P1, public P2 {

};

int main() { 
    C c; // error: cannot declare variable 'c' to be of abstract type 'C'

    return 0;
}
```

如果繼承的父類具有同名的實作方法，會造成實例呼叫的方法版本模棱兩可：
```c++
#include <iostream>
using namespace std;

class P1 {
public:
    virtual void foo() {
        cout << "P1:foo" << endl;
    }
};

class P2 {
public:
    virtual void foo() {
        cout << "P2:foo" << endl;
    }
};

class C : public P1, public P2 {

};

int main() { 
    C c;

    c.foo();   // error: request for member 'foo' is ambiguous

    return 0;
}
```

必須明確指定給 P1 或 P2 型態：
```c++
C c;

P1 &p1 = c;
p1.foo();    // 顯示 P1:foo

P2 &p2 = c;
p2.foo();    // 顯示 P2:foo
```

若從兩個父類繼承了同名且可見的值域，也會有類似問題：
```c++
#include <iostream>
using namespace std;

class P1 {
public:
    int x = 10;
};

class P2 {
public:
    int x = 20;
};

class C : public P1, public P2 {

};

int main() { 
    C c;

    int x = c.x; // error: request for member 'x' is ambiguous

    return 0;
}
```
必須明確指定給 P1 或 P2 型態：
```c++
C c;

P1 &p1 = c;
cout << p1.x << endl;  // 10

P2 &p2 = c;
cout << p2.x << endl;  // 20
```