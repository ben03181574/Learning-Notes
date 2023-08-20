# C++ 流程控制
## **目錄**
---
1. [C++ if/switch](#1-c-ifswitch)
2. [C++ for/while](#2-c-forwhile)
3. [C++ break/continue/goto](#3-c-breakcontinuegoto)
---  
<br/>  

## **1. C++ if/switch**  
```c++
if(條件式一) {
    陳述一;
}
else if(條件式二) {
    陳述句二;
} 
else if(條件式三） {
    陳述句三;
}
else {
    陳述句四;
}
``` 

由於if 與 else 是就最近的一組來配對，因此下方兩組程式碼是等價的。所以，建議撰寫陳述句時一律加 `'}'` 以避免混淆。
```c++
if(條件式一)
    if(條件式二)
        陳述句一;
else
    陳述句二;
```
```c++
if(條件式一) {
    if(條件式二)
        陳述句一;
    else
    陳述句二;
}
```
<hr/>

switch 的括號，當中置放要取出數值的變數或運算式，取得數值之後，會與 case 設定的數字或字元比對，如果符合就執行以下的陳述句，直到遇到 break 後離開 switch 區塊，若沒有符合的數值或字元，會執行 default 後的陳述句，default 不需要時可以省略。
```c++
switch(變數名稱或運算式) {
    case 符合數字或字元:
        陳述句一;
        break;
    case 符合數字或字元:
        陳述句二;
        break;
    default:
        陳述三;
        break;
}
```

## **2. C++ for/while**  
```c++
for(初始式; 判斷式; 遞增式) {
    陳述句一;
    陳述句二;
}
```
<hr/>

while 如果迴圈本體只有一個陳述句，{} 可以省略不寫（但不建議）
```c++
while(條件式) {
    陳述句一;
    陳述句二;
}
```

do while 稱為後測式迴圈，它會先執行迴圈本體，然後再進行條件判斷。do while 是以分號（;）作為結束
```
do {
    陳述句一;
    陳述句二;
    ....
} while(條件式);
```
## **3. C++ break/continue/goto**  
break 可以離開目前 switch、for、while、do while 區塊，並前進至區塊後下一個陳述句，在 switch 主要用來結束陳述句進行至下一個 case 的比對，在 for、while 與 do while，主要用於中斷目前的迴圈執行。

continue 的作用與 break 類似，使用於迴圈，不同的是 break 會結束區塊的執行，而 continue 只會當次迴圈，並跳回迴圈區塊開頭繼續下一迴圈，而不是離開迴圈。

goto 是個很方便，但不被建議使用的語法，濫用的話會破壞程式的架構、使得程式難以閱讀，事實上，在完全不使用 goto 的情況下，也能撰寫程式。goto 可以在程式中任意跳躍，跳躍前必須先設定好目的地，跳躍時必須指定目的地。

START 就是 goto 的一個目標標籤（Label），後面使用冒號，標籤可以出現在程式的任一個地方。
```c++
BEGIN: 
    int input = 0; 
    cout << "輸入一數："; 
    cin >> input; 

    if(input == 0) {
        goto ERROR; 
    }

    cout << "100 / " << input 
         << " = " << 100.0 / input 
         << endl; 

    return 0; 

ERROR: 
    cout << "除數不可為 0" << endl; 
    goto BEGIN;
}
```