## Потокобезопасный вектор

Тут вам нужно написать обертку над `std::vector`, однако единственное изменение данной структуры - добавление нового элемента в конец.  
Так как элементы никогда не меняются, то читать такой вектор могут сразу много нитей, однако при выполнении `PushBack` может произойти реаллокация, из-за чего на время добавления элемента необходимо сделать так, чтобы чтений не происходило.

---
Вам должны понадобиться [`std::shared_mutex`](https://en.cppreference.com/w/cpp/thread/shared_mutex) и [`std::shared_lock`](https://en.cppreference.com/w/cpp/thread/shared_lock).  
Также подумайте о том, в каких именно случая происходит реаллокация.
